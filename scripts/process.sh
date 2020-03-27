##!/bin/bash

# config
use_docker=false
fps=30 # desired_fps
before=5 # in seconds: starting time from when splatting is detected
duration=10 # in seconds: clip duration



if [ $# !=  1 ]; then
    echo "usage: ../scripts/process.sh [absolute-path-to-video]"
fi
if [ ! -e "$1" ]; then
    echo "$1 not found."
    exit -1
fi
dname=$(dirname "$1") # dirname where video exists
bname=$(basename "$1") # video filename without directory path

outdir="${dname}/${bname%.*}"
mkdir -p "${outdir}"

if [ ! -e ./main ]; then
    echo "./main not found."
fi


# 可変フレームレートを固定フレームレート化(cfr->vfr)しfixed_fps.mp4に出力
if [ ${use_docker} ]; then
    docker run --rm --gpus=all -v "${outdir}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -i "/input/${bname}" -pix_fmt yuv420p -vcodec hevc_nvenc -q:v 0 -r ${fps} -vsync 1 /output/fixed_fps.mp4
else
    ffmpeg -stats -y -i "$1" -pix_fmt yuv420p -q:v 0 -r ${fps} -vsync 1 "${outdir}/fixed_fps.mp4"
fi

# c++のプログラムを走らせ，fixed_fps.mp4でのsplatのフレームを取得し，yarareta.logなどに出力
./main "${outdir}/fixed_fps.mp4" "${outdir}"

yarareta_list="${outdir}/yarareta_list.txt"
echo -n > "${yarareta_list}"

for frame in $(cat "${outdir}/yarareta.log")
do
    if [ ${use_docker} ]; then
        docker run --rm --gpus=all -v "${outdir}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "/input/${bname}" -t ${duration} /output/yarareta_${frame}.mp4
    else
        ffmpeg -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "$1" -t ${duration} "./${outdir}/yarareta_${frame}.mp4"
    fi
    if [ ${use_docker} ]; then
        echo "file '/output/yarareta_${frame}.mp4'" >> "${yarareta_list}"
    else
        echo "file '${outdir}/yarareta_${frame}.mp4'" >> "${yarareta_list}"
    fi
done

if [ ${use_docker} ]; then
    docker run --rm --gpus=all -v "${outdir}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -f concat -safe 0 -i "/output/yarareta_list.txt" -c copy "/output/yarareta_compilation.mp4"
else
    ffmpeg -stats -y -f concat -safe 0 -i "${outdir}/yarareta_list.txt" -c copy "${outdir}/yarareta_compilation.mp4"
fi


# やられた！のyararetaをtaoshitaに置換してコピペ
taoshita_list="${outdir}/taoshita_list.txt"
echo -n > "${taoshita_list}"

for frame in $(cat "${outdir}/taoshita.log")
do
    if [ ${use_docker} ]; then
        docker run --rm --gpus=all -v "${outdir}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "/input/${bname}" -t ${duration} /output/taoshita_${frame}.mp4
    else
        ffmpeg -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "$1" -t ${duration} "./${outdir}/taoshita_${frame}.mp4"
    fi
    if [ ${use_docker} ]; then
        echo "file '/output/taoshita_${frame}.mp4'" >> "${taoshita_list}"
    else
        echo "file '${outdir}/taoshita_${frame}.mp4'" >> "${taoshita_list}"
    fi
done

if [ ${use_docker} ]; then
    docker run --rm --gpus=all -v "${outdir}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -f concat -safe 0 -i "/output/taoshita_list.txt" -c copy "/output/taoshita_compilation.mp4"
else
    ffmpeg -stats -y -f concat -safe 0 -i "${outdir}/taoshita_list.txt" -c copy "${outdir}/taoshita_compilation.mp4"
fi


##!/bin/bash

# config
use_docker=true
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

if [ ! -e ./main ]; then
    echo "./main not found."
fi


# 可変フレームレートを固定フレームレート化(cfr->vfr)しfixed_fps.mp4に出力
if [ ${use_docker} ]; then
    docker run --rm --gpus=all -v "${PWD}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -i "/input/${bname}" -pix_fmt yuv420p -vcodec hevc_nvenc -q:v 0 -r ${fps} -vsync 1 /output/fixed_fps.mp4
else
    ffmpeg -stats -y -i "$1" -pix_fmt yuv420p -q:v 0 -r ${fps} -vsync 1 fixed_fps.mp4
fi

# c++のプログラムを走らせ，fixed_fps.mp4でのsplatのフレームを取得し，yarareta.logなどに出力
./main fixed_fps.mp4

for frame in $(cat yarareta.log)
do
    if [ ${use_docker} ]; then
        docker run --rm --gpus=all -v "${PWD}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "/input/${bname}" -t ${duration} /output/yarareta_${frame}.mp4
    else
        ffmpeg -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "$1" -t ${duration} yarareta_${frame}.mp4
    fi
done

for frame in $(cat taoshita.log)
do
    if [ ${use_docker} ]; then
        docker run --rm --gpus=all -v "${PWD}:/output" -v "${dname}:/input:ro" ffmpeg-nvenc -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "/input/${bname}" -t ${duration} /output/taoshita_${frame}.mp4
    else
        ffmpeg -stats -y -ss $(echo "${frame} / ${fps} - ${before}" | bc) -i "$1" -t ${duration} taoshita_${frame}.mp4
    fi
done

notify-send owari

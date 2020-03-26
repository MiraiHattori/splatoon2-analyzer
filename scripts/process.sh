##!/bin/bash
if [ $# !=  1 ]; then
    echo "usage: ../scripts/process.sh [absolute-path-to-video]"
fi

cur_dir=$(pwd)
dname=$(dirname "$1")
bname=$(basename "$1")
cd "${dname}"
docker run --rm --gpus=all -v ${PWD}:/tmp ffmpeg-nvenc -stats -y -i "/tmp/${bname}" -pix_fmt yuv420p -vcodec h264_nvenc -q:v 0 -r 30 -vsync 1 /tmp/out.mp4

cd ${cur_dir}
./main "${dname}/out.mp4"

for yarareta in $(cat yarareta.log)
do
    cd "${dname}"
    docker run --rm --gpus=all -v ${PWD}:/tmp ffmpeg-nvenc -stats -y -ss $(echo "${yarareta} / 30 - 5" | bc) -i "/tmp/${bname}" -t 10 /tmp/yarareta_${yarareta}.mp4
    cd ${cur_dir}
done

for taoshita in $(cat taoshita.log)
do
    cd "${dname}"
    docker run --rm --gpus=all -v ${PWD}:/tmp ffmpeg-nvenc -stats -y -ss $(echo "${taoshita} / 30 - 5" | bc) -i "/tmp/${bname}" -t 10 /tmp/taoshita_${taoshita}.mp4
    cd ${cur_dir}
done

notify-send owari

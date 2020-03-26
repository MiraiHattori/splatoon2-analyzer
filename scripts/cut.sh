#!/bin/bash

ffmpeg -i ~/splatted.mp4 -filter_complex trim=start_frame=4100,setpts=PTS-STARTPTS -frames 600 output.mp4

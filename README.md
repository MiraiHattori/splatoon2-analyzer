# Splatoon2 日本語版 動画解析ソフトウェア

## はじめに
これは，Nintendo SwitchのSplatoon2というゲームにおける動画解析のためのリポジトリです．
このリポジトリは[ネットワークサービスにおける任天堂の著作物の利用に関するガイドライン](https://www.nintendo.co.jp/networkservice_guideline/ja/index.html)に従っています．


## 動作確認に用いた環境
Ubuntu 19.10

aptで入れるもの
```bash
$ sudo apt install g++ libopencv-dev cmake make build-essential
```

Nvidiaドライバをお持ちの方はお好みで以下をするとffmpegがはやくなると思います．
```bash
$ sudo apt install cuda-drivers nvidia-container-toolkit
```

[ffmpeg-nvenc](https://github.com/myoshimi/ffmpeg-nvenc)のrootディレクトリで
```bash
#!/bin/bash
docker build -t ffmpeg-nvenc .
```

## 実行方法
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
$ ../scripts/process.sh [path-to-video] # ./mainの部分にはいったらtail -f *.logなどを見るとよい
```

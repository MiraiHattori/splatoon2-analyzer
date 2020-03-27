#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

// やられた！の部分のROI
cv::Rect roi_yarareta(890, 420, 140, 40);
// FIXME 何でやられたかを表示する部分
cv::Rect roi_cause(890, 420, 140, 40);
cv::Rect roi_taoshita(744, 1006, 435, 35);
cv::Mat yarareta_template = cv::imread("../yarareta.png");
cv::Mat taoshita_template = cv::imread("../taoshita.png");

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "usage: ./main [path-to-video] [output-dir]" << std::endl;
        return EXIT_FAILURE;
    }
    cv::VideoCapture cap(argv[1]);
    if (not cap.isOpened()) {
        std::cerr << "could not open video" << std::endl;
        return EXIT_FAILURE;
    }
    int frame_num = 0;
    constexpr int thresh = 300; // frame
    constexpr int mabiki = 5; // process only every 5 frames
    int yarareta_last_frame_num = -thresh - 1;
    int taoshita_last_frame_num = -thresh - 1;
    // ログに何フレーム目にイベントが起きたかを出力する
    std::ofstream yarareta_ofs(std::string(argv[2]) + "/yarareta.log");
    std::ofstream taoshita_ofs(std::string(argv[2]) + "/taoshita.log");
    cv::Mat src;
    while (cap.read(src)) {
        frame_num++;
        // 高速化のためフレームを間引く
        if (frame_num % mabiki != 0) {
            continue;
        }

        // やられた！
        cv::Mat yarareta_src = src(roi_yarareta);
        cv::Mat yarareta_result;
        cv::matchTemplate(yarareta_src, yarareta_template, yarareta_result, CV_TM_CCOEFF_NORMED);
        cv::Point yarareta_max_pt;
        double yarareta_max_val;
        cv::minMaxLoc(yarareta_result, NULL, &yarareta_max_val, NULL, &yarareta_max_pt);
        if (yarareta_max_val > 0.5 and frame_num - yarareta_last_frame_num > thresh) {
            // 最後に倒されたとき何フレーム目か
            yarareta_last_frame_num = frame_num;
            /*
            cv::Rect yarareta_rect(yarareta_max_pt.x + roi_yarareta.x,
                    yarareta_max_pt.y + roi_yarareta.y,
                    yarareta_template.cols,
                    yarareta_template.rows);
            cv::rectangle(src, yarareta_rect, cv::Scalar(0, 255, 255), 3);
            */
            // std::cout << frame_num << " " << yarareta_max_val << std::endl;
            yarareta_ofs << frame_num << std::endl;
            std::stringstream ss;
            ss << "yarareta_" << frame_num << ".png";
            /*
            cv::imwrite(ss.str(), src);
            // 何で倒されたか
            ss.str("");
            ss.clear(std::stringstream::goodbit);
            ss << frame_num << "_cause.png";
            cv::imwrite(ss.str(), src(roi_cause));
            */
        }
        // たおした！
        cv::Mat taoshita_src = src(roi_taoshita);
        cv::Mat taoshita_result;
        cv::matchTemplate(taoshita_src, taoshita_template, taoshita_result, CV_TM_CCOEFF_NORMED);
        cv::Point taoshita_max_pt;
        double taoshita_max_val;
        cv::minMaxLoc(taoshita_result, NULL, &taoshita_max_val, NULL, &taoshita_max_pt);
        // TODO 複数人倒したときの処理
        if (taoshita_max_val > 0.5 and frame_num - taoshita_last_frame_num > thresh) {
            // 最後に倒したとき何フレーム目か
            taoshita_last_frame_num = frame_num;
            /*
            cv::Rect taoshita_rect(taoshita_max_pt.x + roi_taoshita.x,
                    taoshita_max_pt.y + roi_taoshita.y,
                    taoshita_template.cols,
                    taoshita_template.rows);
            cv::rectangle(src, taoshita_rect, cv::Scalar(0, 255, 255), 3);
            */
            // std::cout << frame_num << " " << taoshita_max_val << std::endl;
            taoshita_ofs << frame_num << std::endl;
            /*
            std::stringstream ss;
            ss << "taoshita_" << frame_num << ".png";
            cv::imwrite(ss.str(), src(roi_taoshita));
            */
        }
    }
    return 0;
}

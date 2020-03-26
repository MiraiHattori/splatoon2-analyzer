#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

int yarareta_offset_x = 890;
int yarareta_offset_y = 420;
int yarareta_width = 140;
int yarareta_height = 40;
int cause_offset_x = 890;
int cause_offset_y = 420;
int cause_width = 140;
int cause_height = 40;
int taoshita_offset_x = 744;
int taoshita_offset_y = 1006;
int taoshita_width = 435;
int taoshita_height = 35;
cv::Mat yarareta_template = cv::imread("../yarareta.png");
cv::Mat taoshita_template = cv::imread("../taoshita.png");

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: ./main [path-to-video]" << std::endl;
        return EXIT_FAILURE;
    }
    cv::VideoCapture cap(argv[1]);
    if (not cap.isOpened()) {
        std::cerr << "could not open video" << std::endl;
        return EXIT_FAILURE;
    }
    int cnt = 0;
    constexpr int thresh = 300; // frame
    int yarareta_last_cnt = -thresh - 1;
    int taoshita_last_cnt = -thresh - 1;
    cv::Mat src;
    std::ofstream yarareta_ofs("./yarareta.log");
    std::ofstream taoshita_ofs("./taoshita.log");
    while (cap.read(src)) {
        cnt++;

        // やられた！
        cv::Mat yarareta = src(cv::Rect(yarareta_offset_x, yarareta_offset_y, yarareta_width, yarareta_height));
        cv::Mat yarareta_result;
        cv::matchTemplate(yarareta, yarareta_template, yarareta_result, CV_TM_CCOEFF_NORMED);
        cv::Rect yarareta_rect(0, 0, yarareta_template.cols, yarareta_template.rows);
        cv::Point yarareta_max_pt;
        double yarareta_max_val;
        cv::minMaxLoc(yarareta_result, NULL, &yarareta_max_val, NULL, &yarareta_max_pt);
        if (yarareta_max_val > 0.8 and cnt - yarareta_last_cnt > thresh) {
            yarareta_last_cnt = cnt;
            yarareta_rect.x = yarareta_max_pt.x + yarareta_offset_x;
            yarareta_rect.y = yarareta_max_pt.y + yarareta_offset_y;
            cv::rectangle(src, yarareta_rect, cv::Scalar(0, 255, 255), 3);
            std::cout << cnt << " " << yarareta_max_val << std::endl;
            yarareta_ofs << cnt << std::endl;
            std::stringstream ss;
            ss << "yarareta_" << cnt << ".png";
            cv::imwrite(ss.str(), src);
            cv::imshow("src", src);
            ss.str("");
            ss.clear(std::stringstream::goodbit);
            ss << cnt << "_cause.png";
            cv::imwrite(ss.str(), src(cv::Rect(cause_offset_x, cause_offset_y, cause_width, cause_height)));
        }
        // たおした！
        cv::Mat taoshita = src(cv::Rect(taoshita_offset_x, taoshita_offset_y, taoshita_width, taoshita_height));
        cv::Mat taoshita_result;
        cv::matchTemplate(taoshita, taoshita_template, taoshita_result, CV_TM_CCOEFF_NORMED);
        cv::Rect taoshita_rect(0, 0, taoshita_template.cols, taoshita_template.rows);
        cv::Point taoshita_max_pt;
        double taoshita_max_val;
        cv::minMaxLoc(taoshita_result, NULL, &taoshita_max_val, NULL, &taoshita_max_pt);
        // TODO 複数人(実は複数人にならない?)
        if (taoshita_max_val > 0.5 and cnt - taoshita_last_cnt > thresh) {
            taoshita_last_cnt = cnt;
            taoshita_rect.x = taoshita_max_pt.x + taoshita_offset_x;
            taoshita_rect.y = taoshita_max_pt.y + taoshita_offset_y;
            cv::rectangle(src, taoshita_rect, cv::Scalar(0, 255, 255), 3);
            cv::imshow("src", src);
            std::cout << cnt << " " << taoshita_max_val << std::endl;
            taoshita_ofs << cnt << std::endl;
            std::stringstream ss;
            ss << "taoshita_" << cnt << ".png";
            cv::imwrite(ss.str(), src(cv::Rect(taoshita_offset_x, taoshita_offset_y, taoshita_width, taoshita_height)));
        }
    }
    return 0;
}

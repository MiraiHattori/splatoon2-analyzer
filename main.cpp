#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>

int main()
{
    std::cout << "hello" << std::endl;
    // cv::Mat src = cv::imread("../scene.jpg");
    // cv::Mat roi = src(cv::Rect(600, 300, 400, 200));
    cv::Mat str_img = cv::imread("../yarareta.png");

    cv::VideoCapture cap("/home/future731/splatted.mp4");
    if (not cap.isOpened()) {
        std::cerr << "could not open video" << std::endl;
        return EXIT_FAILURE;
    }
    cv::namedWindow("yarareta", 1);
    cv::namedWindow("cause", 1);
    auto ocr = cv::text::OCRTesseract::create("");

    int cnt = 0;
    int roi_offset_x = 840;
    int roi_offset_y = 410;
    int roi_width = 240;
    int roi_height = 60;
    cv::Mat src;
    while (cap.read(src)) {
        cnt++;
        cv::Mat roi = src(cv::Rect(roi_offset_x, roi_offset_y, roi_width, roi_height));
        cv::Mat result;
        cv::matchTemplate(roi, str_img, result, CV_TM_CCOEFF_NORMED);
        cv::Rect roi_rect(0, 0, str_img.cols, str_img.rows);
        cv::Point max_pt;
        double max_val;
        cv::minMaxLoc(result, NULL, &max_val, NULL, &max_pt);
        roi_rect.x = max_pt.x + roi_offset_x;
        roi_rect.y = max_pt.y + roi_offset_y;
        if (max_val > 0.45) {
            cv::rectangle(src, roi_rect, cv::Scalar(0, 255, 255), 3);
            double time = cnt * 1.0 / 30.0;
            std::cout << time << " " << max_val << std::endl;
            std::stringstream ss;
            ss << cnt << ".png";
            cv::imwrite(ss.str(), src);
        }
        cv::Mat cause = src(cv::Rect(740, 360, 430, 50));
        cv::imshow("yarareta", roi);
        cv::imshow("cause", cause);
        char c = cv::waitKey(1);
        if (c == 'q' or c == 0x1b) {
            break;
        }
    }
    return 0;
}

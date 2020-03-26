#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/text.hpp>

int main()
{
    cv::Mat src = cv::imread("../yarareta.png");
    cv::namedWindow("yarareta", 1);
    auto ocr = cv::text::OCRTesseract::create("/usr/share/tesseract-ocr/4.00/tessdata/", "jpn");
    std::string result;
    ocr->run(src, result);
    std::cout << result << std::endl;
    cv::imshow("yarareta", src);
    cv::waitKey();
    return 0;
}

cmake_minimum_required(VERSION 3.13.4)
project(splatted-detection)

find_package(OpenCV REQUIRED)

add_executable(main main.cpp)
target_include_directories(main
    PUBLIC ${OpenCV_INCLUDE_DIR}
)
target_link_libraries(main
    PRIVATE ${OpenCV_LIBRARIES}
)

add_executable(ocr ocr.cpp)
target_include_directories(ocr
    PUBLIC ${OpenCV_INCLUDE_DIR}
)
target_link_libraries(ocr
    PRIVATE ${OpenCV_LIBRARIES}
)

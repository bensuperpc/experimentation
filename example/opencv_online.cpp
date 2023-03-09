#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <stdio.h>

auto main(int argc, char** argv) -> int
{
    if (argc != 2) {
        std::cout << "usage: File to open" << std::endl;
        return -1;
    }

    std::ifstream bin_file(std::string(argv[1]), std::ios::binary);

    if (!bin_file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return -1;
    }
    std::vector<uint8_t> file_buffer((std::istreambuf_iterator<char>(bin_file)), (std::istreambuf_iterator<char>()));

    bin_file.close();

    // Write the file to disk

    std::ofstream bin_file2(std::string(argv[1]) + ".copy", std::ios::binary);

    if (!bin_file2.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return -1;
    }

    bin_file2.write(reinterpret_cast<char*>(file_buffer.data()), file_buffer.size());

    bin_file2.close();


    /*
    cv::Mat image;
    image = cv::imread(argv[1], 1);

    if (!image.data) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    std::vector<Pixel> pixels;

    pixels.reserve(image.rows * image.cols);
#pragma omp parallel for schedule(auto)
    for (int i = 0; i < image.rows; ++i) {
        cv::Vec3b* pixel = image.ptr<cv::Vec3b>(i);
        for (int j = 0; j < image.cols; ++j) {
            auto it = std::find_if(pixels.begin(),
                                   pixels.end(),
                                   [&](const Pixel& p)
                                   { return p.r == pixel[j][2] && p.g == pixel[j][1] && p.b == pixel[j][0]; });

#pragma omp critical
            if (it != pixels.end()) {
                ++it->count;
            } else {
                pixels.push_back({pixel[j][2], pixel[j][1], pixel[j][0], 1});
            }
        }
    }

    std::sort(pixels.begin(), pixels.end(), [](const Pixel& a, const Pixel& b) { return a.count > b.count; });

    std::cout << std::left << std::setw(6) << "r" << std::setw(6) << "g" << std::setw(6) << "b" << std::setw(8)
              << "count" << std::endl;
    for (const auto& p : pixels) {
        std::cout << std::left << std::setw(6) << static_cast<int>(p.r) << std::setw(6) << static_cast<int>(p.g)
                  << std::setw(6) << static_cast<int>(p.b) << std::setw(8) << p.count << std::endl;
    }
    */

    /*
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
    */
    return 0;
}

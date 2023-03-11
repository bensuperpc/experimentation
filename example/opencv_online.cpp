#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <stdio.h>

std::vector<char> read_file(const std::string& file_name)
{
    std::ifstream bin_file(file_name, std::ios::binary);

    if (!bin_file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return {};
    }
    std::vector<char> file_buffer =
        std::vector<char>(std::istreambuf_iterator<char>(bin_file), std::istreambuf_iterator<char>());

    bin_file.close();

    return file_buffer;
}

void write_file(const std::string& file_name, std::vector<char>& file_buffer)
{
    std::ofstream out_stream(file_name, std::ios::binary);

    if (!out_stream.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    out_stream.write(reinterpret_cast<char*>(file_buffer.data()), file_buffer.size());
    out_stream.close();
}

auto main(int argc, char** argv) -> int
{
    if (argc != 2) {
        std::cout << "usage: File to open" << std::endl;
        return -1;
    }

    std::string input_image_name = argv[1];
    std::vector<char> file_buffer_in = read_file(input_image_name);

    std::cout << "File size: " << file_buffer_in.size() << std::endl;
    std::cout << "First 1 byte: " << static_cast<int>(file_buffer_in[0]) << std::endl;

    // Generate the image from the buffer (black and white)
    size_t image_size = std::ceil(std::sqrt(file_buffer_in.size()));

    std::cout << "Image size: " << image_size << std::endl;

    // Create the image
    cv::Mat image(image_size, image_size, CV_8UC3, cv::Scalar(128, 56, 45));

    for (size_t i = 0; i < file_buffer_in.size(); i++) {
        int x = i % image_size;
        int y = i / image_size;
        image.ptr<cv::Vec3b>(y)[x][0] = file_buffer_in[i];
        image.ptr<cv::Vec3b>(y)[x][1] = file_buffer_in[i];
        image.ptr<cv::Vec3b>(y)[x][2] = file_buffer_in[i];
    }

    // Write the image to disk
    cv::imwrite(std::string(argv[1]) + ".gen.png", image);

    // Read the image from disk and generate the buffer to write to disk
    cv::Mat image_out = cv::imread(std::string(argv[1]) + ".gen.png");

    std::vector<char> file_buffer_out;

    int termination = 0;
    for (size_t i = 0; i < image_size; i++) {
        for (size_t j = 0; j < image_size; j++) {
            if (image_out.ptr<cv::Vec3b>(i)[j][0] != image_out.ptr<cv::Vec3b>(i)[j][1] ||
                image_out.ptr<cv::Vec3b>(i)[j][1] != image_out.ptr<cv::Vec3b>(i)[j][2])
                {
                    break;
                }

            file_buffer_out.push_back(image_out.ptr<cv::Vec3b>(i)[j][0]);
        }
    }

    std::cout << "File size: " << file_buffer_out.size() << std::endl;
    std::cout << "First 1 byte: " << static_cast<int>(file_buffer_out[0]) << std::endl;

    write_file(std::string(argv[1]) + ".gen.final.png", file_buffer_out);

    return 0;
}

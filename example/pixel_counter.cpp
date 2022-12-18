#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <omp.h>

struct Pixel {
  uchar r, g, b;
  size_t count;
};

auto main(int argc, char** argv) -> int
{
  if (argc != 2) {
    printf("usage: DisplayImage.out <Image_Path>\n");
    return -1;
  }
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

      auto it = std::find_if(pixels.begin(), pixels.end(), [&](const Pixel& p) {
        return p.r == pixel[j][2] && p.g == pixel[j][1] && p.b == pixel[j][0];
      });

      #pragma omp critical
      if (it != pixels.end()) {
        ++it->count;
      } else {
        pixels.push_back({pixel[j][2], pixel[j][1], pixel[j][0], 1});
      }
    }
  }
  
  std::sort(pixels.begin(), pixels.end(), [](const Pixel& a, const Pixel& b) {
    return a.count > b.count;
  });

  for (const auto& p : pixels) {
    std::cout << "r: " << static_cast<int>(p.r) << " g: " << static_cast<int>(p.g) << " b: " << static_cast<int>(p.b) << " count: " << p.count << std::endl;
  }

  /*
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  cv::imshow("Display Image", image);
  cv::waitKey(0);
  */
  return 0;
}

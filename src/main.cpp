#include "SpectrogramGenerator.hpp"
#include "fft.hpp"
#include "drawImg.hpp"

#include <iostream>
#include <fstream>

#include <fftw3.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/test/unit_test.hpp>

int main()
{
  std::cout << "Hello world!" << std::endl;
  // Spooky scary skeleton!
  // *GUI
  // get input
  int out_sample_rate = 48000;
  int inputSamples = 4096;
  int height = 1025;
  int width = 500;
  int numOfCol = 500;
  // validate parameters

  SpectrogramGenerator generator(out_sample_rate, inputSamples, height, width, numOfCol);
  std::cout << "Decoder" << std::endl;
  if (generator.setupDecoder("audio/surprise.mp3") != 0)
    return -1;
  if (generator.decodeAudioFile() != 0)
    return -1;

  // temporary - delete later!
  std::ofstream decoded;
  decoded.open("test");
  decoded.write((char *)generator.data_, generator.data_size_ * sizeof(sample_fmt));
  decoded.close();

  generator.processSamples();

  //write outputData from FFT to file
  std::ofstream firer;
  firer.open("wyniki.txt");

  for (uint i = 0; i < generator.transformation_->specBuf.size(); ++i)
  {
    //firer << std::endl << "new frame " << i << std::endl;
    for (int j = 0; j < 256 / 2 + 1; ++j)
    {
      firer << (generator.transformation_->specBuf[i][j]) << std::endl;
      // }
    }
  }
  firer.close();

  generator.plotSpectrogram();
  //SpecImage picture(1025, 500, 4096, 500);
  // if (picture.createImage(generator.transformation_->specBuf))
  //   std::cout << "\nImage created\n";

  //drawing spectrogram
  // cv::Mat img(1024, 1000, CV_16UC1, cv::Scalar(0));
  // if (img.empty())
  // {
  //   std::cout << "\n Error - image not created \n";
  //   return -1;
  // }

  // const int length = 1;
  // std::vector<cv::Point> tempVec;

  // for (int i = 0; i < 1000; ++i)
  // {

  //   for (int j = 0; j < 1024; ++j)
  //   {
  //     cv::Point p3(length * i, 1 * j);
  //     //cv::Point p4(length * i + length, 1 * j);
  //     cv::Scalar colorIn((generator.transformation_->specBuf[i][j] / 60 + 1) * 65536);
  //     cv::line(img, p3, p3, colorIn, 4);
  //   }
  // }

  // cv::namedWindow("Spectrogram", cv::WINDOW_AUTOSIZE);
  // cv::imshow("Spectrogram", img);

  // cv::waitKey(0);

  // cv::destroyWindow("Spectrogram");

  // generator.transform(*start, size/ *end);
  // generator.createImage(*out_start);
}
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include "GeneratorConfiguration.hpp"
#include "ImageGenerator.hpp"
#include "DataComputer.hpp"
#include "Decoder.hpp"

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;

#include <boost/test/unit_test.hpp>

int add(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_CASE(generate_image)
{
    GeneratorConfiguration config;
    config.fft_out_frame_count_ = 2049;
    config.img_fft_per_img_ = 500;
    config.out_sample_rate_ = 44100;
    config.img_time_per_img_ = 30000;
    config.img_scale_time_interval_ = 500;
    config.img_height_ = 900;
    config.img_width_ = 1600;

    ImageGenerator *image = new ImageGenerator(&config);
    cv::Mat *testImg = new cv::Mat();

    BOOST_CHECK_EQUAL(image->getOutputImage()->size(), testImg->size());

    image->saveImage("1");

    BOOST_CHECK(image->getImage()->size() != image->getOutputImage()->size());
}

BOOST_AUTO_TEST_CASE(process_data)
{
    const int SIZE_OF_CHUNK = 10000;
    GeneratorConfiguration config;
    config.fft_out_frame_count_ = 2049;
    config.fft_in_frame_count_ = 4096;
    config.fft_delta_frame_ = 0.4 * config.fft_in_frame_count_;

    DataComputer *transformation = new DataComputer(&config);

    BOOST_CHECK_EQUAL(transformation->getPtrTransforms()->size(), 0);

    SampleFormat data[SIZE_OF_CHUNK];
    std::fill_n(data, SIZE_OF_CHUNK, 1);
    transformation->processSamples(data, SIZE_OF_CHUNK);
    BOOST_CHECK(transformation->getPtrTransforms()->size() > 0);

    int num_of_FFT = 1 + (SIZE_OF_CHUNK - config.fft_in_frame_count_) / config.fft_delta_frame_;
    BOOST_CHECK_EQUAL(transformation->getPtrTransforms()->size(), num_of_FFT);
}

BOOST_AUTO_TEST_CASE(open_file)
{
  GeneratorConfiguration config;
  const std::string FILE_NAME = "./audio/440hz.mp3";
  Decoder decoder(&config);
  decoder.openFile(FILE_NAME);
  BOOST_CHECK(decoder.getAVFormatCtx());
}

BOOST_AUTO_TEST_CASE(setup)
{
  GeneratorConfiguration config;
  config.out_sample_rate_ = 44100;
  const std::string FILE_NAME = "./audio/440hz.mp3";
  Decoder decoder(&config);
  decoder.openFile(FILE_NAME);
  decoder.setup();
  BOOST_CHECK(decoder.getAVCodecCtx());
  BOOST_CHECK(decoder.getSwr());
  BOOST_CHECK(decoder.getAVPacket());
  BOOST_CHECK(decoder.getAVFrame());
}

BOOST_AUTO_TEST_CASE(decode)
{
  SampleFormat *data = nullptr;
  int data_size = -1;
  GeneratorConfiguration config;
  config.fft_in_frame_count_ = 1000;
  config.out_sample_rate_ = 44100;
  config.fft_delta_frame_ = 500;
  const std::string FILE_NAME = "./audio/440hz.mp3";
  Decoder decoder(&config);
  decoder.openFile(FILE_NAME);
  decoder.setup();
  decoder.decode(&data, &data_size);
  BOOST_CHECK(data);
  BOOST_CHECK_NE(data_size, -1);
}
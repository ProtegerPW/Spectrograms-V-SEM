#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include <iostream>

#include <boost/python.hpp>

namespace bp = boost::python;

struct GeneratorConfiguration {
  uint out_sample_rate,
       img_width,
       img_height,
       fft_input_size,
       fft_per_img;
  float fft_overlapping;
  //
  void read();
};

std::ostream& operator<<(std::ostream& os, const GeneratorConfiguration& cfg);

#endif
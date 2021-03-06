/**
 * @file DataComputer.hpp
 * @author Jakub Marczuk
 * @brief File containing DataComputer class
 * @version 1.0
 * @date 2021-01-12
 */

#ifndef DATACOMPUTER_HPP
#define DATACOMPUTER_HPP

#include "GeneratorConfiguration.hpp"
#include "Decoder.hpp"

#include <vector>
#include <complex>
#include <fftw3.h>
#include <math.h>
#include <cstring>
#include <omp.h>

/// Class used to process raw audio data into displayable format using FFT.
/// * FFT transforms discrete audio signal from time domain into frequency domain.
/// * Transformed signal is stored in DataComputer::transforms_
class DataComputer
{
public:
  ///@param cfg structure holding parameters required during spectrogram generation process
  ///@see GeneratorConfiguration
  DataComputer(const GeneratorConfiguration *cfg);
  ~DataComputer();

  ///Process discrete audio signal from time domain into frequency domain
  ///@param data input discrete audio signal
  ///@param data_size number of input samples
  ///@note output samples are normalized using logarithmic scale (resulting range: <-60;0>)
  void processSamples(const SampleFormat *data, uint data_size);

  /// apply Hanning window on computed data chunk in order to properly run FFT
  ///@param curr_window_head pointer to first element of processed data chunk
  void hanningWindow(const SampleFormat *curr_window_head);

  /// calculate FFT, write normalized transform output to transforms_
  void compute();

  std::vector<float *> *getPtrTransforms();

private:
  const uint FFT_INPUT_SAMPLES;  // size of input data chunk
  const uint FFT_OUTPUT_SAMPLES; // size of output transform
  const float DELTA_FRAME;       // offset between subsequent FFT input data chunks

  fftwf_plan plan_;              // module responsible for transform calculation

  float *input_window_;          // input data chunk buffer
  fftwf_complex *output_buffer_; // buffer storing complex FFT output

  std::vector<float *> transforms_; //vector to store arrays of scores of each FFT transformation
};

#endif
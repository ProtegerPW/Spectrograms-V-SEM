#include "SpectrogramGenerator.hpp"
#include "GeneratorConfiguration.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  if (argv[1] == nullptr || argc > 2)
  {
    std::cerr << "Wrong number of parameters" << std::endl;
    std::cout << "Usage: ./run.sh <path_to_file>" << std::endl;
    return -1;
  }
  std::string file_name = argv[1];

  GeneratorConfiguration config;
  try
  {
    config.read();
    config.validateParameters();
    config.processParameters();
    std::cout << config;
  }
  catch (boost::python::error_already_set const &)
  {
    PyErr_Print();
    return -1;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  SpectrogramGenerator generator(&config);

  try
  {
    generator.openFile(file_name);
    generator.setupDecoder();
    generator.decodeAudioFile();
    generator.processSamples();
    generator.generateSpectrograms();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return -1;
  }
}
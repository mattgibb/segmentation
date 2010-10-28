#include <boost/program_options.hpp>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char * argv[])
{
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "Display this help message.")
    ("input", po::value<std::string>(), "Name of input file.  Alternative syntax to first positional parameter.")
    ("output", po::value<std::string>(), "name of output file. Alternative syntax to second positional parameter.")
  ;
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
 
  // if --help is specified, or the usage is wrong
  if ( vm.count("help") || !vm.count("input") || !vm.count("output") ) {
    cout << "\nUsage: " << argv[0] << " input output\n\n";
    cout << desc << "\n";
    return 0;
  }
  
}

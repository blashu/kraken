#include "Settings.h"

#include <boost/program_options.hpp>
#include <vector>

using namespace std;
namespace po = boost::program_options;

namespace kraken
{
  Settings::Settings( int argc, const char** argv )
  {
    _argc = argc;
    _argv = argv;
  }

  string Settings::path_to_bin()
  {
    return _path_to_bin;
  }

  bool Settings::run()
  {
    po::variables_map vm;
    po::options_description desc("Allowed options");

    try
    {
      desc.add_options()
          ("help", "produce help message")
          ("include-path,I", po::value<string>()->composing(), "include path");

      store(po::parse_command_line(_argc, _argv, desc), vm);
    }
    catch(std::exception const&  ex)
    {
      cout << "Can't init settings. " << ex.what() << endl;
      return false;
    }

    if( vm.count("help") )
    {
      std::cout << desc << std::endl;
      return false;
    }

    if( false == vm.count("include-path") )
    {
      std::cout << desc << std::endl;
      return false;
    }

    _path_to_bin = vm[ "include-path" ].as<string>();
    return true;
  }
}

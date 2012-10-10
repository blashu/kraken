#include "StdAfx.h"

#include "Settings.h"

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
    po::options_description desc("Allowed options");
  
    desc.add_options()
      ("help", "produce help message")
      ("exe", po::value<string>()->required(), "path to executable file");

    po::variables_map vm;
    store(po::parse_command_line(_argc, _argv, desc), vm);

    if( vm.count("help") )
    {
      std::cout << desc << std::endl;
      return false;
    }

    _path_to_bin = vm[ "exe" ].as<string>();

    return true;
  }
};

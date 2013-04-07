#ifndef __H_SETTINGS__
#define __H_SETTINGS__

#pragma once

#include <string>

using namespace std;

namespace kraken
{
  class Settings
  {
    public:
      Settings( int argc, const char** argv );

      string path_to_bin();

      bool run();

    private:
      int _argc;
      const char** _argv;

      string _path_to_bin;
  };
};

#endif
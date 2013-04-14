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
      enum class ActionType
      {
        UNKNOWN,
        SHOW_INSTR_SET,
        SHOW_ALL_INSTR
      };

      Settings( int argc, const char** argv );

      string path_to_bin();

      bool run();

      ActionType action();

    private:
      int _argc;
      const char** _argv;

      string _path_to_bin;

      ActionType _actionType;
  };
}

#endif

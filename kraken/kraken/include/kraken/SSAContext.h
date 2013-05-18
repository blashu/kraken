#ifndef SSACONTEXT_H
#define SSACONTEXT_H

#include "SSAGraph.h"

namespace kraken
{
  class SSABuilder
  {
    public:
      SSABuilder();

      bool build(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint);

    private:
      SSAGraph _graph;
  };
}

#endif // SSACONTEXT_H

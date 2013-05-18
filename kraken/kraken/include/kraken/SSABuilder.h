#ifndef __H_SSA_BUILDER__
#define __H_SSA_BUILDER__

#include "SSAGraph.h"

namespace kraken
{
  class SSABuilder
  {
    public:
      SSABuilder();

      SSABuilder(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint);

      bool build(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint);

    private:
      SSAGraph _graph;
  };
}

#endif

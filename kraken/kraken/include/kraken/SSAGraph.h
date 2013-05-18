#ifndef SSAGRAPH_H
#define SSAGRAPH_H

#include <kraken/kraken.h>
#include <kraken/AsmCode.h>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "SSAStatement.h"

using namespace std;

namespace kraken
{
  class SSAGraph
  {
    public:
      SSAGraph();

      bool fill(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint);

    private:
      static SSAStatement* add_handler(SSAGraph* graph, AsmCode& asmCode);
      static SSAStatement* sub_handler(SSAGraph* graph, AsmCode& asmCode);
      static SSAStatement* push_handler(SSAGraph* graph, AsmCode& asmCode);
      static SSAStatement* mov_handler(SSAGraph* graph, AsmCode& asmCode);
      static SSAStatement* jmp_handler(SSAGraph* graph, AsmCode& asmCode);

      typedef SSAStatement* (*instr_handler_callback)(SSAGraph*, AsmCode&);

      static std::map<std::string, instr_handler_callback> _instructionHandlersMap;

      std::map<va_t, int> _vaInstructionMap;
      boost::ptr_vector<SSAStatement> _statements;
  };
}

#endif // SSAGRAPH_H

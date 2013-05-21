#ifndef SSAGRAPH_H
#define SSAGRAPH_H

#include <kraken/kraken.h>
#include <kraken/AsmCode.h>
#include <map>
#include <vector>
#include <boost/ptr_container/ptr_map.hpp>

#include "SSAStatement.h"

using namespace std;

namespace kraken
{
  class SSAGraph
  {
    public:
      SSAGraph();

      bool fill(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint);

      typedef vector<SSAStatement> statement_chain;
      typedef statement_chain& statement_chain_ref;

    private:
      static vector<SSAStatement> add_handler(AsmCode& asmCode);
      static vector<SSAStatement> sub_handler(AsmCode& asmCode);
      static vector<SSAStatement> push_handler(AsmCode& asmCode);
      static vector<SSAStatement> mov_handler(AsmCode& asmCode);
      static vector<SSAStatement> jmp_handler(AsmCode& asmCode);

      typedef vector<SSAStatement> (*instr_handler_callback)(AsmCode&);

      static std::map<std::string, instr_handler_callback> _instructionHandlersMap;

      std::map<va_t, statement_chain> _vaStatementChainMap;
  };
}

#endif // SSAGRAPH_H

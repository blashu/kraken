#include "include/kraken/SSAContext.h"

using namespace kraken;

SSABuilder::SSABuilder()
{
}

bool SSABuilder::build(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint)
{
  return _graph.fill( instructionMap, entryPoint );
}

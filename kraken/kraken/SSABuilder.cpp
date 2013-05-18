#include "Include/kraken/SSABuilder.h"

using namespace kraken;

SSABuilder::SSABuilder()
{
}


SSABuilder::SSABuilder(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint)
{
  _graph.fill( instructionMap, entryPoint );
}

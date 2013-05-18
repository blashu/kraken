#include "include/kraken/SSAGraph.h"

#include <kraken/SSAConstArgument.h>
#include <boost/log/trivial.hpp>

using namespace kraken;

std::map<std::string, SSAGraph::instr_handler_callback> SSAGraph::_instructionHandlersMap;

SSAStatement* SSAGraph::add_handler(SSAGraph* graph, AsmCode& asmCode)
{
  return NULL;
}

SSAStatement* SSAGraph::sub_handler(SSAGraph* graph, AsmCode& asmCode)
{
  return NULL;
}

SSAStatement* SSAGraph::push_handler(SSAGraph* graph, AsmCode& asmCode)
{
  return NULL;
}

SSAStatement* SSAGraph::mov_handler(SSAGraph* graph, AsmCode& asmCode)
{
  return NULL;
}

SSAStatement* SSAGraph::jmp_handler(SSAGraph* graph, AsmCode& asmCode)
{

    SSADefinition definition( asmCode.Instruction.Mnemonic, asmCode.VirtualAddr );

    auto address = asmCode.Instruction.AddrValue;

    if( address < 0 )
    {
      BOOST_LOG_TRIVIAL(error) << "Unable to parse jmp instruction on address " << address;
      return NULL;
    }


    auto ssaConstAddress = SSAConstArgument::create_const_arg( address );

    SSAExpression expression(reinterpret_cast<SSAExpressionArgument*>(ssaConstAddress), NULL, SSAExpression::UnconditionalJmp );

    return new SSAStatement( definition, SSAStatement::JmpType, expression );
}

SSAGraph::SSAGraph()
{
  if ( _instructionHandlersMap.empty() )
  {
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "add ", add_handler ) );
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "sub ", sub_handler ) );
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "push ", push_handler ) );
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "mov ", mov_handler ) );
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "jmp ", jmp_handler ) );
  }
}

bool SSAGraph::fill(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint)
{
  for ( auto instrIt : instructionMap)
  {
    AsmCode* asmCode = instrIt->second;
    auto handlerIt = _instructionHandlersMap.find( asmCode->Instruction.Mnemonic );

    if ( _instructionHandlersMap.end() == handlerIt )
    {
      BOOST_LOG_TRIVIAL(error) << "Unable to generate SSA statement for instruction "
                               << asmCode->Instruction.Mnemonic
                               << "because there is no handler for it.";
      return false;
    }

    auto statement = handlerIt->second( this, *asmCode );
    if( NULL == statement )
    {
      BOOST_LOG_TRIVIAL(error) << "Unable to generate SSA statement for instruction "
                               << asmCode->Instruction.Mnemonic
                               << "because handler couldn't generate one.";
      return false;
    }

    _statements.push_back( statement );
    _vaInstructionMap.insert( pair<va_t, int>( asmCode->VirtualAddr, _statements.size() - 1 ) );
  }

  return true;
}

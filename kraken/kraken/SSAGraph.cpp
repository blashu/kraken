#include "include/kraken/SSAGraph.h"

#include <kraken/SSAConstArgument.h>
#include <boost/log/trivial.hpp>
#include <sstream>

using namespace std;
using namespace kraken;

std::map<std::string, SSAGraph::instr_handler_callback> SSAGraph::_instructionHandlersMap;

SSAGraph::statement_chain SSAGraph::add_handler(AsmCode& asmCode)
{
  return statement_chain();
}

SSAGraph::statement_chain SSAGraph::sub_handler(AsmCode& asmCode)
{
  return statement_chain();
}

SSAGraph::statement_chain SSAGraph::push_handler(AsmCode& asmCode)
{
  return statement_chain();
}

shared_ptr<SSADefinition> arg2def( va_t va, const Argument& arg, SSAGraph::statement_chain_ref chain )
{
  if( arg.ArgType & REGISTER_TYPE )
  {
    return make_shared<SSADefinition>( va, arg.ArgMnemonic );
  }

  if( arg.ArgType == MEMORY_TYPE )
  {
    if( arg.SegmentReg == ST_DSReg )
    {
      auto dsMemoryDef = make_shared<SSADefinition>( va, string("ds"), int( arg.Memory.Displacement ) );

      SSAExpression expression(make_shared<SSADefinition>( va, string("ds") ), SSAExpression::Add, SSAConstArgument::create_const_arg( arg.Memory.Displacement ));
      chain.push_back( SSAStatement( dsMemoryDef, SSAStatement::Assignment, expression ) );

      BOOST_LOG_TRIVIAL(debug) << chain.back().to_string();
      return dsMemoryDef;
    }
  }

  return NULL;
}

shared_ptr<SSAExpressionArgument> arg2expr( va_t va, const Argument& arg, SSAGraph::statement_chain_ref chain )
{
  auto def = arg2def( va, arg, chain );

  if( def )
  {
    return def;
  }

  if( arg.ArgType & CONSTANT_TYPE )
  {
    stringstream constConverter;
    uint64_t constant;
    constConverter << arg.ArgMnemonic;
    constConverter >> hex >> constant;

    return SSAConstArgument::create_const_arg( constant );
  }

  return NULL;
}

SSAGraph::statement_chain SSAGraph::mov_handler(AsmCode& asmCode)
{
  statement_chain chain;

  auto firstArgDef = arg2def( asmCode.VirtualAddr, asmCode.Argument1, chain );
  auto secondArgDef = arg2expr( asmCode.VirtualAddr, asmCode.Argument2, chain );

  if( NULL == firstArgDef || NULL == secondArgDef )
  {
    BOOST_LOG_TRIVIAL(error) << "Unable to parse mov instruction. va: " << asmCode.VirtualAddr;
    return vector<SSAStatement>();
  }

  SSAExpression expression(firstArgDef, SSAExpression::Add, secondArgDef);
  chain.push_back( SSAStatement( firstArgDef, SSAStatement::Assignment, expression ) );

  BOOST_LOG_TRIVIAL(debug) << chain.back().to_string();
  return chain;
}

SSAGraph::statement_chain SSAGraph::jmp_handler(AsmCode& asmCode)
{
  statement_chain chain;
  auto definition = std::shared_ptr<SSADefinition>( new SSADefinition( asmCode.VirtualAddr ) );

  auto address = asmCode.Instruction.AddrValue;

  if( address <= 0 )
  {
    BOOST_LOG_TRIVIAL(error) << "Unable to parse jmp instruction on address " << address;
    return vector<SSAStatement>();
  }

  auto ssaConstAddress = SSAConstArgument::create_const_arg( address );
  SSAExpression expression(SSAExpression::UnconditionalJmp, ssaConstAddress);

  chain.push_back( SSAStatement( definition, SSAStatement::JmpType, expression ) );
  BOOST_LOG_TRIVIAL(debug) << chain.back().to_string();
  return chain;
}

SSAGraph::SSAGraph()
{
  if ( _instructionHandlersMap.empty() )
  {
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "jmp ", jmp_handler ) );
    _instructionHandlersMap.insert( pair<std::string, instr_handler_callback>( "mov ", mov_handler ) );
  }
}

bool SSAGraph::fill(boost::ptr_map<va_t, AsmCode> instructionMap, va_t entryPoint)
{
  int countOfInstructions = 0;
  int countOfAnalyzedInstruction = 0;

  for ( auto instrIt : instructionMap)
  {
    countOfInstructions++;
    AsmCode* asmCode = instrIt->second;

    auto handlerIt = _instructionHandlersMap.find( asmCode->Instruction.Mnemonic );
    if ( _instructionHandlersMap.end() == handlerIt )
    {
      BOOST_LOG_TRIVIAL(error) << "Unable to generate SSA statement for instruction "
                               << asmCode->CompleteInstr
                               << " because there is no handler for it.";
      continue;
    }

    instr_handler_callback handler = handlerIt->second;
    auto statementChain = handler( *asmCode );
    if( statementChain.empty() )
    {
      BOOST_LOG_TRIVIAL(error) << "Unable to generate SSA statement for instruction "
                               << asmCode->CompleteInstr
                               << " because handler couldn't generate one.";
      continue;
    }

    countOfAnalyzedInstruction++;
    _vaStatementChainMap.insert( pair<va_t, statement_chain>( asmCode->VirtualAddr, statementChain ) );
  }

  BOOST_LOG_TRIVIAL(info) << "There are " << countOfAnalyzedInstruction << " instructions converted to SSA form.";

  return true;
}

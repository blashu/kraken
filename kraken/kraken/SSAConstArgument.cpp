#include "include/kraken/SSAConstArgument.h"

using namespace kraken;

std::map<uint64_t, std::shared_ptr<SSAConstArgument>> SSAConstArgument::_numToSSAConstArgMap;

SSAConstArgument::SSAConstArgument(uint64_t constant)
{
  _constant = constant;
}

std::shared_ptr<SSAConstArgument> SSAConstArgument::create_const_arg(uint64_t constant)
{
  auto iter = _numToSSAConstArgMap.find( constant );

  if ( iter != _numToSSAConstArgMap.end() )
  {
    return iter->second;
  }

  auto ssaConst = std::shared_ptr<SSAConstArgument>( new SSAConstArgument( constant ) );

  _numToSSAConstArgMap.insert( std::make_pair( constant, ssaConst ) );

  return ssaConst;
}

std::string SSAConstArgument::to_string(std::string format) const
{
  return std::to_string( _constant );
}

#include "include/kraken/SSAConstArgument.h"

using namespace kraken;

boost::ptr_map<uint64_t, SSAConstArgument> SSAConstArgument::_numToSSAConstArgMap;

SSAConstArgument::SSAConstArgument(uint64_t constant)
{
  _constant = constant;
}

SSAConstArgument* SSAConstArgument::create_const_arg(uint64_t constant)
{
  auto iter = _numToSSAConstArgMap.find( constant );

  if ( iter != _numToSSAConstArgMap.end() )
  {
    return iter->second;
  }

  auto ssaConst = new SSAConstArgument( constant );

  _numToSSAConstArgMap.insert( constant, ssaConst );

  return ssaConst;
}

std::string SSAConstArgument::to_string(std::string format) const
{
  return std::to_string( _constant );
}

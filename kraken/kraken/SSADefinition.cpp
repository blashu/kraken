#include "include/kraken/SSADefinition.h"

#include <sstream>

using namespace kraken;

SSADefinition::SSADefinition(std::string baseName, va_t virtualAddress, int index)
{
  _baseName = baseName;
  _va = virtualAddress;
  _index = index;
}

std::string SSADefinition::name() const
{
  std::stringstream name;

  if ( _index != 0 )
  {
    const char* sign = ( _index > 0 ? "+" : "" );

    name << "m[" << _baseName << sign << _index << "]";
  }
  else
  {
    name << _baseName;
  }

  if( _va != 0 )
  {
    name << "_" << _va;
  }

  return name.str();
}

std::string SSADefinition::to_string(std::string format) const
{
  return name();
}

#ifndef SSACONSTARGUMENT_H
#define SSACONSTARGUMENT_H

#include "SSAExpressionArgument.h"

#include <boost/ptr_container/ptr_map.hpp>

namespace kraken
{
  class SSAConstArgument : SSAExpressionArgument
  {
    public:
      SSAConstArgument(uint64_t constant);

      ~SSAConstArgument(){}

      static SSAConstArgument* create_const_arg(uint64_t constant);

      std::string to_string(std::string format = "") const;

    private:
      int _constant;

      static boost::ptr_map<uint64_t, SSAConstArgument> _numToSSAConstArgMap;
  };
}

#endif // SSACONSTARGUMENT_H

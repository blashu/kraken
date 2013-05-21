#ifndef SSACONSTARGUMENT_H
#define SSACONSTARGUMENT_H

#include "SSAExpressionArgument.h"

#include <map>
#include <memory>

namespace kraken
{
  class SSAConstArgument : public SSAExpressionArgument
  {
    public:
      SSAConstArgument(uint64_t constant);

      ~SSAConstArgument(){}

      static std::shared_ptr<SSAConstArgument> create_const_arg(uint64_t constant);

      std::string to_string(std::string format = "") const;

    private:
      int _constant;

      static std::map<uint64_t, std::shared_ptr<SSAConstArgument>> _numToSSAConstArgMap;
  };
}

#endif // SSACONSTARGUMENT_H

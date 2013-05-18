#ifndef SSAEXPRESSIONARGUMENT_H
#define SSAEXPRESSIONARGUMENT_H

#include "kraken.h"
#include <string>

namespace kraken
{
  class SSAExpressionArgument
  {
    public:
      virtual ~SSAExpressionArgument() = 0;

      virtual std::string to_string(std::string format = 0) const = 0;
  };

  inline SSAExpressionArgument::~SSAExpressionArgument()
  {
  }
}


#endif // SSAEXPRESSIONARGUMENT_H

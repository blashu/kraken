#ifndef SSAEXPRESSION_H
#define SSAEXPRESSION_H

#include "SSAExpressionArgument.h"

#include <memory>

namespace kraken
{
  class SSAExpression
  {
    public:
      enum ExpressionType
      {
        Constant = 0,
        UnconditionalJmp,
        ConditionalJmp,
        Add,

        ET_COUNT
      };

      static const char* ExpressionTypeStrings[ET_COUNT];

      SSAExpression(ExpressionType expressionType, std::shared_ptr<SSAExpressionArgument> arg1);

      SSAExpression(std::shared_ptr<SSAExpressionArgument> arg1,
                    ExpressionType expressionType,
                    std::shared_ptr<SSAExpressionArgument> arg2);

      std::string to_string() const;

    private:
      SSAExpression(){}

      std::shared_ptr<SSAExpressionArgument> _operand1;
      std::shared_ptr<SSAExpressionArgument> _operand2;

      ExpressionType _type;
  };
}

#endif // SSAEXPRESSION_H

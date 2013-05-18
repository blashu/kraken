#include "include/kraken/SSAExpression.h"

using namespace kraken;

const char* SSAExpression::ExpressionTypeStrings[ET_COUNT] = {
  "Constant",
  "go to",
  "ConditionalJmp",
  "+"
};

SSAExpression::SSAExpression(const SSAExpressionArgument* arg1, const SSAExpressionArgument* arg2, SSAExpression::ExpressionType expressionType)
{
  _operand1 = arg1;
  _operand2 = arg2;
  _type = expressionType;
}

std::string SSAExpression::to_string() const
{
  switch(_type)
  {
    case ExpressionType::UnconditionalJmp:
      return std::string( ExpressionTypeStrings[ _type ] ) + " " + _operand1->to_string();
    default:
      return _operand1->to_string() + " " + ExpressionTypeStrings[ _type ] + " " + _operand2->to_string();
  }
}

#include "include/kraken/SSAExpression.h"
#include <cassert>

using namespace kraken;

const char* SSAExpression::ExpressionTypeStrings[ET_COUNT] = {
  "Constant",
  "go to",
  "ConditionalJmp",
  "+"
};

SSAExpression::SSAExpression(ExpressionType expressionType, const SSAExpressionArgument* arg1)
{
  assert(NULL != arg1);

  _operand1 = arg1;
  _operand2 = NULL;
  _type = expressionType;
}

SSAExpression::SSAExpression(const SSAExpressionArgument* arg1, SSAExpression::ExpressionType expressionType, const SSAExpressionArgument* arg2)
{
  assert(NULL != arg1);
  assert(NULL != arg2);

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

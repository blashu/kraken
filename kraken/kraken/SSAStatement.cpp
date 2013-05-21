#include "include/kraken/SSAStatement.h"

using namespace kraken;

SSAStatement::SSAStatement(std::shared_ptr<SSADefinition> definition,
                           StatementType statementType,
                           SSAExpression expression)
  : _definition(definition), _expression(expression)
{
  _operationType = statementType;
}

std::string SSAStatement::to_string()
{
  auto definitionStr = _definition->to_string();

  if( definitionStr.empty() )
  {
    return _expression.to_string();
  }
  return _definition->to_string() + " := " + _expression.to_string();
}

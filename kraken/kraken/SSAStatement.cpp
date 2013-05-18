#include "include/kraken/SSAStatement.h"

using namespace kraken;

SSAStatement::SSAStatement(SSADefinition definition, StatementType statementType, SSAExpression expression)
  : _definition(definition), _expression(expression)
{
  _operationType = statementType;
}

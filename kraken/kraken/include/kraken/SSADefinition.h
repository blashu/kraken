#ifndef SSADEFINITION_H
#define SSADEFINITION_H

#include "SSAExpressionArgument.h"

namespace kraken
{
  class SSADefinition : public SSAExpressionArgument
  {
    public:
      SSADefinition(){}

      SSADefinition(va_t virtualAddress);

      SSADefinition(va_t virtualAddress, std::string baseName, int index = 0);

      ~SSADefinition(){}

      std::string to_string(std::string format = "") const;

      std::string name() const;

    private:
      /*!
       * Stores the name of the base object ( register \ memory \ etc )
       */
      std::string _baseName;

      /*!
       * In the current implementation virtual address is used as a subscript.
       *
       * Subscript is required to distinguish different definitions of the same base object ( register \ memory \ etc )
       */
      va_t _va;

      /*!
       * Index is used when definition is array member.
       *
       * It makes it easier to represent in the form of m[ base_def_name + index ]_subscript,
       * where base_def_name is the name of the definition which represents the base of the array,
       * index stores the offset in bytes for the defined member and subscript is the regular subscript
       */
      int _index;
  };
}
#endif // SSADEFINITION_H

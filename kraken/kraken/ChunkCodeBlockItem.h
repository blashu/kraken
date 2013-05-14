#ifndef __H_CHUNK_CODE_BLOCK_ITEM__
#define __H_CHUNK_CODE_BLOCK_ITEM__

#include "include/kraken/CodeBlockItemInterface.h"
#include "include/kraken/AsmCode.h"

namespace kraken {
  namespace internal
  {
    class ChunkCodeListing;

    class ChunkCodeBlockItem : CodeBlockItemInterface
    {
      public:
        ChunkCodeBlockItem( ChunkCodeListing* codeListing, const AsmCode* asmCode );
        virtual ~ChunkCodeBlockItem();

        virtual va_t va();
        virtual string to_string(const string& format = "");
        virtual bool is_branch();
        virtual const vector<code_item_location_t>& go_to();

      private:
        ChunkCodeListing* _codeListing;
        const AsmCode* _asmCode;
        std::vector<code_item_location_t> _goTo;
    };
  }

}

#endif

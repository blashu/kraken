#ifndef __H_CHUNK_CODE_BLOCK__
#define __H_CHUNK_CODE_BLOCK__

#include "include/kraken/CodeBlockInterface.h"
#include "ChunkCodeBlockItem.h"
#include "include/kraken/CodeChunk.h"

namespace kraken {
  namespace internal
  {
    class ChunkCodeBlock : public CodeBlockInterface
    {
      public:
        ChunkCodeBlock( const CodeChunk* codeChunk );
        virtual ~ChunkCodeBlock();

        virtual CodeBlockItemInterface* get_item_by_id(int id);
        virtual CodeBlockItemInterface* get_item_by_va(va_t virtualAddress);
        virtual int get_item_count();

      private:
        const CodeChunk* _codeChunk;
        std::vector<ChunkCodeBlockItem> _items;

    };
  }
}

#endif

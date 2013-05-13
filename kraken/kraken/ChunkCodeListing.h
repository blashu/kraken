#ifndef __H_CHUNK_CODE_LISTING__
#define __H_CHUNK_CODE_LISTING__

#include "include/kraken/CodeListingInterface.h"
#include "include/kraken/CodeChunk.h"
#include "include/kraken/Disassembler.h"
#include "ChunkCodeBlock.h"

#include <memory>

namespace kraken {
  namespace internal {
    KRAKEN_API_ class ChunkCodeListing : CodeListingInterface
    {
      public:
        ChunkCodeListing( std::shared_ptr<Disassembler> disassembler );
        virtual ~ChunkCodeListing();

        virtual CodeBlockInterface* get_block_by_id(int id);
        virtual CodeBlockItemInterface* get_item_by_id(int blockId, int itemId);
        virtual CodeBlockItemInterface* get_item_by_va(va_t virtualAddress);
        virtual int block_count();
        virtual int item_count();

      private:
        std::shared_ptr<Disassembler> _disassembler;
        std::vector<ChunkCodeBlock> _blocks;
    };
  }
}

#endif // CHUNKCODELISTING_H

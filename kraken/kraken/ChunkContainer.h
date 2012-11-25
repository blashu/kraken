#ifndef __H_INSTRUCTION_CONTAINER__
#define __H_INSTRUCTION_CONTAINER__

#include <vector>
#include <queue>
#include <string>

#include "Disassembler.h"

using namespace std;
using namespace kraken;

KRAKEN_API_ class ChunkContainer
{
  public:
    typedef vector<CodeChunk> chunk_container_t;
    typedef chunk_container_t::const_iterator chunk_container_iter;

    ChunkContainer() {}

    ChunkContainer(const Disassembler& disassembler);

    // Fills chunk container with disassembled chunks of code, while handling
    // unconditional branch instructions and possible chunk intersections
    virtual bool fill(const Disassembler& disassembler);
    
    virtual ~ChunkContainer() {};

    virtual inline chunk_container_iter begin() const;

    virtual inline chunk_container_iter end() const;

    virtual const CodeChunk& front() const;

    virtual const CodeChunk& back() const;

  private:
    /////////////////////////////////////////
    // fields
    typedef vector<CodeChunk> code_collection_t;

    code_collection_t _codeCollection;

    /////////////////////////////////////////
    // functions
    bool is_instruct_decoded( rva_t address );

    virtual CodeChunk disassemble_next_code_chunk(queue<rva_t>& jumpInstructionQueue, const Disassembler& disassemble);

    // OPTIMIZE: this function is not optimized
    // Checks if there is a code chunk that intersects with the one that is passed to this function
    // and returns iterator to that chunk, otherwise returns iterator to end
    virtual code_collection_t::iterator check_if_intersects(const CodeChunk& codeChunk);

    virtual void merge_code_chunks(CodeChunk& resultChunk, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk);
};

inline ChunkContainer::chunk_container_iter ChunkContainer::begin() const
{
  return _codeCollection.begin();
}

inline ChunkContainer::chunk_container_iter ChunkContainer::end() const
{
  return _codeCollection.end();
}

inline const CodeChunk& ChunkContainer::front() const
{
  return _codeCollection.front();
}

inline const CodeChunk& ChunkContainer::back() const
{
  return _codeCollection.back();
}

#endif
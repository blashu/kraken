#ifndef __H_INSTRUCTION_CONTAINER__
#define __H_INSTRUCTION_CONTAINER__

#include <vector>
#include <queue>
#include <string>

#include "PeDisassembler.h"
#include "CodeChunk.h"

using namespace std;
using namespace kraken;

KRAKEN_API_ class ChunkContainer
{
  private:
    typedef vector<CodeChunk> code_collection_t;

    // Vector with parsed chunks of code
    code_collection_t _codeCollection;

    // Raw code
    vector<unsigned char> _codeBuff;

  public:
    ChunkContainer(){}

    ChunkContainer(const vector<unsigned char> &memBuff, size_t startCodeSection, rva_t virtualAddress);

    virtual bool fill(const vector<unsigned char> &memBuff, size_t startCodeSection, rva_t virtualAddress);
    
    virtual ~ChunkContainer() {};

  private:
    // Calculate offset in the memory based on the passed virtual address
    virtual int rva_to_offset(const int &rva);

    // Disassemble code chunk
    virtual CodeChunk disassemble_code_chunk(queue<AsmCode>& jumpInstructionQueue, const Disassembler* disassemble);

    // OPTIMIZE: this function is not optimized
    // Checks if there is a code chunk that intersects with the one that is passed to this function
    // and returns iterator to that chunk, otherwise returns iterator to end
    virtual code_collection_t::iterator check_if_intersects(const CodeChunk& codeChunk);

    virtual void merge_code_chunks(CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk);
};

#endif
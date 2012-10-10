#ifndef __H_INSTRUCTION_CONTAINER__
#define __H_INSTRUCTION_CONTAINER__

#include <vector>
#include <queue>
#include <string>
#include <BeaEngine\BeaEngine.h>

#include "CodeChunk.h"

using namespace std;

KRAKEN_API_ class CodeContainer
{
  private:
    typedef vector<CodeChunk> code_collection_t;

    // Vector with parsed chunks of code
    code_collection_t _codeCollection;

    // Raw code
    vector<unsigned char> _codeBuff;

  public:
    CodeContainer(const vector<unsigned char> &memBuff, size_t startCodeSection, CodeChunk::rva_t virtualAddress);
    ~CodeContainer() {};

    // Get the container in which the code chunks are stored
    inline vector<CodeChunk> get_code_collection() const;

  private:
    // Calculate offset in the memory based on the passed virtual address
    int rva_to_offset(const int &rva);

    // Disassemble code chunk
    CodeChunk disassemble_code_chunk(queue<DISASM>& jumpInstructionQueue);

    // OPTIMIZE: this function is not optimized
    // Checks if there is a code chunk that intersects with the one that is passed to this function
    // and returns iterator to that chunk, otherwise returns iterator to end
    code_collection_t::iterator check_if_intersects(const CodeChunk& codeChunk);

    void merge_code_chunks(CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk);
};

inline vector<CodeChunk> CodeContainer::get_code_collection() const
{
  return _codeCollection;
};

#endif
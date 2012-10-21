#include "..\kraken\ChunkContainer.h"

#include <gmock\gmock.h>

class MockChunkContainer : public ChunkContainer
{
  public:
    MOCK_METHOD3(fill, bool (const vector<unsigned char> &memBuff, size_t startCodeSection, rva_t virtualAddress));
    MOCK_METHOD1(rva_to_offset, int (const int &rva));
    MOCK_METHOD2(disassemble_code_chunk, CodeChunk (queue<AsmCode>& jumpInstructionQueue, const Disassembler& disassemble));
    MOCK_METHOD1(check_if_intersects, code_collection_t::iterator (const CodeChunk& codeChunk));
    MOCK_METHOD3(merge_code_chunks, void (CodeChunk& destination, const CodeChunk& firstCodeChunk, const CodeChunk& secondCodeChunk));
};
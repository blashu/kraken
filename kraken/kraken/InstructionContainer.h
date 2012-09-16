#ifndef __H_INSTRUCTION_CONTAINER__
#define __H_INSTRUCTION_CONTAINER__

#include <vector>
#include <queue>
#include <string>
#include <BeaEngine\BeaEngine.h>

#include "CodeChunk.h"

using namespace std;

// OPTIMIZE: code chunks are stored in vector, which causes extra operations when finding intersections between chunks 
// because we are removing them from vector, combining them in one chunk and re-adding it to vector. This may cause 
// some problems and it may be a good idea to combine them inside an existing code chunk or use list container instead of vector
class InstructionContainer
{
    private:
        typedef vector<CodeChunk> code_collection_t;

        // Vector with parsed chunks of code
        code_collection_t _codeCollection;

        // Raw code
        vector<unsigned char> _codeBuff;
        
    public:
        InstructionContainer(const vector<unsigned char> &memBuff, int* startCodeSection, long long virtualAddress);
        ~InstructionContainer();

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

inline vector<CodeChunk> InstructionContainer::get_code_collection() const
{
    return _codeCollection;
};

#endif
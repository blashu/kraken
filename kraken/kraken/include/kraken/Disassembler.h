#ifndef __H_DISASSEMBLER__
#define __H_DISASSEMBLER__

#include "Decoder.h"
#include "CodeChunkMap.h"

#include <vector>
#include <queue>
#include <string>
#include <map>
#include <functional>

using namespace std;
using namespace kraken;

KRAKEN_API_ class Disassembler
{
  public:
    typedef vector<CodeChunk> chunk_container_t;
    typedef chunk_container_t::const_iterator chunk_container_iter;

    Disassembler() {}

    // Fills chunk container with disassembled chunks of code, while handling
    // unconditional branch instructions and possible chunk intersections
    virtual bool fill(const Decoder& decode);
    
    virtual ~Disassembler() {}

    virtual inline chunk_container_iter begin() const;

    virtual inline chunk_container_iter end() const;

    virtual const CodeChunk& front() const;

    virtual const CodeChunk& back() const;

    virtual void go_through_chunks(std::function<void (const CodeChunk&)> process_instr) const;

    virtual void go_through_instructions(std::function<void (const AsmCode&)> process_instr) const;

    virtual int get_chunk_count() const;

    virtual int get_instruction_count() const;

  private:
    /////////////////////////////////////////
    // fields
    typedef vector<CodeChunk> code_collection_t;

    code_collection_t _codeCollection;

    CodeChunkMap _chunkMap;

    /////////////////////////////////////////
    // functions
    bool is_instruct_decoded( va_t address );

    virtual CodeChunk disassemble_next_code_chunk( queue<va_t>& jumpInstructionQueue, const Decoder& disassemble );

    // OPTIMIZE: this function is not optimized
    // Checks if there is a code chunk that intersects with the one that is passed to this function
    // and returns iterator to that chunk, otherwise returns iterator to end
    virtual code_collection_t::iterator check_if_intersects(const CodeChunk& codeChunk);

    virtual void merge_code_chunks(CodeChunk& resultChunk,
                                   const CodeChunk& firstCodeChunk,
                                   const CodeChunk& secondCodeChunk);
};

inline Disassembler::chunk_container_iter Disassembler::begin() const
{
  return _codeCollection.begin();
}

inline Disassembler::chunk_container_iter Disassembler::end() const
{
  return _codeCollection.end();
}

inline const CodeChunk& Disassembler::front() const
{
  return _codeCollection.front();
}

inline const CodeChunk& Disassembler::back() const
{
  return _codeCollection.back();
}

#endif

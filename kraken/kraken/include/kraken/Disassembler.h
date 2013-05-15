#ifndef __H_DISASSEMBLER__
#define __H_DISASSEMBLER__

#include "Decoder.h"
#include "CodeChunk.h"

#include <vector>
#include <queue>
#include <string>
#include <map>
#include <functional>
#include <boost/ptr_container/ptr_map.hpp>

using namespace std;

namespace kraken
{
  namespace interval
  {
    class ChunkCodeListing;
  }

  KRAKEN_API_ class Disassembler
  {
    public:
      Disassembler() {}

      // Fills chunk container with disassembled chunks of code, while handling
      // unconditional branch instructions and possible chunk intersections
      virtual bool do_work(const Decoder& decode);

      virtual ~Disassembler() {}

      virtual inline const_chunk_container_iter begin() const;

      virtual inline const_chunk_container_iter end() const;

      virtual const CodeChunk& front() const;

      virtual const CodeChunk& back() const;

      virtual void go_through_chunks(std::function<void (const CodeChunk&)> process_instr) const;

      virtual void go_through_instructions(std::function<void (const AsmCode&)> process_instr) const;

      virtual int get_chunk_count() const;

      virtual int get_instruction_count() const;

      friend class kraken::internal::ChunkCodeListing;
    private:
      /////////////////////////////////////////
      // fields
      typedef vector<CodeChunk> code_collection_t;

      code_collection_t _codeCollection;

      boost::ptr_map<va_t, AsmCode> _asmCodeMap;

      /////////////////////////////////////////
      // functions
      bool is_instruct_decoded( va_t address );

      void disassemble_next_jump( const Decoder& disassemble, queue<va_t>& jumpInstructionQueue );

      // OPTIMIZE: this function is not optimized
      // Checks if there is a code chunk that intersects with the one that is passed to this function
      // and returns iterator to that chunk, otherwise returns iterator to end
      code_collection_t::iterator check_if_intersects(const CodeChunk& codeChunk);

      void merge_code_chunks(CodeChunk& resultChunk,
                                     const CodeChunk& firstCodeChunk,
                                     const CodeChunk& secondCodeChunk);

      void fill_code_collection_using_asm_map();
  };

  inline const_chunk_container_iter Disassembler::begin() const
  {
    return _codeCollection.begin();
  }

  inline const_chunk_container_iter Disassembler::end() const
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
}

#endif

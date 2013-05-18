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
  class Decompiler;

  namespace interval
  {
    class ChunkCodeListing;
  }

  KRAKEN_API_ class Disassembler
  {
    public:
      Disassembler() {}

      virtual ~Disassembler() {}

      // Fills chunk container with disassembled chunks of code, while handling
      // unconditional branch instructions and possible chunk intersections
      virtual bool do_work(const Decoder& decode);


      virtual inline const_chunk_container_iter begin() const;

      virtual inline const_chunk_container_iter end() const;

      virtual const CodeChunk& front() const;

      virtual const CodeChunk& back() const;

      virtual void go_through_chunks(std::function<void (const CodeChunk&)> process_instr) const;

      virtual void go_through_instructions(std::function<void (const AsmCode&)> process_instr) const;

      virtual int get_chunk_count() const;

      int get_instruction_count() const;

      friend class kraken::internal::ChunkCodeListing;

      friend class kraken::Decompiler;
    private:
      /////////////////////////////////////////
      // fields
      typedef vector<CodeChunk> code_collection_t;

      code_collection_t _codeCollection;
      boost::ptr_map<va_t, AsmCode> _instructionMap;

      /////////////////////////////////////////
      // functions
      void log_instruction_statics();
      void disassemble_next_jump( const Decoder& disassemble, queue<va_t>& jumpInstructionQueue );
      void fill_code_collection_using_instruction_map();
      bool is_instruct_decoded( va_t address );
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

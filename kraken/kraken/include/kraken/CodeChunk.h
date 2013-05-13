#ifndef __H_CODE_CHUNK__
#define __H_CODE_CHUNK__

#include <vector>
#include <functional>

#include "kraken.h"
#include "AsmCode.h"

using namespace std;

namespace kraken
{
  namespace internal
  {
    class ChunkCodeListing;
  }

  KRAKEN_API_ class CodeChunk
  {
    public:
      CodeChunk(){}

      CodeChunk(const vector<AsmCode> &codeChunk)
      {
        _codeChunk = codeChunk;
      }

      CodeChunk(const CodeChunk &codeChunk)
      {
        _codeChunk = codeChunk._codeChunk;
      }

      bool operator ==( const CodeChunk &otherChunk ) const;

      void go_through_instructions(std::function<void (const AsmCode&)> process_instr) const;

      inline vector<AsmCode>::const_iterator begin() const;

      inline vector<AsmCode>::const_iterator end() const;

      inline const AsmCode& front() const;

      inline const AsmCode& back() const;

      // Get RVA of the first instruction in this code chunk
      inline va_t first_va() const;

      // Get RVA of the last instruction in this code chunk
      inline va_t last_va() const;

      // Checks if passed rva is contained in this code chunk
      inline bool CodeChunk::is_address_included(va_t address) const;

      // Check if the passed code chunk intersects with\ is included in the current code chunk
      inline bool intersects_with(const CodeChunk& codeChunk) const;

      // Check if the passed code chunk is included in the current code chunk
      inline bool includes(const CodeChunk& codeChunk) const;

      // Add instruction to the code chunk
      inline void add_to_chunk(const AsmCode& disasm);

      // Append one code chunk to the other based on iterator to vector
      inline void add_to_chunk(vector<AsmCode>::const_iterator begin, vector<AsmCode>::const_iterator end);

      // Count of chunks.
      inline size_t size() const;

      friend class kraken::internal::ChunkCodeListing;
      private:
        // Instructions, contained in this code chunk
        vector<AsmCode> _codeChunk;
  };

  typedef vector<CodeChunk> chunk_container_t;
  typedef chunk_container_t::const_iterator const_chunk_container_iter;

  inline vector<AsmCode>::const_iterator CodeChunk::begin() const
  {
    return _codeChunk.begin();
  }

  inline vector<AsmCode>::const_iterator CodeChunk::end() const
  {
    return _codeChunk.end();
  }

  inline const AsmCode& CodeChunk::front() const
  {
    return _codeChunk.front();
  }

  inline const AsmCode& CodeChunk::back() const
  {
    return _codeChunk.back();
  }

  inline va_t CodeChunk::first_va() const
  {
    return _codeChunk.empty() ? -1 : _codeChunk.begin()->VirtualAddr;
  }

  inline va_t CodeChunk::last_va() const

  {
    return _codeChunk.empty() ? -1 : _codeChunk.back().VirtualAddr;
  }

  inline bool CodeChunk::is_address_included(va_t address) const
  {
    if( address < 0 )
    {
      return false;
    }

    return ( ( address >= first_va() ) &&
      ( address <= last_va() ) );
  }

  inline bool CodeChunk::intersects_with(const CodeChunk& codeChunk) const
  {
    return is_address_included( codeChunk.first_va() ) || is_address_included( codeChunk.last_va() );
  }

  inline bool CodeChunk::includes(const CodeChunk& codeChunk) const
  {
    return is_address_included( codeChunk.first_va() ) && is_address_included( codeChunk.last_va() );
  }

  inline void CodeChunk::add_to_chunk(const AsmCode& disasm)
  {
    _codeChunk.push_back( disasm );
  }

  inline void CodeChunk::add_to_chunk(vector<AsmCode>::const_iterator begin,
                                      vector<AsmCode>::const_iterator end)
  {
    _codeChunk.insert( _codeChunk.end(), begin, end );
  }

  inline size_t CodeChunk::size() const
  {
    return _codeChunk.size();
  }
}

#endif

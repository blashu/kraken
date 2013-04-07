#ifndef __H_CODE_CHUNK__
#define __H_CODE_CHUNK__

#include "kraken.h"

#include <vector>
#include "AsmCode.h"

using namespace std;
using namespace kraken;

KRAKEN_API_ class CodeChunk
{
  public:
    typedef vector<AsmCode> code_chunk_t;
    typedef code_chunk_t::const_iterator code_chunk_iter;

    CodeChunk(){};

    CodeChunk(const vector<AsmCode> &codeChunk)
    {
      _codeChunk = codeChunk;
    }

    CodeChunk(const CodeChunk &codeChunk)
    {
      _codeChunk = codeChunk._codeChunk;
    }

    bool operator ==( const CodeChunk &otherChunk ) const;

    inline code_chunk_iter begin() const;

    inline code_chunk_iter end() const;

    inline const AsmCode& front() const;

    inline const AsmCode& back() const;

    // Get RVA of the first instruction in this code chunk
    inline rva_t first_rva() const;

    // Get RVA of the last instruction in this code chunk
    inline rva_t last_rva() const;

    // Checks if passed rva is contained in this code chunk
    inline bool CodeChunk::is_address_included(rva_t address) const;

    // Check if the passed code chunk intersects with\ is included in the current code chunk
    inline bool intersects_with(const CodeChunk& codeChunk) const;

    // Check if the passed code chunk is included in the current code chunk
    inline bool includes(const CodeChunk& codeChunk) const;

    // Add instruction to the code chunk
    inline void add_to_chunk(const AsmCode& disasm);

    // Append one code chunk to the other based on iterator to vector
    inline void add_to_chunk(code_chunk_t::const_iterator begin, code_chunk_t::const_iterator end);

    // Cound of chunks.
    inline size_t size() const;

    private:
      // Instructions, contained in this code chunk
      code_chunk_t _codeChunk;
};

inline CodeChunk::code_chunk_iter CodeChunk::begin() const
{
  return _codeChunk.begin();
}

inline CodeChunk::code_chunk_iter CodeChunk::end() const
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

inline rva_t CodeChunk::first_rva() const
{
  return _codeChunk.empty() ? -1 : _codeChunk.begin()->VirtualAddr;
};

inline rva_t CodeChunk::last_rva() const

{
  return _codeChunk.empty() ? -1 : _codeChunk.back().VirtualAddr;
};

inline bool CodeChunk::is_address_included(rva_t address) const
{
  if( address < 0 )
  {
    return false;
  }

  return ( ( address >= first_rva() ) &&
    ( address <= last_rva() ) );
}

inline bool CodeChunk::intersects_with(const CodeChunk& codeChunk) const
{
  return is_address_included( codeChunk.first_rva() ) || is_address_included( codeChunk.last_rva() );
};

inline bool CodeChunk::includes(const CodeChunk& codeChunk) const
{
  return is_address_included( codeChunk.first_rva() ) && is_address_included( codeChunk.last_rva() );
};

inline void CodeChunk::add_to_chunk(const AsmCode& disasm)
{
  _codeChunk.push_back( disasm );
};

inline void CodeChunk::add_to_chunk(code_chunk_t::const_iterator begin, code_chunk_t::const_iterator end)
{
  _codeChunk.insert( _codeChunk.end(), begin, end );
}

inline size_t CodeChunk::size() const
{
  return _codeChunk.size();
}

#endif

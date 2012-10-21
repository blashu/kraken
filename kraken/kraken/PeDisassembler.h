#ifndef __H_PE_DISASSEMBLER__
#define __H_PE_DISASSEMBLER__

#include "PEStructs.h"
#include "Disassembler.h"
#include <string>

KRAKEN_API_ class PeDisassembler : public Disassembler
{
  public:
    PeDisassembler() {};
    PeDisassembler( const std::string &path ) {};

    virtual ~PeDisassembler() {};

    virtual int disassemble(AsmCode *disasmResult) const;

    bool load( const std::string &path );

    const unsigned char* buf();

    template <typename T>
    const T* buf( size_t offset );

    CodeChunk disassemble_code_chunk(rva_t instrAddr);

    int rva_to_offset(rva_t rva);

    rva_t entry_point();

  private:
    /////////////////////////////////////////
    // fields
    std::vector<unsigned char> _fileBuf;

    SECTION_HEADERS _sectionHeaders;

    const IMAGE_NT_HEADERS32* _imageNtHeader32;

    /////////////////////////////////////////
    // functions
    PeDisassembler( const PeDisassembler& ){};

    bool is_it_pe_file();

    bool load_file_in_filebuf( const std::string &path );
  
    bool fill_pe_struct_fields();
};

#endif
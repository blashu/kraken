#ifndef __H_PE_DISASSEMBLER__
#define __H_PE_DISASSEMBLER__

#include <BeaEngine\BeaEngine.h>
#include "PEStructs.h"
#include "Disassembler.h"
#include <string>

KRAKEN_API_ class PeDisassembler : public Disassembler
{
  public:
    PeDisassembler() {}

    virtual ~PeDisassembler() {}

    virtual int disassemble(AsmCode *disasmResult) const;

    // Disassemble code chunk
    virtual CodeChunk disassemble_code_chunk(rva_t instrAddr) const;

    // Returns the entry point of the contained file
    virtual rva_t entry_point() const;

    // Calculate offset in the memory based on the passed virtual address
    offset_t rva_to_offset(rva_t rva) const;

    // Load specified file
    bool load(const std::string &path);
        
  private:
    /////////////////////////////////////////
    // fields
    std::vector<unsigned char> _fileBuf;

    SECTION_HEADERS _sectionHeaders;

    const kraken::IMAGE_NT_HEADERS32* _imageNtHeader32;

    /////////////////////////////////////////
    // functions
    PeDisassembler(const PeDisassembler&){}

    template <typename T>
    const T* buf(size_t offset) const;

    const unsigned char* buf() const;

    bool is_it_pe_file();

    bool load_file_in_filebuf(const std::string &path);
  
    bool fill_pe_struct_fields();

    Argument convert_argument(const ARGTYPE &sourceArg) const;

    void convert_instruction(const INSTRTYPE &source, Instruction &destination);
};

#endif

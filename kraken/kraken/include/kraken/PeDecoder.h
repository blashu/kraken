#ifndef __H_PE_DECODER__
#define __H_PE_DECODER__

#include <BeaEngine/BeaEngine.h>
#include "internal/PEStructs.h"
#include "Decoder.h"

#include <string>
#include <vector>
#include <map>

KRAKEN_API_ class PeDecoder : public Decoder
{
  public:
    PeDecoder();
    virtual ~PeDecoder() {}

    /*!
     * Loads binary file and prepares one to decode internal instructions.
     *
     * \param path Path to file.
     * \return true if decoding is success otherwise false.
     */
    bool load(const std::string &path);

    /*!
     * Decode instruction by virtual address.
     *
     * \param[in]  virtualAddr  Virtual address of decoding instruction.
     * \param[out] asmCode      Decoded instruction.
     * \return true if decoding is success otherwise false.
     */
    virtual bool decode(va_t instrVirtualAddr, AsmCode *asmCode) const;

     /*!
     * Returns the entry point of binary file.
     *
     * \return Entry point.
     */
    virtual va_t entry_point() const;
        
    private:
    /////////////////////////////////////////
    // fields
    bool _isAlreadyLoaded;

    bool _resultOfFirstLoad;

    std::vector<unsigned char> _fileBuf;

    SECTION_HEADERS _sectionHeaders;

    const IMAGE_NT_HEADERS32* _imageNtHeader32;

    std::map<va_t, std::string> _funcNameMap;

    /////////////////////////////////////////
    // functions
    PeDecoder(const PeDecoder&){}

    void try_replace_func_name_by_import_name(AsmCode *asmCode) const;

    rva_t rva_to_va(rva_t rva) const;

    offset_t va_to_offset(va_t va) const;

    offset_t rva_to_offset(rva_t rva) const;

    template <typename T>
    const T* buf(size_t offset) const;

    const unsigned char* buf() const;

    bool is_it_pe_file();

    bool load_file_in_filebuf(const std::string &path);
  
    bool fill_pe_struct_fields();

    void parse_import_data_table();

    static void convert_argument(const ARGTYPE &source, Argument &destination);

    static void convert_instruction(const INSTRTYPE &source, InstrType &destination);
};

#endif

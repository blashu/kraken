#ifndef __H_PE_DECODER__
#define __H_PE_DECODER__

#include <string>
#include <vector>

#include <BeaEngine/BeaEngine.h>
#include "internal/PEStructs.h"
#include "Decoder.h"

KRAKEN_API_ class PeDecoder : public Decoder
{
  public:
    PeDecoder();
    virtual ~PeDecoder() {}

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

    /*!
     * Loads binary file and prepares one to decode internal instructions.
     *
     * \param path Path to file.
     * \return true if decoding is success otherwise false.
     */
    bool load(const std::string &path);
        
  private:
    /////////////////////////////////////////
    // fields
    bool _isAlreadyLoaded;

    bool _resultOfFirstLoad;

    std::vector<unsigned char> _fileBuf;

    SECTION_HEADERS _sectionHeaders;

    const IMAGE_NT_HEADERS32* _imageNtHeader32;

    /////////////////////////////////////////
    // functions
    PeDecoder(const PeDecoder&){}

    offset_t va_to_offset(va_t va) const;

    template <typename T>
    const T* buf(size_t offset) const;

    const unsigned char* buf() const;

    bool is_it_pe_file();

    bool load_file_in_filebuf(const std::string &path);
  
    bool fill_pe_struct_fields();

    static void convert_argument(const ARGTYPE &source, Argument &destination);

    static void convert_instruction(const INSTRTYPE &source, InstrType &destination);
};

#endif

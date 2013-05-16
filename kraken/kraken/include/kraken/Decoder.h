#ifndef __H_DECODER__
#define __H_DECODER__

#include "kraken.h"
#include "AsmCode.h"

using namespace kraken;

KRAKEN_API_ class Decoder
{
  public:
    /*!
    * Decode instruction by virtual address.
    *
    * \param[in]  virtualAddr  Virtual address of decoding instruction.
    * \param[out] asmCode      Decoded instruction.
    * \return true if decoding is success otherwise false.
    */
    virtual bool decode(va_t instrVirtualAddr, AsmCode *asmCode) const = 0;

    /*!
    * Returns the entry point of binary file.
    *
    * \return Entry point.
    */
    virtual va_t entry_point() const = 0;
};

#endif

#ifndef __H_KRAKEN__
#define __H_KRAKEN__

#ifdef _MSC_VER

#if KRAKEN_CREATE_SHARED_LIBRARY
  #define KRAKEN_API_ __declspec(dllexport)
#elif KRAKEN_LINKED_AS_SHARED_LIBRARY
  #define KRAKEN_API_ __declspec(dllimport)
#else
  #define KRAKEN_API_
#endif

namespace kraken
{
  // Virtual addres of instruction in loaded image.
  typedef long long va_t;

  // Relative virtual address. Equals va minus image base.
  typedef long long rva_t;

  // Type contains instruction offset in file.
  // For dll and exe can be calculate as RVA - IMAGE_SECTION_HEADER.VirtualAddress + IMAGE_SECTION_HEADER.PointerToRawData
  typedef long long offset_t;
}

#endif  // _MSC_VER

#endif

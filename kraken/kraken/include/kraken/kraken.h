#ifdef _MSC_VER

#if KRAKEN_CREATE_SHARED_LIBRARY
  #define KRAKEN_API_ __declspec(dllexport)
#elif KRAKEN_LINKED_AS_SHARED_LIBRARY
  #define KRAKEN_API_ __declspec(dllimport)
#else
  #define KRAKEN_API_
#endif

#endif  // _MSC_VER
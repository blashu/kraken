#ifdef _MSC_VER

# if KRAKEN_CREATE_SHARED_LIBRARY
#  define KRAKEN_API_ __declspec(dllexport)
# else
#  define KRAKEN_API_ __declspec(dllimport)
#endif // KRAKEN_CREATE_SHARED_LIBRARY

#endif  // _MSC_VER
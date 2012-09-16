#ifndef __H_PE_FILE_LOADER__
#define __H_PE_FILE_LOADER__

#pragma once

#include <string>
#include <vector>

namespace kraken
{
  class PEFileLoader
  {
    public:
      bool load( const std::string &path );

      const unsigned char* buf();

      template <typename T>
      const T* buf( size_t offset );

    private:
      std::vector<unsigned char> _fileBuf;


      bool is_it_pe_file();

      bool load_file_in_filebuf( const std::string &path );
  };
};

#endif
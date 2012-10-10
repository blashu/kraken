#include "StdAfx.h"

#include "PEFileLoader.h"
#include "PEStructs.h"

#include <fstream>

using namespace std;
namespace fs = boost::filesystem;

namespace kraken
{
  ////////////////////////////////////////////////////
  // public members
  bool PEFileLoader::load( const std::string &path )
  {
    if( false == load_file_in_filebuf( path ) )
    {
      _fileBuf = vector<unsigned char>();
      return false;
    }

    if( false == is_it_pe_file() )
    {
      _fileBuf = vector<unsigned char>();
      return false;
    }

    return true;
  }

  const unsigned char* PEFileLoader::buf()
  {
    return ( _fileBuf.empty() ? ( (const unsigned char*) NULL ) : &(_fileBuf[ 0 ]) );
  }

  template <typename T>
  const T* PEFileLoader::buf( size_t offset )
  {
    return ( offset >= _fileBuf.size() ? (const T*) NULL : (const T*) &(_fileBuf[ offset ]) );
  }


  ////////////////////////////////////////////////////
  // private members
  bool PEFileLoader::is_it_pe_file()
  {
    const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );

    if( NULL == dosHeader )
    {
      cout << "File too small. Can't read dos header." << endl;
      return false;
    }

    if( IMAGE_DOS_SIGNATURE != dosHeader->e_magic )
    {
      cout << "File isn't PE file. It hasn't got IMAGE_DOS_SIGNATURE." << endl;
      return false;
    }

    const IMAGE_NT_HEADERS32 *imageNtHeader32;

    if( NULL == ( imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( dosHeader->e_lfanew ) ) )
    {
      cout << "File too small. Can't read image NT headers 32." << endl;
      return false;
    }

    if( IMAGE_NT_SIGNATURE != imageNtHeader32->Signature )
    {
      cout << "File isn't PE file. It hasn't got IMAGE_NT_SIGNATURE." << endl;
      return false;
    }

    return true;
  }

  bool PEFileLoader::load_file_in_filebuf( const std::string &fileName )
  {
    fs::path filePath( fileName );

    if( false == fs::exists( filePath ) )
    {
      cout << fileName << " doesn't exist." << endl;
      return false;
    }

    if( false == fs::is_regular_file( filePath ) )
    {
      cout << fileName << " isn't file." << endl;
      return false;
    }

    if( true == fs::is_empty( filePath ) )
    {
      cout << fileName << " is empty." << endl;
    }

    ifstream fileStream( fileName, ios::binary | ios::in );
    boost::uintmax_t fileSize = fs::file_size( filePath );
    typedef std::istream_iterator<unsigned char> istream_iterator;

    _fileBuf = vector<unsigned char>();
    _fileBuf.reserve( fileSize );

    fileStream >> std::noskipws;
    copy( istream_iterator(fileStream),
          istream_iterator(),
			    std::back_inserter(_fileBuf));

    return true;
  }
}
#include "include/kraken/PeDecoder.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <cstdio>

using namespace std;
namespace fs = boost::filesystem;

PeDecoder::PeDecoder()
{
  _isAlreadyLoaded = false;
  _resultOfFirstLoad = false;
}

////////////////////////////////////////////////////
// public members
bool PeDecoder::load( const std::string &path )
{
  if( _isAlreadyLoaded )
  {
    return _resultOfFirstLoad;
  }
  _resultOfFirstLoad = false;

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

  if( false == fill_pe_struct_fields() )
  {
    return false;
  }

  _resultOfFirstLoad = true;
  return true;
}

bool PeDecoder::decode(va_t virtualAddr, AsmCode *asmCode) const
{
  DISASM beaEngineCode;

  beaEngineCode.EIP = (size_t)( buf() + va_to_offset( virtualAddr ) );
  beaEngineCode.VirtualAddr = virtualAddr;
  beaEngineCode.Archi = 0;

  int length = Disasm( &beaEngineCode );
  if( length <= 0 )
  {
    return false;
  }

  asmCode->length = length;

  assert( sizeof( asmCode->CompleteInstr ) == sizeof( beaEngineCode.CompleteInstr ) );
  memcpy( asmCode->CompleteInstr, beaEngineCode.CompleteInstr, sizeof( asmCode->CompleteInstr ) );

  asmCode->Eip = beaEngineCode.EIP;
  asmCode->VirtualAddr = beaEngineCode.VirtualAddr;
  asmCode->Archi = beaEngineCode.Archi;

  convert_instruction( beaEngineCode.Instruction, asmCode->Instruction );

  convert_argument( beaEngineCode.Argument1, asmCode->Argument1 );
  convert_argument( beaEngineCode.Argument2, asmCode->Argument2 );
  convert_argument( beaEngineCode.Argument3, asmCode->Argument3 );

  try_replace_func_name_by_import_name(asmCode);

  return true;
}

va_t PeDecoder::entry_point() const
{
  return _imageNtHeader32->OptionalHeader.AddressOfEntryPoint + _imageNtHeader32->OptionalHeader.ImageBase;
}

////////////////////////////////////////////////////
// private members
void PeDecoder::try_replace_func_name_by_import_name(AsmCode *asmCode) const
{
  static const char instrStr[] = "call ";
  static const int maxFuncName = INSTRUCT_LENGTH - sizeof( instrStr );

  if( BranchType::CallType == asmCode->Instruction.BranchType )
  {
    if( kraken::MEMORY_TYPE == asmCode->Argument1.ArgType &&
        kraken::ST_DSReg == asmCode->Argument1.SegmentReg &&
        0x00 == asmCode->Argument1.Memory.BaseRegister ) // not sure is last condition really necessary but as is
    {
      auto funcNameIt = _funcNameMap.find( asmCode->Argument1.Memory.Displacement );
      if( _funcNameMap.end() != funcNameIt )
      {

        string funcName = funcNameIt->second;
        int funcNameLength = ( funcName.size() > maxFuncName ) ? maxFuncName : funcName.size();

        char* destination = asmCode->CompleteInstr;
        memcpy( destination, instrStr, sizeof( instrStr ) - 1 );
        destination += sizeof( instrStr ) - 1;

        memcpy( destination, funcName.c_str(), funcNameLength );
        destination += funcNameLength;

        destination[0] = '\0';
      }
    }
  }
}

const unsigned char* PeDecoder::buf() const
{
  return ( _fileBuf.empty() ? ( (const unsigned char*) NULL ) : &(_fileBuf[ 0 ]) );
}

//TODO validate that fileBuf contains the last byte of T type (check sizeof(T) ).
template <typename T>
const T* PeDecoder::buf( size_t offset ) const
{
  return ( offset >= _fileBuf.size() ? (const T*) NULL : (const T*) &(_fileBuf[ offset ]) );
}

bool PeDecoder::is_it_pe_file()
{
  const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );

  if( NULL == dosHeader )
  {
    BOOST_LOG_TRIVIAL(info) << "File too small. Can't read dos header." << endl;
    return false;
  }

  if( IMAGE_DOS_SIGNATURE != dosHeader->e_magic )
  {
    BOOST_LOG_TRIVIAL(info) << "File isn't PE file. It hasn't got IMAGE_DOS_SIGNATURE." << endl;
    return false;
  }

  const IMAGE_NT_HEADERS32 *imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( dosHeader->e_lfanew );
  if( NULL == imageNtHeader32 )
  {
    BOOST_LOG_TRIVIAL(info) << "File too small. Can't read image NT headers 32." << endl;
    return false;
  }

  if( IMAGE_NT_SIGNATURE != imageNtHeader32->Signature )
  {
    BOOST_LOG_TRIVIAL(info) << "File isn't PE file. It hasn't got IMAGE_NT_SIGNATURE." << endl;
    return false;
  }

  return true;
}

bool PeDecoder::fill_pe_struct_fields()
{
  const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );
  if( NULL == dosHeader )
  {
    return false;
  }
  size_t ntHeaderOffset = dosHeader->e_lfanew;

  _imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( ntHeaderOffset );
  if( NULL == _imageNtHeader32 )
  {
    return false;
  }
  size_t sectionHeadersOffset = sizeof( IMAGE_NT_HEADERS32 ) + ntHeaderOffset;

  _sectionHeaders.SectionHeaders = buf<IMAGE_SECTION_HEADER>( sectionHeadersOffset );
  if( NULL == _sectionHeaders.SectionHeaders )
  {
    return false;
  }
  _sectionHeaders.NumberOfSections = _imageNtHeader32->FileHeader.NumberOfSections;

  parse_import_data_table();

  return true;
}

void PeDecoder::parse_import_data_table()
{
  if( IMAGE_DIRECTORY_ENTRY_IMPORT >= _imageNtHeader32->OptionalHeader.NumberOfRvaAndSizes )
  {
    BOOST_LOG_TRIVIAL(info) << "Import directory isn't found.";
    return;
  }
  BOOST_LOG_TRIVIAL(info) << "Import directory is found.";

  auto importDataDirectory = _imageNtHeader32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

  // search section contains image import directory
  for( int i = 0; i < _sectionHeaders.NumberOfSections; ++i )
  {
    auto sectionHeader = _sectionHeaders.SectionHeaders[ i ];

    size_t startVirtAddr = sectionHeader.VirtualAddress;
    size_t endVirtAddr = startVirtAddr + sectionHeader.SizeOfRawData;

    if( startVirtAddr <= importDataDirectory.VirtualAddress && importDataDirectory.VirtualAddress < endVirtAddr )
    {
      if( 0x00 == sectionHeader.SizeOfRawData )
      {
        return;
      }

      IMAGE_IMPORT_DESCRIPTOR zeroImageImportDescriptor = {0};
      IMAGE_THUNK_DATA zeroImageThunkData = {0};

      for( const IMAGE_IMPORT_DESCRIPTOR* imageImportDescriptor = buf<IMAGE_IMPORT_DESCRIPTOR>( sectionHeader.PointerToRawData );
           0x00 != memcmp( &zeroImageImportDescriptor, imageImportDescriptor, sizeof( IMAGE_IMPORT_DESCRIPTOR ) );
           imageImportDescriptor += 1 )
      {
        const char* name = buf<char>( rva_to_offset( imageImportDescriptor->Name ) );
        BOOST_LOG_TRIVIAL(info) << "imported dll is found: " << name;

        va_t vaOfCurrentFunctionThunk = rva_to_va( imageImportDescriptor->FirstThunk );
        offset_t offsetOfFirstFunctionThunk = rva_to_offset( imageImportDescriptor->FirstThunk );
        for( const IMAGE_THUNK_DATA* imageThunkData = buf<IMAGE_THUNK_DATA>( offsetOfFirstFunctionThunk );
             0x00 != memcmp( &zeroImageThunkData, imageThunkData, sizeof( IMAGE_THUNK_DATA ) );
             imageThunkData += 1 )
        {
          const IMAGE_IMPORT_BY_NAME* imageImportByName = buf<IMAGE_IMPORT_BY_NAME>( rva_to_offset( imageThunkData->u1.Function ) );

          _funcNameMap[ vaOfCurrentFunctionThunk ] = imageImportByName->Name;

          vaOfCurrentFunctionThunk += sizeof( IMAGE_THUNK_DATA );
        }
      }

      break;
    }
  }

  BOOST_LOG_TRIVIAL(info) << "Count of imported functions: " << _funcNameMap.size();
}

bool PeDecoder::load_file_in_filebuf( const std::string &fileName )
{
  fs::path filePath( fileName );

  if( false == fs::exists( filePath ) )
  {
    BOOST_LOG_TRIVIAL(info) << fileName << " doesn't exist." << endl;
    return false;
  }

  if( false == fs::is_regular_file( filePath ) )
  {
    BOOST_LOG_TRIVIAL(info) << fileName << " isn't file." << endl;
    return false;
  }

  if( true == fs::is_empty( filePath ) )
  {
    BOOST_LOG_TRIVIAL(info) << fileName << " is empty." << endl;
  }

  ifstream fileStream( fileName, ios::binary | ios::in );
  boost::uintmax_t fileSize = fs::file_size( filePath );
  typedef std::istream_iterator<unsigned char> istream_iterator;

  _fileBuf = vector<unsigned char>();
  _fileBuf.reserve( fileSize );

  fileStream >> std::noskipws;
  copy( istream_iterator( fileStream ),
        istream_iterator(),
        std::back_inserter( _fileBuf ) );

  return true;
}

rva_t PeDecoder::rva_to_va(rva_t rva) const
{
  return rva + _imageNtHeader32->OptionalHeader.ImageBase;
}

offset_t PeDecoder::va_to_offset(va_t va) const
{
  rva_t rva = va - _imageNtHeader32->OptionalHeader.ImageBase;

  for( int i = 0; i < _sectionHeaders.NumberOfSections; ++i )
  {
    size_t startVirtAddr = _sectionHeaders.SectionHeaders[ i ].VirtualAddress;
    size_t endVirtAddr = startVirtAddr + _sectionHeaders.SectionHeaders[ i ].SizeOfRawData;

    if( startVirtAddr <= rva && rva < endVirtAddr )
    {
      return rva - startVirtAddr + _sectionHeaders.SectionHeaders[ i ].PointerToRawData;
    }
  }

  return -1;
}

offset_t PeDecoder::rva_to_offset(rva_t rva) const
{
  return va_to_offset( rva_to_va( rva ) );
}

void PeDecoder::convert_instruction(const INSTRTYPE &source, InstrType &destination)
{
  destination.AddrValue = source.AddrValue;
  destination.BranchType = (BranchType)source.BranchType;
  destination.Opcode = source.Opcode;

  assert( sizeof( source.Mnemonic ) == sizeof( destination.Mnemonic ) );
  memcpy( destination.Mnemonic, source.Mnemonic, sizeof( destination.Mnemonic ) );
}

void PeDecoder::convert_argument(const ARGTYPE &sourceArg, Argument &destination)
{
  destination.AccessMode              = sourceArg.AccessMode;
  destination.ArgPosition             = sourceArg.ArgPosition;
  destination.ArgSize                 = sourceArg.ArgSize;
  destination.ArgType                 = (ArgumentType)sourceArg.ArgType;
  destination.SegmentReg              = (SegType)sourceArg.SegmentReg;
  memcpy( destination.ArgMnemonic, sourceArg.ArgMnemonic, sizeof( sourceArg.ArgMnemonic ) );

  destination.Memory.BaseRegister     = sourceArg.Memory.BaseRegister;
  destination.Memory.Displacement     = sourceArg.Memory.Displacement;
  destination.Memory.IndexRegister    = sourceArg.Memory.IndexRegister;
  destination.Memory.Scale            = sourceArg.Memory.Scale;
}

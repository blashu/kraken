#include "include/kraken/PeDecoder.h"

#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

PeDecoder::PeDecoder()
{
  _isAlreadyLoaded = false;
}

int PeDecoder::decode(va_t instrVirtualAddr, AsmCode *asmCode) const
{
  DISASM beaEngineCode;

  beaEngineCode.EIP = (size_t)( buf() + va_to_offset( instrVirtualAddr ) );
  beaEngineCode.VirtualAddr = instrVirtualAddr;
  beaEngineCode.Archi = 0;

  int length = Disasm( &beaEngineCode );

  assert( sizeof( asmCode->CompleteInstr ) == sizeof( beaEngineCode.CompleteInstr ) );
  memcpy( asmCode->CompleteInstr, beaEngineCode.CompleteInstr, sizeof( asmCode->CompleteInstr ) );

  asmCode->Eip = beaEngineCode.EIP;
  asmCode->VirtualAddr = beaEngineCode.VirtualAddr;
  asmCode->Archi = beaEngineCode.Archi;

  asmCode->Instruction.AddrValue = beaEngineCode.Instruction.AddrValue;
  asmCode->Instruction.BranchType = (BranchType)beaEngineCode.Instruction.BranchType;

  assert( sizeof( asmCode->Instruction.Mnemonic ) == sizeof( beaEngineCode.Instruction.Mnemonic ) );
  memcpy( asmCode->Instruction.Mnemonic, beaEngineCode.Instruction.Mnemonic, sizeof( asmCode->Instruction.Mnemonic ) );

  asmCode->Argument1 = convert_argument( beaEngineCode.Argument1 );
  asmCode->Argument2 = convert_argument( beaEngineCode.Argument2 );
  asmCode->Argument3 = convert_argument( beaEngineCode.Argument3 );

  return length;
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

////////////////////////////////////////////////////
// private members
bool PeDecoder::is_it_pe_file()
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

bool PeDecoder::fill_pe_struct_fields()
{
  const IMAGE_DOS_HEADER *dosHeader = buf<IMAGE_DOS_HEADER>( 0 );
  size_t ntHeaderOffset = dosHeader->e_lfanew;

  _imageNtHeader32 = buf<IMAGE_NT_HEADERS32>( ntHeaderOffset );
  size_t sectionHeadersOffset = sizeof( IMAGE_NT_HEADERS32 ) + ntHeaderOffset;

  _sectionHeaders.SectionHeaders = buf<IMAGE_SECTION_HEADER>( sectionHeadersOffset );

  if( NULL == _sectionHeaders.SectionHeaders )
  {
    return false;
  }

  _sectionHeaders.NumberOfSections = _imageNtHeader32->FileHeader.NumberOfSections;

  return true;
}

bool PeDecoder::load_file_in_filebuf( const std::string &fileName )
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
  copy( istream_iterator( fileStream ),
        istream_iterator(),
        std::back_inserter( _fileBuf ) );

  return true;
}

va_t PeDecoder::entry_point() const
{
  return _imageNtHeader32->OptionalHeader.AddressOfEntryPoint + _imageNtHeader32->OptionalHeader.ImageBase;
}

offset_t PeDecoder::va_to_offset(va_t virtAddr) const
{
  virtAddr -= _imageNtHeader32->OptionalHeader.ImageBase;

  for( int i = 0; i < _sectionHeaders.NumberOfSections; ++i )
  {
    size_t startVirtAddr = _sectionHeaders.SectionHeaders[ i ].VirtualAddress;
    size_t endVirtAddr = startVirtAddr + _sectionHeaders.SectionHeaders[ i ].SizeOfRawData;

    if( startVirtAddr <= virtAddr && virtAddr < endVirtAddr )
    {
      return virtAddr - startVirtAddr + _sectionHeaders.SectionHeaders[ i ].PointerToRawData;
    }
  }

  return -1;
}

void PeDecoder::convert_instruction(const INSTRTYPE &source, InstrType &destination)
{
  destination.AddrValue = source.AddrValue;
  destination.BranchType = (BranchType)source.BranchType;
}

Argument PeDecoder::convert_argument(const ARGTYPE &sourceArg) const
{
  Argument convertedArg;
  convertedArg.AccessMode = sourceArg.AccessMode;
  memcpy( convertedArg.ArgMnemonic, sourceArg.ArgMnemonic, sizeof( sourceArg.ArgMnemonic ) );
  convertedArg.ArgPosition = sourceArg.ArgPosition;
  convertedArg.ArgSize = sourceArg.ArgSize;
  convertedArg.ArgType = (ArgumentType)sourceArg.ArgType;
  convertedArg.SegmentReg = sourceArg.SegmentReg;
  convertedArg.Memory.BaseRegister = sourceArg.Memory.BaseRegister;
  convertedArg.Memory.Displacement = sourceArg.Memory.BaseRegister;
  convertedArg.Memory.IndexRegister = sourceArg.Memory.IndexRegister;
  convertedArg.Memory.Scale = sourceArg.Memory.Scale;
  return convertedArg;
}

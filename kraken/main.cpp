#include "Settings.h"

#include <kraken.h>

#include <PeDecoder.h>
#include <Disassembler.h>

#include <iostream>
#include <iomanip>
#include <set>

using namespace kraken;

std::string complete2short_instr( const char* completeInstruction )
{
  const char* whiteCharPtr;

  if( NULL == ( whiteCharPtr = strchr( completeInstruction, ' ' ) ) )
  {
    return completeInstruction;
  }

  return std::string( completeInstruction, whiteCharPtr );
}

void show_all_instr(const PeDecoder& peDecoder, const Disassembler& disassem)
{
  disassem.go_through_instructions([&peDecoder](const AsmCode& asmCode){
    offset_t offset = peDecoder.rva_to_offset( asmCode.VirtualAddr );

    cout << "0x" << std::setw(8) << std::hex << std::setfill('0')
         << offset << "\t" << asmCode.CompleteInstr << endl;
  });
}

void show_instr_set(const PeDecoder& peDecoder, const Disassembler& disassem)
{
  std::set<std::string> instructionList;
  cout << "Exe file contains next list of instruction:" << endl;

  disassem.go_through_instructions([&peDecoder, &instructionList](const AsmCode& asmCode){
    instructionList.insert( complete2short_instr( asmCode.CompleteInstr ) );
  });

  for( auto it = instructionList.begin(), end = instructionList.end(); it != end; ++it )
  {
    cout << *it << endl;
  }
  cout << instructionList.size() << endl;
}

int main( int argc, const char** argv )
{
  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  PeDecoder peDecoder;
  cout << "Loading file \"" << settings.path_to_bin() << "\"..." << endl;
  if( false == peDecoder.load( settings.path_to_bin() ) )
  {
    return EXIT_FAILURE;
  }

  cout << settings.path_to_bin() << " is successfully loaded." << endl;

  Disassembler disassem( peDecoder );

  if( Settings::ActionType::SHOW_ALL_INSTR == settings.action() )
  {
    show_all_instr(peDecoder, disassem);
  }

  if( Settings::ActionType::SHOW_INSTR_SET == settings.action() )
  {
    show_instr_set(peDecoder, disassem);
  }

  return EXIT_SUCCESS;
}

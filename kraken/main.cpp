#include "Settings.h"

#include <kraken.h>

#include <PeDisassembler.h>
#include <Disassembler.h>

#include <iostream>
#include <set>
#include <vector>

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

int main( int argc, const char** argv )
{
  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  PeDisassembler peDisassembler;
  cout << "Loading file \"" << settings.path_to_bin() << "\"..." << endl;
  if( false == peDisassembler.load( settings.path_to_bin() ) )
  {
    return EXIT_FAILURE;
  }

  cout << settings.path_to_bin() << " is successfully loaded." << endl;

  std::set<std::string> instructionList;
  Disassembler disassem( peDisassembler );

  for(auto chunkIt = disassem.begin(), end = disassem.end(); chunkIt != end; ++chunkIt )
  {
    for(vector<AsmCode>::const_iterator instrIt = chunkIt->begin(), endInstrIt = chunkIt->end();
        instrIt != endInstrIt;
        ++instrIt)
    {
      instructionList.insert( complete2short_instr( instrIt->CompleteInstr ) );
    }
  }

  int count = 0;
  cout << "Exe file contains next list of instruction:" << endl;
  for( auto it = instructionList.begin(), end = instructionList.end(); it != end; ++it )
  {
    cout << *it << endl;
    count++;
  }
  cout << count << endl;

  return EXIT_SUCCESS;
}

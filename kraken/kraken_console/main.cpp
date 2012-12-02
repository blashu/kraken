#include "Settings.h"

#include <kraken.h>
#include <iostream>

#include <PeDisassembler.h>
#include <ChunkContainer.h>

using namespace kraken;

int main( int argc, const char** argv )
{
  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  PeDisassembler peDisassembler;
  cout << "Loading file \"" << settings.path_to_bin() << "\"..." << endl;
  if( peDisassembler.load( settings.path_to_bin() ) )
  {
    cout << settings.path_to_bin() << " is successfully loaded." << endl;
  }

  // auto codeChunk = peDisassembler.disassemble_code_chunk( peDisassembler.entry_point() );

  ChunkContainer chunkContainer(peDisassembler);

  for( auto begin = chunkContainer.begin(), end = chunkContainer.end(); begin != end; ++begin )
  {
    for ( auto beginChunk = begin->begin(), endChunk = begin->end(); beginChunk != endChunk; ++beginChunk)
    {
      (void) printf("%.8X %s\n",(int) beginChunk->VirtualAddr, &beginChunk->CompleteInstr);
    }
  }

  return EXIT_SUCCESS;
}

#include "Settings.h"
#include "PEFileLoader.h"

#include <kraken.h>
#include <iostream>

#include <PeDisassembler.h>


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

  auto codeChunk = peDisassembler.disassemble_code_chunk( peDisassembler.entry_point() );
  for( auto begin = codeChunk.begin(), end = codeChunk.end(); begin != end; ++begin )
  {
    (void) printf("%.8X %s\n",(int) begin->VirtualAddr, 
                                    &begin->CompleteInstr);
  }

  return EXIT_SUCCESS;
}
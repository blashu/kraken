
#include "Settings.h"
#include "PEFileLoader.h"

#include <kraken.h>
#include <iostream>

using namespace kraken;

int main( int argc, const char** argv )
{
  Settings settings( argc, argv );

  if( false == settings.run() )
  {
    return EXIT_FAILURE;
  }

  PEFileLoader fileLoader;
  cout << "Loading file \"" << settings.path_to_bin() << "\"..." << endl;
  if( fileLoader.load( settings.path_to_bin() ) )
  {
    cout << settings.path_to_bin() << " is successfully loaded." << endl;
  }

  return EXIT_SUCCESS;
}

/*! @file
*  This is an example of the PIN tool that demonstrates some basic PIN APIs 
*  and could serve as the starting point for developing your first PIN tool
*/

#include "pin.H"
#include <iostream>
#include <fstream>
#include <iomanip>

#include <set>

/* ================================================================== */
// Global variables 
/* ================================================================== */

UINT64 insCount = 0;        //number of dynamically executed instructions
UINT64 bblCount = 0;        //number of dynamically executed basic blocks
UINT64 threadCount = 0;     //total number of threads, including main thread

std::set<UINT32> headInstrOffset;

std::ostream * out = &cerr;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
*  Print out help message.
*/
INT32 Usage()
{
  cerr << "This tool prints out the number of dynamically executed " << endl <<
    "instructions, basic blocks and threads in the application." << endl << endl;

  cerr << KNOB_BASE::StringKnobSummary() << endl;

  return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

UINT32 GetOffsetInMainExecutableByAddrint( ADDRINT addr )
{
  UINT32 offset = 0;

  PIN_LockClient();
  IMG img = IMG_FindByAddress( addr );

  if( IMG_Valid( img ) &&
      IMG_IsMainExecutable( img ) )
  {
    offset = addr - IMG_StartAddress( img );
  }
  
  PIN_UnlockClient();
  return offset;
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/*!
* Insert call to the CountBbl() analysis routine before every basic block 
* of the trace.
* This function is called every time a new trace is encountered.
* @param[in]   trace    trace to be instrumented
* @param[in]   v        value specified by the tool in the TRACE_AddInstrumentFunction
*                       function call
*/
VOID Trace(TRACE trace, VOID *v)
{
  // Visit every basic block in the trace
  for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
  {
    INS headIns = BBL_InsHead(bbl);
    UINT32 headRva = GetOffsetInMainExecutableByAddrint( INS_Address( headIns ) );

    if( ( 0x00 != headRva ) &&
        ( headInstrOffset.find( headRva ) == headInstrOffset.end() ) ) 
    {
      bblCount++;
      insCount += BBL_NumIns(bbl);

      headInstrOffset.insert( headRva );

      INS tailIns = BBL_InsTail( bbl );
      UINT32 tailRva = GetOffsetInMainExecutableByAddrint( INS_Address( tailIns ) );

      *out << StringFromAddrint( headRva ) << " - "
           << StringFromAddrint( tailRva ) << std::endl;
    }
  }
}

/*!
* Print out analysis results.
* This function is called when the application exits.
* @param[in]   code            exit code of the application
* @param[in]   v               value specified by the tool in the 
*                              PIN_AddFiniFunction function call
*/
VOID Fini(INT32 code, VOID *v)
{
  *out <<  "===============================================" << endl;
  *out <<  "TraceBin analysis results: " << endl;
  *out <<  "Number of instructions: " << insCount  << endl;
  *out <<  "Number of basic blocks: " << bblCount  << endl;
  *out <<  "===============================================" << endl;
}

/*!
* The main procedure of the tool.
* This function is called when the application image is loaded but not yet started.
* @param[in]   argc            total number of elements in the argv array
* @param[in]   argv            array of command line arguments, 
*                              including pin -t <toolname> -- ...
*/
int main(int argc, char *argv[])
{
  // Initialize PIN library. Print help message if -h(elp) is specified
  // in the command line or the command line is invalid 
  if( PIN_Init(argc,argv) )
  {
    return Usage();
  }

  // Register function to be called to instrument traces
  TRACE_AddInstrumentFunction(Trace, 0);

  // Register function to be called when the application exits
  PIN_AddFiniFunction(Fini, 0);

  // Start the program, never returns
  PIN_StartProgram();

  return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */

#include "include/kraken/Decompiler.h"

#include <boost/log/trivial.hpp>

const char* kraken::DecompPhaseName( DecompPhases dp )
{
  switch( dp )
  {
    break; case DP_ERROR:
      return "Error while decompiling";

    break; case DP_TARGET_FILE_IS_NOT_SET:
      return "Target file isn't set (can't continue decompile)";

    break; case DP_LOAD_BINARY:
      return "Load binary data";

    break; case DP_DISASSEMBLE:
      return "Disassemble";

    break; case DP_RUNTIME_TRACE:
      return "Runtime trace";

    break; case DP_GENERATE_SSA:
      return "Generate SSA";

    break; case DP_END:
      return "End";

    default:
      return "Unknown phase";
  }
}

kraken::Decompiler::Decompiler()
{
  _nextPhase = DP_TARGET_FILE_IS_NOT_SET;
  _isRuntimeTraceEnable = false;
  _decoder = NULL;
}

Decompiler::~Decompiler()
{
  if( NULL != _decoder )
  {
    delete _decoder;
  }
}

void Decompiler::set_target(const string& path)
{
  _pathToTarget = path;
  _nextPhase = DP_LOAD_BINARY;
}

void kraken::Decompiler::enable_runtime_trace_phase()
{
  _isRuntimeTraceEnable = true;
}

void kraken::Decompiler::disable_runtime_trace_phase()
{
  _isRuntimeTraceEnable = false;
}

kraken::DecompPhases kraken::Decompiler::decompile()
{
  DecompPhases currentPhase;
  do
  {
    currentPhase = decompile_next_phase();
  } while ( ( currentPhase != DP_END ) && ( currentPhase != DP_ERROR ) );

  return currentPhase;
}


kraken::DecompPhases kraken::Decompiler::decompile_next_phase()
{
  BOOST_LOG_TRIVIAL(info) << "==== " << "Start decomipe phase: "
                          << DecompPhaseName(_nextPhase) << " ====";

  switch( _nextPhase )
  {
    break; case DP_TARGET_FILE_IS_NOT_SET:
    {
      BOOST_LOG_TRIVIAL(warning) << "Target file to decompile wasn't set.";
      return DP_TARGET_FILE_IS_NOT_SET;
    }

    break; case DP_LOAD_BINARY:
    {
      PeDecoder* peDecoder = new PeDecoder();
      if( peDecoder->load( _pathToTarget ) )
      {
        BOOST_LOG_TRIVIAL(info) << "File is sucessfully loaded.";
        _decoder = peDecoder;
      }
      else
      {
        BOOST_LOG_TRIVIAL(error) << "Can't load target file.";
        _nextPhase = DP_ERROR;
        delete peDecoder;
      }
    }

    break; case DP_DISASSEMBLE:
    {
      if( _disassem.fill(*_decoder) )
      {
        BOOST_LOG_TRIVIAL(info) << "File is sucessfully disassembled.";
      }
      else
      {
        BOOST_LOG_TRIVIAL(info) << "Can't disassemble file.";
        _nextPhase = DP_ERROR;
      }
    }

    break; case DP_RUNTIME_TRACE:
    {
      if( false == _isRuntimeTraceEnable )
      {
        BOOST_LOG_TRIVIAL(info) << "Runtime trace phase is disabled.";
        _nextPhase = static_cast<DecompPhases>( _nextPhase + 1 );
        return decompile_next_phase();
      }
    }

    break; case DP_GENERATE_SSA:


    break; case DP_END:
      return DP_END;

    default:
      //TODO: log about unknown command
      BOOST_LOG_TRIVIAL(error) << "Unknown phase occured while decompiling.";
  }
  _nextPhase = static_cast<DecompPhases>( _nextPhase + 1 );
  return static_cast<DecompPhases>( _nextPhase - 1 );
}

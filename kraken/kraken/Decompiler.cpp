#include "include/kraken/Decompiler.h"
#include "ChunkCodeListing.h"

using namespace kraken;
using namespace kraken::internal;

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

Decompiler::Decompiler()
{
  _nextPhase = DP_TARGET_FILE_IS_NOT_SET;
  _isRuntimeTraceEnable = false;
  _disassembler = std::make_shared<Disassembler>(Disassembler());
}

Decompiler::~Decompiler()
{}

void Decompiler::set_target(const string& path)
{
  _pathToTarget = path;
  _nextPhase = DP_LOAD_BINARY;
}

void Decompiler::enable_runtime_trace_phase()
{
  _isRuntimeTraceEnable = true;
}

void Decompiler::disable_runtime_trace_phase()
{
  _isRuntimeTraceEnable = false;
}

DecompPhases Decompiler::decompile()
{
  DecompPhases currentPhase;
  do
  {
    currentPhase = decompile_next_phase();
  } while ( ( currentPhase != DP_END ) && ( currentPhase != DP_ERROR ) );

  return currentPhase;
}


DecompPhases Decompiler::decompile_next_phase()
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
        _decoder = std::shared_ptr<Decoder>( peDecoder );
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
      _disassembler = std::make_shared<Disassembler>(Disassembler());

      if( _disassembler->do_work(*_decoder) )
      {
        BOOST_LOG_TRIVIAL(info) << "File is sucessfully disassembled.";
      }
      else
      {
        _disassembler.reset();
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

CodeListingInterface* Decompiler::low_level_listing()
{
  if( _lowLevelListing )
  {
    return &(*_lowLevelListing);
  }

  if( false == _disassembler )
  {
    return NULL;
  }

  _lowLevelListing = std::shared_ptr<CodeListingInterface>(reinterpret_cast<CodeListingInterface*>( new ChunkCodeListing(_disassembler)) );
  return &(*_lowLevelListing);
}

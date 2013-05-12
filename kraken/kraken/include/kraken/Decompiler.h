#ifndef __H_DECOMPILER__
#define __H_DECOMPILER__

#include <string>
#include <kraken/PeDecoder.h>
#include <kraken/Disassembler.h>

#include "kraken.h"

namespace kraken {
  /*!
  * Pahses of decompilation.
  */
  enum DecompPhases
  {
    DP_ERROR = -1,

    DP_TARGET_FILE_IS_NOT_SET = 0,
    DP_LOAD_BINARY,
    DP_DISASSEMBLE,
    DP_RUNTIME_TRACE,
    DP_GENERATE_SSA,
    DP_END
  };

  const char* DecompPhaseName( DecompPhases dp );

  // Class contain decompile methods that can be run step by step or all at once.
  KRAKEN_API_ class Decompiler
  {
    public:
      Decompiler();
      ~Decompiler();

      void set_target(const std::string &path);

      void enable_runtime_trace_phase();
      void disable_runtime_trace_phase();

      kraken::DecompPhases decompile();

      /*!
       * Decompile next phase.
       *
       * \return Processesed phase.
       */
      DecompPhases decompile_next_phase();

    private:
      std::string _pathToTarget;
      Decoder* _decoder;
      Disassembler _disassem;

      bool _isRuntimeTraceEnable;
      DecompPhases _nextPhase;


  };
}

#endif

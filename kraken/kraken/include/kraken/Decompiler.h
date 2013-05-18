#ifndef __H_DECOMPILER__
#define __H_DECOMPILER__

//TODO After fix issue #3: replace pedecoder with decoder and move pedecoder to cpp
#include "PeDecoder.h"
#include "Disassembler.h"
#include "CodeListingInterface.h"
#include "SSABuilder.h"

#include <string>
#include <memory>

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

      CodeListingInterface* low_level_listing();

    private:
      std::string _pathToTarget;

      std::shared_ptr<Decoder> _decoder;

      std::shared_ptr<Disassembler> _disassembler;
      std::shared_ptr<CodeListingInterface> _lowLevelListing;

      std::shared_ptr<SSABuilder> _ssaBuilder;

      bool _isRuntimeTraceEnable;
      DecompPhases _nextPhase;
  };
}

#endif

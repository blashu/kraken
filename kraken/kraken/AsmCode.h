#ifndef __H_ASM_CODE__
#define __H_ASM_CODE__

#include <string.h>
#include "BasicTypes.h"

namespace kraken
{
  #define INSTRUCT_LENGTH 64

  typedef long long rva_t;

  enum SPECIAL_INFO
  {
    UNKNOWN_OPCODE = -1,
    OUT_OF_BLOCK = 0,

    /* === mask = 0xff */
    NoTabulation      = 0x00000000,
    Tabulation        = 0x00000001,

    /* === mask = 0xff00 */
    MasmSyntax        = 0x00000000,
    GoAsmSyntax       = 0x00000100,
    NasmSyntax        = 0x00000200,
    ATSyntax          = 0x00000400,

    /* === mask = 0xff0000 */
    PrefixedNumeral   = 0x00010000,
    SuffixedNumeral   = 0x00000000,

    /* === mask = 0xff000000 */
    ShowSegmentRegs   = 0x01000000
  };

  enum BranchType
  {
    Undefined = 0,
    JO = 1,
    JC,
    JE,
    JA,
    JS,
    JP,
    JL,
    JG,
    JB,
    JECXZ,
    JmpType,
    CallType,
    RetType,
    JNO = -1,
    JNC = -2,
    JNE = -3,
    JNA = -4,
    JNS = -5,
    JNP = -6,
    JNL = -7,
    JNG = -8,
    JNB = -9
  };

  enum ArgumentType
  {
    NO_ARGUMENT = 0x10000000,
    REGISTER_TYPE = 0x20000000,
    MEMORY_TYPE = 0x40000000,
    CONSTANT_TYPE = (int)0x80000000,

    MMX_REG = 0x10000,
    GENERAL_REG = 0x20000,
    FPU_REG = 0x40000,
    SSE_REG = 0x80000,
    CR_REG = 0x100000,
    DR_REG = 0x200000,
    SPECIAL_REG = 0x400000,
    MEMORY_MANAGEMENT_REG = 0x800000,
    SEGMENT_REG = 0x1000000,

    RELATIVE_ = 0x4000000,
    ABSOLUTE_ = 0x8000000,

    READ = 0x1,
    WRITE = 0x2,

    REG0 = 0x1,
    REG1 = 0x2,
    REG2 = 0x4,
    REG3 = 0x8,
    REG4 = 0x10,
    REG5 = 0x20,
    REG6 = 0x40,
    REG7 = 0x80,
    REG8 = 0x100,
    REG9 = 0x200,
    REG10 = 0x400,
    REG11 = 0x800,
    REG12 = 0x1000,
    REG13 = 0x2000,
    REG14 = 0x4000,
    REG15 = 0x8000
  };

  struct MemoryType
  {
    Int32 BaseRegister;
    Int32 IndexRegister;
    Int32 Scale;
    Int64 Displacement;
  };

  struct Instruction
  {
    rva_t AddrValue;
    BranchType BranchType;
  };

  struct Argument
  {
    char ArgMnemonic[32];
    ArgumentType ArgType;
    Int32 ArgSize;
    Int32 ArgPosition;   
    UInt32 AccessMode;
    MemoryType Memory;
    UInt32 SegmentReg;
  };

  struct AsmCode
  {
    rva_t VirtualAddr;
    size_t Eip;
    unsigned int Archi;
    Instruction Instruction;
    Argument Argument1;
    Argument Argument2;
    Argument Argument3;
    char CompleteInstr[INSTRUCT_LENGTH];

    AsmCode()
    {
      VirtualAddr = 0;
      Eip = 0;
      Archi = 0;
      CompleteInstr[0] = '\0';
      Instruction.AddrValue = 0;
      Instruction.BranchType = Undefined;
    }

    // TODO change comparation principle so that it would be based on start rva, Instruction and size of the AsmCode
    inline bool operator == (const AsmCode& otherAsmCode) const
    {
      return this->VirtualAddr == otherAsmCode.VirtualAddr;
    }
  };
}

#endif
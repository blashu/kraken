#ifndef __H_ASM_CODE__
#define __H_ASM_CODE__

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

  struct Instruction
  {
    rva_t AddrValue;

    BranchType BranchType;
  };

  struct AsmCode
  {
    rva_t VirtualAddr;
    size_t Eip;
    Instruction Instruction;
    unsigned int Archi;
    char CompleteInstr[INSTRUCT_LENGTH];
  };
}

#endif
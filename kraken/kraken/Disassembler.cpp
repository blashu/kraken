#include "Disassembler.h"

#include <BeaEngine\BeaEngine.h>

using namespace kraken;

int Disassembler::disassemble(AsmCode *disasmResult) const
{
  DISASM disasm;

  disasm.EIP = disasmResult->Eip;
  disasm.VirtualAddr = disasmResult->VirtualAddr;
  
  int length = Disasm( &disasm );

  memcpy( disasmResult->CompleteInstr, disasm.CompleteInstr, sizeof( disasmResult->CompleteInstr ) );
  disasmResult->Eip = disasm.EIP;
  disasmResult->VirtualAddr = disasm.VirtualAddr;
  disasmResult->Instruction.AddrValue = disasm.Instruction.AddrValue;
  disasmResult->Instruction.BranchType = (BranchType)disasm.Instruction.BranchType;
  disasmResult->Archi = disasm.Archi;

  return length;
}
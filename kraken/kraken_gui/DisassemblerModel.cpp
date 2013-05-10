#include <QSet>

#include "DisassemblerModel.h"

DisassemblerModel::DisassemblerModel(QString pathToFile)
{
  _peDecoder.load( pathToFile.toUtf8().constData() );
  _disassembler.fill( _peDecoder );

  setStringList(getProgramListing());
}

QStringList DisassemblerModel::getProgramListing()
{
  QStringList appListing;

  int count = 0;

  PeDecoder *peDecoder = &_peDecoder; // hack to make lambda expression work
  _disassembler.go_through_chunks([peDecoder, &appListing, &count]( const CodeChunk& chunk ){

    appListing << QString( "Chunk #%1" ).arg( count );

    chunk.go_through_instructions( [peDecoder, &appListing]( const AsmCode& asmCode ){
      offset_t offset = peDecoder->rva_to_offset( asmCode.VirtualAddr );

      appListing << QString( "0x%1\t%2" ).arg( offset, 8, 16, QChar( '0' ) ).arg( asmCode.CompleteInstr );
    });

    appListing << QString( "" );

    count++;
  });

  return appListing;
}

QStringList DisassemblerModel::getUsedInstructionsList()
{
  QSet<QString> instructionSet;

  _disassembler.go_through_instructions([&instructionSet](const AsmCode& asmCode){
    instructionSet.insert( asmCode.Instruction.Mnemonic );
  });

  QStringList usedInstructionList = QStringList::fromSet(instructionSet);

  usedInstructionList.sort();

  return usedInstructionList;
}

void DisassemblerModel::showProgramListing()
{
  setStringList(getProgramListing());
}

void DisassemblerModel::showUsedInstructions()
{
  setStringList(getUsedInstructionsList());
}

int DisassemblerModel::getChunkCount()
{
  return _disassembler.get_chunk_count();
}

int DisassemblerModel::getInstructionCount()
{
  return _disassembler.get_instruction_count();
}

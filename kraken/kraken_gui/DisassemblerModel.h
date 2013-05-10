#ifndef __H_DISASSEMBLER_MODEL__
#define __H_DISASSEMBLER_MODEL__

#include <QString>
#include <QStringListModel>

#include <kraken/Disassembler.h>
#include <kraken/PeDecoder.h>

class DisassemblerModel : public QStringListModel
{
  private:
    Disassembler _disassembler;
    PeDecoder _peDecoder;

    QStringList getProgramListing();
    QStringList getUsedInstructionsList();
    QString complete2short_instr( const char* completeInstruction );
    DisassemblerModel() {}

  public:
    DisassemblerModel( QString string );

    void showProgramListing();
    void showUsedInstructions();

    int getChunkCount();
    int getInstructionCount();
};

#endif

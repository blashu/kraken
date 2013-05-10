#ifndef __H_DISASSEMBLER_MODEL__
#define __H_DISASSEMBLER_MODEL__

#include <QString>
#include <QVariant>
#include <QStringListModel>

#include <kraken/PeDecoder.h>
#include <kraken/Disassembler.h>

class DisassemblerModel : public QStringListModel
{
  public:
    DisassemblerModel( QString string );

    void showProgramListing();
    void showUsedInstructions();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int getChunkCount();
    int getInstructionCount();

  private:
    Disassembler _disassembler;
    PeDecoder _peDecoder;

    QStringList getProgramListing();
    QStringList getUsedInstructionsList();
    DisassemblerModel() {}
};

#endif

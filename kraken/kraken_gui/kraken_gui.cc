#include <QApplication>

#include <kraken/PeDecoder.h>
#include <kraken/Disassembler.h>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
//    PeDecoder peDecoder;
//    peDecoder.load("simple_exe/simple_exe.exe");

//    Disassembler disassembler(peDecoder);

  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}

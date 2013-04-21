#include <QtGui/QGuiApplication>
#include <PeDecoder.h>
#include <Disassembler.h>

#include "MainWindow.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
//    PeDecoder peDecoder;
//    peDecoder.load("simple_exe/simple_exe.exe");

//    Disassembler disassembler(peDecoder);

    QGuiApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.showExpanded();

    return app.exec();
}

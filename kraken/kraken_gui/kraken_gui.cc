#include <QApplication>
#include <QLabel>
#include <PeDisassembler.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PeDisassembler peDisassembler;

    peDisassembler.load("simple_exe/simple_exe.exe");



    QLabel hello("Hello world!");
    hello.show();

    return app.exec();
}

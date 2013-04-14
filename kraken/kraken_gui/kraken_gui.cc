#include <QApplication>
#include <QLabel>
#include <PeDecoder.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PeDecoder peDecoder;

    peDecoder.load("simple_exe/simple_exe.exe");

    QLabel hello("Hello world!");
    hello.show();

    return app.exec();
}

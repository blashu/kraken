#include <QFileDialog>

#include <kraken/PeDecoder.h>
#include <kraken/Disassembler.h>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLoad, SIGNAL(triggered()), SLOT(loadFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile()
{
    PeDecoder peDecoder;
    peDecoder.load("simple_exe/simple_exe.exe");
    Disassembler disassembler(peDecoder);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load executable"), "", tr("Executable files (*.exe)"));

    ui->plainTextEdit->appendPlainText(fileName);
}

#include "mainwindow.h"

#include <kraken/PeDecoder.h>
#include <kraken/Disassembler.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

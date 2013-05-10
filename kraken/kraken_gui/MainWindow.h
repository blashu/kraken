#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>

#include "DisassemblerModel.h"
#include "ui_mainwindow.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    Ui::MainWindow *_ui;
    DisassemblerModel *_model;

  private slots:
    void showUsedInstructions();
    void showProgramListing();
    void loadFile();
};

#endif // MAINWINDOW_H

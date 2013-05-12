#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QItemSelectionModel>

#include "CodeModel.h"
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
    CodeModel *_codeModel;
    QItemSelectionModel *_selectionModel;

  private slots:
    void showProgramListing();
    void showItemIndexes();
    void loadFile();
};

#endif // MAINWINDOW_H

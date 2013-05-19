#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QItemSelectionModel>
#include "FindVaDialog.h"

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
    Ui::MainWindow* _ui;
    CodeModel* _codeModel;
    QItemSelectionModel* _selectionModel;
    FindVaDialog* _findVaDialog;

  private slots:
    void showProgramListing();
    void loadFile();
    void showCodeItemContextMenu(QPoint);
    void showFindVaDialog();
    void findVa(const QString&);
};

#endif // MAINWINDOW_H

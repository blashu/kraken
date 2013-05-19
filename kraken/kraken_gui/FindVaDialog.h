#ifndef FINDVADIALOG_H
#define FINDVADIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>

class FindVaDialog : public QDialog
{
    Q_OBJECT

  public:
    FindVaDialog(QWidget *parent = 0);
    QString getFindText();

  public slots:
    void findClicked();

  signals:
    void findVa(const QString&);

  private:
    QPushButton *_findButton;
    QLineEdit *_lineEdit;
};

#endif // FINDVADIALOG_H

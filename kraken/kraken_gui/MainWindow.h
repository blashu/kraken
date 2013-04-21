#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtQuick/QQuickView>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QQuickView
{
    Q_OBJECT

    public:
        explicit MainWindow(QWindow *parent = 0);
        ~MainWindow();

        void addImportPath(const QString &path);

        void showExpanded();

    private:
};

#endif // MAINWINDOW_H

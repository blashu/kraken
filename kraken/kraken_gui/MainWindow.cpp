#include "MainWindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>


MainWindow::MainWindow(QWindow *parent)
    : QQuickView(parent)
{
    setSource(QUrl("qrc:/main.qml"));
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QQuickView::SizeRootObjectToView);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showExpanded()
{
    show();
}

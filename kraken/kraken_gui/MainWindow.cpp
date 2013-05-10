#include <QFileDialog>

#include "DisassemblerModel.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _model = NULL;

    // making statistics block look neat
    _ui->statisticsText->viewport()->setAutoFillBackground( false );
    _ui->statisticsText->setBackgroundRole( QPalette::Window );
    _ui->statisticsText->viewport()->setMouseTracking( true );
    _ui->statisticsText->viewport()->setCursor( Qt::ArrowCursor );

    connect( _ui->actionLoad, SIGNAL( triggered() ), SLOT( loadFile() ) );
    connect( _ui->actionShowProgramListing, SIGNAL( triggered() ), SLOT( showProgramListing() ) );
    connect( _ui->actionShowUsedInstructions, SIGNAL( triggered() ), SLOT( showUsedInstructions() ) );
}

MainWindow::~MainWindow()
{
  delete _model;
  delete _ui;
}

void MainWindow::loadFile()
{
    QString pathToFile = QFileDialog::getOpenFileName( this, tr( "Load executable" ), "", tr( "Executable files (*.exe)" ) );

    if ( _model != NULL )
    {
      _ui->listView->setModel( NULL );
      delete _model;
    }

    _model = new DisassemblerModel( pathToFile );
    _ui->listView->setModel( _model );

    _ui->statisticsText->clear();
    _ui->statisticsText->appendPlainText( QString( "Chunk count: %1\nInstruction count: %2" ).arg( _model->getChunkCount() ).arg( _model->getInstructionCount() ) );

    _ui->actionShowProgramListing->setEnabled( true );
    _ui->actionShowUsedInstructions->setEnabled( true );

    //_ui->plainTextEdit->appendPlainText( appListing );
}

void MainWindow::showProgramListing()
{
  _model->showProgramListing();
}

void MainWindow::showUsedInstructions()
{
  _model->showUsedInstructions();
}

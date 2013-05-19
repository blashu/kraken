#include "mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow)
{
  _ui->setupUi( this );

  _codeModel = NULL;
  _findVaDialog = new FindVaDialog( this );

  _ui->codeView->setContextMenuPolicy( Qt::CustomContextMenu );

  _ui->actionFind_va->setDisabled( true );

  // making statistics block look neat
  _ui->statisticsText->viewport()->setAutoFillBackground( false );
  _ui->statisticsText->setBackgroundRole( QPalette::Window );
  _ui->statisticsText->viewport()->setMouseTracking( true );
  _ui->statisticsText->viewport()->setCursor( Qt::ArrowCursor );

  connect( _ui->codeView, SIGNAL( customContextMenuRequested( QPoint ) ), SLOT( showCodeItemContextMenu( QPoint ) ) );
  connect( _ui->actionLoad, SIGNAL( triggered() ), SLOT( loadFile() ) );
  connect( _ui->actionShowProgramListing, SIGNAL( triggered() ), SLOT( showProgramListing() ) );
  connect( _ui->actionFind_va, SIGNAL( triggered() ), SLOT( showFindVaDialog() ) );
  connect( _findVaDialog, SIGNAL( findVa( const QString& ) ), SLOT( findVa( const QString& ) ) );
}

MainWindow::~MainWindow()
{
  delete _ui;
}

void MainWindow::loadFile()
{
  QString pathToFile = QFileDialog::getOpenFileName( this, tr( "Load executable" ), "", tr( "Executable files (*.exe)" ) );

  if ( pathToFile == NULL  )
  {
    return;
  }

  if ( _codeModel != NULL )
  {
    _ui->codeView->setModel( NULL );

    delete _selectionModel;
    delete _codeModel;
  }

  _ui->actionFind_va->setEnabled( true );

  _codeModel = new CodeModel( pathToFile, this );
  _selectionModel = new QItemSelectionModel( _codeModel, this );

  _ui->codeView->setModel( _codeModel );
  _ui->codeView->setSelectionModel( _selectionModel );

  _ui->statisticsText->clear();
  _ui->statisticsText->appendPlainText( QString( "Chunk count: %1\nInstruction count: %2" ).arg( _codeModel->getCodeBlockCount() ).arg( _codeModel->getCodeBlockItemCount() ) );

  _ui->actionShowProgramListing->setEnabled( true );
}

void MainWindow::showProgramListing()
{
  _codeModel->showProgramListing();
}

void MainWindow::showCodeItemContextMenu(QPoint pos)
{
  QMenu myMenu;
  auto goToFirstReferenceAction = myMenu.addAction( tr( "Go to first reference" ) );

  if ( !_codeModel->isCodeItemBranchType( _ui->codeView->currentIndex() ) )
  {
    goToFirstReferenceAction->setDisabled( true );
  }
  else
  {
    goToFirstReferenceAction->setEnabled( true );
  }

  QAction* selectedAction = myMenu.exec( _ui->codeView->mapToGlobal( pos ) );

  if ( goToFirstReferenceAction == selectedAction )
  {
    auto firstReferencedCodeItem = _codeModel->getFirstReferencedCodeItemIndex( _selectionModel->currentIndex() );
    _selectionModel->setCurrentIndex( firstReferencedCodeItem, QItemSelectionModel::ClearAndSelect );
  }
}

void MainWindow::showFindVaDialog()
{
  _findVaDialog->show();
}

void MainWindow::findVa(const QString& va)
{
  auto itemIndex = _codeModel->getCodeItemByVa( va );
  _selectionModel->setCurrentIndex( itemIndex, QItemSelectionModel::ClearAndSelect );
}

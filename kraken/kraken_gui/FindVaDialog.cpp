#include "FindVaDialog.h"

FindVaDialog::FindVaDialog(QWidget *parent) :
  QDialog(parent)
{
  QLabel* findLabel = new QLabel( tr( "Enter virtual address:" ), this );

  _lineEdit = new QLineEdit( this );
  _lineEdit->setText( tr( "0x00400000" ) );

  _findButton = new QPushButton( tr( "&Find" ), this );

  //connect( _findButton, SIGNAL( clicked() ), SLOT( accept() ) );
  connect( _findButton, SIGNAL( clicked() ), SLOT( findClicked() ) );

  QHBoxLayout *layout = new QHBoxLayout( this );
  layout->addWidget( findLabel );
  layout->addWidget( _lineEdit );
  layout->addWidget( _findButton );

  setLayout( layout );
  setWindowTitle( tr( "Find Virtual Address" ) );
}


void FindVaDialog::findClicked()
{
  QString text = _lineEdit->text();

  if ( false == text.isEmpty() )
  {
    emit findVa( text );
  }
}

#include "ErrorDialog.h"
#include "ui_ErrorDialog.h"

ErrorDialog::ErrorDialog(const char* reason, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ErrorDialog)
{
	ui->setupUi(this);
	ui->label_2->setText(reason);
}

ErrorDialog::~ErrorDialog()
{
	delete ui;
}

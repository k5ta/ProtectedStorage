#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
	class ErrorDialog;
}

class ErrorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ErrorDialog(const char* reason, QWidget *parent = nullptr);
	~ErrorDialog();

private slots:

private:
	Ui::ErrorDialog *ui;
};

#endif

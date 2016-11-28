#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

#include <QMainWindow>
#include "ProtectedStorage.h"
#include "HelpDialog.h"

namespace Ui {
	class StorageWindow;
}

class StorageWindow : public QMainWindow
{
	Q_OBJECT

public:
	StorageWindow(const char* path, QWidget* parent = nullptr);
	~StorageWindow();

private slots:
	void howToUse();

	void about();

	void error(const char* reason);

	void rootFileClicked();

	void mountFileClicked();

	void createClicked();

	void destroyClicked();

	void showPassword();

	void dontShowPassword();

	void closeEvent(QCloseEvent* event) override;


private:
	Ui::StorageWindow *ui;

	ProtectedStorage* storage = nullptr;

	std::string appPath;

	HelpDialog * howToDialog, *aboutDialog;

	void setupMenu();

	void setupConnections();

	void setupHelp();

	bool createStorage();

	bool destroyStorage();
};

#endif // STORAGEWINDOW_H

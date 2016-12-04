#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

#include "ProtectedStorage.h"
#include "HelpDialog.h"
#include <QMainWindow>
#include <memory>

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
	std::unique_ptr<Ui::StorageWindow> ui;

	std::unique_ptr<ProtectedStorage> storage = nullptr;

	std::string appPath;

	std::unique_ptr<HelpDialog> howToDialog, aboutDialog;

	void setupMenu();

	void setupConnections();

	void setupHelp();

	void setupButtons(bool createAvailable);

	bool checkField();

	bool createStorage();

	bool destroyStorage();
};

#endif // STORAGEWINDOW_H

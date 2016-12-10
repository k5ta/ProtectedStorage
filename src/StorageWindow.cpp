#include "StorageWindow.h"
#include "ui_StorageWindow.h"
#include <QFileDialog>


StorageWindow::StorageWindow(const char* path, QWidget* parent) :
	QMainWindow(parent),
	ui(std::make_unique<Ui::StorageWindow>()),
	storage(ProtectedStorage::getInstance()),
	appPath(path)
{
	ui->setupUi(this);
	this->setupMenu();
	this->setupConnections();
	this->setupBoxes();
	this->setupButtons(true);
	ui->pushButton_2->setDisabled(true);
}


void StorageWindow::setupMenu() {
	QAction* createAction = new QAction("Create storage", this);
	connect(createAction, SIGNAL(triggered()), this, SLOT(createClicked()));
	QAction* destroyAction = new QAction("Destroy storage", this);
	connect(destroyAction, SIGNAL(triggered()), this, SLOT(destroyClicked()));

	QMenu* menu = new QMenu("Menu", this);
	menu->addActions( { createAction, destroyAction } );

	QAction* howToAction = new QAction("How to use", this);
	connect(howToAction, SIGNAL(triggered()), this, SLOT(howToUse()));
	QAction* aboutAction = new QAction("About", this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	QMenu* help = new QMenu("Help", this);
	help->addActions( { howToAction, aboutAction} );

	this->menuBar()->addMenu(menu);
	this->menuBar()->addMenu(help);
}

void StorageWindow::setupConnections() {
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(createClicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(destroyClicked()));
	connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(rootFileClicked()));
	connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(mountFileClicked()));
	connect(ui->pushButton_3, SIGNAL(pressed()), this, SLOT(showPassword()));
	connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(dontShowPassword()));
}


void StorageWindow::setupBoxes() {
	this->howToBox.setWindowTitle("How to use");
	this->howToBox.setText("At first, fill in all the fields:\n"
			"* Root directory - where you want to store\nyour encrypted data.\n"
			"* Mount directory - where you want to work with it.\n"
			"* Password - need to encrypt and decrypt data. "
			"It's length should be at least 8 characters.\n"
			"Then, push the button \"Create storage\" to create it.\n\n"
			"To destroy the storage, push the button \"Destroy storage\".\n\n"
			"Remember: you can't create more than one storage at the same time.\n"
			"If an error occurs, the program will report about it and offer some solutions.");
	this->howToBox.setModal(false);

	this->aboutBox.setWindowTitle("About");
	this->aboutBox.setText("Protected storage 0.97 RC\n\n"
			"Created on C++ using FUSE library and Qt\n"
			"Provides the functionality of creating and using protected storage with any data.\n"
			"The mounted directory is similar to an regular logical disk.\n"
			"\nWish you will find the using of this\n"
			"program convenient and useful.\n\n2016, Tarasov Kirill");
	this->aboutBox.setModal(false);
}


void StorageWindow::setupButtons(bool createAvailable) {
	ui->pushButton->setEnabled(createAvailable);
	ui->toolButton->setEnabled(createAvailable);
	ui->toolButton_2->setEnabled(createAvailable);
	ui->lineEdit->setEnabled(createAvailable);
	ui->lineEdit_2->setEnabled(createAvailable);
	ui->lineEdit_3->setEnabled(createAvailable);

	ui->pushButton_2->setEnabled(!createAvailable);
}


StorageWindow::~StorageWindow() {
}


void StorageWindow::closeEvent(QCloseEvent* event) {
	this->howToBox.close();
	this->aboutBox.close();
	if (storage.get()->isMounted())
		while(!this->destroyStorage()) {};
	QMainWindow::closeEvent(event);
}


bool StorageWindow::createStorage() {
	auto root = ui->lineEdit->text().toStdString();
	if (root.empty()) {
		this->error("Root directory field is empty.\nPlease, fill in all the fields.");
		return false;
	}
	auto mount = ui->lineEdit_2->text().toStdString();
	if (mount.empty()) {
		this->error("Mount directory field is empty.\nPlease, fill in all the fields.");
		return false;
	}
	auto pass = ui->lineEdit_3->text().toStdString();
	if (pass.empty()) {
		this->error("Password field is empty.\nPlease, fill in all the fields.");
		return false;
	}
	if (pass.length() < 8) {
		this->error("Wrong password length.\nIt should be at least 8 characters.");
		return false;
	}

	storageDataArray data;
	data.size = 5;
	auto arrPtr = std::make_unique<char*[]>(data.size);
	arrPtr[0] = const_cast<char*>(appPath.c_str());
	arrPtr[1] = const_cast<char*>("-f");
	arrPtr[2] = const_cast<char*>(root.c_str());
	arrPtr[3] = const_cast<char*>(mount.c_str());
	arrPtr[4] = const_cast<char*>(pass.c_str());
	data.arr = arrPtr.get();

	auto a = storage->createStorage(data);

	if (a == storageCreateStatus::alreadyCreated) {
		this->error("Protected storage is already created.\nDestroy it and then create a new.");
		return false;
	}
	if (a == storageCreateStatus::errorInCreating) {
		this->error("Something went wrong when creating\nthe storage."
					"\nCheck the fields of root and\nmount dirictories.");
		return false;
	}

	return true;
}


bool StorageWindow::destroyStorage() {
	auto status = storage->destroyStorage();
	if (status == storageDestroyStatus::nothingToDestroy) {
		this->error("There is no storage to destroy.");
		return false;
	}
	if (status == storageDestroyStatus::errorInDestroying) {
		this->error("Can't destroy storage.\nPlease, close all files in it.");
		return false;
	}
	return true;
}


void StorageWindow::howToUse() {
	howToBox.show();
}


void StorageWindow::about() {
	aboutBox.show();
}


void StorageWindow::error(const char* reason) {
	QMessageBox::critical(this, "ERROR", reason);
	//mb.setWindowTitle("ERROR");
	//mb.setText(reason);
	//mb.setModal(true);
	//mb.exec();
}

void StorageWindow::rootFileClicked() {
	QFileDialog qfd;
	qfd.setFileMode(QFileDialog::FileMode::DirectoryOnly);
	auto dir = qfd.getExistingDirectory();
	if (dir.length())
		ui->lineEdit->setText(dir);
}


void StorageWindow::mountFileClicked() {
	QFileDialog qfd;
	qfd.setFileMode(QFileDialog::FileMode::DirectoryOnly);
	auto dir = qfd.getExistingDirectory();
	if (dir.length())
		ui->lineEdit_2->setText(dir);
}


void StorageWindow::createClicked() {
	if (this->createStorage()) {
		this->setupButtons(false);
	}
}


void StorageWindow::destroyClicked() {
	if (this->destroyStorage()) {
		this->setupButtons(true);
	}
}


void StorageWindow::showPassword() {
	ui->lineEdit_3->setEchoMode(QLineEdit::EchoMode::Normal);
}


void StorageWindow::dontShowPassword() {
	ui->lineEdit_3->setEchoMode(QLineEdit::EchoMode::Password);
}

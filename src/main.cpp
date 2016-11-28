#include "StorageWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StorageWindow w(argv[0]);
	w.show();
	return a.exec();
}

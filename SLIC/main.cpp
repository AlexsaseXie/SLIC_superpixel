#include "SLIC.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SLIC w;
	w.show();
	return a.exec();
}

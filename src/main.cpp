#include <iostream>

#include <QtGui/QApplication>

#include <GL/freeglut.h>

#include "window.h"

using std::cout;
using std::endl;

int main(int argc, char*argv[]) {
	glutInit(&argc, argv);

	QApplication app(argc, argv);

	int result = 0;
	{
		window win;
		win.showMaximized();
		result = app.exec();
	}

	return result;
}

#include <iostream>

#include <QtGui/QApplication>

#include <GL/freeglut.h>

#include "window.h"
#include "obj.h"

using std::cout;
using std::endl;

int main(int argc, char*argv[]) {
	if(argc == 0) {
		cout << "usage: demo <file.obj>" << endl;
		return 1;
	}

	glutInit(&argc, argv);

	QApplication app(argc, argv);

	int result = 0;
	{
		obj object(argv[1]);

		window win;
		win.setObject(object);
		win.showMaximized();
		result = app.exec();
	}

	return result;
}

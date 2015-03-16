#include <iostream>

#include <QtGui/QApplication>

#include <GL/freeglut.h>

#include "window.h"
#include "obj.h"
#include "grid.h"

using std::cout;
using std::endl;

int main(int argc, char*argv[]) {
	if(argc == 1) {
		cout << "usage: demo <file.obj>" << endl;
		return 1;
	}

	glutInit(&argc, argv);

	QApplication app(argc, argv);

	int result = 0;
	{
		obj object(argv[1]);
		object.normalize();

		// level 7 = 128x128x128
		grid g(7, object.bbox());

        // sample the object and write the result to the grid
        auto elem = g.element_size();
        const float minSample = std::min(std::min(elem[0], elem[1]), elem[2]) / 2.0f;

        for(const auto& v : object.sample(minSample))
            g.set(v);

		// and make a window to display this data
		window win;
		win.setObject(object);
		win.setGrid(g);
		win.showMaximized();
		result = app.exec();
	}

	return result;
}

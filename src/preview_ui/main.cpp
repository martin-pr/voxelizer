#include <iostream>

#include <tbb/task_scheduler_init.h>

#include <QtGui/QApplication>

#include <GL/freeglut.h>

#include "window.h"
#include "factory.tpp"
#include "grid.h"
#include "sampler.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char*argv[]) {
	if(argc == 1) {
		cout << "usage: demo <file>" << endl;
		return 1;
	}

	glutInit(&argc, argv);

	QApplication app(argc, argv);

	int result = 0;
	{
		std::unique_ptr<mesh> object(factory<mesh>::create(argv[1]));
		if(object.get() == NULL) {
			cerr << "Cannot load file " << argv[1] << endl;
			return 1;
		}

		//object->normalize();

		// level 8 = 256x256x256
		grid g(7, object->bbox());

		// initialise tbb
		tbb::task_scheduler_init init;

		// sample the object and write the result to the grid
		{
			sampler s(g);
			s.add(*object);
		}

		// and make a window to display this data
		window win;
		win.setObject(std::move(object));
		win.setGrid(g);
		win.showMaximized();
		result = app.exec();
	}

	return result;
}

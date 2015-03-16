#include "window.h"

#include <GL/glut.h>

#include "viewport.h"
#include "bbox.h"
#include "grid.h"
#include "array_maths.h"

using std::cout;
using std::endl;

namespace {
	void draw_bbox(const ::bbox& box) {
		glPushMatrix();
		const std::array<float, 3> tr = box.min + (box.max - box.min) / 2.0f;
		glTranslatef(tr[0], tr[1], tr[2]);
		glScalef(box.max[0] - box.min[0], box.max[1] - box.min[1], box.max[2] - box.min[2]);
		glutWireCube(1.0f);
		glPopMatrix();
	}
}

//////////////////////////////////////////

window::window() : m_calllist(0) {
	m_viewport = new viewport(this);
	setCentralWidget(m_viewport);

	connect(m_viewport, SIGNAL(render(float)), this, SLOT(render(float)));
}

window::~window() {
}

void window::setObject(const obj& o) {
	m_object = o;
}

void window::setGrid(const grid& g) {
	m_grid = std::unique_ptr<grid>(new grid(g));
}

void window::render(float dt) {
	if(m_calllist == 0) {
		m_calllist = glGenLists(1);

		glNewList(m_calllist, GL_COMPILE);

		glColor3f(0,0.5,0);
		glBegin(GL_LINES);
		for(int x = -10; x <= 10; ++x) {
			glVertex3f(x,0,-10);
			glVertex3f(x,0,10);

			glVertex3f(-10, 0, x);
			glVertex3f(10, 0, x);
		}
		glEnd();

		glColor3f(1,1,1);
		glBegin(GL_LINES);
		for(auto f = m_object.faces().begin(); f != m_object.faces().end(); ++f)
			for(unsigned char v=0;v<3;++v) {
				glVertex3fv(m_object.vertices()[(*f)[v]].data());
				glVertex3fv(m_object.vertices()[(*f)[(v+1)%3]].data());
			}

		glEnd();


		if(m_grid.get() != NULL) {
			glColor3f(1, 0.5, 0.5);
			m_grid->visit_active(&draw_bbox);
		}

		glEndList();
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glCallList(m_calllist);

	glPopAttrib();
}

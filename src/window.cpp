#include "window.h"

#include "viewport.h"

window::window() {
	m_viewport = new viewport(this);
	setCentralWidget(m_viewport);

	connect(m_viewport, SIGNAL(render(float)), this, SLOT(render(float)));
}

void window::setObject(const obj& o) {
	m_object = o;
}

void window::render(float dt) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);

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

	glPopAttrib();
}

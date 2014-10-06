#include "window.h"

#include "viewport.h"

window::window() {
	m_viewport = new viewport(this);
	setCentralWidget(m_viewport);

	connect(m_viewport, SIGNAL(render(float)), this, SLOT(render(float)));
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

	glPopAttrib();
}

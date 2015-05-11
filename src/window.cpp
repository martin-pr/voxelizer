#include "window.h"

#include <GL/glut.h>

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>

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

window::window() : m_meshCalllist(0) {
	QWidget* central = new QWidget();
	setCentralWidget(central);

	QHBoxLayout* mainLayout = new QHBoxLayout(central);

	m_viewport = new viewport(this);
	mainLayout->addWidget(m_viewport, 1);

	QWidget* panel = new QWidget();
	panel->setMinimumWidth(200);
	mainLayout->addWidget(panel);

	QVBoxLayout* panelLayout = new QVBoxLayout(panel);
	panelLayout->addWidget(new QLabel("Display levels:"));
	m_slider = new QSlider(Qt::Horizontal);
	m_slider->setMinimum(0);
	m_slider->setMaximum(1);
	panelLayout->addWidget(m_slider);

	m_mesh = new QCheckBox("display mesh");
	panelLayout->addWidget(m_mesh);

	m_bbox = new QCheckBox("display bbox");
	m_bbox->setChecked(true);
	panelLayout->addWidget(m_bbox);

	panelLayout->addWidget(new QWidget(), 1);

	connect(m_viewport, SIGNAL(render(float)), this, SLOT(render(float)));
}

window::~window() {
}

void window::setObject(std::unique_ptr<mesh>&& o) {
	m_object = std::move(o);
}

void window::setGrid(const grid& g) {
	m_grid = std::unique_ptr<grid>(new grid(g));
	m_slider->setMaximum(m_grid->depth()-1);
}

void window::render(float dt) {

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);

	// ground plane
	glColor3f(0,0.5,0);
	glBegin(GL_LINES);
	for(int x = -10; x <= 10; ++x) {
		glVertex3f(x,0,-10);
		glVertex3f(x,0,10);

		glVertex3f(-10, 0, x);
		glVertex3f(10, 0, x);
	}
	glEnd();

	// mesh
	if(m_mesh->isChecked()) {
		if(m_meshCalllist == 0) {
			m_meshCalllist = glGenLists(1);

			glNewList(m_meshCalllist, GL_COMPILE);

			glColor3f(1,1,1);
			glBegin(GL_LINES);
			for(auto f = m_object->faces().begin(); f != m_object->faces().end(); ++f)
				for(unsigned char v=0;v<3;++v) {
					glVertex3fv(m_object->vertices()[(*f)[v]].data());
					glVertex3fv(m_object->vertices()[(*f)[(v+1)%3]].data());
				}

			glEnd();

			glEndList();
		}

		glCallList(m_meshCalllist);
	}

	// bboxes
	if(m_bbox->isChecked()) {
		if(m_object.get() != NULL) {
			while(m_calllists.size() <= (unsigned)m_slider->value())
				m_calllists.push_back(0);

			if(m_calllists[m_slider->value()] == 0) {
				m_calllists[m_slider->value()] = glGenLists(1);

				glNewList(m_calllists[m_slider->value()], GL_COMPILE);

				if(m_grid.get() != NULL) {
					glColor3f(1, 0.5, 0.5);
					m_grid->visit_active(&draw_bbox, m_slider->value());
				}

				glEndList();
			}

			glCallList(m_calllists[m_slider->value()]);
		}
	}

	glPopAttrib();
}

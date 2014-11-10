#include "viewport.h"

#include <cassert>
#include <iostream>
#include <cmath>

#include <QtGui/QMouseEvent>

#include <GL/glu.h>

namespace {
	std::vector<viewport*> s_instances;
}

viewport::viewport(QWidget* parent) : QGLWidget(parent, (s_instances.size()>0 ? s_instances[0] : NULL)), m_sceneDistance(10), m_sceneRotationX(30), m_sceneRotationY(30), m_mouseX(0), m_mouseY(0) {
	s_instances.push_back(this);

	setMouseTracking(true);
}

viewport::~viewport() {
	int index = -1;
	for(unsigned a=0;a<s_instances.size();a++)
		if(s_instances[a] == this)
			index = a;
	assert(index >= 0);
	s_instances.erase(s_instances.begin()+index);
}

void viewport::initializeGL() {
	glClearColor(0,0,0,0);
	resizeGL(width(), height());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void viewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,0,m_sceneDistance, 0,0,0, 0,1,0);

    glRotatef(m_sceneRotationY, 1, 0, 0);
    glRotatef(m_sceneRotationX, 0, 1, 0);

	emit render(0.0f);

	update();
}

void viewport::resizeGL(int w, int h) {
	QGLWidget::resizeGL(w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, (float)w / (float)h, 0.01, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void viewport::mouseMoveEvent(QMouseEvent* event) {
	if(event->buttons() & Qt::LeftButton) {
		m_sceneRotationX += (float)(event->x() - m_mouseX) / (float)width() * 360.0;
		m_sceneRotationY += (float)(event->y() - m_mouseY) / (float)height() * 360.0;
	}

	if(event->buttons() & Qt::RightButton)
		m_sceneDistance /= pow(10, (float)(event->y() - m_mouseY) / (float)height());

	m_mouseX = event->x();
	m_mouseY = event->y();
}


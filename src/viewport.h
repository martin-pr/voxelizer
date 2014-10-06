#pragma once

#include <vector>
#include <boost/noncopyable.hpp>
#include <QtOpenGL/QGLWidget>

class viewport : public QGLWidget, public boost::noncopyable {
	Q_OBJECT

	signals:
		void render(float dt);

	public:
		viewport(QWidget* parent);
		virtual ~viewport();

	protected:
		virtual void initializeGL();
		virtual void paintGL();
		virtual void resizeGL(int w, int h);
		virtual void mouseMoveEvent(QMouseEvent* event);

	private:
		float m_sceneDistance, m_sceneRotationX, m_sceneRotationY;
		int m_mouseX, m_mouseY;
};

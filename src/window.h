#pragma once

#include <memory>

#include <GL/freeglut.h>

#include <QtGui/QMainWindow>
#include <QtGui/QCheckBox>

#include "mesh.h"

class viewport;
class grid;
class QSlider;

class window : public QMainWindow {
	Q_OBJECT

	public:
		window();
		virtual ~window();

		void setObject(std::unique_ptr<mesh>&& o);
		void setGrid(const grid& g);

	protected:

	private slots:
		void render(float dt);

	private:
		viewport* m_viewport;
		QSlider* m_slider;

		QCheckBox *m_mesh, *m_bbox;

		std::unique_ptr<mesh> m_object;
		std::unique_ptr<grid> m_grid;

		std::vector<GLuint> m_calllists;
		GLuint m_meshCalllist;
};

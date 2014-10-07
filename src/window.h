#pragma once

#include <QtGui/QMainWindow>

#include "obj.h"

class viewport;

class window : public QMainWindow {
	Q_OBJECT

	public:
		window();

		void setObject(const obj& o);

	protected:

	private slots:
		void render(float dt);

	private:
		viewport* m_viewport;
		obj m_object;
};

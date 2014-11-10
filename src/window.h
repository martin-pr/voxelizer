#pragma once

#include <memory>

#include <QtGui/QMainWindow>

#include "obj.h"

class viewport;
class grid;

class window : public QMainWindow {
	Q_OBJECT

	public:
		window();
		virtual ~window();

		void setObject(const obj& o);
		void setGrid(const grid& g);

	protected:

	private slots:
		void render(float dt);

	private:
		viewport* m_viewport;
		obj m_object;
		std::unique_ptr<grid> m_grid;
};

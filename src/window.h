#pragma once

#include <QtGui/QMainWindow>

class viewport;

class window : public QMainWindow {
	Q_OBJECT

	public:
		window();

	protected:

	private slots:
		void render(float dt);

	private:
		viewport* m_viewport;
};

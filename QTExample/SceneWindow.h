#pragma once

#include <QWindow>

class SceneWindow : public QWindow
{
public:
	SceneWindow();

protected:
	void exposeEvent(QExposeEvent*) override;

	void resizeEvent(QResizeEvent*) override;

	void closeEvent(QCloseEvent*) override;

	void wheelEvent(QWheelEvent*) override;

	void mouseMoveEvent(QMouseEvent*) override;

	void mousePressEvent(QMouseEvent*)override;
	void mouseReleaseEvent(QMouseEvent*)override;

	bool event(QEvent* event) override;

	void render();

private:
	bool m_initialized = false;
};
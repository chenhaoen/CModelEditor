#include <QTimer>
#include <QResizeEvent>
#include <qpa/qplatformnativeinterface.h>

#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "logo.h"

#include "SceneWindow.h"

SceneWindow::SceneWindow()
{
	setSurfaceType(QSurface::VulkanSurface);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &SceneWindow::requestUpdate);
	timer->start(1/60.0f);
}

const bgfx::ViewId kClearView = 0;

void SceneWindow::exposeEvent(QExposeEvent*)
{
	if (isExposed()) {
		if (!m_initialized) {
			m_initialized = true;

			bgfx::renderFrame();
			// Initialize bgfx using the native window handle and window resolution.
			bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
			QPlatformNativeInterface* native = QGuiApplication::platformNativeInterface();
			init.platformData.ndt = static_cast<Display*>(native->nativeResourceForWindow("display", nullptr));
			init.platformData.nwh = reinterpret_cast<void*>(winId());
#elif BX_PLATFORM_OSX
			init.platformData.nwh = reinterpret_cast<void*>(winId());
#elif BX_PLATFORM_WINDOWS
			init.platformData.nwh = reinterpret_cast<void*>(winId());
#endif
			int width = this->width();
			int height = this->height();
			init.resolution.width = (uint32_t)width;
			init.resolution.height = (uint32_t)height;
			init.resolution.reset = BGFX_RESET_VSYNC;
			bgfx::init(init);

			bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
			bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
		}
	}
}

void SceneWindow::resizeEvent(QResizeEvent* event)
{
	if (!m_initialized) {
		return;
	}

	if (event->size() == event->oldSize())
	{
		return;
	}

	bgfx::reset((uint32_t)event->size().width(), (uint32_t)event->size().height(), BGFX_RESET_VSYNC);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void SceneWindow::closeEvent(QCloseEvent*)
{
	
}

void SceneWindow::wheelEvent(QWheelEvent* event)
{
}

bool mouseClicked = false;
static QPoint lastPoint;

void SceneWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (!mouseClicked)
	{
		return;
	}

}



void SceneWindow::mousePressEvent(QMouseEvent* event)
{

}

void SceneWindow::mouseReleaseEvent(QMouseEvent* event)
{
}

bool SceneWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(kClearView);
		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(this->width() / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(this->height() / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
		// Enable stats or debug text.
		// Advance to next frame. Process submitted rendering primitives.
		bgfx::setDebug(false ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		bgfx::frame();

		return true;
	}

	return QWindow::event(event);
}

void SceneWindow::render()
{
	requestUpdate();
}

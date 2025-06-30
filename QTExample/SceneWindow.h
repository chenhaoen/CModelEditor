#pragma once

#include <QWindow>

#include <RefCntAutoPtr.hpp>
#include <RenderDevice.h>
#include <DeviceContext.h>
#include <SwapChain.h>
#include <BasicMath.hpp>

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

	Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  m_pDevice;
	Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pImmediateContext;
	Diligent::RefCntAutoPtr<Diligent::ISwapChain>     m_pSwapChain;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
};
#pragma once

#include <QWindow>

#include <RefCntAutoPtr.hpp>
#include <RenderDevice.h>
#include <DeviceContext.h>
#include <SwapChain.h>
#include <BasicMath.hpp>

#include "Scene/Camera.h"

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

private:
	void CreatePipelineState();
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void render();
private:
	bool m_initialized = false;

	Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  m_pDevice;
	Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pImmediateContext;
	Diligent::RefCntAutoPtr<Diligent::ISwapChain>     m_pSwapChain;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
	Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> m_pSRB;

	Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeVertexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_CubeIndexBuffer;
	Diligent::RefCntAutoPtr<Diligent::IBuffer>                m_VSConstants;

	Camera m_camera;
};
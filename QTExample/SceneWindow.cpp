#include <QTimer>
#include <QResizeEvent>
#include <qpa/qplatformnativeinterface.h>
#include <QApplication>

#include "SceneWindow.h"

#if VULKAN_SUPPORTED
#include <EngineFactoryVk.h>
#endif

static const char* VSSource = R"(

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}
)";

// Pixel shader simply outputs interpolated vertex color
static const char* PSSource = R"(

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
)";

SceneWindow::SceneWindow()
{
	setSurfaceType(QSurface::VulkanSurface);

	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &SceneWindow::requestUpdate);
	timer->start(1/60.0f);
}

void SceneWindow::exposeEvent(QExposeEvent*)
{
	if (isExposed()) {
		if (!m_initialized) {
			m_initialized = true;

            Diligent::Win32NativeWindow Window{(void*)winId()};

            Diligent::SwapChainDesc SCDesc;

# if EXPLICITLY_LOAD_ENGINE_VK_DLL
            // Load the dll and import GetEngineFactoryD3D12() function
            Diligent::GetEngineFactoryVkType GetEngineFactoryVk = Diligent::LoadGraphicsEngineVk();
            Diligent::IEngineFactoryVk* pFactoryVk = GetEngineFactoryVk();
#else
            Diligent::IEngineFactoryVk* pFactoryVk = Diligent::GetEngineFactoryVk();
#endif
            Diligent::EngineVkCreateInfo EngineCI;
            pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_pDevice, &m_pImmediateContext);
            pFactoryVk->CreateSwapChainVk(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_pSwapChain);

            Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;

            // Pipeline state name is used by the engine to report issues.
            // It is always a good idea to give objects descriptive names.
            PSOCreateInfo.PSODesc.Name = "Simple triangle PSO";

            // This is a graphics pipeline
            PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;

            // clang-format off
            // This tutorial will render to a single render target
            PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
            // Set render target format which is the format of the swap chain's color buffer
            PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_pSwapChain->GetDesc().ColorBufferFormat;
            // Use the depth buffer format from the swap chain
            PSOCreateInfo.GraphicsPipeline.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
            // Primitive topology defines what kind of primitives will be rendered by this pipeline state
            PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            // No back face culling for this tutorial
            PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
            // Disable depth testing
            PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
            // clang-format on

            Diligent::ShaderCreateInfo ShaderCI;
            // Tell the system that the shader source code is in HLSL.
            // For OpenGL, the engine will convert this into GLSL under the hood.
            ShaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_GLSL;
            // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
            ShaderCI.Desc.UseCombinedTextureSamplers = true;
            // Create a vertex shader
            Diligent::RefCntAutoPtr<Diligent::IShader> pVS;
            {
                ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
                ShaderCI.EntryPoint = "main";
                ShaderCI.Desc.Name = "Triangle vertex shader";
                ShaderCI.Source = VSSource;
                m_pDevice->CreateShader(ShaderCI, &pVS);
            }

            // Create a pixel shader
            Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
            {
                ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
                ShaderCI.EntryPoint = "main";
                ShaderCI.Desc.Name = "Triangle pixel shader";
                ShaderCI.Source = PSSource;
                m_pDevice->CreateShader(ShaderCI, &pPS);
            }

            // Finally, create the pipeline state
            PSOCreateInfo.pVS = pVS;
            PSOCreateInfo.pPS = pPS;
            m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
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
        m_pImmediateContext->ClearStats();

        Diligent::ITextureView* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* pDSV = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
        // Let the engine perform required state transitions
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state in the immediate context
        m_pImmediateContext->SetPipelineState(m_pPSO);

        // Typically we should now call CommitShaderResources(), however shaders in this example don't
        // use any resources.

        Diligent::DrawAttribs drawAttrs;
        drawAttrs.NumVertices = 3; // We will render 3 vertices
        m_pImmediateContext->Draw(drawAttrs);

        m_pImmediateContext->Flush();
        m_pSwapChain->Present();
		return true;
	}

	return QWindow::event(event);
}

void SceneWindow::render()
{
	requestUpdate();
}

#include <QTimer>
#include <QResizeEvent>
#include <qpa/qplatformnativeinterface.h>
#include <QApplication>

#include <fstream>

#include "QT/SceneWindow.h"
#include "FileSystem.h"

#include <MapHelper.hpp>

#if VULKAN_SUPPORTED
#include <EngineFactoryVk.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct UniformBufferObject {
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);
};

std::vector<char> readFile(const std::string_view& filename)
{
    std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    buffer.push_back('\0');

    return buffer;
}

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

#if PLATFORM_WIN32
            Diligent::Win32NativeWindow Window{(void*)winId()};
#endif

#if PLATFORM_LINUX
            Diligent::LinuxNativeWindow Window;
            Window.WindowId = (uint32_t)(winId());

            QPlatformNativeInterface* native = QGuiApplication::platformNativeInterface();
            Display* display = static_cast<Display*>(native->nativeResourceForWindow("display", nullptr));
            Window.pDisplay = display;
#endif

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

            CreatePipelineState();
            CreateVertexBuffer();
            CreateIndexBuffer();
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

glm::mat4 getViewMatrix(){
    glm::mat4 result;
    glm::mat4 rotM = glm::mat4(1.0f);
    glm::mat4 transM;

    rotM = glm::rotate(rotM, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotM = glm::rotate(rotM, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 translation = glm::vec3(0.0f,0.0f,-5.0f);
    transM = glm::translate(glm::mat4(1.0f), translation);
    result = transM * rotM;
    return result;
}

UniformBufferObject ubo{};

bool SceneWindow::event(QEvent* event)
{
	if (event->type() == QEvent::UpdateRequest)
	{
        static std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> frameTime = currentTime - lastTime;
        lastTime = currentTime;

        m_pImmediateContext->ClearStats();

        Diligent::ITextureView* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* pDSV = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
        // Let the engine perform required state transitions
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            // Map the buffer and write current world-view-projection matrix
            Diligent::MapHelper<UniformBufferObject> CBConstants(m_pImmediateContext, m_VSConstants, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);


            //ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            const auto& SCDesc = m_pSwapChain->GetDesc();

            float AspectRatio = static_cast<float>(SCDesc.Width) / static_cast<float>(SCDesc.Height);

            ubo.model = glm::rotate(ubo.model, glm::radians(static_cast<float>(frameTime.count() * 0.1f)),glm::vec3(0.0f, 1.0f, 0.0f));

            ubo.view = getViewMatrix();
            //ubo.view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

            ubo.proj = glm::perspective(glm::radians(45.0f), AspectRatio, 0.1f, 1000.0f);

            //ubo.proj[1][1] *= -1;
            *CBConstants = ubo;
        }
        // Bind vertex and index buffers
        const uint64_t offset = 0;
        Diligent::IBuffer* pBuffs[] = { m_CubeVertexBuffer };
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_pPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_pSRB, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        Diligent::DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType = Diligent::VT_UINT32; // Index type
        DrawAttrs.NumIndices = 36;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);

        m_pImmediateContext->Flush();
        m_pSwapChain->Present();
		return true;
	}

	return QWindow::event(event);
}

void SceneWindow::CreatePipelineState()
{
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
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
    // clang-format on

    const std::string& localAppDataDirectory = che::FileSystem::getAppDir();
    const std::string& vertexShaderPath = localAppDataDirectory + "/resources/shaders/ModelVS.glsl";
    const std::string& fragmentShaderPath = localAppDataDirectory +"/resources/shaders/ModelFS.glsl";

    const std::vector<char> vertexSource = readFile(vertexShaderPath);
    const std::vector<char> fragSource = readFile(fragmentShaderPath);

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
        ShaderCI.Source = vertexSource.data();
        m_pDevice->CreateShader(ShaderCI, &pVS);

        Diligent::BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(UniformBufferObject);
        CBDesc.Usage = Diligent::USAGE_DYNAMIC;
        CBDesc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
        m_pDevice->CreateBuffer(CBDesc, nullptr, &m_VSConstants);
    }

    // Create a pixel shader
    Diligent::RefCntAutoPtr<Diligent::IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Triangle pixel shader";
        ShaderCI.Source = fragSource.data();
        m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
// Define vertex shader input layout
    Diligent::LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, false},
        // Attribute 1 - vertex color
        Diligent::LayoutElement{1, 0, 3, Diligent::VT_FLOAT32, false}
    };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    // Finally, create the pipeline state
    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

    m_pPSO->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "UniformBufferObject")->Set(m_VSConstants);

    // Create a shader resource binding object and bind all static resources in it
    m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
}

void SceneWindow::CreateVertexBuffer()
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
    };

    // Cube vertices

    //      (-1,+1,+1)________________(+1,+1,+1)
    //               /|              /|
    //              / |             / |
    //             /  |            /  |
    //            /   |           /   |
    //(-1,-1,+1) /____|__________/(+1,-1,+1)
    //           |    |__________|____|
    //           |   /(-1,+1,-1) |    /(+1,+1,-1)
    //           |  /            |   /
    //           | /             |  /
    //           |/              | /
    //           /_______________|/
    //        (-1,-1,-1)       (+1,-1,-1)
    //

    constexpr Vertex CubeVerts[8] =
    {
        { glm::vec3{-1, -1, -1},  glm::vec3{1, 0, 0}},
        { glm::vec3{-1, +1, -1},  glm::vec3{0, 1, 0}},
        { glm::vec3{+1, +1, -1},  glm::vec3{0, 0, 1}},
        { glm::vec3{+1, -1, -1},  glm::vec3{1, 1, 1}},

        { glm::vec3{-1, -1, +1},  glm::vec3{1, 1, 0}},
        { glm::vec3{-1, +1, +1},  glm::vec3{0, 1, 1}},
        { glm::vec3{+1, +1, +1},  glm::vec3{1, 0, 1}},
        { glm::vec3{+1, -1, +1},  glm::vec3{0.2f, 0.2f, 0.2f}},
    };

    // Create a vertex buffer that stores cube vertices
    Diligent::BufferDesc VertBuffDesc;
    VertBuffDesc.Name = "Cube vertex buffer";
    VertBuffDesc.Usage = Diligent::USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
    VertBuffDesc.Size = sizeof(CubeVerts);
    Diligent::BufferData VBData;
    VBData.pData = CubeVerts;
    VBData.DataSize = sizeof(CubeVerts);
    m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_CubeVertexBuffer);
}

void SceneWindow::CreateIndexBuffer()
{
    // clang-format off
    constexpr uint32_t Indices[] =
    {
        2,0,1, 2,3,0,
        4,6,5, 4,7,6,
        0,7,4, 0,3,7,
        1,0,4, 1,4,5,
        1,5,2, 5,6,2,
        3,6,7, 3,2,6
    };
    // clang-format on

    Diligent::BufferDesc IndBuffDesc;
    IndBuffDesc.Name = "Cube index buffer";
    IndBuffDesc.Usage = Diligent::USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
    IndBuffDesc.Size = sizeof(Indices);
    Diligent::BufferData IBData;
    IBData.pData = Indices;
    IBData.DataSize = sizeof(Indices);
    m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);
}

void SceneWindow::render()
{
	requestUpdate();
}

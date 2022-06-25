#include "TestApp.h"

TestApp::TestApp()
{
}

void TestApp::GetKeyPress(KeyCode key) noexcept {
    //ABR_DEBUG_OUTPUT(L"CALLBACK: GetKeyDown!" + std::to_wstring(key));
    if (key == KeyCode_K) {
        rendererManager->GetRenderer()->CleanBackBuffer();
    }
}

void TestApp::GetMouseLeftPress(float x, float y) noexcept
{
    //rendererManager->GetRenderer()->SetPixel(x, y, RGB(255, 255, 255));
}

void TestApp::Update(float delta_time) noexcept
{
    AppBase::Update(delta_time);
    controlManager->Update(this);
    DrawScene();
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void TestApp::Init() noexcept {
    ABR_DEBUG_OUTPUT("TestApp Create!");
    controlManager = Singleton<ControlManager>::GetInstance();
    rendererManager = Singleton<RendererManager>::GetInstance();

    renderer = dynamic_cast<Rasterizer*>(rendererManager->GetRenderer());
    BuildGeometryBuffer();
    BuildVertexLayout();
    BuildVertexShader();
}

void TestApp::BuildGeometryBuffer()
{
    Vertex vertices[] =
    {
       { -1.0f, -1.0f, -1.0f, 255, 255, 255 },
       { -1.0f, +1.0f, -1.0f, 255, 255, 255 },
       { +1.0f, +1.0f, -1.0f, 255, 255, 255 },
       { +1.0f, -1.0f, -1.0f, 255, 255, 255 },
       { -1.0f, -1.0f, +1.0f, 255, 255, 255 },
       { -1.0f, +1.0f, +1.0f, 255, 255, 255 },
       { +1.0f, +1.0f, +1.0f, 255, 255, 255 },
       { +1.0f, -1.0f, +1.0f, 255, 255, 255 }
    };
    renderer->mVertexBufferDesc.ByteWidth = sizeof(vertices);
    renderer->CreateBuffer(&renderer->mVertexBufferDesc, vertices, pVertexBuffer);

    UINT indices[] =
    {
        // 前表面
        0, 1, 2,
        0, 2, 3,
        // 后表面
        4, 6, 5,
        4, 7, 6,
        // 左表面
        4, 5, 1,
        4, 1, 0,
        // 右表面
        3, 2, 6,
        3, 6, 7,
        // 上表面
        1, 5, 6,
        1, 6, 2,
        // 下表面
        4, 0, 3,
        4, 3, 7
    };

    renderer->mIndexBufferDesc.ByteWidth = sizeof(indices);
    renderer->CreateBuffer(&renderer->mIndexBufferDesc, indices, pIndexBuffer);
}

void TestApp::BuildVertexLayout()
{
    ABR_INPUT_ELEMENT_DESC vertexDesc[] = {
        0,
        9,
    };
    renderer->IASetInputLayout(vertexDesc, 2);
}

void TestApp::BuildVertexShader()
{
    renderer->VSSetShader(new TestVertexShader);
}

void TestApp::DrawScene()
{
    renderer->IASetPrimitiveTopology(ABR_PRIMITIVE_TOPOLOGY::ABR_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer->IASetVertexBuffers(pVertexBuffer);
    renderer->IASetIndexBuffers(pIndexBuffer);

    renderer->DrawIndexed(36, 0);
}

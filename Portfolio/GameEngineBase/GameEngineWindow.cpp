#include "GameEngineWindow.h"
#include "GameEngineDebug.h"
#include <assert.h>
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        GameEngineWindow::GetInst().Off();
        break;
    case WM_CLOSE:
        GameEngineWindow::GetInst().Off();
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

GameEngineWindow* GameEngineWindow::Inst_ = new GameEngineWindow();

void GameEngineWindow::RegClass(HINSTANCE _hInst)
{
    // ������ Ŭ���� ���
    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInst;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "GameEngineWindowClass";
    wcex.hIconSm = nullptr;
    RegisterClassExA(&wcex);
}

void GameEngineWindow::CreateGameWindow(HINSTANCE _hInst, const std::string& _Title)
{
    if (nullptr != hWnd_)
    {
        MsgBoxAssert("�����츦 2�� ������ �߽��ϴ�.");
        return;
    }

    Title_ = _Title;
    hInst_ = _hInst; 
    RegClass(_hInst);

    hWnd_ = CreateWindowExA(0L, "GameEngineWindowClass", Title_.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

    hDC_ = GetDC(hWnd_);

    if (!hWnd_)
    {
        return;
    }
}

void GameEngineWindow::ShowGameWindow()
{ 
    if (nullptr == hWnd_)
    {
        MsgBoxAssert("���� �����찡 ��������� �ʾҽ��ϴ� ȭ�鿡 ����Ҽ� �����ϴ�.");
        return;
    }

    ShowWindow(hWnd_, SW_SHOW);
    UpdateWindow(hWnd_);
}

void GameEngineWindow::MessageLoop(void(*_InitFunction)(), void(*_LoopFunction)())
{
    if (nullptr != _InitFunction)
    {
        _InitFunction();
    }

    MSG msg;

    while (windowOn_)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ���⼭ ���� ����
        
        if (nullptr == _LoopFunction)
        {
            continue;
        }

        _LoopFunction();
    }
}

void GameEngineWindow::SetWindowScaleAndPosition(float4 _pos, float4 _scale)
{
    scale_ = _scale;
    RECT rect = { 0, 0, static_cast<long>(_scale.x), static_cast<long>(_scale.y)};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    SetWindowPos(hWnd_, nullptr, _pos.ix(), _pos.iy(), rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
}

void GameEngineWindow::Off()
{
    windowOn_ = false;
}

GameEngineWindow::GameEngineWindow()
    : Title_("")
    , hInst_(nullptr)
    , hWnd_(nullptr)
    , windowOn_(true)
    , hDC_(nullptr)
    , scale_({})
{
}

GameEngineWindow::~GameEngineWindow()
{
    // hInst_�� �̱����̰� ���� �������ش�. 
    // + �Ʒ��� dc�� window�� ���� �Լ��� �����ؾߵǰ� ������� �ؾ� ������ ����.

    if (nullptr != hDC_)
    {
        ReleaseDC(hWnd_, hDC_);
        hDC_ = nullptr;
    }
    if (nullptr != hWnd_)
    {
        DestroyWindow(hWnd_);
        hWnd_ = nullptr;
    }
}

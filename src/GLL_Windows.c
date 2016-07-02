#include <GLL/GLL_API.h>
#include <GLL/GLL_OpenGL.h>
#include "GLL_Private.h"

static const PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    24,
    8,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
};

static HWND     dummyHwnd;
static HDC      dummyDc;
static HGLRC    dummyContext;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

GLL_API void* GLL_LoadFunction(const char *name)
{
    void* p = wglGetProcAddress(name);
    if (p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1))
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = GetProcAddress(module, name);
    }
    return p;
}

GLL_API void GLL_CreateDummyContext(void)
{
    WNDCLASSW windowClass;

    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_NOCLOSE;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = NULL;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = L"GLL_DummyClass";
    RegisterClassW(&windowClass);

    HWND win = CreateWindowExW(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
            L"GLL_DummyClass",
            L"",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT,
            0,
            0,
            0,
            NULL,
            NULL,
            NULL,
            NULL
            );

    HDC dc = GetWindowDC(win);
    int pf = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pf, &pfd);
    HGLRC gl = wglCreateContext(dc);
    wglMakeCurrent(dc, gl);

    dummyHwnd = win;
    dummyDc = dc;
    dummyContext = gl;
}

GLL_API void GLL_DestroyDummyContext(void)
{
    wglMakeCurrent(dummyDc, NULL);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyHwnd, dummyDc);
    DestroyWindow(dummyHwnd);
}


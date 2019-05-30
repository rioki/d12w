// d12w - The C++ DirectX 12 Wrapper
// Copyright (c) 2019 Sean Farrell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Window.h"

#include <stdexcept>
#include <d12w/util.h>

namespace d12w::example
{
    constexpr auto windowClassName = L"D12W-EXAMPLE-WINDOW";

    LRESULT CALLBACK Window_WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch(msg)
        {
            case WM_CREATE:
            {
                CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
                break;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_SIZE:
            {
                /*Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                if (window != NULL)
                {
                    window->handle_resize();
                }*/
                break;
            }
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void CreateWindowClass(HINSTANCE hInst)
    {
        static bool done = false;

        if (done == false)
        {
            WNDCLASSEX wClass;
            ZeroMemory(&wClass,sizeof(WNDCLASSEX));
            wClass.cbClsExtra    = NULL;
            wClass.cbSize        = sizeof(WNDCLASSEX);
            wClass.cbWndExtra    = NULL;
            wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
            wClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
            wClass.hIcon         = NULL;
            wClass.hIconSm       = NULL;
            wClass.hInstance     = hInst;
            wClass.lpfnWndProc   = (WNDPROC)Window_WinProc;
            wClass.lpszClassName = windowClassName;
            wClass.lpszMenuName  = NULL;
            wClass.style         = CS_HREDRAW|CS_VREDRAW;

            if(!RegisterClassExW(&wClass))
            {
                D12W_THROW(std::runtime_error, d12w::util::GetLastError());
            }

            done = true;
        }
    }

    Window::Window(int width, int height, const std::string_view caption)
    {
        using d12w::util::widen;

        HINSTANCE hInst = GetModuleHandleA(NULL);
        CreateWindowClass(hInst);

        hWnd = CreateWindowExW(NULL, windowClassName, widen(caption).data(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInst, this);
        if (hWnd == nullptr)
        {
            D12W_THROW(std::runtime_error, d12w::util::GetLastError());
        }
    }

    void Window::show(int cmd)
    {
        ShowWindow(hWnd, cmd);
    }

    void Window::close()
    {
        DestroyWindow(hWnd);
    }

    void Window::run()
    {
        auto msg = MSG{0};

        while(GetMessageA(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
}
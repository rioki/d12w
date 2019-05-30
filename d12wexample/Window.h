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

#ifndef _D12W_EXAMPLE_WINDOW_H_
#define _D12W_EXAMPLE_WINDOW_H_

#include <string_view>
#include <windows.h>

namespace d12w::example
{
    /*!
     * Win32 Window
     *
     * 
     */
    class Window
    {
    public:
        /*!
         * Create a top level window.
         *
         * The constructor will create a fully valid window with HWND,
         * but it will be hidden. Use Show to make the window visible
         * and Run to make the window process window messages. 
         *
         * @param width the width of the window
         * @param height the height of the window
         * @param caption the caption of the window
         *
         * @throws std::runtime_error if it failed to create the window
         */
        Window(int width, int height, const std::string_view caption);

        /*!
         * Show the widow.
         *
         * This function calls ShowWindow with the given cmd.
         * When calling from the WinMain, you generally should 
         * pass the nCmdShow from the WinMain, to conform with the
         * expected Win32 launch behavior.
         *
         * @param cmd the command to show
         */
        void Show(int cmd = SW_SHOW);

        /*!
         * Close the window and end the main loop exectuion.
         */
        void Close();

        /*!
         * Run the windows main loop.
         *
         * This function will run until eiher Close is called or
         * Windows sends a window destroy event (ALT+F4 / X Button).
         *
         * @see Close
         */
        void Run();

    private:
        HWND hWnd = nullptr;
    };
}

#endif

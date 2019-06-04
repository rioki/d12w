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

#ifndef _D12W_DXGI_FACTORY_H_
#define _D12W_DXGI_FACTORY_H_

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>

#include "../defines.h"
#include "../ComPtr.h"

namespace d12w::dxgi
{
    class Adapter;

    /*!
     * DirectX Graphic Interface Factory
     *
     * This wrapper implements IDXGIFactory4
     */
    class D12W_EXPORT Factory
    {
    public:

        /*!
         * Create a DXGI Factor.
         */
        Factory();

        Factory(const Factory&) = delete;

        ~Factory();

        Factory& operator = (const Factory&) = delete;

        /*!
         * Provides an adapter which can be provided to D3D12CreateDevice to use the WARP renderer.
         *
         * @return A gdi::Adapter interface pointer to the adapter.
         */
        std::shared_ptr<Adapter> EnumWarpAdapter();

        /*!
         * Enumerates the adapters (video cards).
         */
        std::vector<std::shared_ptr<Adapter>> EnumAdapters();

        /*!
         * Enumerates both adapters (video cards) with or without outputs.
         */
        std::vector<std::shared_ptr<Adapter>> EnumAdapters1();

    private:
        ComPtr<IDXGIFactory4> factory4;
    };
}

#endif

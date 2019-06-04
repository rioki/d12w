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

#include "Factory.h"

#include "Adapter.h"

#pragma comment(lib, "DXGI.lib")

namespace d12w::dxgi
{
    Factory::Factory()
    {
        auto createFactoryFlags = 0u;
        #ifndef NDEBUG
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
        #endif

        auto hr = CreateDXGIFactory2(createFactoryFlags, factory4.UUID(), reinterpret_cast<void**>(&factory4));
        D12W_CHECK_SUCCESS(hr);
    }

    Factory::~Factory() = default;

    std::shared_ptr<Adapter> Factory::EnumWarpAdapter()
    {
        ComPtr<IDXGIAdapter1> adapter1;
        auto hr = factory4->EnumWarpAdapter(adapter1.UUID(), reinterpret_cast<void**>(&adapter1));
        D12W_CHECK_SUCCESS(hr);

        return std::shared_ptr<Adapter>{new Adapter{adapter1}};
    }

    std::vector<std::shared_ptr<Adapter>> Factory::EnumAdapters()
    {
        auto result = std::vector<std::shared_ptr<Adapter>>{};
        auto hr = HRESULT{0};
        auto i = 0u;
        do
        {
            auto adapter = ComPtr<IDXGIAdapter>{};
            hr = factory4->EnumAdapters(i, &adapter);
            if (hr != DXGI_ERROR_NOT_FOUND)
            {
                D12W_CHECK_SUCCESS(hr);
                result.push_back(std::shared_ptr<Adapter>{new Adapter{adapter}});
            }
            i++;
        }
        while (hr != DXGI_ERROR_NOT_FOUND);
        return result;
    }

    std::vector<std::shared_ptr<Adapter>> Factory::EnumAdapters1()
    {
        auto result = std::vector<std::shared_ptr<Adapter>>{};
        auto hr = HRESULT{0};
        auto i = 0u;
        do
        {
            auto adapter1 = ComPtr<IDXGIAdapter1>{};
            hr = factory4->EnumAdapters1(i, &adapter1);
            if (hr != DXGI_ERROR_NOT_FOUND)
            {
                D12W_CHECK_SUCCESS(hr);
                result.push_back(std::shared_ptr<Adapter>{new Adapter{adapter1}});
            }
            i++;
        }
        while (hr != DXGI_ERROR_NOT_FOUND);
        return result;
    }
}
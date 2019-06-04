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

#include "Adapter.h"

namespace d12w::dxgi
{
    Adapter::~Adapter() = default;

    DXGI_ADAPTER_DESC Adapter::GetDesc()
    {
        auto result = DXGI_ADAPTER_DESC{0};
        auto hr = adapter4->GetDesc(&result);
        D12W_CHECK_SUCCESS(hr);
        return result;
    }

    DXGI_ADAPTER_DESC1 Adapter::GetDesc1()
    {
        auto result= DXGI_ADAPTER_DESC1{0};
        auto hr = adapter4->GetDesc1(&result);
        D12W_CHECK_SUCCESS(hr);
        return result;
    }

    DXGI_ADAPTER_DESC2 Adapter::GetDesc2()
    {
        auto result = DXGI_ADAPTER_DESC2{0};
        auto hr = adapter4->GetDesc2(&result);
        D12W_CHECK_SUCCESS(hr);
        return result;
    }

    DXGI_ADAPTER_DESC3 Adapter::GetDesc3()
    {
        auto result = DXGI_ADAPTER_DESC3{0};
        auto hr = adapter4->GetDesc3(&result);
        D12W_CHECK_SUCCESS(hr);
        return result;
    }

    Adapter::Adapter(ComPtr<IDXGIAdapter> adapter)
    : adapter4(adapter.As<IDXGIAdapter4>()) {}

    Adapter::Adapter(ComPtr<IDXGIAdapter1> adapter1)
    : adapter4(adapter1.As<IDXGIAdapter4>()) {}
}

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

#include "Debug.h"

#include "../util.h"

#pragma comment(lib, "D3D12.lib")

namespace d12w::d3d
{
    Debug::Debug() 
    {
        auto hr = D3D12GetDebugInterface(debug1.UUID(), reinterpret_cast<void**>(&debug1));
        D12W_CHECK_SUCCESS(hr);

        hr = D3D12GetDebugInterface(debug1.UUID(), reinterpret_cast<void**>(&debug2));
        D12W_CHECK_SUCCESS(hr);
    }

    void Debug::EnableDebugLayer()
    {
        D12W_ASSERT(debug1);
        debug1->EnableDebugLayer();
    }

    void Debug::SetEnableGPUBasedValidation(bool enable)
    {
        D12W_ASSERT(debug1);
        debug1->SetEnableGPUBasedValidation(enable ? TRUE : FALSE);
    }

    void Debug::SetEnableSynchronizedCommandQueueValidation(bool enable)
    {
        D12W_ASSERT(debug1);
        debug1->SetEnableSynchronizedCommandQueueValidation(enable ? TRUE : FALSE);
    }

    void Debug::SetGPUBasedValidationFlags(D3D12_GPU_BASED_VALIDATION_FLAGS flags)
    {
        D12W_ASSERT(debug2);
        debug2->SetGPUBasedValidationFlags(flags);
    }
}

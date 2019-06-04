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

#include <Windows.h>

#include <d12w/d12w.h>

#include "Window.h"

bool useWrap = false;

auto GetAdapter(bool useWarp)
{
    auto dxgiFactory = d12w::dxgi::Factory{};
    if (useWarp)
    {
        return dxgiFactory.EnumWarpAdapter();
    }
    else
    {
        auto maxMem = 0u;
        auto bestAdapter = 0;
        auto adapters = dxgiFactory.EnumAdapters1();
        for (auto i = 0u; i < adapters.size(); i++)
        {
            auto desc = adapters[i]->GetDesc1();
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE == 0 && 
                maxMem < desc.DedicatedVideoMemory /*&&
                d3d::Device::TryCreate(adapters[i])*/)
            {
                bestAdapter = i;
                maxMem = desc.DedicatedVideoMemory;
            }
        }
        return adapters[bestAdapter];
    }
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    try
    {
        // Enable the DirectX debug layer in debug builds. 
        // This needs to be done befor any calls to DirectX.
        #ifndef NDEBUG
        auto debug = d12w::d3d::Debug{};
        debug.EnableDebugLayer();
        #endif

        auto window = d12w::example::Window{800, 600, "D12W Example"};

        auto adapter = GetAdapter(useWrap);
        
        

        window.Show(nCmdShow);
        window.Run();
        return 0;
    }
    catch (std::exception& ex)
    {
        MessageBoxA(NULL, ex.what(), "Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
    catch (...)
    {
        MessageBoxA(NULL, "Unknown exception.", "Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
}

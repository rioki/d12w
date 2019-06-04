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

#ifndef _D12W_DXGI_ADAPTER_H_
#define _D12W_DXGI_ADAPTER_H_

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
    /*!
     * DirectX Graphic Interface Adapter
     *
     * This wrapper implements IDXGIAdapter1 through IDXGIAdapter4
     */
    class D12W_EXPORT Adapter
    {
    public:

        Adapter(const Adapter&) = delete;

        ~Adapter();

        Adapter& operator = (const Adapter&) = delete;

        /*!
         * Gets a DXGI 1.0 description of an adapter (or video card).
         *
         * On feature level 9 graphics hardware, GetDesc1 returns zeros for the PCI ID in the VendorId,
         * DeviceId, SubSysId, and Revision members of DXGI_ADAPTER_DESC1 and “Software Adapter” for 
         * the description string in the Description member.
         *
         * @return A a DXGI_ADAPTER_DESC structure that describes the adapter.
         */ 
        DXGI_ADAPTER_DESC GetDesc();

        /*!
         * Gets a DXGI 1.1 description of an adapter (or video card).
         *
         * On feature level 9 graphics hardware, GetDesc1 returns zeros for the PCI ID in the VendorId,
         * DeviceId, SubSysId, and Revision members of DXGI_ADAPTER_DESC1 and “Software Adapter” for 
         * the description string in the Description member.
         *
         * @return A a DXGI_ADAPTER_DESC1 structure that describes the adapter.
         */ 
        DXGI_ADAPTER_DESC1 GetDesc1();

        /*!
         * Gets a DXGI 1.1 description of an adapter (or video card).
         *
         * On feature level 9 graphics hardware, GetDesc1 returns zeros for the PCI ID in the VendorId,
         * DeviceId, SubSysId, and Revision members of DXGI_ADAPTER_DESC1 and “Software Adapter” for 
         * the description string in the Description member.
         *
         * @return A a DXGI_ADAPTER_DESC1 structure that describes the adapter.
         */ 
        DXGI_ADAPTER_DESC2 GetDesc2();

        /*!
         * Gets a Microsoft DirectX Graphics Infrastructure (DXGI) 1.6 description of an adapter or video card. This description includes information about ACG compatibility.
         *
         * On feature level 9 graphics hardware, early versions of GetDesc3 (GetDesc1, and GetDesc) return 
         * zeros for the PCI ID in the VendorId, DeviceId, SubSysId, and Revision members of the adapter description 
         * structure and “Software Adapter” for the description string in the Description member. GetDesc3 and GetDesc2 
         * return the actual feature level 9 hardware values in these members.
         *
         * @return A a DXGI_ADAPTER_DESC3 structure that describes the adapter.
         */ 
        DXGI_ADAPTER_DESC3 GetDesc3();

    private:
        ComPtr<IDXGIAdapter4> adapter4;

        explicit
        Adapter(ComPtr<IDXGIAdapter> adapter);
        explicit
        Adapter(ComPtr<IDXGIAdapter1> adapter1);

    friend class Factory;
    };
}

#endif

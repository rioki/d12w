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

#ifndef _D12W_DEBUG_H_
#define _D12W_DEBUG_H_

#include <d3d12.h>

#include "../defines.h"
#include "../ComPtr.h"

namespace d12w::d3d
{
    /*!
     * Wrap D3D Debug API
     *
     * This class wraps the ID3D12Debug1 and ID3D12Debug2 API.
     */
    class D12W_EXPORT Debug
    {
    public:
        /*!
         * Create debug object.
         *
         * This constructor will query the required interfaces.
         */ 
        Debug();

        Debug(const Debug&) = delete;
        ~Debug() = default;
        Debug& operator = (const Debug&) = delete;

        /*!
         * Enables the debug layer.
         */
        void EnableDebugLayer();

        /*!
         * This method enables or disables GPU-Based Validation (GBV) before creating a device with the debug layer enabled.
         *
         * @param Enable true to enable GPU-Based Validation, otherwise false.
         *
         * @note GPU-Based Validation can only be enabled/disabled prior to creating a device. 
         * By default, GPU-Based Validation is disabled. To disable GPU-Based Validation after 
         * initially enabling it the device must be fully released and recreated.
         */
        void SetEnableGPUBasedValidation(bool enable);

        /*!
         * Enables or disables dependent command queue synchronization when using a D3D12 device with the debug layer enabled.
         *
         * @param Enable true to enable Dependent Command Queue Synchronization, otherwise false.
         * 
         * @note Dependent Command Queue Synchronization is a D3D12 Debug Layer feature that gives 
         * the debug layer the ability to track resource states more accurately when enabled. 
         * Dependent Command Queue Synchronization is enabled by default.
         *
         * When Dependent Command Queue Synchronization is enabled, the debug layer holds back actual 
         * submission of GPU work until all outstanding fence Wait conditions are met. This gives the
         * debug layer the ability to make reasonable assumptions about GPU state (such as resource states)
         * on the CPU-timeline when multiple command queues are potentially doing concurrent work.
         *
         * With Dependent Command Queue Synchronization disabled, all resource states tracked by the 
         * debug layer are cleared each time ID3D12CommandQueue::Signal is called. This results in 
         * significantly less useful resource state validation.
         *
         * Disabling Dependent Command Queue Synchronization may reduce some debug layer performance 
         * overhead when using multiple command queues. However, it is suggested to leave it enabled 
         * unless this overhead is problematic. Note that applications that use only a single command 
         * queue will see no performance changes with Dependent Command Queue Synchronization disabled.
         */
        void SetEnableSynchronizedCommandQueueValidation(bool enable);

        /*!
         * This method configures the level of GPU-based validation that the debug device is to perform at runtime.
         *
         * @param Flags Specifies the level of GPU-based validation to perform at runtime.
         *
         * @note This method overrides the default behavior of GPU-based validation so it must be 
         * called before creating the D3D12 Device. These settings can't be changed or cancelled 
         * after the device is created. If you want to change the behavior of GPU-based validation 
         * at a later time, the device must be destroyed and recreated with different parameters.
         */
        void SetGPUBasedValidationFlags(D3D12_GPU_BASED_VALIDATION_FLAGS flags);

    private:
        ComPtr<ID3D12Debug1> debug1;
        ComPtr<ID3D12Debug2> debug2;
    };
}

#endif

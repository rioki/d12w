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

#ifndef _D12W_COMPTR_H_
#define _D12W_COMPTR_H_

#include <atomic>
#include <algorithm>

#include "defines.h"
#include "util.h"

namespace d12w
{
    /*!
     * COM Smart Pointer
     */
    template <typename ComClass>
    class D12W_EXPORT ComPtr
    {
    public:
        
        ComPtr() = default;

        ComPtr(ComClass* obj)
        : object(object) 
        {
            AddRef();
        }

        ComPtr(const ComPtr<ComClass>& other)
        : object(other.object)
        {
            AddRef();
        }

        ~ComPtr()
        {
            Release();
        }

        ComPtr<ComClass>& operator = (const ComPtr<ComClass>& other)
        {
            ComPtr<ComClass> tmp(other);
            Swap(tmp);
            return *this;
        }

        operator ComClass* ()
        {
            return object;
        }

        operator const ComClass* () const
        {
            return object;
        }

        operator void** ()
        {
            return reinterpret_cast<void**>(&object);
        }

        ComClass* operator -> ()
        {
            D12W_ASSERT(object != nullptr);
            return object;
        }

        const ComClass* operator -> () const
        {
            D12W_ASSERT(object != nullptr);
            return object;
        }

        constexpr IID UUID() const
        {
            return __uuidof(ComClass);
        }

        void Swap(const ComPtr<ComClass>& other) noexcept
        {
            std::swap<ComClass*>(object, other.object);
        }

    private:
        ComClass* object = nullptr;

        void AddRef()
        {
            if (object)
            {
                object->AddRef();
            }
        }

        void Release()
        {
            if (object)
            {
                object->Release();
                object = nullptr;
            }
        }
    };
}

#endif

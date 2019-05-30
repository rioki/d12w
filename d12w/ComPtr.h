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
        
        /*! 
         * Default Constructor
         *
         * This constructor will create a null pointer. 
         */
        ComPtr() = default;

        /*!
         * Wrapping Constructor
         *
         * This constructor takes a reference to a "naked" pointer 
         * to a COM object, it will increase the reference count, because
         * it is assumed that the pointer will be properly managed elsewhere.
         *
         * @param obj the COM object to wrap
         *
         * @warning If you create a COM object and pass it to ComPtr via this constructor
         * you must properly release your reference once the ComPtr is constructed,
         * or else you will leak memory.
         */
        ComPtr(ComClass* obj)
        : object(object) 
        {
            AddRef();
        }

        /*!
         * Copy Constructor
         *
         * Make a new reference with increaded reference count.
         *
         * @param other the ComPtr to copy
         */
        ComPtr(const ComPtr<ComClass>& other)
        : object(other.object)
        {
            AddRef();
        }

        /*!
         * Reduce the reference count.
         */
        ~ComPtr()
        {
            Release();
        }

        /*!
         * Assignment Operator
         *
         * Make a shalow copy of the ComPtr and increate the reference count.
         *
         * @param other the ComPtr to take a refernce from
         */
        ComPtr<ComClass>& operator = (const ComPtr<ComClass>& other)
        {
            ComPtr<ComClass> tmp(other);
            Swap(tmp);
            return *this;
        }

        /*!
         * Is Equal Operator 
         *
         * Check if the two ComPtr point to the same object.
         *
         * @note Only local address comparision is done. If by means
         * of COM two local proxy object point to the same COM object,
         * this function will return false.
         *
         * @param rhs the ComPtr to compare with
         */
        bool operator == (const ComPtr<ComClass>& rhs)
        {
            return object == rhs.object;
        }

        /*!
         * Is Not Equal Operator 
         *
         * Check if the two ComPtr point to a differen object.
         *
         * @note Only local address comparision is done. If by means
         * of COM two local proxy object point to the same COM object,
         * this function will return true.
         *
         * @param rhs the ComPtr to compare with
         */
        bool operator != (const ComPtr<ComClass>& rhs)
        {
            return object != rhs.object;
        }

        /*!
         * Cast to Bool Operator
         *
         * This operator if provided for the convinience
         * to check if the underlying pointer is valid.
         *
         * @return true if the managed COM object it not null
         */
        operator bool () const
        {
            return object != nullptr;
        }

        /*!
         * Cast to Pointer Operator
         * 
         * This operator provides the means to pass the underlying
         * COM object to other COM functions as argument. 
         *
         * @return a reference to the underlying COM object
         *
         * @{
         */
        operator ComClass* ()
        {
            return object;
        }
        operator const ComClass* () const
        {
            return object;
        }
        /*! @} */

        /*!
         * Address Operator
         *
         * The address operator is provided to get the underlying 
         * pointer to pointer. This is commonly needed during COM object
         * initialization.
         *
         * @return the address of the pointer to COM object as void**
         * 
         * @note No const version is provided, since generally speaking
         * this should never be needed.
         */
        void** operator & ()
        {
            return reinterpret_cast<void**>(&object);
        }

        /*!
         * Arrow Operator
         *
         * This operator allows to use ComPtr like any other pointer
         * to call functions on the undelying object.
         *
         * @return a reference to the underlying COM object to be
         * used with the arrow operator.
         *
         * @{
         */
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
        /*! @} */

        /*!
         * Get the UUID for the managed COM interface.
         *
         * @return the UUID for the managed COM interface.
         */
        constexpr IID UUID() const
        {
            return __uuidof(ComClass);
        }

        /*!
         * Exception safe swap.
         *
         * This function is used to implement construct and swap semantics.
         *
         * @param other the other ComPtr to swap internals with.
         */
        void Swap(ComPtr<ComClass>& other) noexcept
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

    /*!
     * std::swap compatible swap function
     *
     * This function only exsits to allow overload resolution of std::swap
     * call locations to resolve to the interal Swap function.
     *
     * @param lhs the ComPtr to swap rhs with
     * @param rhs the ComPtr to swap lhs with
     */
    template <typename ComClass>
    void swap(ComPtr<ComClass>& lhs, ComPtr<ComClass>& rhs)
    {
        lhs.Swap(rhs);
    }
}

#endif

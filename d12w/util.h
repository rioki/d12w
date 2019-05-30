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

#ifndef _D12W_DBG_H_
#define _D12W_DBG_H_

#include <vector>
#include <string>
#include <sstream>
#include <winerror.h>

#include "defines.h"

#ifndef NDEBUG
/*!
 * Throw an exception with callstack.
 *
 * In debug builds this macro will throw an exception with full callstack 
 * In release builds this macro will just throw the exception.
 *
 * @param EX the ection class to throw
 * @param MSG the message to set into the exception
 */
#define D12W_THROW(EX, MSG) ::d12w::util::ThrowWithCallstack<EX>(__FUNCTION__, MSG)

/*!
 * Asseert that throws an exception with callstack.
 *
 * In debug builds this macro will check the given condition and 
 * if the confition is false, throw an exception.
 * 
 * @param COND the confition to check
 */
#define D12W_ASSERT(COND) if (static_cast<bool>(COND) == false) { ::d12w::util::HandleAssert(__FUNCTION__, #COND); }

/*!
 * Check if a HRESULT is a success state.
 *
 * This macro will check if a given HRESULT is FAILED and if that is the case throw and exception.
 * In debug builds this will be an exception with full callstack. 
 *
 * @param HR the HRESULT to check
 */
#define D12W_CHECK_SUCCESS(HR) if (FAILED(HR)) { ::d12w::util::HandleHrFailed(__FUNCTION__, HR); }
#else
#define D12W_THROW(EX, MSG) throw EX(MSG)
#define D12W_ASSERT(COND)
#define D12W_CHECK_SUCCESS(HR) if (FAILED(HR)) { throw std::runtime_error(::d12w::util::GetErrorMessage(HR)); }
#endif

namespace d12w::util
{   
#ifndef NDEBUG
    /*!
     * Single entry in the callstack,
     */
    struct StackFrame
    {
        uint64_t address;       //!< function address
        std::string name;       //!< function name
        std::string module;     //!< module name
        unsigned int line;      //!< line of the function
        std::string file;       //!< file of the function
    };

    /*!
     * Pull call stack
     *
     * This debug utility function pulls a callstack. To pull a readable
     * callstack you need to have all PDBs next to the program.
     *
     * @return the callstack
     * 
     * @note This function is only available in debug builds.
     */
    D12W_EXPORT
    std::vector<StackFrame> StackTrace();

    /*!
     * Handle asset failure.
     *
     * This debug utility function is used to throw an exception with call stack
     * if an assertion failed. You should use the D12W_ASSERT makro to use
     * this function.
     *
     * @param func the calling function
     * @param cond the assert condition as string
     *
     * @note This function is only available in debug builds.
     *
     * @see D12W_ASSERT
     */
    [[noreturn]] D12W_EXPORT
    void HandleAssert(const std::string_view func, const std::string_view cond);

    /*!
     * Handle HRESULT failure.
     *
     * This debug utility function is used to convert a HRESULT failure
     * into an exception with call stack. You should use the D12W_CHECK_SUCCESS
     * to check a HRESULT and invoke this function in the case of an error.
     *
     * @param func the calling function
     * @param hr the HRESULT that failed
     *
     * @note This function is only available in debug builds.
     *
     * @see D12W_CHECK_SUCCESS
     */
    [[noreturn]] D12W_EXPORT
    void HandleHrFailed(const std::string_view func, HRESULT hr);

    /*!
     * Throw exception with callstack.
     *
     * This debug utility function will pull a full callstack and append it to 
     * the given error message. You should use D12W_THROW macro instead of this 
     * function.
     *
     * @param func the calling function
     * @param msg the message to print
     *
     * @note This function is only available in debug builds.
     *
     * @see D12W_THROW 
     */
    template <typename Exception> [[noreturn]]
	void ThrowWithCallstack(const std::string_view func, const std::string_view msg) 
	{
		std::stringstream buff;
		buff << func << ": msg \n";
		buff << "\n";

		std::vector<StackFrame> stack = StackTrace();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].address << ": " << stack[i].name << "(" << std::dec << stack[i].line << ") in " << stack[i].module << "\n";
		}

		throw Exception(buff.str());
	}
 #endif

    /*!
     * Convert Win32's error code to a string.
     *
     * This utilir function converts a HRESULT or DWORD error code
     * to a string with the help of FormatMessageA. 
     *
     * @param errorid either HRESULT or DWORD Win32 error code.
     * @return the given error code as human readable string.
     */
    D12W_EXPORT
    std::string GetErrorMessage(int32_t errorid);

    /*!
     * Get the last Win32 error.
     *
     * This utility function get the last error from Win32's GetLastError
     * and runst it through GetErrorMessage to get the error code as string.
     *
     * @return the last Win32 error as string
     */
    D12W_EXPORT
    std::string GetLastError();

    /*!
     * Converts UTF-8 narow string to  wide UTF-16 / UCS-2 string.
     *
     * @param value the UTF-8 narow string to convert 
     * @return UTF-16 / UCS-2 wide string
     *
     * @throws std::logic_error if the conversion failed
     */
    D12W_EXPORT
    std::wstring widen(const std::string_view value);

    /*!
     * Converts wide UTF-16 / UCS-2 string to UTF-8 narow string.
     *
     * @param value the UTF-16 / UCS-2 wide string to convert 
     * @return UTF-8 narow string
     *
     * @throws std::logic_error if the conversion failed
     */
    D12W_EXPORT
    std::string narrow(const std::wstring_view value);
}

#endif

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

#ifndef NDEBUG

#define D12W_THROW(EX, MSG) ::d12w::dbg::handle_throw<EX>(__FUNCTION__, MSG)
#define D12W_ASSERT(COND) if ((COND) == false) { ::d12w::dbg::handle_assert(__FUNCTION__, #COND); }

namespace d12w::dbg
{
    std::string basename(const std::string& file);

    struct StackFrame
    {
        uint64_t address;
        std::string name;
        std::string module;
        unsigned int line;
        std::string file;
    };

    std::vector<StackFrame> stack_trace();
    
    template <typename Exception>
	void handle_throw(const std::string_view func, const std::string_view msg) 
	{
		std::stringstream buff;
		buff << func << ": msg \n";
		buff << "\n";

		std::vector<StackFrame> stack = stack_trace();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].address << ": " << stack[i].name << "(" << std::dec << stack[i].line << ") in " << stack[i].module << "\n";
		}

		throw Exception(buff.str());
	}
 
    void handle_assert(const std::string_view func, const std::string_view cond);
}

#else
#define D12W_THROW(EX, MSG) throw EX(MSG)
#define D12W_ASSERT(COND)
#endif

#endif

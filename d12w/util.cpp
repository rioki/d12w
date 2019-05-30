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

#include "util.h"

#include <windows.h>
#include <stdio.h>
#include <intrin.h>
#include <dbghelp.h>
#include <array>

#pragma comment(lib, "dbghelp.lib")

namespace d12w::util
{
#ifndef NDEBUG
    std::string basename(const std::string& file)
    {
        size_t i = file.find_last_of("\\/");
        if (i == std::string::npos)
        {
            return file;
        }
        else
        {
            return file.substr(i + 1);
        }
    }

    std::vector<StackFrame> StackTrace()
    {
        #if _WIN64
        DWORD machine = IMAGE_FILE_MACHINE_AMD64;
        #else
        DWORD machine = IMAGE_FILE_MACHINE_I386;
        #endif
        HANDLE process = GetCurrentProcess();
        HANDLE thread  = GetCurrentThread();
                
        if (SymInitialize(process, NULL, TRUE) == FALSE)
        {
            return std::vector<StackFrame>(); 
        }

        SymSetOptions(SYMOPT_LOAD_LINES);
        
        CONTEXT context = {};
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);

        #if _WIN64
        STACKFRAME frame = {};
        frame.AddrPC.Offset = context.Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rbp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode = AddrModeFlat;
        #else
        STACKFRAME frame = {};
        frame.AddrPC.Offset = context.Eip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Ebp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Esp;
        frame.AddrStack.Mode = AddrModeFlat;
        #endif
       
        bool first = true;

        std::vector<StackFrame> frames;
        while (StackWalk(machine, process, thread, &frame, &context , NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
        {
            StackFrame f = {};
            f.address = frame.AddrPC.Offset;
            
            #if _WIN64
            DWORD64 moduleBase = 0;
            #else
            DWORD moduleBase = 0;
            #endif

            moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);

            char moduelBuff[MAX_PATH];            
            if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduelBuff, MAX_PATH))
            {
                f.module = basename(moduelBuff);
            }
            else
            {
                f.module = "Unknown Module";
            }
            #if _WIN64
            DWORD64 offset = 0;
            #else
            DWORD offset = 0;
            #endif
            char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
            PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
            symbol->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 255;
            symbol->MaxNameLength = 254;

            if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol))
            {
                f.name = symbol->Name;
            }
            else
            {
                f.name = "Unknown Function";
            }
            
            IMAGEHLP_LINE line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
            
            DWORD offset_ln = 0;
            if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line))
            {
                f.file = line.FileName;
                f.line = line.LineNumber;
            }
            else
            {
                f.line = 0;
            } 

            if (!first)
            { 
                frames.push_back(f);
            }
            first = false;
        }

        SymCleanup(process);

        return frames;
    }
    
    void HandleAssert(const std::string_view func, const std::string_view cond)
    {
        std::stringstream buff;
        buff << "Assertion '" << cond << "' failed! \n";
        ThrowWithCallstack<std::logic_error>(func, buff.str());
    }

    void HandleHrFailed(const std::string_view func, HRESULT hr)
    {
        ThrowWithCallstack<std::logic_error>(func, GetErrorMessage(hr));
    }
#endif

    std::string GetErrorMessage(int32_t errorid)
    {
        auto buffer = std::array<char, 1024>{};
        auto landId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
        auto flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        auto stringSize = FormatMessageA(flags, NULL, errorid, landId, buffer.data(), static_cast<DWORD>(buffer.size()), NULL);
        return std::string(buffer.data(), stringSize);
    }
    
    std::string GetLastError()
    {
        auto error = ::GetLastError();
        return GetErrorMessage(error);
    }

    std::wstring widen(const std::string_view value)
    {
        if (value.empty())
        {
            return std::wstring();
        }
        
        std::vector<wchar_t> buff(value.size() + 126);
        int r = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), buff.data(), static_cast<int>(buff.size()));
        if (r == 0)
        {
            D12W_THROW(std::logic_error, GetLastError());;       
        }
        else
        {
            return std::wstring(buff.data());
        }    
    }

    std::string narrow(const std::wstring_view value)
    {
        if (value.empty())
        {
            return std::string();
        }
        
        std::vector<char> buff(value.size() * 2);
        int r = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), buff.data(), static_cast<int>(buff.size()), NULL, NULL);
        if (r == 0)
        {
            D12W_THROW(std::logic_error, GetLastError());;        
        }
        else
        {
            return std::string(buff.data());
        }
    }
}

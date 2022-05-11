/****************************************************************************
 Copyright (c) 2021-2022 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#pragma once

#include "../Config.h"
#if USE_MEMORY_LEAK_DETECTOR

    #if CC_PLATFORM == CC_PLATFORM_WINDOWS || CC_PLATFORM == CC_PLATFORM_NX_WINDOWS
        #include <Windows.h>
    #endif
    #include <cstdint>
    #include "../Macros.h"
    #include "base/std/container/string.h"
    #include "base/std/container/vector.h"

namespace cc {

    #define MAX_STACK_FRAMES  64
    #define MAX_SYMBOL_LENGTH 255

/**
 * A single frame of callstack.
 */
struct CC_DLL StackFrame {
    ccstd::string module;
    ccstd::string file;
    ccstd::string function;
    uint32_t line{0};

    ccstd::string toString();
};

/**
 * An utility class used to backtrace callstack.
 */
class CC_DLL CallStack {
public:
    static ccstd::string basename(const ccstd::string &path);

    static ccstd::vector<void *> backtrace();
    static ccstd::vector<StackFrame> backtraceSymbols(const ccstd::vector<void *> &callstack);

    #if CC_PLATFORM == CC_PLATFORM_WINDOWS || CC_PLATFORM == CC_PLATFORM_NX_WINDOWS
    static void initSym();
    static void cleanupSym();

private:
    static HANDLE _process;
    #endif
};

} // namespace cc

#endif

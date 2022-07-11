
/****************************************************************************
 Copyright (c) 2022 Xiamen Yaji Software Co., Ltd.
 
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

#include "base/Macros.h"
#include <iostream>
#include "base/std/container/string.h"
#include "base/std/container/vector.h"

namespace cc {

class CC_DLL FilePath {
public:
    FilePath();
    FilePath(const FilePath& that);
    FilePath(const ccstd::string& path);
    ~FilePath();
    FilePath& operator=(const FilePath& that);
    bool operator==(const FilePath& that) const;

    static constexpr char kCurrentDirectory[] = ".";
    static constexpr char kParentDirectory[] = "..";
    static constexpr char kExtensionSeparator = '.';

    ccstd::vector<ccstd::string> getComponents() const;
    const ccstd::string& value() const { return _path; }

    FilePath baseName() const;
    FilePath dirName() const;
    void stripTrailingSeparatorsInternal();
    ccstd::string finalExtension(bool tolower = true) const;
    FilePath removeFinalExtension() const;
    FilePath removeExtension() const;
    FilePath addExtension(const std::string& extension) const;
    FilePath replaceExtension(const std::string& extension) const;

    FilePath append(const FilePath& component) const;
    FilePath append(const std::string& component) const;
    bool isPathAbsolute(const std::string& path);

    ccstd::string normalizePath();

private:
    ccstd::string _path;
};

} // namespace cc
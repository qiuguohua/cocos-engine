
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
#include "cocos/core/filesystem/LocalFileSystem.h"
#include "cocos/core/filesystem/BaseFileSystem.h"

namespace cc {
class FilePath; 
class ZipFile;
class CC_DLL ZipFileSystem : public BaseFileSystem {
public:
    ZipFileSystem(const FilePath& assetsPath);
    ~ZipFileSystem() override;
    bool exist(const FilePath& filepath) const override;
    BaseFileHandle* open(const FilePath& filepath) override;

    bool createDirectory(const FilePath& path) override {
        return false;
    }
    int64_t getFileSize(const FilePath& filepath) override {
        return 0;
     }
    bool removeFile(const FilePath& filepath) override {
        return false;
    }
    bool renameFile(const FilePath& oldFilepath, const FilePath& newFilepath) override {
        return false;
    }
    bool removeDirectory(const FilePath& dirPath) {
        return false;
    }
    ccstd::string getWritablePath() const override {
        return "";
    }
private:
    ZipFile* _zipFile;
};

}
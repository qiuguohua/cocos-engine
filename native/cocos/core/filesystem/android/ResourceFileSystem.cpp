
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

#include "core/filesystem/android/ResourceFileSystem.h"
#include "core/filesystem/android/ResourceFileHandle.h"

namespace cc {
namespace {
const std::string kAssetsFolderName = "@assets/";
}

AAssetManager *ResourceFileSystem::assetmanager = nullptr;

ResourceFileSystem::ResourceFileSystem() {
    addSearchPath("",true);
    _defaultResRootPath = kAssetsFolderName;
    addSearchPath("Resources", true);
    addSearchPath("data", true);
}

void ResourceFileSystem::setassetmanager(AAssetManager *a) {
    if (nullptr == a) {
        //LOGD("setassetmanager : received unexpected nullptr parameter");
        return;
    }

    cc::ResourceFileSystem::assetmanager = a;
}


bool ResourceFileSystem::exist(const FilePath& path) const {

    if (!ResourceFileSystem::assetmanager) {
        return false;
    }
    if (path.value().empty()) {
        return false;
    }

    std::string dirPath = path.value();
    //if (!isAbsolutePath(dirPath.c_str())) {
    //    dirPath = fullPathForFilename(dirPath.c_str());
    //}

    if (dirPath[dirPath.length() - 1] == '/') {
        dirPath[dirPath.length() - 1] = '\0';
    }
    const char *s = dirPath.c_str();
    if (dirPath.find(_defaultResRootPath) == 0) {
        s += _defaultResRootPath.length();
    }
    AAsset *aa = AAssetManager_open(ResourceFileSystem::assetmanager, s, AASSET_MODE_UNKNOWN);
    if (aa) {
        AAsset_close(aa);
        return true;
    }
    AAssetDir *dir = AAssetManager_openDir(ResourceFileSystem::assetmanager, s);
    if (dir && AAssetDir_getNextFileName(dir)) {
        AAssetDir_close(dir);
        return true;
    }
    return false;
}

BaseFileHandle* ResourceFileSystem::open(const FilePath& filename) {
    if (!ResourceFileSystem::assetmanager) {
        return nullptr;
    }
    AAsset *asset = AAssetManager_open(assetmanager, filename.value().c_str(), AASSET_MODE_UNKNOWN);
    if (nullptr == asset) {
        return nullptr;
    }
    return new ResourceFileHandle(filename, asset);
}

bool ResourceFileSystem::isAbsolutePath(const std::string &strPath) const {
    return strPath.find(kAssetsFolderName) == 0;
}

}
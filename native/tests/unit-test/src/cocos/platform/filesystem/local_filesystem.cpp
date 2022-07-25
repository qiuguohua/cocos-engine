/****************************************************************************
 Copyright (c) 2021 Xiamen Yaji Software Co., Ltd.

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

#include "cocos/platform/filesystem/LocalFileSystem.h"
#include "cocos/platform/filesystem/IFileSystem.h"
#include "gtest/gtest.h"

#if CC_PLATFORM == CC_PLATFORM_WINDOWS
#include "cocos/platform/win32/Utils-win32.h"
#include <Windows.h>
#endif

TEST(LocalFileSystemTest, EmptyInterfaceTest) {
    cc::LocalFileSystem fs;
    cc::FilePath path("");
    EXPECT_EQ(fs.createDirectory(path), false);
    EXPECT_EQ(fs.removeDirectory(path), false);
    EXPECT_EQ(fs.removeFile(path), false);
    EXPECT_EQ(fs.renameFile(path, path), false);
    EXPECT_EQ(fs.getFileSize(path), 0);
    EXPECT_EQ(fs.getUserAppDataPath().value(), "");
    EXPECT_EQ(fs.isAbsolutePath(cc::FilePath("/")), true);
    EXPECT_EQ(fs.isAbsolutePath(cc::FilePath("a")), false);
}

TEST(LocalFilesystemHandleTest, allTest) {
    cc::LocalFileSystem fs;
    cc::FilePath path("cocos_test.json");
    if (fs.pathExists(path)) {
#if CC_PLATFORM == CC_PLATFORM_WINDOWS
        DeleteFile(cc::StringUtf8ToWideChar(path.value()).c_str());
#endif
    }
    std::string text = "abc";
    std::string textAppend = "123";

    {
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::READ_ONLY);
        EXPECT_EQ(handle.get(), nullptr);
    }

    {
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::WRITE_ONLY);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(fs.pathExists(path), true);
        EXPECT_EQ(handle->write((uint8_t*)text.c_str(), text.size()), true);
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->size(), text.size());
        EXPECT_EQ(handle->seek(0), true);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t*)buf, text.size()), false);
    }

    {
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::READ_ONLY);
        EXPECT_NE(handle.get(), nullptr);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t*)buf, text.size()), true);
        EXPECT_EQ(std::string(buf), text);
        EXPECT_EQ(handle->write((uint8_t*)buf, text.size()), false);
    }

    {
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::READ_WRITE);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(fs.pathExists(path), true);
        EXPECT_EQ(handle->write((uint8_t *)text.c_str(), text.size()), true);
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->seek(0), true);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t *)buf, text.size()), true);
        EXPECT_EQ(memcmp(buf, text.c_str(), text.length()), 0);
    }

    {
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::APPEND);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(handle->size(), text.size());
        handle->write((uint8_t *)textAppend.c_str(), textAppend.size());
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->size(), text.size() + textAppend.size());
        EXPECT_EQ(handle->seek(0), true);
        char buf[18] = {0};
        EXPECT_EQ(handle->read((uint8_t *)buf, 18), true);
        EXPECT_EQ(memcmp(buf, text.c_str(), text.length()), 0);
        EXPECT_EQ(memcmp(buf + text.length(), textAppend.c_str(), textAppend.length()), 0);
    }
    {
        // test seek
        auto handle = fs.open(path, cc::IFileSystem::AccessFlag::READ_ONLY);
        EXPECT_EQ(handle->seek(text.length()), true);
        EXPECT_EQ(handle->tell(), text.length());
        EXPECT_EQ(handle->seek(0, cc::IFileHandle::MoveMethod::FILE_SEEK_END), true);
        EXPECT_EQ(handle->tell(), text.length() + textAppend.length());
        int pos = 5;
        EXPECT_EQ(handle->seek(pos, cc::IFileHandle::MoveMethod::FILE_SEEK_END), true);
        EXPECT_EQ(handle->tell(), text.length() + textAppend.length() + pos);

        EXPECT_EQ(handle->seek(text.length()), true);
        EXPECT_EQ(handle->tell(), text.length());
        EXPECT_EQ(handle->seek(pos, cc::IFileHandle::MoveMethod::FILE_SEEK_CUR), true);
        EXPECT_EQ(handle->tell(), text.length() + pos);
    }

    #if CC_PLATFORM == CC_PLATFORM_WINDOWS
    DeleteFile(cc::StringUtf8ToWideChar(path.value()).c_str());
    #endif
}

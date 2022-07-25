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
#include "gtest/gtest.h"

#include "cocos/platform/win32/WindowsFileSystem.h"


TEST(WindowsFileSystemTest, directoryTest) {
    auto fs = std::make_unique<cc::WindowsFileSystem>();
    cc::FilePath testPath("CocosTestPath");
    if (fs->pathExists(testPath)) {
        fs->removeDirectory(testPath);
    }
    EXPECT_EQ(fs->createDirectory(testPath), true);
    EXPECT_EQ(fs->createDirectory(testPath), true);
    EXPECT_EQ(fs->pathExists(testPath), true);
    cc::FilePath subPath = testPath.append("createPathA").append("createPathB").append("createPathC");
    EXPECT_EQ(fs->createDirectory(subPath), true);
    EXPECT_EQ(fs->pathExists(subPath), true);
    EXPECT_EQ(fs->removeDirectory(subPath), true);
    EXPECT_EQ(fs->pathExists(subPath), false);

    cc::FilePath filePath("CocosTestPath/test.test");
    fs->open(filePath, cc::IFileSystem::AccessFlag::WRITE_ONLY);
    EXPECT_EQ(fs->pathExists(filePath), true);
    EXPECT_EQ(fs->removeDirectory(testPath), true);
    EXPECT_EQ(fs->pathExists(testPath), false);
    EXPECT_EQ(fs->pathExists(filePath), false);
}

TEST(WindowsFileSystemTest, isAbsoluteTest) {
    auto fs = std::make_unique<cc::WindowsFileSystem>();
    cc::FilePath testPath("CocosTestPath");
    EXPECT_EQ(fs->isAbsolutePath(testPath), false);

    testPath = cc::FilePath("/cocos");
    EXPECT_EQ(fs->isAbsolutePath(testPath), false);

    testPath = cc::FilePath("c:/cocos");
    EXPECT_EQ(fs->isAbsolutePath(testPath), true);
}


TEST(WindowsFileHandleTest, allTest) {
    cc::WindowsFileSystem fs;
    cc::FilePath path("CocosTestPath");
    if (!fs.pathExists(path)) {
        fs.createDirectory(path);
    }

    cc::FilePath filepath = path.append("test.txt");
    std::string text = "abc";
    std::string textAppend = "123";

    {
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::READ_ONLY);
        EXPECT_EQ(handle.get(), nullptr);
    }

    {
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::WRITE_ONLY);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(fs.pathExists(filepath), true);
        EXPECT_EQ(handle->write((uint8_t *)text.c_str(), text.size()), true);
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->size(), text.size());
        EXPECT_EQ(handle->seek(0), true);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t *)buf, text.size()), false);
    }

    {
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::READ_ONLY);
        EXPECT_NE(handle.get(), nullptr);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t *)buf, text.size()), true);
        EXPECT_EQ(std::string(buf), text);
        EXPECT_EQ(handle->write((uint8_t *)buf, text.size()), false);
    }

    {
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::READ_WRITE);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(fs.pathExists(filepath), true);
        EXPECT_EQ(handle->write((uint8_t *)text.c_str(), text.size()), true);
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->seek(0), true);
        char buf[6] = {0};
        EXPECT_EQ(handle->read((uint8_t *)buf, text.size()), true);
        EXPECT_EQ(memcmp(buf, text.c_str(), text.length()), 0);
    }

    {
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::APPEND);
        EXPECT_NE(handle.get(), nullptr);
        EXPECT_EQ(handle->size(), text.size());
        handle->write((uint8_t *)textAppend.c_str(), textAppend.size());
        EXPECT_EQ(handle->flush(), true);
        EXPECT_EQ(handle->size(), text.size() + textAppend.size());
        EXPECT_EQ(handle->seek(0), true);
        char buf[18] = {0};
        int64_t s = handle->tell();
        EXPECT_EQ(handle->read((uint8_t *)buf, 18), true);
        s = handle->tell();
        EXPECT_EQ(memcmp(buf, text.c_str(), text.length()), 0);
        EXPECT_EQ(memcmp(buf + text.length(), textAppend.c_str(), textAppend.length()), 0);
    }
    {
        // test seek
        auto handle = fs.open(filepath, cc::IFileSystem::AccessFlag::READ_ONLY);
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

    fs.removeDirectory(path);
}

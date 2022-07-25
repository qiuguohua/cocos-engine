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
#include "cocos/platform/filesystem/FilePath.h"


TEST(FilePathTest, EqualTest) {
    cc::FilePath pathA = "a/b";
    cc::FilePath pathB = pathA;
    EXPECT_EQ(pathA == pathB, true);

    pathA = cc::FilePath("b/c");
    EXPECT_EQ(pathA == pathB, false);
}

TEST(FilePathTest, EmptyTest) {
    cc::FilePath path;
    EXPECT_EQ(path.value(), "");
    EXPECT_EQ(path.empty(), true);
}

TEST(FilePathTest, ClearTest) {
    cc::FilePath path("a/b");
    EXPECT_EQ(path.value(), "a/b");

    path.clear();
    EXPECT_EQ(path.value(), "");
}

TEST(FilePathTest, RemoveLastSeparatorTest) {
    cc::FilePath path("/a/");
    path.removeLastSeparator();
    EXPECT_EQ(path.value(), "/a");

    path = cc::FilePath("/");
    path.removeLastSeparator();
    EXPECT_EQ(path.value(), "/");

    path = cc::FilePath("//");
    path.removeLastSeparator();
    EXPECT_EQ(path.value(), "/");
    path = cc::FilePath("/////////");
    path.removeLastSeparator();
    EXPECT_EQ(path.value(), "/");

    path = cc::FilePath("a//b///////");
    path.removeLastSeparator();
    EXPECT_EQ(path.value(), "a//b");
}

TEST(FilePathTest, ValueTest) {
    std::string p("..");
    std::string p1("/..");
    int pos = p.find("..");
    pos = p1.find("..");
    cc::FilePath path("a\\b\\c");
    EXPECT_EQ(path.value(), "a/b/c");
    EXPECT_EQ(path[0], 'a');
    EXPECT_EQ(path[1], '/');
    
    path = cc::FilePath(".");
    EXPECT_EQ(path.value(), "");

    path = cc::FilePath("./");
    EXPECT_EQ(path.value(), "");

    path = cc::FilePath("./b");
    EXPECT_EQ(path.value(), "b");

    path = cc::FilePath("a/./b");
    EXPECT_EQ(path.value(), "a/b");

    path = cc::FilePath("a/../b");
    EXPECT_EQ(path.value(), "b");

    path = cc::FilePath("a/../");
    EXPECT_EQ(path.value(), "");

    path = cc::FilePath("a/..");
    EXPECT_EQ(path.value(), "");

    path = cc::FilePath("../b");
    EXPECT_EQ(path.value(), "../b");

    path = cc::FilePath("a/b/../../c");
    EXPECT_EQ(path.value(), "c");

    path = cc::FilePath("/../c");
    EXPECT_EQ(path.value(), "/../c");
}

TEST(FilePathTest, BaseNameTest) {
    cc::FilePath path("a/b/c.txt");
    EXPECT_EQ(path.baseName().value(), "c.txt");

    path = cc::FilePath("a/b/c");
    EXPECT_EQ(path.baseName().value(), "c");

    path = cc::FilePath("text");
    EXPECT_EQ(path.baseName().value(), "text");

    path = cc::FilePath("");
    EXPECT_EQ(path.baseName().value(), "");
}


TEST(FilePathTest, DirNameTest) {
    cc::FilePath path("a/b/c.txt");
    EXPECT_EQ(path.dirName().value(), "a/b");

    path = cc::FilePath("a/b/c");
    EXPECT_EQ(path.dirName().value(), "a/b");

    path = cc::FilePath("text");
    EXPECT_EQ(path.dirName().value(), "");

    path = cc::FilePath("");
    EXPECT_EQ(path.dirName().value(), "");
}

TEST(FilePathTest, FinalExtgensionTest) {
    cc::FilePath path("a/b/c.txt");
    EXPECT_EQ(path.finalExtension(), ".txt");

    path = cc::FilePath("a/b/c");
    EXPECT_EQ(path.finalExtension(), "");

    path = cc::FilePath("a.txt");
    EXPECT_EQ(path.finalExtension(), ".txt");

    path = cc::FilePath("a.tar.gz");
    EXPECT_EQ(path.finalExtension(), ".gz");

    path = cc::FilePath("a.tar.TEXT");
    EXPECT_EQ(path.finalExtension(), ".text");
    EXPECT_EQ(path.finalExtension(false), ".TEXT");
}

TEST(FilePathTest, removeFinalExtensionTest) {
    cc::FilePath path("a/b/c.txt");
    path = path.removeFinalExtension();
    EXPECT_EQ(path.value(), "a/b/c");

    path = cc::FilePath("a");
    path = path.removeFinalExtension();
    EXPECT_EQ(path.value(), "a");

    path = cc::FilePath("a.tar.gz");
    path = path.removeFinalExtension();
    EXPECT_EQ(path.value(), "a.tar");
}

TEST(FilePathTest, appendTest) {
    cc::FilePath path;
    path = path.append("a");
    EXPECT_EQ(path.value(), "a");

    path = path.append("b");
    EXPECT_EQ(path.value(), "a/b");

    path = path.append("c/d");
    EXPECT_EQ(path.value(), "a/b/c/d");


    path = path.append("e/f\0/d");
    EXPECT_EQ(path.value(), "a/b/c/d/e/f");

    path = path.append("..");
    EXPECT_EQ(path.value(), "a/b/c/d/e/f/..");
}

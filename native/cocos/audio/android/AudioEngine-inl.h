/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#pragma once

#include <SLES/OpenSLES.h>
#if CC_PLATFORM == CC_PLATFORM_ANDROID
#include <SLES/OpenSLES_Android.h>
#elif CC_PLATFORM == CC_PLATFORM_OPENHARMONY
#include <SLES/OpenSLES_Platform.h>
#endif
#include <functional>
#include <string>
#include <unordered_map>

#include "base/Ref.h"
#include "base/Utils.h"

#define MAX_AUDIOINSTANCES 13

#define ERRORLOG(msg) log("fun:%s,line:%d,msg:%s", __func__, __LINE__, #msg)

namespace cc {

struct CustomEvent;

class IAudioPlayer;
class AudioPlayerProvider;

class AudioEngineImpl;

class AudioEngineImpl : public cc::Ref {
public:
    AudioEngineImpl();
    ~AudioEngineImpl() override;

    bool  init();
    int   play2d(const std::string &filePath, bool loop, float volume);
    void  setVolume(int audioID, float volume);
    void  setLoop(int audioID, bool loop);
    void  pause(int audioID);
    void  resume(int audioID);
    void  stop(int audioID);
    void  stopAll();
    float getDuration(int audioID);
    float getDurationFromFile(const std::string &filePath);
    float getCurrentTime(int audioID);
    bool  setCurrentTime(int audioID, float time);
    void  setFinishCallback(int audioID, const std::function<void(int, const std::string &)> &callback);

    void uncache(const std::string &filePath);
    void uncacheAll();
    void preload(const std::string &filePath, const std::function<void(bool)> &callback);

    void onResume();
    void onPause();

    void setAudioFocusForAllPlayers(bool isFocus);

private:
    // engine interfaces
    SLObjectItf _engineObject;
    SLEngineItf _engineEngine;

    // output mix interfaces
    SLObjectItf _outputMixObject;

    //audioID,AudioInfo
    std::unordered_map<int, IAudioPlayer *>                                _audioPlayers;
    std::unordered_map<int, std::function<void(int, const std::string &)>> _callbackMap;

    // UrlAudioPlayers which need to resumed while entering foreground
    std::unordered_map<int, IAudioPlayer *> _urlAudioPlayersNeedResume;

    AudioPlayerProvider *_audioPlayerProvider;

    int _audioIDIndex;

    bool _lazyInitLoop;
};

} // namespace cc

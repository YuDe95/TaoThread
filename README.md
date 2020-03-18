# TaoThread

## status
| [Windows][win-link]| [Ubuntu][ubuntu-link]|[MacOS][macos-link]|[Android][android-link]|[IOS][ios-link]|
|---------------|---------------|-----------------|-----------------|----------------|
| ![win-badge]  | ![ubuntu-badge]      | ![macos-badge] |![android-badge]   |![ios-badge]   |


|[License][license-link]| [Release][release-link]|[Download][download-link]|[Issues][issues-link]|[Wiki][wiki-links]|
|-----------------|-----------------|-----------------|-----------------|-----------------|
|![license-badge] |![release-badge] | ![download-badge]|![issues-badge]|![wiki-badge]|

[win-link]: https://github.com/JaredTao/TaoThread/actions?query=workflow%3AWindows "WindowsAction"
[win-badge]: https://github.com/JaredTao/TaoThread/workflows/Windows/badge.svg  "Windows"

[ubuntu-link]: https://github.com/JaredTao/TaoThread/actions?query=workflow%3AUbuntu "UbuntuAction"
[ubuntu-badge]: https://github.com/JaredTao/TaoThread/workflows/Ubuntu/badge.svg "Ubuntu"

[macos-link]: https://github.com/JaredTao/TaoThread/actions?query=workflow%3AMacOS "MacOSAction"
[macos-badge]: https://github.com/JaredTao/TaoThread/workflows/MacOS/badge.svg "MacOS"

[android-link]: https://github.com/JaredTao/TaoThread/actions?query=workflow%3AAndroid "AndroidAction"
[android-badge]: https://github.com/JaredTao/TaoThread/workflows/Android/badge.svg "Android"

[ios-link]: https://github.com/JaredTao/TaoThread/actions?query=workflow%3AIOS "IOSAction"
[ios-badge]: https://github.com/JaredTao/TaoThread/workflows/IOS/badge.svg "IOS"

[release-link]: https://github.com/jaredtao/TaoThread/releases "Release status"
[release-badge]: https://img.shields.io/github/release/jaredtao/TaoThread.svg?style=flat-square "Release status"

[download-link]: https://github.com/jaredtao/TaoThread/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/jaredtao/TaoThread/total.svg?style=flat-square "Download status"

[license-link]: https://github.com/jaredtao/TaoThread/blob/master/LICENSE "LICENSE"
[license-badge]: https://img.shields.io/badge/license-MIT-blue.svg "MIT"


[issues-link]: https://github.com/jaredtao/TaoThread/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[wiki-links]: https://github.com/jaredtao/TaoThread/wiki "wiki"
[wiki-badge]: https://img.shields.io/badge/github-wiki-181717.svg?maxAge=60 "wiki"

## 简介

对Qt QThread的简单封装。

Qt的线程有两种场景。

1. 如果是比较独立的任务，不需要交互的，直接QThread::run, 或者QRunable, QtConcurrent都可以。

2. 如果是要交互的，就得QObject moveToThread，或者别的什么事件机制的写法。

有交互的情况下，如果用前面的那几种，得自己管事件循环，不然会把UI卡住。

所以我做了一些封装，使线程不会卡住UI，还能像run一样方便。

## 说明

2019/6/13更新 

直接调用封装好的 ThreadPool::getInstance()->work() 函数即可，传递两个lambda作为参数。
第一个lambda是线程池里运行的任务。需要返回任务结果为true 或者false。
第二个lambda是任务结束后，在主线程执行的任务。带一个bool参数为任务的执行结果。
```C++
ThreadPool::getInstance()->work(
                [id](){
                    qWarning() << QThread::currentThreadId() << "do" << id;
                    return true;
                },
                [this, id](bool result){
                    qWarning() <<QThread::currentThreadId() << "work result:" << result << id;
                    showId();
                });
```
![](preview.png)

--

2019/9/30 更新
ThreadPool没有取消操作，直接杀线程会对ThreadPool造成不良的影响。
为了支持取消操作，封装了Controller-worker模型。杀QThread是可控的。
```C++
taskId = ThreadController::getInstance()->work(
                [id](){
                    qWarning() << QThread::currentThreadId() << "do" << id;
                    return true;
                },
                [this, id](bool result){
                    qWarning() <<QThread::currentThreadId() << "work result:" << result << id;
                    showId();
                });
```
取消操作是
```C++
ThreadController::getInstance()->cancel(taskId);
```


对Qt QThread的简单封装。

Qt的线程有两种场景。

如果是比较独立的任务，不需要交互的，直接QThread::run, 或者QRunable, QtConcurrent都可以。

如果是要交互的，就得QObject moveToThread，或者别的什么事件机制的写法。有交互的情况下，如果用前面的那几种，得自己管事件循环，不然会把UI卡住。

而我这种signal(lambda)的玩法，不会卡住UI，还能像run一样方便。

![](preview.png)
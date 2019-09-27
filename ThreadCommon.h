#pragma once
#include <QRunnable>
#include <QThreadPool>
#include <QThread>
#include <functional>
namespace TaoThread
{
using WorkCallback = std::function<bool()>;
using WorkResultCallback = std::function<void(bool)>;

}

#include "ConcurrentQueue.h"
#include "XOPStandardHeaders.h"

ConcurrentQueue<std::string> queue;

void XOPNotice_ts(std::string str)
{
  queue.push(str);
}

void XOPNotice_ts(const char *noticeStr)
{
  XOPNotice_ts(std::string(noticeStr));
}

void OutputQueuedNotices()
{
  if(!RunningInMainThread())
  {
    return;
  }

  queue.apply_to_all([](std::string str) { XOPNotice(str.c_str()); });
}

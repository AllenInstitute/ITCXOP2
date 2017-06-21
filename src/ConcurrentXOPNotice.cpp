#include "ConcurrentQueue.h"
#include "XOPStandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

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

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef MEM_USAGE_H
#define MEM_USAGE_H

#ifdef __linux__
// #include <proc/readproc.h>
// #include <unistd.h>
// // #include <sys/resource.h>
#include <sys/sysinfo.h>
#endif

#ifdef __APPLE__
#include <mach/task.h>
#include <mach/mach_traps.h>
#include <mach/mach.h>
#include <unistd.h>
#include <err.h>
#include <sys/param.h>
#include <mach-o/ldsyms.h>
#endif

/**
 * @ingroup ndn-helpers
 * @brief Utility class to evaluate current usage of RAM
 */
class MemUsage {
public:
  /**
   * @brief Get memory utilization in bytes
   */
  static inline int64_t
  Get()
  {
#if defined(__linux__)
    /*
    /proc/[pid]/statm
                  Provides information about memory usage, measured in pages.  The
                  columns are:

                      size       (1) total program size
                                 (same as VmSize in /proc/[pid]/status)
                      resident   (2) resident set size
                                 (same as VmRSS in /proc/[pid]/status)
                      share      (3) shared pages (i.e., backed by a file)
                      text       (4) text (code)
                      lib        (5) library (unused in Linux 2.6)
                      data       (6) data + stack
                      dt         (7) dirty pages (unused in Linux 2.6)

    Reference: http://man7.org/linux/man-pages/man5/proc.5.html
    */
    std::ifstream is("/proc/self/statm");
    if (!is.bad() && !is.eof()) {
      unsigned long vm;
      unsigned long rss;
      is >> vm  // the first number: virtual memory
        >> rss; // the second number: resident set size

      return rss * getpagesize();
    }
    else {
      return -1;
    }

#elif defined(__APPLE__)
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS
        != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count)) {
      return -1; // something is wrong
    }

    return t_info.resident_size;
#endif
    // other systems are not yet supported
    return -1;
  }
};

#endif // MEM_USAGE_H

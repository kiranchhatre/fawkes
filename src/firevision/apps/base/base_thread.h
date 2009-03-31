
/***************************************************************************
 *  base_thread.h - FireVision Base Thread
 *
 *  Created: Tue May 29 16:40:10 2007
 *  Copyright  2006-2007  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL file in the doc directory.
 */

#ifndef __FIREVISION_APPS_BASE_BASE_THREAD_H_
#define __FIREVISION_APPS_BASE_BASE_THREAD_H_

#include <core/threading/thread.h>
#include <core/threading/thread_notification_listener.h>
#include <core/utils/lock_map.h>

#include <aspect/blocked_timing.h>
#include <aspect/logging.h>
#include <aspect/vision_master.h>
#include <aspect/clock.h>
#include <aspect/thread_producer.h>
#include <aspect/configurable.h>

#include <fvutils/base/vision_master.h>
#include <string>

class FvAcquisitionThread;
namespace fawkes {
  class Barrier;
}

class FvBaseThread
: public fawkes::Thread,
  public fawkes::BlockedTimingAspect,
  public fawkes::LoggingAspect,
  public fawkes::VisionMasterAspect,
  public fawkes::ClockAspect,
  public fawkes::ThreadProducerAspect,
  public fawkes::ConfigurableAspect,
  public fawkes::ThreadNotificationListener,
  public VisionMaster
{
 public:
  FvBaseThread();
  virtual ~FvBaseThread();

  virtual void init();
  virtual void loop();
  virtual void finalize();

  virtual VisionMaster *  vision_master();

  virtual Camera *  register_for_camera(const char *camera_string,
					fawkes::Thread *thread,
					bool raw = false);
  virtual void      unregister_thread(fawkes::Thread *thread);

  virtual bool thread_started(fawkes::Thread *thread) throw();
  virtual bool thread_init_failed(fawkes::Thread *thread) throw();

 private:
  void cond_recreate_barrier(unsigned int num_cyclic_threads);

 private:
  fawkes::LockMap<std::string, FvAcquisitionThread *> aqts;
  fawkes::LockMap<std::string, FvAcquisitionThread *>::iterator ait;
  unsigned int _aqt_timeout;

  fawkes::LockMap<Thread *, FvAcquisitionThread *> started_threads;

  fawkes::Barrier *aqt_barrier;
};


#endif

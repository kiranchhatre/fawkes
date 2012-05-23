
/***************************************************************************
 *  act_thread.h - Robotino act thread
 *
 *  Created: Sun Nov 13 16:05:34 2011
 *  Copyright  2011  Tim Niemueller [www.niemueller.de]
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

#ifndef __PLUGINS_ROBOTINO_ACT_THREAD_H_
#define __PLUGINS_ROBOTINO_ACT_THREAD_H_

#include <core/threading/thread.h>
#include <aspect/blocked_timing.h>
#include <aspect/logging.h>
#include <aspect/configurable.h>
#include <aspect/blackboard.h>

#include <string>

namespace rec {
  namespace robotino {
    namespace com {
      class Com;
      class OmniDrive;
    }
  }
}

namespace fawkes {
  class MotorInterface;
}

class RobotinoSensorThread;

class RobotinoActThread
: public fawkes::Thread,
  public fawkes::BlockedTimingAspect,
  public fawkes::LoggingAspect,
  public fawkes::ConfigurableAspect,
  public fawkes::BlackBoardAspect
{
 public:
  RobotinoActThread(RobotinoSensorThread *sensor_thread);

  virtual void init();
  virtual void loop();
  virtual void finalize();

 /** Stub to see name in backtrace for easier debugging. @see Thread::run() */
 protected: virtual void run() { Thread::run(); }

 private:
  RobotinoSensorThread           *sensor_thread_;
  rec::robotino::com::Com        *com_;
  rec::robotino::com::OmniDrive  *omni_drive_;
  unsigned int                    last_seqnum_;
  fawkes::MotorInterface         *motor_if_;

  float        cfg_max_vx_;
  float        cfg_max_vy_;
  float        cfg_max_omega_;
};


#endif
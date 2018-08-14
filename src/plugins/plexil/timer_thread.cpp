
/***************************************************************************
 *  timer_thread.cpp - timer thread
 *
 *  Created: Mon Aug 13 16:21:32 2018
 *  Copyright  2006-2018  Tim Niemueller [www.niemueller.de]
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

#include "timer_thread.h"

#include <core/threading/mutex.h>
#include <core/threading/mutex_locker.h>
#include <core/threading/wait_condition.h>

using namespace fawkes;

/** @class PlexilTimerThread "timer_thread.h"
 * Timer support class.
 * @author Tim Niemueller
 */

/** Constructor. */
PlexilTimerThread::PlexilTimerThread()
: Thread("PlexilTimerThread", Thread::OPMODE_WAITFORWAKEUP)
{
	mutex_ = new Mutex();
	waitcond_ = new WaitCondition(mutex_);
	aborted_ = false;
}


/** Empty destructor. */
PlexilTimerThread::~PlexilTimerThread()
{
}

void
PlexilTimerThread::loop()
{
	fawkes::MutexLocker lock(mutex_);

	aborted_ = false;
	bool woken = false;
	while (! woken && ! aborted_) {
		woken = waitcond_->abstimed_wait(wait_until_.get_sec(), wait_until_.get_nsec());
	}
	if (! aborted_) {
		lock.unlock();
		listener_->timer_event();
	}
}

/** Start timer non-blocking.
 * This starts a timer for an absolute timer.
 * Invokes listener once timer is up.
 * @param listener listener to notify on timer event
 * @param wait_until point in time to wait for, must be in the future or timer event never occurs.
 */
void
PlexilTimerThread::start_timer(CallbackListener *listener, const fawkes::Time &wait_until)
{
	fawkes::MutexLocker lock(mutex_);
	wait_until_ = wait_until;
	fawkes::Time now(Clock::instance());
	listener_ = listener;
	wakeup();
}


/** Abort a currently running timer. */
void
PlexilTimerThread::abort_timer()
{
	mutex_->lock();
	aborted_ = true;
	waitcond_->wake_all();
	mutex_->unlock();
}

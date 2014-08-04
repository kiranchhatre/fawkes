
/***************************************************************************
 *  act_thread.cpp - Kinova plugin Act Thread
 *
 *  Created: Tue Jun 04 13:13:20 2013
 *  Copyright  2013  Bahram Maleki-Fard
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

#include "act_thread.h"
#include "types.h"
#include "arm_kindrv.h"
#include "arm_dummy.h"

#include <interfaces/JacoInterface.h>

using namespace fawkes;

/** @class KinovaActThread "act_thread.h"
 * Jaco Arm control thread.
 *
 * @author Bahram Maleki-Fard
 */

/** Constructor.
 * @param thread_name thread name
 */
KinovaActThread::KinovaActThread(KinovaInfoThread *info_thread,
                                 KinovaGotoThread *goto_thread,
                                 KinovaGotoThread *goto_thread_2nd,
                                 KinovaOpenraveBaseThread *openrave_thread)
  : Thread("KinovaActThread", Thread::OPMODE_WAITFORWAKEUP),
    BlockedTimingAspect(BlockedTimingAspect::WAKEUP_HOOK_ACT)
{
  __arm.arm = NULL;
  __arm.iface = NULL;
  __dual_arm.left.arm = NULL;
  __dual_arm.left.iface = NULL;
  __dual_arm.right.arm = NULL;
  __dual_arm.right.iface = NULL;

  __info_thread = info_thread;
  __openrave_thread = openrave_thread;
  if( goto_thread_2nd == NULL ) {
    // single-arm config
    __arm.goto_thread = goto_thread;
  } else {
    // dual-arm config
    __dual_arm.left.goto_thread = goto_thread;
    __dual_arm.right.goto_thread = goto_thread_2nd;
  }

  _submit_iface_changes = NULL;
  _is_initializing = NULL;
  _process_msgs = NULL;
}


/** Destructor. */
KinovaActThread::~KinovaActThread()
{
}

/** Initialize.
 * Depending on single or dual_arm setup (defined by config flag),
 * appropriate arms are loaded and then initialized if required to.
 * This method also sets the correct function pointers that are used in
 * the main loop() method. */
void
KinovaActThread::init()
{
  __cfg_auto_init       = config->get_bool("/hardware/jaco/auto_initialize");
  __cfg_auto_calib      = config->get_bool("/hardware/jaco/auto_calibrate");
  __cfg_is_dual_arm     = config->get_bool("/hardware/jaco/dual_arm/active");

  std::string cfg_arm = config->get_string("/hardware/jaco/arm");

  if( cfg_arm.compare("libkindrv")
   && cfg_arm.compare("dummy") )
    throw fawkes::Exception("Bad config entry /hardware/jaco/arm '%s'", cfg_arm.c_str());

  if(__cfg_is_dual_arm) {
    // set function pointers for dual-arm setup
    _submit_iface_changes = &KinovaActThread::_submit_iface_dual;
    _is_initializing = &KinovaActThread::_is_initializing_dual;
    _process_msgs = &KinovaActThread::_process_msgs_dual;

    // dual-arm setup needs additional setup and config entries
    std::string l_name  = config->get_string("/hardware/jaco/dual_arm/left/name");
    std::string l_iface = config->get_string("/hardware/jaco/dual_arm/left/interface");
    std::string r_name  = config->get_string("/hardware/jaco/dual_arm/right/name");
    std::string r_iface = config->get_string("/hardware/jaco/dual_arm/right/interface");

    // create the two JacoArm objects and assign left/right correctly
    try {
      std::vector<KinovaArm*> arms;
      if( !cfg_arm.compare("dummy") ) {
        __dual_arm.left.arm  = new KinovaArmDummy(l_name.c_str());
        __dual_arm.right.arm = new KinovaArmDummy(r_name.c_str());

      } else {
        arms.push_back( new KinovaArmKindrv() );
        arms.push_back( new KinovaArmKindrv() );

        for( unsigned int i=0; i<arms.size(); ++i) {
          logger->log_debug(name(), "compare l_name '%s' to '%s' ...", l_name.c_str(), arms[i]->get_name().c_str());
          if( l_name.compare(arms[i]->get_name()) == 0) {
            __dual_arm.left.arm = arms[i];
            arms.erase(arms.begin() + i);
            logger->log_info(name(), "Successfully connected arm '%s' as left arm", __dual_arm.left.arm->get_name().c_str());
            break;
          }
        }
        for( unsigned int i=0; i<arms.size(); ++i) {
          logger->log_debug(name(), "compare r_name '%s' to '%s' ...", r_name.c_str(), arms[i]->get_name().c_str());
          if( r_name.compare(arms[i]->get_name()) == 0) {
            __dual_arm.right.arm = arms[i];
            arms.erase(arms.begin() + i);
            logger->log_info(name(), "Successfully connected arm '%s' as right arm", __dual_arm.right.arm->get_name().c_str());
            break;
          }
        }
        if( arms.size() > 0 )
          logger->log_error(name(), "Could not associate %u arms! Check arm names in config, first unassociated is '%s'",
                            arms.size(), arms[0]->get_name().c_str());

      }
    } catch(fawkes::Exception &e) {
      logger->log_error(name(), "Could not connect to both JacoArms. Exception follows.");
      throw;
    }

    // open interface for writing
    try {
      __dual_arm.left.iface  = blackboard->open_for_writing<JacoInterface>(l_iface.c_str());
      __dual_arm.right.iface = blackboard->open_for_writing<JacoInterface>(r_iface.c_str());

    } catch(fawkes::Exception &e) {
      logger->log_warn(name(), "Could not open JacoInterfaces interface for writing. Exception follows.");
      delete __dual_arm.left.arm;
      delete __dual_arm.right.arm;
      throw;

    }

    // register arms in other threads
    __info_thread->register_arm(&__dual_arm.left);
    __info_thread->register_arm(&__dual_arm.right);
    __openrave_thread->register_arm(&__dual_arm.left);
    __openrave_thread->register_arm(&__dual_arm.right);
    __dual_arm.left.goto_thread->register_arm(&__dual_arm.left);
    __dual_arm.right.goto_thread->register_arm(&__dual_arm.right);

    // initialize arms
    _initialize_dual();

  } else {
    // single arm setup; need less considerations, just connect to first arm
    _submit_iface_changes = &KinovaActThread::_submit_iface_single;
    _is_initializing = &KinovaActThread::_is_initializing_single;
    _process_msgs = &KinovaActThread::_process_msgs_single;

    try {
      if( !cfg_arm.compare("dummy") ) {
        __arm.arm = new KinovaArmDummy("JacoDummy");
      } else {
        __arm.arm = new KinovaArmKindrv();
      }

    } catch(fawkes::Exception &e) {
      logger->log_error(name(), "Could not connect to JacoArm. Exception follows.");
      throw;
    }

    // open interface for writing
    try {
      __arm.iface = blackboard->open_for_writing<JacoInterface>("JacoArm");
    } catch(fawkes::Exception &e) {
      logger->log_error(name(), "Could not open JacoInterface interface for writing. Exception follows.");
      delete __arm.arm;
      throw;
    }

    // register arm in other threads
    __info_thread->register_arm(&__arm);
    __openrave_thread->register_arm(&__arm);
    __arm.goto_thread->register_arm(&__arm);

    // initalize arms
    _initialize_single();
  }
}

/** Finalize.
 * Close all writing interfaces and delete JacoArm instances.
 */
void
KinovaActThread::finalize()
{
  if( __cfg_is_dual_arm ) {
    try {
      blackboard->close(__dual_arm.left.iface);
      blackboard->close(__dual_arm.right.iface);
    } catch(fawkes::Exception& e) {
      logger->log_warn(name(), "Could not close JacoInterface interfaces. Er:%s", e.what_no_backtrace());
    }

    delete __dual_arm.left.arm;
    delete __dual_arm.right.arm;

  } else {
    try {
      blackboard->close(__arm.iface);
    } catch(fawkes::Exception& e) {
      logger->log_warn(name(), "Could not close JacoInterface interface. Er:%s", e.what_no_backtrace());
    }

    delete __arm.arm;
  }
}

/** Main loop.
 * The structure is pretty obvious, as it uses function pointers to perform the
 * action based on what kind of setup we have (single_arm or dual_arm).
 */
void
KinovaActThread::loop()
{
  // firts of all, submit interface updates (that other threads might have done)!
  (this->*_submit_iface_changes)();

  // check if still initializing
  if( (this->*_is_initializing)() )
    return;

  // process incoming interface messages
  (this->*_process_msgs)();

  // finally, again submit interface updates
  (this->*_submit_iface_changes)();
}



/* ##########################################################################
 *  private methods , referenced to by the function pointers from loop().
 *
 *  we have one for each single_arm and dual_arm setup.
 * ########################################################################## */
/** Initialize and/or calibrate single arm, if requested by config flags */
void
KinovaActThread::_initialize_single()
{
  //check if we need to initialize arm
  if( !__arm.arm->initialized() && __cfg_auto_init ) {
    logger->log_debug(name(), "Initializing arm, wait until finished");
    __arm.arm->initialize();
    __arm.iface->set_final(false);
    //__arm.goto_thread->pos_ready();

  } else if( __arm.arm->initialized() && __cfg_auto_calib ) {
    __arm.goto_thread->pos_ready();
  }

  __arm.iface->set_initialized(__arm.arm->initialized());
  __arm.iface->write();
}


/** Initialize and/or calibrate both arms, if requested by config flags */
void
KinovaActThread::_initialize_dual()
{
  //check initialization status
  if( !__dual_arm.left.arm->initialized() && __cfg_auto_init ) {
    logger->log_debug(name(), "Initializing left arm, wait until finished");
    __dual_arm.left.arm->initialize();
    __dual_arm.left.iface->set_final(false);
    //__dual_arm.left.goto_thread->pos_ready();

  } else if( __dual_arm.left.arm->initialized() && __cfg_auto_calib ) {
    __dual_arm.left.goto_thread->pos_ready();
  }

  if( !__dual_arm.right.arm->initialized() && __cfg_auto_init ) {
    logger->log_debug(name(), "Initializing right arm, wait until finished");
    __dual_arm.right.arm->initialize();
    __dual_arm.right.iface->set_final(false);
    __dual_arm.right.goto_thread->pos_ready();

  } else if( __dual_arm.right.arm->initialized() && __cfg_auto_calib ) {
    __dual_arm.right.goto_thread->pos_ready();
  }

  __dual_arm.left.iface->set_initialized(__dual_arm.left.arm->initialized());
  __dual_arm.left.iface->write();
  __dual_arm.right.iface->set_initialized(__dual_arm.right.arm->initialized());
  __dual_arm.right.iface->write();
}


/** Check if arm is being initialized. */
bool
KinovaActThread::_is_initializing_single()
{
  __arm.iface->set_initialized(__arm.arm->initialized());

  if( !__arm.arm->initialized() && __cfg_auto_init ) {
    logger->log_debug(name(), "wait for arm to initialize");
    //__arm.initialized = __arm.iface->is_final();
    return true;
  }

  return false;
}

/** Check if any of the two arms is being initialized. */
bool
KinovaActThread::_is_initializing_dual()
{
  __dual_arm.left.iface->set_initialized(__dual_arm.left.arm->initialized());
  __dual_arm.right.iface->set_initialized(__dual_arm.right.arm->initialized());

  if( !(__dual_arm.left.arm->initialized() && __dual_arm.right.arm->initialized()) && __cfg_auto_init ) {
    logger->log_debug(name(), "wait for arms to initialize");
    //__dual_arm.left.initialized = __dual_arm.left.iface->is_final();
    //__dual_arm.right.initialized = __dual_arm.right.iface->is_final();
    return true;
  }

  return false;
}

/** Submit changes made to JacoInterface for the single arm.
 * Not much done here. */
void
KinovaActThread::_submit_iface_single()
{
  __arm.iface->write();
}

/** Submit changes made to JacoInterfaces for the both left and right arm.
 * Not much done here. */
void
KinovaActThread::_submit_iface_dual()
{
  __dual_arm.left.iface->write();
  __dual_arm.right.iface->write();
}


/** Process messages received for single-arm setup. */
void
KinovaActThread::_process_msgs_single()
{
  _process_msgs_arm(__arm);
}


/** Process messages received for dual-arm setup.
 * No performance yet, as it needs appropriate goto_thread, and probably even
 * a new Interface or new fields for JacoInterface.
*/
void
KinovaActThread::_process_msgs_dual()
{
  _process_msgs_arm(__dual_arm.left);
  _process_msgs_arm(__dual_arm.right);
}

/** Process messages for a given arm.
 * At the moment, only use OpenRAVE for single-arm movement
 * @param arm The struct for the currently using arm
*/
void
KinovaActThread::_process_msgs_arm(jaco_arm_t &arm)
{
  while( ! arm.iface->msgq_empty() ) {
    Message *m = arm.iface->msgq_first(m);
    arm.iface->set_msgid(m->id());
    arm.iface->set_final(false);
    arm.iface->write();

    if( arm.iface->msgq_first_is<JacoInterface::StopMessage>() ) {
      JacoInterface::StopMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: StopMessage rcvd", arm.iface->id());

      arm.goto_thread->stop();

    } else if( arm.iface->msgq_first_is<JacoInterface::CalibrateMessage>() ) {
      JacoInterface::CalibrateMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: CalibrateMessage rcvd", arm.iface->id());

      arm.goto_thread->pos_ready();

    } else if( arm.iface->msgq_first_is<JacoInterface::RetractMessage>() ) {
      JacoInterface::RetractMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: RetractMessage rcvd", arm.iface->id());

      arm.goto_thread->pos_retract();

    } else if( arm.iface->msgq_first_is<JacoInterface::CartesianGotoMessage>() ) {
      JacoInterface::CartesianGotoMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: CartesianGotoMessage rcvd. x:%f  y:%f  z:%f  e1:%f  e2:%f  e3:%f", arm.iface->id(),
                        msg->x(), msg->y(), msg->z(), msg->e1(), msg->e2(), msg->e3());
    #ifdef HAVE_OPENRAVE
      logger->log_debug(name(), "%s: CartesianGotoMessage is being passed to openrave", arm.iface->id());
      std::vector<float> v = __openrave_thread->set_target(msg->x(), msg->y(), msg->z(), msg->e1(), msg->e2(), msg->e3(), &arm);
      if( v.size() == 6 )
        arm.goto_thread->set_target_ang(v.at(0), v.at(1), v.at(2), v.at(3), v.at(4), v.at(5));
    #else
      arm.goto_thread->set_target(msg->x(), msg->y(), msg->z(), msg->e1(), msg->e2(), msg->e3());
    #endif

    } else if( arm.iface->msgq_first_is<JacoInterface::AngularGotoMessage>() ) {
      JacoInterface::AngularGotoMessage *msg = arm.iface->msgq_first(msg);

      logger->log_debug(name(), "%s: AngularGotoMessage rcvd. x:%f  y:%f  z:%f  e1:%f  e2:%f  e3:%f", arm.iface->id(),
                        msg->j1(), msg->j2(), msg->j3(), msg->j4(), msg->j5(), msg->j6());
      arm.goto_thread->set_target_ang(msg->j1(), msg->j2(), msg->j3(), msg->j4(), msg->j5(), msg->j6());

    } else if( arm.iface->msgq_first_is<JacoInterface::MoveGripperMessage>() ) {
      JacoInterface::MoveGripperMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: MoveGripperMessage rcvd. f1:%f  f2:%f  f3:%f", arm.iface->id(),
                        msg->finger1(), msg->finger2(), msg->finger3());

      arm.goto_thread->move_gripper(msg->finger1(), msg->finger2(), msg->finger3());

    } else if( arm.iface->msgq_first_is<JacoInterface::JoystickPushMessage>() ) {
      JacoInterface::JoystickPushMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: JoystickPush %u rcvd", arm.iface->id(), msg->button());

      arm.arm->push_joystick(msg->button());

    } else if( arm.iface->msgq_first_is<JacoInterface::JoystickReleaseMessage>() ) {
      JacoInterface::JoystickReleaseMessage *msg = arm.iface->msgq_first(msg);
      logger->log_debug(name(), "%s: JoystickRelease rcvd", arm.iface->id());

      arm.arm->release_joystick();
      arm.iface->set_final(true);

    } else {
      logger->log_warn(name(), "%s: Unknown message received", arm.iface->id());
    }

    arm.iface->msgq_pop();
  }
}


/***************************************************************************
 *  cmdvel_plugin.cpp - Translate ROS Twist messages to Navgiator transrot
 *
 *  Created: Fri Jun 1 13:29:39 CEST 2012
 *  Copyright  2012  Sebastian Reuter
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

#include <core/plugin.h>

#include "cmdvel_thread.h"

using namespace fawkes;

class ROSCmdVelPlugin : public fawkes::Plugin
{
 public:
  /** Constructor.
   * @param config Fawkes configuration
   */
	ROSCmdVelPlugin(Configuration *config)
    : Plugin(config)
  {
    thread_list.push_back(new ROSCmdVelThread());
  }
};

PLUGIN_DESCRIPTION("executes cmd_vel Topic Commands from ROS")
EXPORT_PLUGIN(ROSCmdVelPlugin)

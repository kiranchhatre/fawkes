
/***************************************************************************
 *  vision_master.h - FireVision Vision Master
 *
 *  Created: Wed May 30 10:28:06 2007
 *  Copyright  2006-2007  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __FIREVISION_FVUTILS_BASE_VISION_MASTER_H_
#define __FIREVISION_FVUTILS_BASE_VISION_MASTER_H_

class Camera;
class Thread;

class VisionMaster
{
 public:
  virtual ~VisionMaster();

  virtual Camera *  register_for_camera(const char *camera_string,
					Thread *thread, bool raw = false) = 0;
  virtual void      unregister_thread(Thread *thread)                     = 0;
};



#endif

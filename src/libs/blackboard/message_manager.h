
/***************************************************************************
 *  message_manager.h - BlackBoard message manager
 *
 *  Created: Fri Oct 06 11:29:51 2006
 *  Copyright  2006-2008  Tim Niemueller [www.niemueller.de]
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
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#ifndef __BLACKBOARD_MESSAGE_MANAGER_H_
#define __BLACKBOARD_MESSAGE_MANAGER_H_

#include <interface/mediators/message_mediator.h>

class BlackBoardInterfaceManager;
class Message;

class BlackBoardMessageManager : public MessageMediator
{
 friend class BlackBoard;
 public:
  BlackBoardMessageManager();
  ~BlackBoardMessageManager();

  virtual unsigned int transmit(Message *message);

 private:
  BlackBoardInterfaceManager *__im;

  void set_interface_manager(BlackBoardInterfaceManager *im);
};



#endif

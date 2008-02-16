
/***************************************************************************
 *  network_manager.h - Fawkes network manager
 *
 *  Created: Wed Nov 15 23:52:40 2006
 *  Copyright  2006  Tim Niemueller [www.niemueller.de]
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

#ifndef __FAWKES_NETWORK_MANAGER_H_
#define __FAWKES_NETWORK_MANAGER_H_

class FawkesThreadManager;
class FawkesNetworkServerThread;
class FawkesNetworkHandler;
class FawkesNetworkHub;
class AvahiThread;
class NetworkNameResolver;
class ServicePublisher;
class ServiceBrowser;

class FawkesNetworkManager
{
 public:
  FawkesNetworkManager(FawkesThreadManager *thread_manager, unsigned short int fawkes_port);
  ~FawkesNetworkManager();

  FawkesNetworkHub *     hub();
  NetworkNameResolver *  nnresolver();
  ServicePublisher *     service_publisher();
  ServiceBrowser *       service_browser();

 private:
  unsigned short int          fawkes_port;
  FawkesThreadManager        *thread_manager;
  FawkesNetworkServerThread  *fawkes_network_thread;
  AvahiThread                *avahi_thread;

  NetworkNameResolver        *_nnresolver;
  ServicePublisher           *_service_publisher;
  ServiceBrowser             *_service_browser;
};

#endif

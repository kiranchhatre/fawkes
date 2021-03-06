
/***************************************************************************
 *  configurable.h - Configurable aspect for Fawkes
 *
 *  Created: Fri Jan 12 14:33:10 2007
 *  Copyright  2006-2010  Tim Niemueller [www.niemueller.de]
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#ifndef _ASPECT_CONFIGURABLE_H_
#define _ASPECT_CONFIGURABLE_H_

#include <aspect/aspect.h>
#include <config/config.h>

namespace fawkes {

class ConfigurableAspect : public virtual Aspect
{
 public:
  ConfigurableAspect();
  virtual ~ConfigurableAspect();

  void init_ConfigurableAspect(Configuration *config);

 protected:
  Configuration *config;
};

} // end namespace fawkes

#endif

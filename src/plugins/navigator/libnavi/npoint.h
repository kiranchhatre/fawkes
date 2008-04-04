
/***************************************************************************
 *  npoint.h - Navigator Point
 *
 *  Generated: Tue Jun 05 13:57:09 2007
 *  Copyright  2007  Martin Liebenberg
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
 
#ifndef __NAVIGATOR_LIBNAVI_NPOINT_H_
#define __NAVIGATOR_LIBNAVI_NPOINT_H_

class NPoint
{
 public:
  NPoint();
  virtual ~NPoint();
      
  NPoint(double x, double y);
         
  NPoint &operator=(const NPoint &p);
  NPoint operator+(const NPoint& p) const;
  NPoint& operator+=(const NPoint& p);
  NPoint operator-(const NPoint& p) const;
  NPoint& operator-=(const NPoint& p);
  NPoint operator*(const float& f) const;
  NPoint& operator*=(const float& f);
         
  bool operator==(const NPoint &p);
         
  double x;
  double y;
};

#endif

#*****************************************************************************
#   Makefile Build System for Fawkes: Config Settings specific to Fawkes
#                            -------------------
#   Created on Tue Apr 10 15:29:29 2007
#   Copyright (C) 2006-2007 by Tim Niemueller, AllemaniACs RoboCup Team
#
#*****************************************************************************
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#*****************************************************************************

SYSROOT=

# Add -DDEBUG_THREADING if you run into threading problems like deadlocks.
# Read FawkesDebugging in the Fawkes Trac Wiki on how to use it
CFLAGS_BASE +=	-g -fPIC \
		-Wall -Werror -DBINDIR=\"$(EXEC_BINDIR)\" \
		-DLIBDIR=\"$(EXEC_LIBDIR)\" -DPLUGINDIR=\"$(EXEC_PLUGINDIR)\" \
		-DCONFDIR=\"$(EXEC_CONFDIR)\" -DRESDIR=\"$(EXEC_RESDIR)\" \
		-DBUILDTYPE=\"$(BUILD_TYPE)\" -DLOGDIR=\"$(EXEC_LOGDIR)\"

### Feature checks
ifneq ($(realpath $(BASEDIR)/src/firevision),)
  HAVE_FIREVISION=1
  CFLAGS_BASE += -DHAVE_FIREVISION
endif

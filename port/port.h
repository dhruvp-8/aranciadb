// Copyright (c) 2019 Arancia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_ARANCIA_PORT_PORT_H_
#define STORAGE_ARANCIA_PORT_PORT_H_

#include <string.h>

// Include the appropriate platform specific file below.  If you are
// porting to a new platform, see "port_example.h" for documentation
// of what the new port_<platform>.h file must provide.
#if defined(LEVELDB_PLATFORM_POSIX)
#  include "port/port_posix.h"
#endif

#endif  // STORAGE_ARANCIA_PORT_PORT_H_

#pragma once

// vautogen is a utility in 01-Development Utilities\vautogen.
// The INI-file controls the major, minor and patch numbers.
// vautogen automatically increments the version number each time it is
// called, and creates a header file containing the definitions:
//   KEX_VERSION_DW (written to registry as InstalledVersion)
//   KEX_VERSION_FV (placed in version resources, see .rc files)
//   KEX_VERSION_STR (displayed to users)
//
// Since KexVer.h is included in basically every VxKex project (and the build
// number auto-increments), allowing vautogen.h to be included all the time
// would cause builds to be longer and more annoying. Therefore it is only
// included in resource files (where version resources are defined) and in
// specific .c files that need it (define NEED_VERSION_DEFS before the inclusion
// of any headers).

#if defined(RC_INVOKED) || defined(NEED_VERSION_DEFS)
#  include <vautogen.h>
#endif

#define KEX_WEB_STR			"https://github.com/dotexe1337/VxKex"
#define KEX_BUGREPORT_STR	"https://github.com/dotexe1337/VxKex/issues/new/choose"

// You must keep the following line blank or RC will screech.

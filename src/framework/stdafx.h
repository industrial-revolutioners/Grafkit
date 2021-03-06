#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX

#include "common.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//#pragma warning(push)
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4290)
#pragma warning(disable : 4305)
#pragma warning(disable : 4838)
#pragma warning(disable : 5040)

#ifdef __cplusplus
#include "core/reference.h"
#include "core/memory.h"
#include "core/thread.h"
#include "core/exceptions.h"
#include "core/logger.h"
#include "render/dxtypes.h"
#include "render/renderer.h"
#include "utils/persistence/dynamics.h"
#include "utils/persistence/persistence.h"
#endif //__cplusplus

//#pragma warning(pop)

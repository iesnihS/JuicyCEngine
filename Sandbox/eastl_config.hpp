#pragma once

#define EA_PRAGMA_ONCE_SUPPORTED

#include <EASTL/vector.h>

extern void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
extern void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
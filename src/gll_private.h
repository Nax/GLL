#ifndef GLL_PRIVATE_H
#define GLL_PRIVATE_H

#include <GLL/GLL_API.h>

GLL_API void    GLL_CreateDummyContext(void);
GLL_API void    GLL_DestroyDummyContext(void);
GLL_API void*   GLL_LoadFunction(const char* name);
GLL_API void    GLL_LoadAllFunctions(void);

#endif

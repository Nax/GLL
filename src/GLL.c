#include <gll/gll.h>
#include "gll_private.h"

GLL_EXTERN void GLL_LoadOpenGL(void)
{
    // Make sure we don't init twice
    static int didInitOpenGL = 0;

    if (didInitOpenGL)
        return;

    GLL_CreateDummyContext();
    GLL_LoadAllFunctions();
    GLL_DestroyDummyContext();
    didInitOpenGL = 1;
}


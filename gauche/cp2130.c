/*
 * cp2130.c
 */

#include "cp2130.h"

/*
 * The following function is a dummy one; replace it for
 * your C function definitions.
 */

ScmObj test_cp2130(void)
{
    return SCM_MAKE_STR("cp2130 is working");
}

/*
 * Module initialization function.
 */
extern void Scm_Init_cp2130lib(ScmModule*);

void Scm_Init_cp2130(void)
{
    ScmModule *mod;

    /* Register this DSO to Gauche */
    SCM_INIT_EXTENSION(cp2130);

    /* Create the module if it doesn't exist yet. */
    mod = SCM_MODULE(SCM_FIND_MODULE("cp2130", TRUE));

    /* Register stub-generated procedures */
    Scm_Init_cp2130lib(mod);
}

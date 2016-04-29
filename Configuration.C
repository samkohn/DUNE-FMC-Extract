#ifndef CONFIGURATION_C
#define CONFIGURATION_C
/*
 * This "macro" file contains the system-specific configuration. To use
 * the other macros/scripts on a different system, modify this file
 * accordingly.
 */

char* CFG_OutputDirectory(char* dir)
{
    return strcpy(dir, "/nashome/s/skohn/outputs/");
}

#endif

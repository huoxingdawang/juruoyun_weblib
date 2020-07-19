#ifndef __MAIN_H
#define __MAIN_H
#include "include.h"
#include <stdlib.h>
#ifdef __linux__
#define filename "testfiles/test.mp4"
#define logo_ico "testfiles/logo.ico"
#else
#define filename "testfiles\\test.mp4"
#define logo_ico "testfiles\\logo.ico"
#endif
#endif

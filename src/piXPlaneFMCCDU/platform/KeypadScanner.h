#ifndef PIXPLANEFMCCDU_SRC_KeypadScanner_H_
#define PIXPLANEFMCCDU_SRC_KeypadScanner_H_

#ifdef __x86_64__
#include "x86/KeypadScanner.h"
#elif __arm__
#include "arm/KeypadScanner.h"
#endif

#endif
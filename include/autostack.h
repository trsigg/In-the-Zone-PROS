#ifndef AUTOSTACK_INCLUDED
#define AUTOSTACK_INCLUDED

#include "motorGroupControl.h"

extern bool stacking;

void stackNewCone();
void startStackingTasks();
void stopStackingTasks();

#endif

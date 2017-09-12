#ifndef AUTOSTACK_INCLUDED
#define AUTOSTACK_INCLUDED


double liftHeight();


//#region autostacking
extern bool stacking;
extern int numCones;

void stackNewCone();
void startStackingTasks();
void stopStackingTasks();
//#endregion

#endif

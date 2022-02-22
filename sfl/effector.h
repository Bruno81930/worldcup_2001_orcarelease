#ifndef __EFFECTOR__
#define __EFFECTOR__
#include "SFLShierarchy.h" 
#include "../clangparser/coach_lang_comp.h"
class SFLSEffector
{
public:
    // TIMO:
    SFLSEffector(void);
    /// the method that invokes the right methods
    void doAction (CM_Action* action);
    /*********** the object specific methods ***********/
    void doAction(CM_ActPosition *action);
    void doAction(CM_ActHome *action);
    void doAction(CM_ActBallToRegion *action);
    void doAction(CM_ActBallToPlayer *action);
    void doAction(CM_ActMark *action);
    void doAction(CM_ActMarkLinePlayer *action);
    void doAction(CM_ActMarkLineRegion *action);
    void doAction(CM_ActOffsidesLine *action);
    void doAction(CM_ActHetType *action);
    void doAction(CM_ActNamed *action);
    void doAction(SFLS_ActPosition *action);
    void doAction(SFLS_ActState *action);
    void doAction(SFLS_ActInterceptBall *action);
    void doAction(SFLS_ActMarkLinePlayer *action);
    void doAction(SFLS_ActMark *action);
    void doAction(SFLS_ActBallToPlayer *action);
    void doAction(SFLS_ActCatchBall *action);
    void doAction(SFLS_ActMarkRegionPlayer *action);

    /// not included in the SFLShierarchy:
    //  void doAction(SFLS_ActMarkLineRegionPlayer *action);
    
    // TIMO
    bool movedalready;
};

#endif

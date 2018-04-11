/*
 *  gebicSteppingAction.hh
 *  
 *
 *  
 *	Geant496
 */
//modified 25/11/2013
//re-checked 25/11/2013

#ifndef gebicSteppingAction_h
#define gebicSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class gebicSteppingAction : public G4UserSteppingAction
{
  public:
    gebicSteppingAction();
    virtual ~gebicSteppingAction();
    G4double edep;
    G4double time;
    G4double energy;
    G4double weight;
    G4double particleName;

    virtual void UserSteppingAction(const G4Step*);
};

#endif

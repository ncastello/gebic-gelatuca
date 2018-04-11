/*
 *  gebicEventAction.hh
 *  
 *
 *
 *	Geant496
 */


#ifndef gebicEventAction_h
#define gebicEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class gebicEventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gebicEventAction : public G4UserEventAction
{
  public:
    gebicEventAction();
    virtual ~gebicEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event* anEvent);
    virtual void EndOfEventAction(const G4Event* anEvent);
    
    void SetDrawFlag(G4String val)  {fDrawFlag = val;};
    
  private:
    G4String fDrawFlag;                         // control the drawing of event
    gebicEventActionMessenger*  fEventMessenger;
};

#endif

    





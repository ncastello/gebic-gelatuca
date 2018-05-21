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

#include <vector>

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

    inline std::vector<float> * GetEdep() {return _hitCollection;};

  private:
    G4String fDrawFlag;                         // control the drawing of event
    gebicEventActionMessenger*  fEventMessenger;

    std::vector<float> * _hitCollection;
    G4int _hitCollectionID;
    G4String _hitCollectionName;

    G4double _pulseWidth;
};

#endif



/*
 *  gebicEventActionMessenger.hh
 *  
 *
 *
 *	Geant496
 */
// 

#ifndef gebicEventActionMessenger_h
#define gebicEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class gebicEventAction;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gebicEventActionMessenger: public G4UImessenger
{
  public:
    gebicEventActionMessenger(gebicEventAction*);
    virtual ~gebicEventActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    gebicEventAction*   fEventAction;   
    G4UIcmdWithAString* fDrawCmd;
};

#endif

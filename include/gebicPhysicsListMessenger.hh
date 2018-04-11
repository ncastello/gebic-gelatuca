/*
 *  gebicPhysicsListMessenger.hh
 *  
 *
 * 
 *	Geant496
 */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef gebicPhysicsListMessenger_h
#define gebicPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class gebicPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gebicPhysicsListMessenger: public G4UImessenger
{
public:
  
  gebicPhysicsListMessenger(gebicPhysicsList* );
  virtual ~gebicPhysicsListMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
  
  gebicPhysicsList* fPPhysicsList;

  G4UIdirectory*             fPhysDir;  
  G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
  G4UIcmdWithADoubleAndUnit* fElectCutCmd;
  G4UIcmdWithADoubleAndUnit* fProtoCutCmd;    
  G4UIcmdWithADoubleAndUnit* fAllCutCmd;    
  G4UIcmdWithADoubleAndUnit* fMCutCmd;
  G4UIcmdWithADoubleAndUnit* fECutCmd;
  G4UIcmdWithAString*        fPListCmd;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


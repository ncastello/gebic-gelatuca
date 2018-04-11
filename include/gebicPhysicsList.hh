/*
 *  gebicPhysicsList.hh
 *  
 *
 *  
 *	Geant496
 */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef gebicPhysicsList_h
#define gebicPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicsConstructor;
class gebicPhysicsListMessenger;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gebicPhysicsList: public G4VModularPhysicsList
{
public:
  gebicPhysicsList();
  virtual ~gebicPhysicsList();

  virtual void ConstructParticle();

  virtual void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);

  void SelectPhysicsList(const G4String& name);
  virtual void ConstructProcess();

  void SetTargetCut(G4double val);
//  void SetSupportCut(G4double val);
  void SetDetectorCut(G4double val);
  void SetCapCut(G4double val);
  void SetShieldCuCut(G4double val);
  void SetShieldPbCut(G4double val);

private:

  void AddExtraBuilders(G4bool flagHP);

  // hide assignment operator
  gebicPhysicsList & operator=(const gebicPhysicsList &right);
  gebicPhysicsList(const gebicPhysicsList&);

  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;

  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fRaddecayList;
  G4VPhysicsConstructor*  fParticleList;
  G4VPhysicsConstructor*  fHadPhysicsList;

  std::vector<G4VPhysicsConstructor*>  fHadronPhys;
  G4int fNhadcomp;  

  gebicPhysicsListMessenger* fPMessenger;
  G4ProductionCuts* fDetectorCuts;
  G4ProductionCuts* fTargetCuts;
  G4ProductionCuts* fCapCuts;
  G4ProductionCuts* fShieldCuCuts;
  G4ProductionCuts* fShieldPbCuts;
//  G4ProductionCuts* fSupportCuts;
  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


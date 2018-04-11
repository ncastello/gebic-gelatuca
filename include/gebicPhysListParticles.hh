/*
 *  gebicPhysListParticles.hh
 *  
 *
 * 
 *	Geant496
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef gebicPhysListParticles_h
#define gebicPhysListParticles_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gebicPhysListParticles : public G4VPhysicsConstructor
{
public: 
  gebicPhysListParticles(const G4String& name = "particles");
  virtual ~gebicPhysListParticles();

public: 
  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method is dummy.
  virtual void ConstructProcess() {};

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

/*
 *  gebicPrimaryGeneratorAction.cc
 *  
 *
 *  
 *	Geant496
 */
//re-checked 25/11/2013
// **********************************************************************

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "gebicPrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicPrimaryGeneratorAction::gebicPrimaryGeneratorAction()
{
  particleGun = new G4GeneralParticleSource ();
}

gebicPrimaryGeneratorAction::~gebicPrimaryGeneratorAction()
{
  delete particleGun;
}

void gebicPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
}

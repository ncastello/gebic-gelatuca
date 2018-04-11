/*
 *  gebicPrimaryGeneratorAction.hh
 *  
 *
 * 
 *	Geant496
 */
//
// **********************************************************************

#ifndef gebicPrimaryGeneratorAction_h
#define gebicPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class gebicPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  gebicPrimaryGeneratorAction();
  ~gebicPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4GeneralParticleSource* particleGun;
  
};

#endif

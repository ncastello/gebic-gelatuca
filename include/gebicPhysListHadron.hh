/*
 *  gebicPhysListHadron.hh
 *  
 *
 * 
 *  Geant496
 */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef gebicPhysListHadron_h
#define gebicPhysListHadron_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4HadronElasticProcess.hh"

#include "G4ProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4IonInelasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"


class gebicPhysListHadron : public G4VPhysicsConstructor 
{
  public:
    gebicPhysListHadron(const G4String& name = "hadron");
    virtual ~gebicPhysListHadron();

  public:
  // Construct particle and physics
    virtual void ConstructParticle() {};
  //
    virtual void ConstructProcess(); 

  private:

  G4HadronElasticProcess  fTheElasticProcess;
  G4ProtonInelasticProcess fTheProtonInelastic;
  G4NeutronInelasticProcess  fTheNeutronInelastic;
  G4HadronElasticProcess* fTheNeutronElasticProcess;
  G4HadronFissionProcess* fTheFissionProcess;
  G4HadronCaptureProcess* fTheCaptureProcess;
  G4DeuteronInelasticProcess* fTheDeuteronInelasticProcess;
  G4TritonInelasticProcess* fTheTritonInelasticProcess;
  G4AlphaInelasticProcess* fTheAlphaInelasticProcess;
  G4IonInelasticProcess* fTheIonInelasticProcess;
};

#endif

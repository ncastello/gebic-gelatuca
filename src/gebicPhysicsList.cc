/*
 *  gebicPhysicsList.cc
 *
 *
 *
 *	Geant496
 */

 //modified 20/10/2015

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "gebicPhysicsList.hh"
#include "gebicPhysicsListMessenger.hh"

#include "gebicPhysListParticles.hh"
#include "G4EmStandardPhysics.hh"
#include "gebicPhysListEmLowEnergy.hh"
#include "gebicPhysListHadron.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProductionCutsTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gebicPhysicsList::gebicPhysicsList() : G4VModularPhysicsList(),
	fCutForGamma(1.*mm), fCutForElectron(1.*mm),
	fCutForPositron(1.*mm), fHadPhysicsList(0),
	fNhadcomp(0),fDetectorCuts(0), fTargetCuts(0),
	fCapCuts(0), fShieldCuCuts(0), fShieldPbCuts(0)
	//fSupportCuts(0)
{
  G4LossTableManager::Instance();
  defaultCutValue = 1.*mm;

  fPMessenger = new gebicPhysicsListMessenger(this);

  SetVerboseLevel(1);

  //default physics
  fParticleList = new G4DecayPhysics();

  //default physics
  fRaddecayList = new G4RadioactiveDecayPhysics();

  // EM physics
  //emPhysicsList = new G4EmStandardPhysics();
  fEmPhysicsList = new G4EmLivermorePhysics();//mod 24/02/2012???
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

gebicPhysicsList::~gebicPhysicsList()
{
  delete fPMessenger;
  delete fRaddecayList;
  delete fEmPhysicsList;
  if (fHadPhysicsList) delete fHadPhysicsList;
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      delete fHadronPhys[i];
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::ConstructParticle()
{
  fParticleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::ConstructProcess()
{
  AddTransportation();
  // em
  fEmPhysicsList->ConstructProcess();
  // decays
  fParticleList->ConstructProcess();
  fRaddecayList->ConstructProcess();
  // had
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      (fHadronPhys[i])->ConstructProcess();
    }
  }
  if (fHadPhysicsList) fHadPhysicsList->ConstructProcess();
  G4cout << "### gebicPhysicsList::ConstructProcess is done" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SelectPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "gebicPhysicsList::SelectPhysicsList: <" << name << ">" << G4endl;
  }
  // default  Had physics
  if (name == "Hadron" && !fHadPhysicsList) {
    fHadPhysicsList = new gebicPhysListHadron("hadron");
  } else if (name == "QGSP_BERT") {
    AddExtraBuilders(false);
    fHadPhysicsList = new G4HadronPhysicsQGSP_BERT("std-hadron");
  } else if (name == "QGSP_BIC" && !fHadPhysicsList) {
    AddExtraBuilders(false);
    fHadPhysicsList = new G4HadronPhysicsQGSP_BIC("std-hadron");
  } else if (name == "QGSP_BERT_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new G4HadronPhysicsQGSP_BERT_HP("std-hadron");
  } else if (name == "QGSP_BIC_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new G4HadronPhysicsQGSP_BIC_HP("std-hadron");
  } else if (name == "LowEnergy_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new gebicPhysListEmLowEnergy("lowe-em");
  } else if (name == "Standard_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new G4EmStandardPhysics();
  } else {
      G4cout << "gebicPhysicsList WARNING wrong or unkonwn <"
	     << name << "> Physics " << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::AddExtraBuilders(G4bool /*flagHP*/)
{
  fNhadcomp = 5;
  fHadronPhys.push_back( new G4EmExtraPhysics("extra EM"));
  // XXX migration to G4.10
  fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
  /*fHadronPhys.push_back( new G4HadronElasticPhysics("elastic",verboseLevel,
						   flagHP));
                           */
  fHadronPhys.push_back( new G4StoppingPhysics("stopping",verboseLevel));
  fHadronPhys.push_back( new G4IonBinaryCascadePhysics("ionBIC"));
  fHadronPhys.push_back( new G4NeutronTrackingCut("Neutron tracking cut"));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetCuts()
{
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");
  G4cout << "world cuts are set" << G4endl;

  if( !fTargetCuts ) SetTargetCut(fCutForElectron);
  G4Region* region = (G4RegionStore::GetInstance())->GetRegion("Target1");
  region->SetProductionCuts(fTargetCuts);
  G4cout << "Target cuts are set" << G4endl;

  if( !fDetectorCuts ) SetDetectorCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("Detector");
  region->SetProductionCuts(fDetectorCuts);
  region = (G4RegionStore::GetInstance())->GetRegion("DetectorDeadLayer");
  region->SetProductionCuts(fDetectorCuts);
  G4cout << "Detector cuts are set" << G4endl;

  if( !fCapCuts ) SetCapCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("Cap");
  region->SetProductionCuts(fCapCuts);
  region = (G4RegionStore::GetInstance())->GetRegion("Holder");
  region->SetProductionCuts(fCapCuts);
  G4cout << "Cap and Holder cuts are set" << G4endl;

  if( !fShieldCuCuts ) SetShieldCuCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("ShieldCu");
  region->SetProductionCuts(fShieldCuCuts);
  G4cout << "Cu shield cuts are set" << G4endl;

  if( !fShieldPbCuts ) SetShieldPbCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("ShieldPb");
  region->SetProductionCuts(fShieldPbCuts);
  region = (G4RegionStore::GetInstance())->GetRegion("LayerPb");
  region->SetProductionCuts(fShieldPbCuts);
  region = (G4RegionStore::GetInstance())->GetRegion("LayerThinPb");
  region->SetProductionCuts(fShieldPbCuts);
  G4cout << "Pb shields cuts are set" << G4endl;

  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetTargetCut(G4double cut)
{
  if( !fTargetCuts ) fTargetCuts = new G4ProductionCuts();

  fTargetCuts->SetProductionCut(cut, idxG4GammaCut);
  fTargetCuts->SetProductionCut(cut, idxG4ElectronCut);
  fTargetCuts->SetProductionCut(cut, idxG4PositronCut);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void gebicPhysicsList::SetDetectorCut(G4double cut)
{
  if( !fDetectorCuts ) fDetectorCuts = new G4ProductionCuts();

  fDetectorCuts->SetProductionCut(cut, idxG4GammaCut);
  fDetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
  fDetectorCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void gebicPhysicsList::SetCapCut(G4double cut)
{
	if( !fCapCuts ) fCapCuts = new G4ProductionCuts();

	fCapCuts->SetProductionCut(cut, idxG4GammaCut);
	fCapCuts->SetProductionCut(cut, idxG4ElectronCut);
	fCapCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void gebicPhysicsList::SetShieldCuCut(G4double cut)
{
	if( !fShieldCuCuts ) fShieldCuCuts = new G4ProductionCuts();

	fShieldCuCuts->SetProductionCut(cut, idxG4GammaCut);
	fShieldCuCuts->SetProductionCut(cut, idxG4ElectronCut);
	fShieldCuCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void gebicPhysicsList::SetShieldPbCut(G4double cut)
{
	if( !fShieldPbCuts ) fShieldPbCuts = new G4ProductionCuts();

	fShieldPbCuts->SetProductionCut(cut, idxG4GammaCut);
	fShieldPbCuts->SetProductionCut(cut, idxG4ElectronCut);
	fShieldPbCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

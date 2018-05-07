/*
 *  gebicEventAction.cc
 *
 *
 *
 * 	Geant496
 *
 */
 //re-checked 25/11/2013

#include "G4ios.hh"
#include "gebicEventActionMessenger.hh"
#include "gebicEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
//#include "G4TrajectoryContainer.hh"
//#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "gebicAnalysisManager.hh"

extern G4bool drawEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::gebicEventAction()
  : fDrawFlag("all")
{
  fEventMessenger = new gebicEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::~gebicEventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventAction::BeginOfEventAction(const G4Event* evt)
{
    gebicAnalysisManager::GetInstance()->BeginOfEvent();
    if(evt->GetEventID() % 10000 == 0)
    {
        G4cout << "Processed event: "<< evt->GetEventID() << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventAction::EndOfEventAction(const G4Event*)
{
  //analysis
  gebicAnalysisManager::GetInstance()->EndOfEvent();

}

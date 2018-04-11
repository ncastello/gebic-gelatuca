/*
 *  gebicRunAction.cc
 *  
 *
 *  Geant496
 *
 */
 //re-checked 25/11/2013
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "gebicRunAction.hh"
#include "gebicAnalysisManager.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicRunAction::gebicRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicRunAction::~gebicRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Creation of the analysis manager
  gebicAnalysisManager* analysis = gebicAnalysisManager::GetInstance();
  analysis->BeginOfRun();
 
  G4int RunN = aRun->GetRunID();
  if ( RunN % 1000 == 0 ) 
    G4cout << "### Run : " << RunN << G4endl;

#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer(); 
      UI->ApplyCommand("/vis/clear/view");
      UI->ApplyCommand("/vis/draw/current");
    } 
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicRunAction::EndOfRunAction(const G4Run* aRun)
{
  // Get the analysis manager
  gebicAnalysisManager* analysis = gebicAnalysisManager::GetInstance();
  analysis->EndOfRun(aRun->GetNumberOfEvent());
 
#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/show/view");
#endif
}

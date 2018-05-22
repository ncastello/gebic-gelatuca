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
#include "gebicEventAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicRunAction::gebicRunAction( gebicEventAction * eventAction ) :
    _eventAction(eventAction)
{
    // Creation of the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Default settings
    // analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("gebic");

    if(this->_eventAction)
    {
        analysisManager->CreateNtuple("gebicMC","HPGe simulation at Canfrac");
//      Probably not valid for Geant4.9
//        analysisManager
//            ->CreateNtupleDColumn("Edep", _eventAction->GetEdep());
        // XXX TO BE REMOVED when migrating 4.10!!!!
        const int ntuple_id = analysisManager->CreateNtupleFColumn("Edep");

        _eventAction->set_ntuple_id(ntuple_id);
        // --->> XXX TO BE REMOVED when migrating 4.10!!!!

        analysisManager->FinishNtuple();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicRunAction::~gebicRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicRunAction::BeginOfRunAction(const G4Run* aRun)
{
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    // The default file name is set in B5RunAction::B5RunAction(),
    // it can be overwritten in a macro
    analysisManager->OpenFile();

    //analysis->BeginOfRun();
    G4int RunN = aRun->GetRunID();
    if ( RunN % 1000 == 0 )
    {
        G4cout << "### Run : " << RunN << G4endl;
    }

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

void gebicRunAction::EndOfRunAction(const G4Run* /*aRun*/)
{
    // save histograms & ntuple
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/show/view");
#endif

}

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
//#include "gebicAnalysisManager.hh"
#include "G4SystemOfUnits.hh"

extern G4bool drawEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::gebicEventAction():
    fDrawFlag("all"),
    fEventMessenger(nullptr),
    _hitCollection(new std::vector<float>),
    _hitCollectionID(-1),
    _hitCollectionName("gebicHitCollection"),
    _pulseWidth(1.*microsecond)
{
    fEventMessenger = new gebicEventActionMessenger(this);
    G4RunManager::GetRunManager()->SetPrintProgress(10000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::~gebicEventAction()
{
    if(fEventMessenger!=nullptr)
    {
        delete fEventMessenger;
        _fEventMessenger=nullptr;
    }
    if(_hitCollection!=nullptr)
    {
        delete _hitCollection;
        _hitCollection=nullptr;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventAction::BeginOfEventAction(const G4Event* /*evt*/)
{
    //gebicAnalysisManager::GetInstance()->BeginOfEvent();
    _hitCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(_hitCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventAction::EndOfEventAction(const G4Event* event)
{
    //analysis
    //gebicAnalysisManager::GetInstance()->EndOfEvent();
    //
    auto analysisManager = G4AnalysisManager::Instance();

    // HCE hit collections of the event
    auto HCE = this->GetHC(event, _hitCollectionID);
    if(HCE==nullptr)
    {
        return;
    }
    if(HCE->entries()==0)
    {
        return;
    }

    // Clean the hitCollection vector and guess the amount of memory to be
    // allocated
    _hitCollection->clear();
    _hitCollection->reserve(HCE->entries());

    // Obtaining the std::vector of hits in order to be able to
    // use the sort algorithm
    auto hits = HCE->GetVector();
    // sorting by time (XXX)
    std::sort(hits->begin(),hits->end(),
            [] (gebicHit *l, gebicHit *r) -> bool { return (*l < *r); } );

    // emulating readout electronic time (very, very simple)
    for(unsigned int i=0; i< hits->size(); ++i )
    {
        const G4double Time0 = (*hits)[i]->GetTime();
        float Edep = (*hits)[i]->GetEdep();

        // Next element as long as exists
        auto k=i+1;
        while(k<=hits->size() &&
                std::fabs(((*hits)[k]->GetTime()- Time0)/second) <= _pulseWidth)
        {
            Edep += (*hits)[k]->GetEnergy();
            ++k;
        }

        // Dump accumulated energy to the histo
        if(fabs(Edep) > 1e-12)
        {
            _hitCollection->push_back(Edep);
        }

        // Recovering index for next iteration
        i=k;
    }

}

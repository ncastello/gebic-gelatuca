/*
 *  gebicEventAction.cc
 *
 *
 *
 * 	Geant496
 *
 */
 //re-checked 25/11/2013

#include "gebicEventActionMessenger.hh"
#include "gebicEventAction.hh"
// using ROOT technology for the analysis (g4rrot.hh)
#include "gebicAnalysisManager.hh"
#include "gebicHit.hh"

#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
//#include "G4TrajectoryContainer.hh"
//#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

extern G4bool drawEvent;

static int __ne=0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::gebicEventAction():
    _drawFlag("all"),
    _eventMessenger(nullptr),
    _hitCollection(new std::vector<float>),
    _hitCollectionID(-1),
    _hitCollectionName("HPGe/gebicHitCollection"),
    _pulseWidth(1.*microsecond)
{
    _eventMessenger = new gebicEventActionMessenger(this);
    G4RunManager::GetRunManager()->SetPrintProgress(10000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventAction::~gebicEventAction()
{
    if(_eventMessenger!=nullptr)
    {
        delete _eventMessenger;
        _eventMessenger=nullptr;
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
    _hitCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(_hitCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventAction::EndOfEventAction(const G4Event* event)
{
    // XXX TO BE REMOVED when migrating 4.10!!!!
    auto analysisManager = G4AnalysisManager::Instance();
    // --> XXX TO BE REMOVED when migrating 4.10!!!!

    // HCE hit collections of the event
    auto HCE = event->GetHCofThisEvent();
    if(HCE==nullptr)
    {
        return;
    }
    auto HC = static_cast<G4THitsCollection<gebicHit>*>(HCE->GetHC(_hitCollectionID));
    if(HC == nullptr || HC->entries()==0)
    {
        return;
    }

    // Clean the hitCollection vector and guess the amount of memory to be
    // allocated
    _hitCollection->clear();
    _hitCollection->reserve(HC->entries());

    // Obtaining the std::vector of hits in order to be able to
    // use the sort algorithm
    auto hits = HC->GetVector();
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
        while(k<hits->size() &&
                std::fabs(((*hits)[k]->GetTime()- Time0)/second) <= _pulseWidth)
        {
            Edep += (*hits)[k]->GetEdep();
            ++k;
        }

        // Dump accumulated energy to the histo
        if(fabs(Edep) > 1e-12)
        {
            _hitCollection->push_back(Edep);
            // XXX TO BE REMOVED when migrating 4.10!!!!
            analysisManager->FillNtupleFColumn(this->_ntupleID,Edep/keV);
            ++__ne;
            // --->XXX TO BE REMOVED when migrating 4.10!!!!
        }
        // Recovering index for next iteration
        i=k;
    }
    analysisManager->AddNtupleRow();
}

#include "gebicSensitiveDetector.hh"

#include "G4SDManager.hh"

#include <algorithm>
#include <cmath>

gebicSensitiveDetector::gebicSensitiveDetector(G4String SDname) :
    G4VSensitiveDetector(SDname),
    _hitsCollection(nullptr),
    _collectionID(-1)
{
    this->collectionName.insert("gebicHitCollection");
}

gebicSensitiveDetector::~gebicSensitiveDetector()
{
}

void gebicSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
    _hitsCollection = new gebicHitCollection(this->SensitiveDetectorName,
            this->collectionName[0]);
    if(_collectionID<0)
    {
        _collectionID = G4SDManager::GetSDMpointer()->GetCollectionID(_hitsCollection);
    }
    HCE->AddHitsCollection(_collectionID,_hitsCollection);
}

G4bool gebicSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // Energy resolution TO BE IMPLEMENTED
    const float edep = step->GetTotalEnergyDeposit();
    // Do not store any hit if energy deposite is ZERO
    if(std::fabs(edep)<1e-12)
    {
        return true;
    }

    auto hit = new gebicHit(step->GetTrack()->GetDefinition()->GetPDGEncoding(),
            step->GetPreStepPoint()->GetGlobalTime(),
            edep);

    _hitsCollection->insert(hit);
    return true;
}

void gebicSensitiveDetector::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{
}




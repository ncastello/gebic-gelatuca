#include "gebicSensitiveDetector.hh"
#include "gebicHit.hh"

#include <algorithm>
#include <cmath>

gebicSensitiveDetector::gebicSensitiveDetector(G4String SDname) :
    G4VSensitiveDetector(SDname),
    _hitsCollections(nullptr),
    _collectionID(-1)
{
    this->collectionName.insert("gebicHitCollection");
}


void gebicSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
    _hitsCollection = new gebicHitCollection(this->SensitiveDetectorName,
            this->collectionName[0]);
    if(_collectionID<0)
    {
        _collectionID = G4SDManager::GetSDMpointer()->GetCollectionID(_hitsCollection);
    }
    HCE->AddHitCollection(_collectionID,_hitsCollection);
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

void gebicSensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
    if(_hitsCollection->entries()==0)
    {
        return;
    }

    // Obtaining the std::vector of hits in order to be able to
    // use the sort algorithm
    auto hits = _hitsCollection->GetVector();
    // sorting by time (XXX)
    std::sort(hits->begin(),hits->end(),
            [] (gebicHit *l, gebicHit *r) -> bool { return (*l < *r); } );

    // emulating readout electronic time (very, very simple)
    for(unsigned int i=0; i< hits->size(); ++i )
    {
        const G4double Time0 = (*hits)[i]->GetTime();
        float DetE = (*hits)[i]->GetEdep();

        // Next element as long as exists
        auto k=i+1;
        while(k<=hits->size() &&
                std::fabs(((*hits)[k]->GetTime()- Time0)/second) <= fPulseWidth)
        {
            DetE += (*hits)[k]->GetEnergy();
            ++k;
        }

        // Dump accumulated energy to the histo
        if(fabs(DetE) > 1e-12)
        {
            fHisto->FillHisto(0,DetE); // Detector histogram
        }

        // Recovering index for next iteration
        i=k;
    }
}




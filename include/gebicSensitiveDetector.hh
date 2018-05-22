#ifndef GEBICSENSITIVEDETECTOR_HH
#define GEBICSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "gebicHit.hh"

class gebicSensitiveDetector : public G4VSensitiveDetector
{
    public:
        /// Constructor
        gebicSensitiveDetector( G4String SDname);
        gebicSensitiveDetector()=delete;
        /// Destructor
        ~gebicSensitiveDetector();

        /// @name methods from base class G4SensitiveDetecgtor
        //@{
        /// Mandatory base class method: it must to be overloaded
        G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

        /// (optional) method of base class G4SensitiveDetector
        void Initialize(G4HCofThisEvent* HCE);
        /// (optional) method of base class G4SensitiveDetector
        void EndOfEvent(G4HCofThisEvent* HCE);
        //@}
    private:
        gebicHitCollection * _hitsCollection;
        G4int _collectionID;
};
#endif

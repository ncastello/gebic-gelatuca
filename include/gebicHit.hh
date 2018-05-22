#ifndef G4GEBICHIT_HH
#define G4GEBICHIT_HH

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

class gebicHit : public G4VHit
{
    public:
        gebicHit(const G4int particleID, float time, float edep);
        gebicHit()=delete;
        ~gebicHit();

        inline int GetParticleID(){return _particleID;};
        inline float GetTime(){return _time;};
        inline float GetEdep(){return _edep;};

        G4bool operator==(const gebicHit &right) const ;
        G4bool operator< (const gebicHit &right) const ;
        G4bool operator<=(const gebicHit &right) const ;


    private:
        int _particleID;
        float _time;
        float _edep;
        float _smearingFactor;

};

//typedef G4THitsCollection<gebicHit> gebicHitCollection;
using gebicHitCollection = G4THitsCollection<gebicHit>;

#endif

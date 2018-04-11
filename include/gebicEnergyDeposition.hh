/*
 *  gebicEnergyDeposition.hh
 *  
 *
 * 
 *	Geant496
 */
//
#ifndef gebicEnergyDeposition_h
#define gebicEnergyDeposition_h 1

#include "globals.hh"

class gebicEnergyDeposition
{
  public:   // with description

    gebicEnergyDeposition();
    gebicEnergyDeposition( const gebicEnergyDeposition &right );
    gebicEnergyDeposition( G4double, G4double, G4double, G4int );
    virtual ~gebicEnergyDeposition();
         // Constructor and virtual destructor

    G4bool operator==(const gebicEnergyDeposition &right) const ;
    G4bool operator< (const gebicEnergyDeposition &right) const ;
    G4bool operator<=(const gebicEnergyDeposition &right) const ;
  // Operators  

  G4double GetEnergy() {return fEnergy;};
  G4double GetTime() {return fTime;};
  G4double GetWeight() {return fWeight;};
  G4int GetPart() {return fPart;};
  // Accessors

  private:

    G4double fEnergy;  
    G4double fTime;    
    G4double fWeight;
    G4int fPart;
};
#endif




/*
 *  gebicEnergyDeposition.cc
 *  
 *
 *  
 *	Geant496
 */
//
//re-checked 25/11/2013
#include "gebicEnergyDeposition.hh"


//
// Default constructor
//
gebicEnergyDeposition::gebicEnergyDeposition()
: fEnergy(0.), fTime(0.), fWeight(1.)
{;}
//
// Specific constructor
//
gebicEnergyDeposition::gebicEnergyDeposition( G4double energy, G4double time, G4double weight, G4int part )
  : fEnergy(energy),
    fTime(time),
    fWeight(weight),
    fPart(part)
{;}


//
// Copy constructor
//
gebicEnergyDeposition::gebicEnergyDeposition( const gebicEnergyDeposition &right )
  : fEnergy(right.fEnergy),
    fTime(right.fTime),
    fWeight(right.fWeight),
    fPart(right.fPart)
{;}

//
// Destructor
//
gebicEnergyDeposition::~gebicEnergyDeposition() {;}

//
// Equivalence operator
//
G4bool gebicEnergyDeposition::operator==( const gebicEnergyDeposition &right ) const
{
  return fTime == right.fTime;
}

//
// Order operators
//
G4bool gebicEnergyDeposition::operator<( const gebicEnergyDeposition &right ) const
{
  return fTime < right.fTime;
}

G4bool gebicEnergyDeposition::operator<=( const gebicEnergyDeposition &right ) const
{
  return fTime <= right.fTime;
}


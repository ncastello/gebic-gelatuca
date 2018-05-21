#include "gebicHit.hh"


gebicHit::gebicHit(const G4int particleID, float time, float edep) :
    _particleID(particleID),
    _time(time),
    _edep(edep),
    _smearingFactor(1.0)
{
}

gebicHit::~gebicHit()
{
}

G4bool gebicHit::operator<( const gebicHit &right ) const
{
	return _time < right._time;
}

G4bool gebicHit::operator<=( const gebicHit &right ) const
{
	return _time <= right._time;
}

G4bool gebicHit::operator==( const gebicHit &right ) const
{
    return _time == right._time;
}


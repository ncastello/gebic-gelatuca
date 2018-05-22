/*
 *  gebicEventAction.hh
 *
 *
 *
 *	Geant496
 */


#ifndef gebicEventAction_h
#define gebicEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>

class gebicEventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class gebicEventAction : public G4UserEventAction
{
    public:
        gebicEventAction();
        virtual ~gebicEventAction();

        // XXX TO BE REMOVED when migrating 4.10!!!!
        inline void set_ntuple_id(const int & id) { _ntupleID = id; }
        // --- XXX TO BE REMOVED when migrating 4.10!!!!

        virtual void BeginOfEventAction(const G4Event* anEvent);
        virtual void EndOfEventAction(const G4Event* anEvent);

        void SetDrawFlag(G4String val)  {_drawFlag = val;};

        inline std::vector<float> * GetEdep() { return _hitCollection; }

  private:
        G4String _drawFlag;                         // control the drawing of event
        gebicEventActionMessenger*  _eventMessenger;

        std::vector<float> * _hitCollection;
        G4int _hitCollectionID;
        G4String _hitCollectionName;

        G4double _pulseWidth;
        // XXX TO BE REMOVED when migrating 4.10
        int _ntupleID;

};

#endif



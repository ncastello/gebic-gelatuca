/*
 *  gebicRunAction.hh
 *
 *
 *
 *  Geant496
 *
 */
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef GEBICRUNACTION_HH
#define GEBICRUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class gebicEventAction;
class G4Run;

class gebicRunAction : public G4UserRunAction
{
    public:
        gebicRunAction(gebicEventAction * eventAction);
        gebicRunAction() = delete;
        ~gebicRunAction();

        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);

    private:
        gebicEventAction * _eventAction;
};

#endif

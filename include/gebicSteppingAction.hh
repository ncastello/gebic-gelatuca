#ifndef gebicSteppingAction_h
#define gebicSteppingAction_h 1

//XXX Provisional class: trying to bypass problem with stuck particles.
//Transported particles infinitely  between VaccCry--LayerPb, probably because
//some geometry detector bug (detector overlap layers, volums, ...) --> check
//geometry
//XXX

#include "G4TransportationManager.hh"

#include "G4UserSteppingAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"


class gebicSteppingAction : public G4UserSteppingAction
{
    public:
        //
        gebicSteppingAction()
        {
            G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetPushVerbosity(0);
        }
        virtual ~gebicSteppingAction(){;}

        virtual void UserSteppingAction(const G4Step* fStep)
            {
                G4Track* fTrack = fStep->GetTrack();
                G4int StepNo = fTrack->GetCurrentStepNumber();
                if(StepNo >= 10000)
                {
                    G4cout << "WARNING (fStopAndKill) stepNo = "<< StepNo << G4endl;
                    fTrack->SetTrackStatus(fStopAndKill);
                }
            }
};

#endif


/*
 *  gebicEventActionMessenger.cc
 *
 *
 * 
 *	Geant496
 */
// 
//re-checked 25/11/2013

#include "gebicEventActionMessenger.hh"

#include "gebicEventAction.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventActionMessenger::gebicEventActionMessenger(gebicEventAction* EvAct)
: fEventAction(EvAct)
{ 
  fDrawCmd = new G4UIcmdWithAString("/event/draw",this);
  fDrawCmd->SetGuidance("Draw the tracks in the event");
  fDrawCmd->SetGuidance("  Choice : none, charged, all (default)");
  fDrawCmd->SetParameterName("choice",true);
  fDrawCmd->SetDefaultValue("all");
  fDrawCmd->SetCandidates("none charged all");
  fDrawCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicEventActionMessenger::~gebicEventActionMessenger()
{
  delete fDrawCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicEventActionMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if(command == fDrawCmd)
    {fEventAction->SetDrawFlag(newValue);}
   
}

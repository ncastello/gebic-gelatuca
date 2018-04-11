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

#ifndef gebicRunAction_h
#define gebicRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class gebicRunAction : public G4UserRunAction
{
  public:
    gebicRunAction();
   ~gebicRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
  private:

};

#endif

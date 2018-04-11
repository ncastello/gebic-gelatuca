/*
 *  gebicHistoMessenger.hh
 *  
 *
 *  Copyright 2011 __LSC__. All rights reserved.
 *	Geant496
 */

#ifndef gebicHistoMessenger_h
#define gebicHistoMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gebicHisto;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gebicHistoMessenger: public G4UImessenger
{
  public:

   gebicHistoMessenger(gebicHisto* );
   virtual ~gebicHistoMessenger();

   virtual void SetNewValue(G4UIcommand* ,G4String );

  private:

   gebicHisto*             fHisto;
   
   G4UIdirectory*          fHistoDir;   
   G4UIcmdWithAString*     fActoryCmd;
   G4UIcmdWithAString*     fIleCmd;
   G4UIcommand*            fHistoCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

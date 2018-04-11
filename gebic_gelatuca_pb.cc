//Geant496
//modified 28/01/2013

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "gebicDetectorConstruction.hh"
#include "gebicPhysicsList.hh"
#include "gebicEventAction.hh"
#include "gebicRunAction.hh"
#include "gebicSteppingAction.hh"
#include "gebicPrimaryGeneratorAction.hh"
#include "gebicAnalysisManager.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Creation of the analysis manager
  gebicAnalysisManager::GetInstance();

  // set mandatory initialization classes

  gebicDetectorConstruction* Detector = new gebicDetectorConstruction;
  runManager->SetUserInitialization(Detector);
  runManager->SetUserInitialization(new gebicPhysicsList);

  // set mandatory user action class
  runManager->SetUserAction(new gebicPrimaryGeneratorAction);
  runManager->SetUserAction(new gebicRunAction);
  runManager->SetUserAction(new gebicEventAction);
  runManager->SetUserAction(new gebicSteppingAction);

#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Initialize G4 kernel
  // do this at run time so the geometry/physics can be changed
  //  runManager->Initialize();

  // get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  if (argc == 1)   // Define UI session for interactive mode.
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
#endif
    }
  else           // Batch mode
    { 
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  
  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  gebicAnalysisManager::Dispose();
  delete runManager;

  return 0;
}

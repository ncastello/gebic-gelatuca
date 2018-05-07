//Geant496
//modified 28/01/2013

#include<iostream>
#include<fstream>
#include<string>

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

// NCM -- 20180504
// Input argument from the main program
struct input_options
{
    input_options()
    {
        // Defaults
        macro  = "";
        outputFilename = "gebic.root";
    }
    std::string macro;
    std::string outputFilename;
};


void display_usage()
{
    std::cout << "\033[37musage:\033[m gebic_gelatuca [OPTIONS] [macro.mac]"
        << std::endl;
    std::cout << std::endl;
    std::cout << "Geant4 simulation of the GeLatuca detector. To run as interactive "
        << " mode (UI) run without any arguments. To run in batch mode just give a macro file"
        << std::endl;
    std::cout << std::endl;
    std::cout << "[OPTIONS]\n -o name of the ROOT output file [gebic.root]\n"
      << " -h show this help" << std::endl;
}

// endNCM


int main(int argc,char** argv)
{
    // NCM --- 20180504
	input_options opt;
    for(int i = 1; i < argc; ++i)
    {
        if( strcmp(argv[i],"-h") == 0 )
        {
            display_usage();
            return 0;
        }
        else if( strcmp(argv[i],"-o") == 0 )
        {
            opt.outputFilename = argv[i+1];
            ++i;
        }
        else
        {
            // Check that the provided input name corresponds to an existing file.
            std::ifstream is(argv[i]);
            if(!is && std::string(argv[i]) != "-h")
            {
                std::cerr << " Macro file '" << argv[i] << "' was not found. \n"
                    << " Program stopped! " << std::endl;
                return -2;
            }
            opt.macro = argv[i];
        }
    }

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

    // Define UI session for interactive mode.
    if(argc == 1)
    {
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        ui->SessionStart();
        delete ui;
#endif
    }
    // Batch mode
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = opt.macro;
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

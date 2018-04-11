/*
 *  gebicSteppingAction.cc
 *  
 *
 * 
 *	Geant496
 */
 //modfied 02/06/2017
// Integer flag: Detector=0, Target=1, LayerPb=2, Holder=3, Cap=4, Support=5, Copper Shield=6, Lead shield=7
//ThinPbLayer=8, Detector Dead Layer=9
#include "G4ios.hh"

#include "gebicSteppingAction.hh"
#include "gebicAnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4ParticleDefinition.hh"



gebicSteppingAction::gebicSteppingAction()
{ }


gebicSteppingAction::~gebicSteppingAction()
{ }


void gebicSteppingAction::UserSteppingAction(const G4Step* fStep) 
{
  G4Track* fTrack = fStep->GetTrack();
  G4int StepNo = fTrack->GetCurrentStepNumber();
  if(StepNo >= 10000) fTrack->SetTrackStatus(fStopAndKill);
  
#ifndef COLLECT
#define COLLECT
#endif

#ifdef COLLECT   


  if (StepNo == 1) {
    if ( (fTrack->GetDefinition()->GetParticleType() == "nucleus") && 
    !( fTrack->GetDefinition()->GetPDGStable()) && 
	 ((fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Target1"))) {
       particleName = G4double(fTrack->GetDefinition()->GetPDGEncoding());
       time = fStep->GetPreStepPoint()->GetGlobalTime() ;
       weight =  fStep->GetPreStepPoint()->GetWeight() ;
      // get the analysis manager
       gebicAnalysisManager::GetInstance()->AddIsotope(particleName, weight, time);
       }
    if (fTrack->GetTrackID() != 1 ){
       if (fTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") {
			// emitted particles
			if (fTrack->GetDefinition()->GetParticleType() != "nucleus") {
				particleName = G4double (fTrack->GetDefinition()->GetPDGEncoding());
				time = fStep->GetPreStepPoint()->GetGlobalTime() ; 
				weight = fStep->GetPreStepPoint()->GetWeight() ;   
				energy = fStep->GetPreStepPoint()->GetKineticEnergy();
				gebicAnalysisManager::GetInstance()->AddParticle(particleName, energy, weight, time);
				}
		}
    }
  }


  // energy deposition
 if (fTrack->GetTrackID()){
      if (fStep->GetTotalEnergyDeposit() ) {
		time = fStep->GetPreStepPoint()->GetGlobalTime() ;
		edep = fStep->GetTotalEnergyDeposit();
		weight = fStep->GetPreStepPoint()->GetWeight() ;
		  
		  
	if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Detector") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,0);
			  //G4cout<<"hit detector"<<G4endl;
		  }	 
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Target1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,1);
			  //G4cout<<"hit tar"<<G4endl;
		  }
          
            
    else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerPb") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,2);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerThinPb1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,8);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerThinPb2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,8);
			  //G4cout<<"hit cap"<<G4endl;
		  }
	
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerThinPb3") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,8);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerThinPb4") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,8);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "LayerThinPb5") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,8);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Holder1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,3);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Holder2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,3);
			  //G4cout<<"hit cap"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Cap1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,4);
			  //G4cout<<"hit cap"<<G4endl;
		  }	
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Cap2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,4);
			  //G4cout<<"hit cap"<<G4endl;
		  }	
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Cap3") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,4);
			  //G4cout<<"hit cap"<<G4endl;
		  }	
		  
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu3") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu4") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu5") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldCu6") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,6);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb3") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb4") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb5") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ShieldPb6") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,7);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
     else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "DetectorDeadLayer1") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,9);
			  //G4cout<<"hit tar"<<G4endl;
		  }
		  
	 else if (fStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "DetectorDeadLayer2") {
		gebicAnalysisManager::GetInstance()->AddEnergy(edep,weight,time,9);
			  //G4cout<<"hit tar"<<G4endl;
		  }
      }
      //    }
  }
  #endif
}

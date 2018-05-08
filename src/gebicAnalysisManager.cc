/*
 *  gebicAnalysisManager.cc
 *
 *
 *
 *	Geant496
 */

//modified 02/06/2017
//re-checked
#include "gebicAnalysisManager.hh"
#include "gebicHisto.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessTable.hh"
#include "G4RadioactiveDecay.hh"
#include "G4TwoVector.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>



gebicAnalysisManager* gebicAnalysisManager::fManager = nullptr;


gebicAnalysisManager* gebicAnalysisManager::GetInstance()
{
    if(!fManager)
    {
        fManager = new gebicAnalysisManager();
    }
    return fManager;
}


void gebicAnalysisManager::Dispose()
{
  delete fManager;
  fManager = 0;
}


//exchange here the maximum energy of the histo
gebicAnalysisManager::gebicAnalysisManager()
: fVerbose(0), fNEvt1(-1), fNEvt2(-2),
  fHistEMax(2870.4*keV), fHistEMin(0.2), fHistNBin(8190), //fHistNBin(8192),
  fTargetThresE(10.0*keV), fDetectorThresE(10.0*keV), fCapThresE(10.0*keV),
  fCuThresE(10.0*keV), fPbThresE(10.0*keV), fPulseWidth(1.*microsecond)
{
  fEdepo.clear();
  fHisto   = new gebicHisto();
  BookHisto();
}



gebicAnalysisManager::~gebicAnalysisManager()
{
  delete fHisto;
}



void gebicAnalysisManager::BookHisto()
{
  fHistEMax = 2870.4*keV;//exchange here maximum energy of the histo
  fHistEMin = 0.2*eV;
  // fHistNBin = 8192;
  fHistNBin = 8190;

  fHisto->Add1D("10","MC - GeLaTuca DAMIC Pb",fHistNBin,fHistEMin,fHistEMax,keV);//give histo title here
}



void gebicAnalysisManager::BeginOfRun()
{
  fHisto->Book();
  G4cout << "gebicAnalysisManager: Histograms are booked and the run has been started" << G4endl;
  G4ProcessTable *pTable = G4ProcessTable::GetProcessTable();
  G4RadioactiveDecay *rDecay = (G4RadioactiveDecay *) pTable->FindProcess("RadiaoctiveDecay","GenericIon");
  if (rDecay != NULL){
	  if (!rDecay->IsAnalogueMonteCarlo()){
		  std::vector<G4RadioactivityTable*> theTables = rDecay->GetTheRadioactivityTables();
		  for (size_t i = 0; i<theTables.size(); i++){
			  theTables[i]->GetTheMap()->clear();
			  G4cout<<"Clear the radioactivity map: 0, new size = "<<theTables[i]->GetTheMap()->size()<<G4endl;
		  }
	  }
  }
}



void gebicAnalysisManager::EndOfRun(G4int nevent)
{
  fHisto->Save();
  G4cout<<"gebicAnalysisManager: Histograms have been saved!"<<G4endl;

  //Output the induced radioactivities in VR mode only
  G4ProcessTable *pTable = G4ProcessTable::GetProcessTable();
  G4RadioactiveDecay *rDecay = (G4RadioactiveDecay *) pTable->FindProcess("RadioactiveDecay", "GenericIon");
  if (rDecay != NULL){
	  if (!rDecay->IsAnalogueMonteCarlo()){
		  G4String fileName = fHisto->GetFileName() + ".activity";
		  std::ofstream outfile (fileName, std::ios::out);
		  std::vector<G4RadioactivityTable*>theTables = rDecay->GetTheRadioactivityTables();
		  for (size_t i=0;i<theTables.size();i++){
			  G4double rate,error;
			  outfile<<"Radioactivity in decay window no. "<<i<<G4endl;
			  outfile<<"Z ,\tA \tE \tActivity (decays/window)"<<G4endl;
			  map<G4ThreeVector,G4TwoVector> *aMap = theTables[i]->GetTheMap();
			  map<G4ThreeVector,G4TwoVector>::iterator iter;
			  for (iter=aMap->begin();iter!=aMap->end();iter++){
				  rate=iter->second.x()/nevent;
				  error = std::sqrt(iter->second.y())/nevent;
				  if (rate<0.) rate = 0.; //statistically it can be <0 but it is unphysical
				  outfile<<iter->first.x()<<"\t"<<iter->first.y()<<"\t"<<iter->first.z()<<"\t"<<rate<<"\t"<<error<<G4endl;
			  }
			  outfile<<G4endl;
		  }
		  outfile.close();
	  }
  }
}



void gebicAnalysisManager::BeginOfEvent()
{
  fEdepo.clear();
}


void gebicAnalysisManager::EndOfEvent()
{
    if (fEdepo.size())
    {
        std::sort(fEdepo.begin(),fEdepo.end());

        G4double DetE = 0.;
        G4double DetW = 0.;
        G4double Time = fEdepo[0].GetTime();
        G4int PART = fEdepo[0].GetPart();
        if (PART == 0)
        {
        	DetE = fEdepo[0].GetEnergy();
    	    DetW = fEdepo[0].GetEnergy()*fEdepo[0].GetWeight();
        }
        else
        {
            DetE=0.;
            DetW=0.;
        }

        for (size_t i = 1; i < fEdepo.size(); i++)
        {
            PART = fEdepo[i].GetPart();
            if (std::fabs((fEdepo[i].GetTime()- Time)/second) <= fPulseWidth)
            {
                if ( PART == 0 )
                {
                    DetE += fEdepo[i].GetEnergy();
                    DetW += fEdepo[i].GetEnergy()*fEdepo[i].GetWeight();
                }
                else
                {
                    DetE = 0.;
                    DetW = 0.;
                }
            }
            else
            {
                // tallying
			    if (DetE)
                {
                    DetW /= DetE;
		    		fHisto->FillHisto(0,DetE,DetW); // Detector histogram
	            }

    			//reset
	    		PART = fEdepo[i].GetPart();
		    	Time = fEdepo[i].GetTime();
			    if (PART == 0)
                {
                    DetE = fEdepo[i].GetEnergy();
		    		DetW = fEdepo[i].GetEnergy()*fEdepo[i].GetWeight();
			    }
    			else
                {
		    		DetE = 0.;
			    	DetW = 0.;
			    }
           }
        }

        //tally the last hit
	    if (DetE)
        {
            DetW /= DetE;
            fHisto->FillHisto(0,DetE,DetW); // Detector histogram
        }
    }
    //AddEnergy(0.,0.,0.,1);
}


void gebicAnalysisManager::AddEnergy(G4double edep, G4double weight, G4double time, G4int part)
{
  if(1 < fVerbose) {
    G4cout << "gebicAnalysisManager::AddEnergy: e(keV)= " << edep
	   << " weight = " << weight << " time (s) = " <<  time
           << G4endl;
  }
 // histo->fillTuple(2, 0, edep);
 // histo->fillTuple(2,1,time);
 // histo->fillTuple(2,2,weight);
 // histo->addRow(2);
  //
  gebicEnergyDeposition A(edep,time,weight,part);
  fEdepo.push_back(A);
}



void gebicAnalysisManager::AddParticle(G4double pid, G4double energy, G4double weight, G4double time)
//void gebicAnalysisManager::AddParticle(G4double pid, G4double energy, G4double weight, G4double time, G4int part )
{
	//G4cout<<"Add particle: "<<pid<<"\t\t"<<energy<<"\t\t"<<weight<<"\t\t"<<time<<G4endl;
/*
  if(1 < verbose) {
    G4cout << "gemcAnalysisManager::AddParticle: " << pid
           << G4endl;
  }
  histo->fillTuple(0,0, pid);
  histo->fillTuple(0,1,energy);
  histo->fillTuple(0,2,time);
  histo->fillTuple(0,3,weight);
  histo->fillTuple(0,4,part);
  histo->addRow(0);
  // now fill th emission spectrum
  if (energy>0.) histo->fillHisto(5,energy,weight);
  */
}


void gebicAnalysisManager::AddIsotope(G4double pid,G4double weight, G4double time)
//void gebicAnalysisManager::AddIsotope(G4double pid,G4double weight, G4double time, G4int part )
{
	//G4cout<<"Add isotope: "<<pid<<"\t\t"<<weight<<"\t\t"<<time<<G4endl;
	/*
  if(1 < verbose) {
    G4cout << "gemcAnalysisManager::AddIsotope: " << pid
           << G4endl;
  }
  histo->fillTuple(1,0,pid);
  histo->fillTuple(1,1,time);
  histo->fillTuple(1,2,weight);
  histo->fillTuple(1,3,part);
  histo->addRow(1);
  */
}

/*
 *  gebicHisto.cc
 *  
 *
 * 
 *	Geant496
 */

//re-checked 25/11/2013
#include "TROOT.h"
#include "TApplication.h"
#include "TGClient.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TH1D.h"
#include "TNtuple.h"

#include "gebicHisto.hh"
#include "gebicHistoMessenger.hh"
#include "G4ParticleTable.hh"

#include "G4Tokenizer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
gebicHisto::gebicHisto()
: fHistName("gebic"), fHistType("root"),
  fNHisto(0), fNTuple(0), fVerbose(0),
  fDefaultAct(1)
{
  fROOThisto.clear();
  fROOTntup.clear();
  fRarray.clear();
  fRcol.clear();

  fActive.clear();
  fBins.clear();
  fXmin.clear();
  fXmax.clear();
  fUnit.clear();
  fIds.clear();
  fTitles.clear();
  fTupleName.clear();
  fTupleId.clear();
  fTupleList.clear();
  fTupleListROOT.clear();

  fMessenger = new gebicHistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gebicHisto::~gebicHisto()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::Book()
{
//  new TApplication("App", ((int *)0), ((char **)0));
  G4String fileNameROOT = fHistName + G4String(".root");
  fHfileROOT = new TFile(fileNameROOT.c_str() ,"RECREATE","ROOT file for gebic");
  G4cout << "Root file: " << fileNameROOT << G4endl;
  // Creating an 1-dimensional histograms in the root directory of the tree
  for(G4int i=0; i<fNHisto; i++) {
    if(fActive[i]) {
      G4String id = G4String("h")+fIds[i];
      fROOThisto[i] = new TH1D(id, fTitles[i], fBins[i], fXmin[i], fXmax[i]);
      G4cout << "ROOT Histo " << fIds[i] << " " << fTitles[i] << " booked " << G4endl;
    }
  }
  // Now the ntuples  
  for(G4int i=0; i<fNTuple; i++) {
    if(fTupleListROOT[i] != "") {
      G4String id = G4String("t")+fTupleId[i];
      G4cout << "Creating Ntuple "<<fTupleId[i] << " in ROOT file: " 
	     << fTupleName[i] << G4endl;
      fROOTntup[i] = new TNtuple(id, fTupleName[i], fTupleListROOT[i]);
      G4cout << "ROOT Ntuple " << id << " " << fTupleName[i] <<" "<< fTupleListROOT[i]<< " booked " << G4endl;
    }
  }

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::Save()
{
  G4cout << "ROOT: files writing..." << G4endl;
  fHfileROOT->Write();
  G4cout << "ROOT: files closing..." << G4endl;
  fHfileROOT->Close();
  delete fHfileROOT;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::Add1D(const G4String& id, const G4String& name, G4int nb, 
                  G4double x1, G4double x2, G4double u)
{
  if(fVerbose > 0) {
    G4cout << "New histogram will be booked: #" << id << "  <" << name 
           << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
           << G4endl;
  }
  fNHisto++;
  x1 /= u;
  x2 /= u;
  fActive.push_back(fDefaultAct);
  fBins.push_back(nb);
  fXmin.push_back(x1);
  fXmax.push_back(x2);
  fUnit.push_back(u);
  fIds.push_back(id);
  fTitles.push_back(name);
  fROOThisto.push_back(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::SetHisto1D(G4int i, G4int nb, G4double x1, G4double x2, G4double u)
{
  if(i>=0 && i<fNHisto) {
    if(fVerbose > 0) {
      G4cout << "Update histogram: #" << i  
             << "  " << nb << "  " << x1 << "  " << x2 << "  " << u 
             << G4endl;
    }
    fBins[i] = nb;
    fXmin[i] = x1;
    fXmax[i] = x2;
    fUnit[i] = u;
  } else {
    G4cout << "gebicHisto::setgebicHisto1D: WARNING! wrong histogram index " << i << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::FillHisto(G4int i, G4double x, G4double w)
{
  if(fVerbose > 1) {
    G4cout << "fill histogram: #" << i << " at x= " << x 
           << "  weight= " << w
           << G4endl;   
  }
  if(i>=0 && i<fNHisto) {
    fROOThisto[i]->Fill(x/fUnit[i],w);
  } else {
    G4cout << "gebicHisto::fill: WARNING! wrong ROOT histogram index " << i << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::ScaleHisto(G4int i, G4double x)
{
  if(fVerbose > 0) {
    G4cout << "Scale histogram: #" << i << " by factor " << x << G4endl;   
  }
  if(i>=0 && i<fNHisto) {
    fROOThisto[i]->Scale(x);
  } else {
    G4cout << "gebicHisto::scale: WARNING! wrong ROOT histogram index " << i << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::AddTuple(const G4String& w1, const G4String& w2, const G4String& w3)
{
  //G4cout << w1 << " " << w2 << " " << w3 << G4endl;
  fNTuple++;
  fTupleId.push_back(w1);
  fTupleName.push_back(w2) ;

  std::vector<float> ar;
  ar.clear();
  for (size_t i = 0; i < 20; i++) ar.push_back(0.);
  fRarray.push_back(ar);
  // convert AIDA header to ROOT header for ntuple
  G4Tokenizer next(w3);
  G4String token = next();
  G4String ROOTList1 = "" ;
  G4int col = 0;
  while ( token != "") {
   token = next();
   if (token == ",") token = next();
   if (token.contains(",")) token.remove(token.size()-1);
   ROOTList1 = ROOTList1 + token + G4String(":");
   col++;
  }
  G4String ROOTList = ROOTList1.substr(0,ROOTList1.length()-2);
 // G4cout << ROOTList << G4endl;
  fTupleListROOT.push_back(ROOTList);
  fROOTntup.push_back(0);
  fRcol.push_back(col-1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::FillTuple(G4int i, const G4String& parname, G4double x)
{
  if(fVerbose > 1) 
    G4cout << "fill tuple # " << i 
	   <<" with  parameter <" << parname << "> = " << x << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::FillTuple(G4int i, G4int col, G4double x)
{
  if(fVerbose > 1) {
    G4cout << "fill tuple # " << i 
	   <<" in column < " << col << "> = " << x << G4endl; 
  }
  if(fROOTntup[i]) (fRarray[i])[col] = float(x);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::FillTuple(G4int i, const G4String& parname, G4String& x)
{
  if(fVerbose > 1) {
    G4cout << "fill tuple # " << i 
	   <<" with  parameter <" << parname << "> = " << x << G4endl; 
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::AddRow(G4int i)
{
  if(fVerbose > 1) G4cout << "Added a raw #" << i << " to tuple" << G4endl; 


  float ar[4];
  for (G4int j=0; j < fRcol[i]; j++) {
//      G4cout << i << " " << Rarray[i][j] << G4endl;
      ar[j] = fRarray[i][j];       
  }  
  if(fROOTntup[i]) fROOTntup[i]->Fill(ar);


} 

//....oooOO0OOooo........oooOO0OOooo....const G4String& getFileName() const;....oooOO0OOooo........oooOO0OOooo....

void gebicHisto::SetFileName(const G4String& nam) 
{
  fHistName = nam;
}

const G4String& gebicHisto::GetFileName() const
{
  return fHistName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void gebicHisto::SetFileType(const G4String& nam) 
{
  fHistType = nam;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const G4String& gebicHisto::FileType() const
{
  return fHistType;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


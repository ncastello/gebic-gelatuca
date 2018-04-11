/*
 *  gebicAnalysisManager.hh
 *  
 *
 * 
 * 	Geant496
 *
 */
#ifndef gebicAnalysisManager_h
#define gebicAnalysisManager_h 1

//---------------------------------------------------------------------------
//
// ClassName:   gebicAnalysisManager
//
// Description: Singleton class to hold analysis parameters and build histograms.
//              User cannot access to the constructor.
//              The pointer of the only existing object can be got via
//              gebicAnalysisManager::GetInstance() static method.
//              The first invokation of this static method makes
//              the singleton object.
//
//----------------------------------------------------------------------------
//


#include "globals.hh"
#include "gebicEnergyDeposition.hh"
#include <vector>



class gebicHisto;

class gebicAnalysisManager
{

public:
  // With description

  static gebicAnalysisManager* GetInstance();
  static void Dispose();

private:

  gebicAnalysisManager();
  ~gebicAnalysisManager();

public: // Without description

  void BookHisto();

  void BeginOfRun();
  void EndOfRun(G4int);

  void BeginOfEvent();
  void EndOfEvent();

  void AddParticle(G4double, G4double, G4double, G4double);
  //void AddParticle(G4double, G4double, G4double, G4double, G4int);
  void AddIsotope(G4double, G4double, G4double);
  //void AddIsotope(G4double, G4double, G4double, G4int);
  void AddEnergy(G4double, G4double, G4double, G4int);

  void SetVerbose(G4int val) {fVerbose = val;};
  G4int GetVerbose() const {return fVerbose;};

  void SetFirstEventToDebug(G4int val) {fNEvt1 = val;};
  G4int FirstEventToDebug() const {return fNEvt1;};
  void SetLastEventToDebug(G4int val) {fNEvt2 = val;};
  G4int LastEventToDebug() const {return fNEvt2;};

  void SetMaxEnergyforHisto(G4double val) {fHistEMax = val;};
  G4double  GetMaxEnergyforHisto() const {return fHistEMax;};
  void SetMinEnergyforHisto(G4double val) {fHistEMin = val;};
  G4double  GetMinEnergyforHisto() const {return fHistEMin;};
  void SetNumBinforHisto(G4int val) {fHistNBin = val;};
  G4int  GeNumBinforHisto() const {return fHistNBin;};

  void SetThresholdEnergyforTarget(G4double val) {fTargetThresE = val;};
  G4double GetThresholdEnergyforTarget () const {return fTargetThresE;};
  void SetThresholdEnergyforDetector(G4double val) {fDetectorThresE = val;};
  G4double GetThresholdEnergyforDetector () const {return fDetectorThresE;};
  void SetThresholdEnergyforCap(G4double val) {fCapThresE = val;};
  G4double GetThresholdEnergyforCap () const {return fCapThresE;}
  void SetThresholdEnergyforCu(G4double val) {fCuThresE = val;};
  G4double GetThresholdEnergyforCu () const {return fCuThresE;}
  void SetThresholdEnergyforPb(G4double val) {fPbThresE = val;};
  G4double GetThresholdEnergyforPb () const {return fPbThresE;}
//  void SetThresholdEnergyforSupport(G4double val) {fSupportThresE = val;};
//  G4double GetThresholdEnergyforSupport () const {return fSupportThresE;}
  void SetPulseWidth(G4double val) {fPulseWidth = val;};
  G4double GetPulseWidth () const {return fPulseWidth;};

private:

  // MEMBERS
  static gebicAnalysisManager* fManager;

  G4int fVerbose;
  G4int fNEvt1;
  G4int fNEvt2; 

  G4double fHistEMax;
  G4double fHistEMin;
  G4int fHistNBin;
 
  G4double fTargetThresE;
  G4double fDetectorThresE;
  G4double fCapThresE;
  G4double fCuThresE;
  G4double fPbThresE;
  //G4double fSupportThresE;
  G4double fPulseWidth;

  // energy depositions for an event
  std::vector <gebicEnergyDeposition> fEdepo;
  //
  gebicHisto*  fHisto;
  
};

#endif

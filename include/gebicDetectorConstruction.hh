/*
 *  gebicDetectorConstruction.hh
 *
 *
 *
 * 	Geant496
 *
 */
//modified 02/06/2017

#ifndef gebicDetectorConstruction_h
#define gebicDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Polycone.hh"
#include <vector>


class G4Tubs;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;

class gebicDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        gebicDetectorConstruction();
        ~gebicDetectorConstruction();

        G4VPhysicalVolume* Construct();
        void ConstructSDandField();

        const G4VPhysicalVolume* GetDetector() {return physiDetector;};

        //G4double getWorldLength() {return fWorldLength;};
        void setTargetMaterial (G4String);
        //void setWorldDiameter(G4double value) {fWorldDiameter = value;};
    private:
        void DefineMaterials();
        //world
	    G4LogicalVolume*   logicWorld;    // pointer to the logical world
	    G4VPhysicalVolume* physiWorld;    // pointer to the physical world

	//target
	//G4LogicalVolume* logicTarget;
	//G4VPhysicalVolume* physiTarget;
	G4LogicalVolume* logicTarget1;
	G4VPhysicalVolume* physiTarget1;


	//detector
	//G4Polycone* solidDetDZ1;
	G4Polycone* solidDetDZ;
	G4Polycone* solidDet;
	G4LogicalVolume*   _logicDetector;  // pointer to the logical Detector
	G4VPhysicalVolume* physiDetector;  // pointer to the physical Detector

	//detector dead layer
	G4LogicalVolume* logicDetectorDeadLayer;
	G4VPhysicalVolume* physiDetectorDeadLayer;
	//G4LogicalVolume* logicDetectorDeadLayer2;
	//G4VPhysicalVolume* physiDetectorDeadLayer2;

	//Cap
	G4LogicalVolume* logicCap1;
	G4VPhysicalVolume* physiCap1;
	G4LogicalVolume* logicCap2;
	G4VPhysicalVolume* physiCap2;
	G4LogicalVolume* logicCap3;
	G4VPhysicalVolume* physiCap3;

	//Vacuum inside cryostat
	G4LogicalVolume* logicVacCryo;
	G4VPhysicalVolume* physiVacCryo;

	//Detector holder
	G4LogicalVolume* logicHolder1;
	G4VPhysicalVolume* physiHolder1;
	G4LogicalVolume* logicHolder2;
	G4VPhysicalVolume* physiHolder2;

	//Pb thin layer around holder inner part (modified 16/03/2012)
	G4LogicalVolume* logicLayerThinPb1;
	G4VPhysicalVolume* physiLayerThinPb1;
	G4LogicalVolume* logicLayerThinPb2;
	G4VPhysicalVolume* physiLayerThinPb2;
	G4LogicalVolume* logicLayerThinPb3;
	G4VPhysicalVolume* physiLayerThinPb3;
	G4LogicalVolume* logicLayerThinPb4;
	G4VPhysicalVolume* physiLayerThinPb4;
	G4LogicalVolume* logicLayerThinPb5;
	G4VPhysicalVolume* physiLayerThinPb5;

	//Pb layer under crystal
	G4LogicalVolume* logicLayerPb;
	G4VPhysicalVolume* physiLayerPb;

	//lead shielding
	G4LogicalVolume* logicPbShield1;
	G4VPhysicalVolume* physiPbShield1;
	G4LogicalVolume* logicPbShield2;
	G4VPhysicalVolume* physiPbShield2;
	G4LogicalVolume* logicPbShield3;
	G4VPhysicalVolume* physiPbShield3;
	G4LogicalVolume* logicPbShield4;
	G4VPhysicalVolume* physiPbShield4;
	G4LogicalVolume* logicPbShield5;
	G4VPhysicalVolume* physiPbShield5;
	G4LogicalVolume* logicPbShield6;
	G4VPhysicalVolume* physiPbShield6;
    G4LogicalVolume* logicPbShield7;
    G4VPhysicalVolume* physiPbShield7;
    G4LogicalVolume* logicPbShield8;
    G4VPhysicalVolume* physiPbShield8;
    G4LogicalVolume* logicPbShield9;
    G4VPhysicalVolume* physiPbShield9;
    G4LogicalVolume* logicPbShield10;
    G4VPhysicalVolume* physiPbShield10;

	//copper shielding
	G4LogicalVolume* logicCuShield1;
	G4VPhysicalVolume* physiCuShield1;
	G4LogicalVolume* logicCuShield2;
	G4VPhysicalVolume* physiCuShield2;
	G4LogicalVolume* logicCuShield3;
	G4VPhysicalVolume* physiCuShield3;
	G4LogicalVolume* logicCuShield4;
	G4VPhysicalVolume* physiCuShield4;
	G4LogicalVolume* logicCuShield5;
	G4VPhysicalVolume* physiCuShield5;
	G4LogicalVolume* logicCuShield6;
	G4VPhysicalVolume* physiCuShield6;


	//gemcDetectorMessenger* detectorMessenger;  // pointer to the Messenger

	G4Material* DefaultMatter;          // Default material
	G4Material* TarMatter;           // Target material
	//G4Material* SupportMatter;
	G4Material* DetectorMatter;         // Detector material
	G4Material* CapMatter;			    //Cap material
	G4Material* VacCryoMatter;
	G4Material* HolderMatter;
	G4Material* LayerPbMatter;
	G4Material* ShieldPbMatter;
	G4Material* ShieldCuMatter;


	//------dimensions

	G4double fWorldLength;            // Full length the world volume
	G4double fWorldDiameter;          // Diameter of  the world volume

	G4double fVacCryoExternalDiameter;
	G4double fVacCryoLength;

	G4double fCapExternalDiameter;
	G4double fCapInternalDiameter;
	G4double fCapLength;
	G4double fCapWindowDiameter;
	G4double fCapWindowWidth;

	G4double fDetectorDiameter;
	G4double fDetectorLength;
	G4double fDetectorBulletRadius;
	G4double fDetectorHoleDiameter;
	G4double fDetectorHoleLength;
	G4double fDetectorHoleBulletRadius;
	G4double fDetectorDeadLayer;
	G4double fDistanceDetectorWindowCap;

	G4double fHolderExternalDiameter;
	G4double fHolderInternalDiameter;
	G4double fHolderLength;
	G4double fHolderBottomWidth;
	G4double fDetectorOverHolder;
	G4double fDetectorOverPb;

	G4double fLayerThinPbWidth;
	G4double fLayerThinPbL1;
	G4double fLayerThinPbL2;
	G4double fLayerThinPbL3;

	G4double fLayerPbExternalDiameter;
	G4double fLayerPbInternalDiameter;
	G4double fLayerPbLength;

	//Dimensions of the Target(s)
	G4double fL1;
	G4double fL2;
    G4double fL3;


//	G4double fSuppIntDiam1;
//	G4double fSuppExtDiam;
//	G4double fSuppH1;
//	G4double fSuppIntDiam2;
//	G4double fSuppH2;

	G4double fShieldCuDim1;
	G4double fShieldCuDim2;
	G4double fShieldCuDim3;
    G4double fShieldCuDim4;
	G4double fShieldCuDim5;
    G4double fShieldCuDim6;


	G4double fShieldPbDim1;
	G4double fShieldPbDim2;
	G4double fShieldPbDim3;
	G4double fShieldPbDim4;
	G4double fShieldPbDim5;
    G4double fShieldPbDim6;
    G4double fShieldPbDim7;


	G4double fAdjust;
	G4double fMarinelliInt;
	G4double fMarinelliDInt;
	G4double fDistanceEndcapCeiling;
	G4double fDistanceEndcapFloor;
   //-------------------

	G4Region*   targetRegion1;
    G4Region*   detectorRegion;
	G4Region*   detectorDeadLayerRegion;
	G4Region*   capRegion;
	G4Region*   shieldPbRegion;
	G4Region*   shieldCuRegion;
//	G4Region*   supportRegion;
	G4Region*   holderRegion;
	G4Region*   layerPbRegion;
	G4Region*   layerThinPbRegion;

};

#endif

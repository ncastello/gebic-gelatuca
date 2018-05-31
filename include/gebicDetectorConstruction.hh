/*
 *  gebicDetectorConstruction.hh
 *
 *
 *
 * 	Geant4.10
 *
 */

#ifndef GEBICDETECTORCONSTRUCTION_H
#define GEBICDETECTORCONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Polycone.hh"
#include "globals.hh"

#include <vector>

class G4Tubs;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
class G4VisAttributes;

class gebicDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        gebicDetectorConstruction();
        virtual ~gebicDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();
        virtual void ConstructSDandField();

        // Set Paratmeters to Target of Pb for DAMIC (UI options) XXX
        void SetTarget(G4String _targetMaterial, G4double _targetLx, G4double _targetLy, G4double _targetLz);

    private:
        void DefineCommands();
        void DefineMaterials();
        void SetG4Polycone();

        //G4GenericMessenger* _detectorMessenger;

        G4String _targetMaterialName;
        G4double _targetLx;
        G4double _targetLy;
        G4double _targetLz;

        // HPGe detector
        G4LogicalVolume* _logicDetector;  // pointer to the logical Detector
        G4LogicalVolume* _logicDetectorDeadLayer;
        G4LogicalVolume* _logicTarget;

        // Materials
        G4Material * _defaultMatter;
        G4Material * _tarMatter;
        G4Material * _shieldPbMatter;
        G4Material * _shieldCuMatter;
        G4Material * _detectorMatter;
        G4Material * _capMatter;
        G4Material * _holderMatter;
        G4Material * _layerPbMatter;
        G4Material * _vacCryoMatter;

        G4LogicalVolume * _logicWorld;

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

    	G4Region* _targetRegion1;
        G4Region* _detectorRegion;
    	G4Region* _detectorDeadLayerRegion;
	    G4Region* _capRegion;
    	G4Region* _shieldPbRegion;
	    G4Region* _shieldCuRegion;
	    G4Region* _holderRegion;
    	G4Region* _layerPbRegion;
	    G4Region* _layerThinPbRegion;

        std::vector<G4VisAttributes*> _visAttrs;
};

#endif

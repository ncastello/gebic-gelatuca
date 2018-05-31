/*
 *  gebicDetectorConstruction.cc
 *
 *
 *
 *	Geant496
 */


//modified 02/06/2017
// !!!! DAMIC Pb piece 150mmx50mx25mm, M=2132.2g
//Geometry: Parallelepiped 114mmx114mmx2.5mm
//over Marinelli central part.
//GELATUCA in cube (bricks) Pb 20 cm and (cube) Cu 10 cm shields.
//detector dead zone considered
//distance between endcap and the bottom of the sample = 9.9 mm in GEANAYET???
//
//
//re-checked
//measured distance between endcap and Cu ceiling 95.5 mm. In simulation considered 90 mm
// --------------------> NCM It is not 85 mm??????? WHY 90mm??
//
//
//measured distance between endcap and Cu floor 265 mm. In simulation considered 260 mm.  NCM (OK!!)
//moved all structure so that the 0 of the coordinate system is on endcap
//
//
//fMov = 350/2-90=85 mm
//
//
//Move everything down with fMov (subtract fMov from the z coordinate of all vectors of Cu and Pb shields)
//


#include "gebicDetectorConstruction.hh"
#include "gebicSensitiveDetector.hh"

#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
//#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4String.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistMaterialBuilder.hh"
#include "G4NistManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4PhysicalConstants.hh"

#include "TROOT.h"
#include "TMath.h"

#include "G4ios.hh"
#include <sstream>

gebicDetectorConstruction::gebicDetectorConstruction() :
    _targetMaterialName(""),
    _targetLx(150.0*mm),
    _targetLy(50.0*mm),
    _targetLz(25.0*mm),
    _logicDetector(nullptr),
    _logicDetectorDeadLayer(nullptr),
    _logicTarget(nullptr),
    _defaultMatter(nullptr),
    _tarMatter(nullptr),
    _shieldPbMatter(nullptr),
    _shieldCuMatter(nullptr),
    _detectorMatter(nullptr),
    _capMatter(nullptr),
    _holderMatter(nullptr),
    _layerPbMatter(nullptr),
    _vacCryoMatter(nullptr),
    _targetRegion1(nullptr),
    _detectorRegion(nullptr),
    _detectorDeadLayerRegion(nullptr),
    _capRegion(nullptr),
    _shieldPbRegion(nullptr),
    _shieldCuRegion(nullptr),
    _holderRegion(nullptr),
    _layerPbRegion(nullptr),
    _layerThinPbRegion(nullptr),
    _visAttrs()
{
    //detectorMessenger = new gebicDetectorMessenger(this);
    DefineMaterials();

	//Dimensions of the World (G4Box)
    fWorldDiameter	=  900.0*mm;
    fWorldLength  = 1050.0*mm;

    //Dimensions of the cryostat
    fVacCryoExternalDiameter  = 105.0*mm;
    fVacCryoLength = 196.0*mm;

    //Dimensions of the cap
    fCapExternalDiameter = 105.0*mm;
	fCapInternalDiameter  = 100.0*mm;
    fCapLength  = 196.0*mm;
	fCapWindowDiameter =  86.6*mm;
    fCapWindowWidth	  =   1.6*mm;

	//Dimensions of the detector (GeLaTuca)
	fDetectorDiameter  =  81.0*mm;
	fDetectorLength	  =  81.0*mm;
	fDetectorBulletRadius  =   6.0*mm;
	fDetectorHoleDiameter  =  14.0*mm;
	fDetectorHoleLength  =  59.0*mm;
	fDetectorHoleBulletRadius =  1.0*mm;
    //fDetectorDeadLayer = 0.5*mm;//dead layer width -- see GeLatuca Canberra docs
    fDetectorDeadLayer = 0.5*mm; //do not put fDetectorDeadLayer>22. mm!!!!!
	fDistanceDetectorWindowCap = 3.0*mm;

    //Dimensions of the holder
	fHolderExternalDiameter	   =  86.6*mm;
    fHolderInternalDiameter    =  82.6*mm;
	//fHolderLength  =  90.0*mm;
    fHolderBottomWidth =  8.0*mm;
	//fHolderOverDetector  =63.6*mm;
    fDetectorOverHolder = 18.1*mm;
	fDetectorOverPb = 8.4*mm;
    //Dimensions of the Pb layer under the Ge crystal
	fLayerPbExternalDiameter   =  82.6*mm;
    fLayerPbInternalDiameter   =   0.0*mm;
	fLayerPbLength			   =  10.0*mm;
    fHolderLength = fDetectorLength-fDetectorOverHolder+fHolderBottomWidth+fLayerPbLength+fDetectorOverPb;


	//Dimensions Pb layer around crystal (modified 16/03/2012)
    fLayerThinPbWidth = 0.3*mm;
	fLayerThinPbL1 = 60.0*mm;
    fLayerThinPbL2 = 22.0*mm;
	fLayerThinPbL3 = 10.0*mm;

    //Dimensions of the Target(s) (DAMIC)
    //_targetLx = 150.0*mm;//x
    //_targetLy = 50.0*mm;//y
    //_targetLz = 25.0*mm;//z

    //Dimensions of the Cu shield
    fShieldCuDim1 = 500.0*mm;
    fShieldCuDim2 = 400.0*mm;
	fShieldCuDim3 = 350.0*mm;
    fShieldCuDim4 = 100.0*mm;


    //Dimensions of the Pb shield
	fShieldPbDim1 = 900.0*mm;
    fShieldPbDim2 = 700.0*mm;
	fShieldPbDim3 = 450.0*mm;
    fShieldPbDim4 = 200.0*mm;
    fShieldPbDim5 = 400.0*mm;
    fShieldPbDim6 = 300.0*mm;
    fShieldPbDim7 = 250.0*mm;

    fMarinelliInt = 104.0*mm;
	fMarinelliDInt = 113.0*mm;
    fDistanceEndcapCeiling = 85.0*mm;
	fDistanceEndcapFloor = 265.0*mm;

}


gebicDetectorConstruction::~gebicDetectorConstruction()
{
    for(auto * visAttributes: _visAttrs)
    {
        if(visAttributes != nullptr)
        {
            delete visAttributes;
            visAttributes = nullptr;
        }
    }
    //delete detectorMessenger;
}

void gebicDetectorConstruction::DefineMaterials()
{
	//Material definition
	//G4String name,symbol;

    G4UnitDefinition::BuildUnitsTable();
	G4NistManager* nistman = G4NistManager::Instance();
	nistman->SetVerbose(1);

	_defaultMatter = nistman->FindOrBuildMaterial("G4_AIR");

    _tarMatter = nistman->FindOrBuildMaterial("G4_Pb");

	_shieldPbMatter = nistman->FindOrBuildMaterial("G4_Pb");
	_shieldCuMatter = nistman->FindOrBuildMaterial("G4_Cu");
	_detectorMatter = nistman->FindOrBuildMaterial("G4_Ge");

	_capMatter = nistman->FindOrBuildMaterial("G4_Al");
	_holderMatter = nistman->FindOrBuildMaterial("G4_Al");
	_layerPbMatter = _shieldPbMatter;
	//_supportMatter = nistman->FindOrBuildMaterial("G4_POLYPROPYLENE");
	_vacCryoMatter = nistman->FindOrBuildMaterial("G4_Galactic");
}

void gebicDetectorConstruction::SetG4Polycone()
{
    /*
     Defining the all arguments to construct the Detector volume as a polycon
        Denote principal axes with
             0 (0 angle with vertical),
             1 (90/4 angle with vertical),
             2 (90/2 angle with vertical),
             3 (3*90/4 angle with vetical)
             4 (90 angle with vertical)
        Denote 1v, 2v, 3v the intersection of DL with axes
    */

    G4double phiStart=0;
    G4double phiTotal=0;

    G4double z0,z1,z1v,z2,z2v,z3,z3v,z4;
    G4double h0,h1,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7,hv,hvv;
    G4double rDZE0,rDZI1,rDZE1,rDZI1v,rDZE1v,rDZI2,rDZE2,rDZI2v;
    G4double rDZE2v,rDZI3,rDZE3,rDZI3v,rDZE3v,rDZI4,rDZE4,rDZIv,rDZEv;
    G4double rDI,rDIv,rDEv;

    // z0, angle with vertical 0
    rDZE0 = this->fDetectorDiameter/2.-this->fDetectorBulletRadius;

    //z... angle with vertical the angles: 0., 90/4., 90/2., 3*90/4 and 90.
    z0 = this->fDetectorBulletRadius*(1-0.);
    z1 = this->fDetectorBulletRadius*(1.-0.924);
    z2 = this->fDetectorBulletRadius*(1.-0.707);
    z3 = this->fDetectorBulletRadius*(1.-0.382);
    z4 = this->fDetectorBulletRadius*(1.-1.);

    h0 = this->fDetectorLength/2.;
    h1 = h0-z1;
    h2 = h0-z2;
    h3 = h0-z3;
    h4 = h0-z4;
    h5 =-h0+this->fDetectorHoleLength;
    h6 = h5-this->fDetectorHoleBulletRadius;
    h7 =-h0;

    rDZE0 = this->fDetectorDiameter/2. - this->fDetectorBulletRadius;
    rDZI1 = rDZE0 + TMath::Sqrt((this->fDetectorBulletRadius-this->fDetectorDeadLayer) * \
                (this->fDetectorBulletRadius - this->fDetectorDeadLayer) - \
                (this->fDetectorBulletRadius -z1)*( this->fDetectorBulletRadius - z1));

    rDZE1 = rDZE0 + TMath::Sqrt(this->fDetectorBulletRadius*this->fDetectorBulletRadius \
            - (this->fDetectorBulletRadius-z1)*(this->fDetectorBulletRadius-z1));



}

G4VPhysicalVolume* gebicDetectorConstruction::Construct()
{
    const bool checkOverlaps = false;

    // ##################### World
	G4Box* solidWorld= new G4Box("world",
            fWorldDiameter/2.,
            fWorldDiameter/2.,
            fWorldLength/2.);

	_logicWorld= new G4LogicalVolume(solidWorld, _defaultMatter, "World", 0, 0, 0);

    G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,
            G4ThreeVector(),
            _logicWorld,
            "World",
            0,
            false,
            checkOverlaps);

	// ##################### Pb shield
	G4Box* basePbShield = new G4Box("basePbShield",
            fShieldPbDim1/2., fShieldPbDim1/2., fShieldPbDim4/2.);
	G4Box* wallPbShield = new G4Box("wall1PbShield",
            fShieldPbDim2/2., fShieldPbDim4/2., fShieldPbDim3/2.);
    G4Box* doorPbShield = new G4Box("doorPbShield",
            fShieldPbDim5/2., fShieldPbDim5/2., fShieldPbDim4/2.);
    G4Box* walltopPbShield = new G4Box("walltopPbShield",
            (fShieldPbDim5+fShieldPbDim7)/2., fShieldPbDim7/2., fShieldPbDim6/2.);

	G4double phiX=0.*deg;
	G4double phiY=0.*deg;
	G4double phiZ=0.*deg;

	G4RotationMatrix rpb1;
    rpb1.rotateX(phiX);
    rpb1.rotateY(phiY);
    rpb1.rotateZ(phiZ);

    G4ThreeVector tpb1 = G4ThreeVector(0.,0.,(-fDistanceEndcapFloor-fShieldCuDim4-fShieldPbDim4/2.));
	G4LogicalVolume* logicPbShield1 =
        new G4LogicalVolume(basePbShield,_shieldPbMatter,"PbShield1",0,0,0);//base Pb
	new G4PVPlacement(
            G4Transform3D(rpb1,tpb1),
            logicPbShield1,
            "PbShield1",
            _logicWorld,
            false,
            checkOverlaps);//base Pb

    G4ThreeVector tpb2 = G4ThreeVector(0.,0.,(fDistanceEndcapCeiling+fShieldCuDim4+fShieldPbDim4/2.));
	G4LogicalVolume* logicPbShield2 =
        new G4LogicalVolume(doorPbShield,_shieldPbMatter,"PbShield2",0,0,0);//door Pb
	new G4PVPlacement(
            G4Transform3D(rpb1,tpb2),
            logicPbShield2,
            "PbShield2",
            _logicWorld,
            false,
            checkOverlaps);//top Pb


    G4ThreeVector tpb3 = G4ThreeVector(fShieldPbDim4/2.,
            (-fWorldDiameter/2.+fShieldPbDim4/2.),
            (-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	G4LogicalVolume* logicPbShield3 = new G4LogicalVolume(wallPbShield,
            _shieldPbMatter,"PbShield3",0,0,0);//front wall Pb
	new G4PVPlacement(
            G4Transform3D(rpb1,tpb3),
            logicPbShield3,
            "PbShield3",
            _logicWorld,
            false,
            checkOverlaps);//front wall Pb


    G4ThreeVector tpb4 = G4ThreeVector(-fShieldPbDim4/2.,
            (fWorldDiameter/2.-fShieldPbDim4/2.),
            (-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	G4LogicalVolume* logicPbShield4 =
        new G4LogicalVolume(wallPbShield,_shieldPbMatter,"PbShield4",0,0,0);//back wall Pb
	new G4PVPlacement(
            G4Transform3D(rpb1,tpb4),
            logicPbShield4,
            "PbShield4",
            _logicWorld,
            false,
            checkOverlaps);//back wall Pb

	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpb2;
    rpb2.rotateX(phiX);
    rpb2.rotateY(phiY);
    rpb2.rotateZ(phiZ);

    G4ThreeVector tpb5 =
        G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4/2.),
                -fShieldPbDim4/2.,
                (-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	G4LogicalVolume* logicPbShield5 =
        new G4LogicalVolume(wallPbShield,_shieldPbMatter,"PbShield5",0,0,0);//left wall Pb
	new G4PVPlacement(
            G4Transform3D(rpb2,tpb5),
            logicPbShield5,
            "PbShield5",
            _logicWorld,
            false,
            checkOverlaps);//left wall Pb

    G4ThreeVector tpb6 =
        G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4/2.),
                fShieldPbDim4/2.,
                (-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	G4LogicalVolume* logicPbShield6 =
        new G4LogicalVolume(wallPbShield,_shieldPbMatter,"PbShield6",0,0,0);//right wall Pb
	new G4PVPlacement(
            G4Transform3D(rpb2,tpb6),
            logicPbShield6,
            "PbShield6",
            _logicWorld,
            false,checkOverlaps);//right wall Pb


    G4ThreeVector tpb7 =
        G4ThreeVector((fShieldPbDim4/2.+25.0*mm),
                (-fWorldDiameter/2.+fShieldPbDim7/2.),
                (fDistanceEndcapCeiling+fShieldPbDim6/2.));
    G4LogicalVolume* logicPbShield7 =
        new G4LogicalVolume(walltopPbShield,_shieldPbMatter,"PbShield7",0,0,0);//front top wall Pb
    new G4PVPlacement(
            G4Transform3D(rpb1,tpb7),
            logicPbShield7,
            "PbShield7",
            _logicWorld,
            false,
            checkOverlaps);//front wall top Pb

    G4ThreeVector tpb8 =
        G4ThreeVector((-fShieldPbDim4/2.-25.0*mm),
                (fWorldDiameter/2.-fShieldPbDim7/2.),
                (fDistanceEndcapCeiling+fShieldPbDim6/2.));
    G4LogicalVolume* logicPbShield8 =
        new G4LogicalVolume(walltopPbShield,_shieldPbMatter,"PbShield8",0,0,0);//back wall top Pb
    new G4PVPlacement(
            G4Transform3D(rpb1,tpb8),
            logicPbShield8,
            "PbShield8",
            _logicWorld,
            false,
            checkOverlaps);//back wall top Pb

    G4ThreeVector tpb9 =
        G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim7/2.),
                (-fShieldPbDim4/2.-25.0*mm),
                (fDistanceEndcapCeiling+fShieldPbDim6/2.));
    G4LogicalVolume* logicPbShield9 =
        new G4LogicalVolume(walltopPbShield,_shieldPbMatter,"PbShield9",0,0,0);//left wall top Pb
    new G4PVPlacement(
            G4Transform3D(rpb2,tpb9),
            logicPbShield9,
            "PbShield9",
            _logicWorld,
            false,
            checkOverlaps);//left wall top Pb

    G4ThreeVector tpb10 =
        G4ThreeVector((fWorldDiameter/2.-fShieldPbDim7/2.),
                (fShieldPbDim4/2.+25.0*mm),
                (fDistanceEndcapCeiling+fShieldPbDim6/2.));
    G4LogicalVolume* logicPbShield10 =
        new G4LogicalVolume(walltopPbShield,_shieldPbMatter,"PbShield10",0,0,0);//right wall top Pb
    new G4PVPlacement(
            G4Transform3D(rpb2,tpb10),
            logicPbShield10,
            "PbShield10",
            _logicWorld,
            false,
            checkOverlaps);//right wall top Pb


	// ##################### Cu shield
	G4Box* baseCuShield = new G4Box("baseCuShield", fShieldCuDim1/2., fShieldCuDim1/2., fShieldCuDim4/2.);
	G4Box* wallCuShield = new G4Box("wall1CuShield", fShieldCuDim2/2., fShieldCuDim4/2., fShieldCuDim3/2.);
    G4Box* doorCuShield = new G4Box("doorCuShield", fShieldCuDim2/2., fShieldCuDim2/2., fShieldCuDim4/2.);

	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;

	G4RotationMatrix rcu1;
    rcu1.rotateX(phiX);
    rcu1.rotateY(phiY);
    rcu1.rotateZ(phiZ);
    G4ThreeVector tcu1 = G4ThreeVector(0.,0.,(-fDistanceEndcapFloor-fShieldCuDim4/2.));
	G4LogicalVolume* logicCuShield1 =
        new G4LogicalVolume(baseCuShield,_shieldCuMatter,"CuShield1",0,0,0);//base Cu
	new G4PVPlacement(
            G4Transform3D(rcu1,tcu1),
            logicCuShield1,
            "CuShield1",
            _logicWorld,
            false,
            checkOverlaps);//base Cu

    G4ThreeVector tcu2 = G4ThreeVector(0.,0.,(fDistanceEndcapCeiling+fShieldCuDim4/2.));
	G4LogicalVolume* logicCuShield2 =
        new G4LogicalVolume(doorCuShield,_shieldCuMatter,"CuShield2",0,0,0);//top Cu
	//NCM remove CuShield2 (TAPA) from the world, to check 60Co ---->
    new G4PVPlacement(
            G4Transform3D(rcu1,tcu2),
            logicCuShield2,
            "CuShield2",
            _logicWorld,
            false,
            checkOverlaps);//top Cu

	//G4ThreeVector tcu3 = G4ThreeVector(fShieldCuDim4/2.,(-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),0.);
    G4ThreeVector tcu3 =
        G4ThreeVector(fShieldCuDim4/2.,
                (-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),
                (-fDistanceEndcapFloor+fShieldCuDim3/2.));
	G4LogicalVolume* logicCuShield3 =
        new G4LogicalVolume(wallCuShield,_shieldCuMatter,"CuShield3",0,0,0);//front wall Cu
	new G4PVPlacement(
            G4Transform3D(rcu1,tcu3),
            logicCuShield3,
            "CuShield3",
            _logicWorld,
            false,
            checkOverlaps);//front wall Cu

	//G4ThreeVector tcu4 = G4ThreeVector(-fShieldCuDim4/2.,(fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),0.);
    G4ThreeVector tcu4 =
        G4ThreeVector(-fShieldCuDim4/2.,
                (fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),
                (-fDistanceEndcapFloor+fShieldCuDim3/2.));
	G4LogicalVolume* logicCuShield4 =
        new G4LogicalVolume(wallCuShield,_shieldCuMatter,"CuShield4",0,0,0);//back wall Cu
	new G4PVPlacement(
            G4Transform3D(rcu1,tcu4),
            logicCuShield4,
            "CuShield4",
            _logicWorld,
            false,
            checkOverlaps);//back wall Cu


	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rcu2;
    rcu2.rotateX(phiX);
    rcu2.rotateY(phiY);
    rcu2.rotateZ(phiZ);
	//G4ThreeVector tcu5 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),-fShieldCuDim4/2.,0.);
    G4ThreeVector tcu5 =
        G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),
                -fShieldCuDim4/2.,
                (-fDistanceEndcapFloor+fShieldCuDim3/2.));
	G4LogicalVolume* logicCuShield5 =
        new G4LogicalVolume(wallCuShield,_shieldCuMatter,"CuShield5",0,0,0);//left wall Cu
	new G4PVPlacement(
            G4Transform3D(rcu2,tcu5),
            logicCuShield5,
            "CuShield5",
            _logicWorld,
            false,
            checkOverlaps);//left wall Cu

	//G4ThreeVector tcu6 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),fShieldCuDim4/2.,0.);
    G4ThreeVector tcu6 =
        G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),
                fShieldCuDim4/2.,
                (-fDistanceEndcapFloor+fShieldCuDim3/2.));
	G4LogicalVolume* logicCuShield6 =
        new G4LogicalVolume(wallCuShield,_shieldCuMatter,"CuShield6",0,0,0);//right wall Cu
	new G4PVPlacement(
            G4Transform3D(rcu2,tcu6),
            logicCuShield6,
            "CuShield6",
            _logicWorld,
            false,
            checkOverlaps);//right wall Cu


	// ##################### VacCryo
	G4Tubs* solidVacCryo = new G4Tubs("solidVacCryo", 0., fVacCryoExternalDiameter/2., fVacCryoLength/2., 0, twopi);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rvc;
    rvc.rotateX(phiX);
    rvc.rotateY(phiY);
    rvc.rotateZ(phiZ);
    G4ThreeVector tvc = G4ThreeVector(0.,0.,-fVacCryoLength/2.);
	G4LogicalVolume* logicVacCryo =
        new G4LogicalVolume(solidVacCryo, _vacCryoMatter, "VacCryo", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rvc,tvc),
            logicVacCryo,
            "VacCryo",
            _logicWorld,
            false,
            checkOverlaps);

	// ##################### Cap
	G4double fCapWidth2 = (fCapExternalDiameter-fCapInternalDiameter)/2.-fCapWindowWidth;
	G4Tubs* solidCap1 =
        new G4Tubs("solidCap1", 0,
                fCapInternalDiameter/2.,
                fCapWindowWidth/2.,
                0, twopi);//window part of cap -- upper part

	G4Tubs* solidCap2 =
        new G4Tubs("solidCap2",
                fCapWindowDiameter/2.,
                fCapInternalDiameter/2.,
                fCapWidth2/2.,
                0, twopi);//window part of cap -- hole part

    G4Tubs* solidCap3 =
        new G4Tubs("solidCap3",
                fCapInternalDiameter/2.,
                fCapExternalDiameter/2.,
                fCapLength/2.,
                0.,twopi);//cap body
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rcap;
    rcap.rotateX(phiX);
    rcap.rotateY(phiY);
    rcap.rotateZ(phiZ);
    G4ThreeVector tcap1 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth/2.));
    G4LogicalVolume* logicCap1 = new G4LogicalVolume(solidCap1, _capMatter, "Cap1", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rcap,tcap1),
            logicCap1,
            "Cap1",
            logicVacCryo,
            false,
            checkOverlaps);

	G4ThreeVector tcap2 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fCapWidth2/2.));
    G4LogicalVolume* logicCap2 = new G4LogicalVolume(solidCap2, _capMatter, "Cap2", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rcap,tcap2), logicCap2
            , "Cap2",
            logicVacCryo,
            false,
            checkOverlaps);

	G4ThreeVector tcap3 = G4ThreeVector(0.,0.,0.);
    G4LogicalVolume* logicCap3 = new G4LogicalVolume(solidCap3, _capMatter, "Cap3", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rcap,tcap3),
            logicCap3,
            "Cap3",
            logicVacCryo,
            false,
            checkOverlaps);


	// ##################### Detector
	G4double phiStart;
	phiStart = 0.;
	G4double phiTotal;
	phiTotal = twopi;
	//G4double adjustRadius = 1.0*mm;


	//
	// Denote principal axes with
    //          0 (0 angle with vertical),
    //          1 (90/4 angle with vertical),
    //          2 (90/2 angle with vertical),
    //          3 (3*90/4 angle with vetical)
	// and 4 (90 angle with vertical)
	// Denote v, 2v, 3v the intersection of DL with axes
    //
	G4double z0,z1,z1v,z2,z2v,z3,z3v,z4;
	G4double h0,h1,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7,hv,hvv;
	G4double rDZE0,rDZI1,rDZE1,rDZI1v,rDZE1v,rDZI2,rDZE2,rDZI2v,rDZE2v,rDZI3,rDZE3,rDZI3v,rDZE3v,rDZI4,rDZE4,rDZIv,rDZEv;
	G4double rDI,rDIv,rDEv;

	// z0, angle with vertical 0
	z0=0.;//=RDZI1
	h0=fDetectorLength/2.;
	rDZE0=fDetectorDiameter/2.-fDetectorBulletRadius;

	//z1, angle with vertical 90/4, sin(90/4)=0.383, cos(90/4)=0.924
	z1=fDetectorBulletRadius*(1.-0.924);
	h1=h0-z1;
	rDZI1=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z1)*(fDetectorBulletRadius-z1));
    rDZE1=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-z1)*(fDetectorBulletRadius-z1));

	//z1v the position where the line with angle with vertical 90/4, sin(90/4)=0.383, cos(90/4)=0.924
    //touches internal part DL
	z1v=z1+fDetectorDeadLayer*0.924;
	h1v=h0-z1v;
    rDZI1v=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z1v)*(fDetectorBulletRadius-z1v));
	rDZE1v=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-z1v)*(fDetectorBulletRadius-z1v));


	//z2, angle with vertical 2*90/4, sin(90/2)=0.707, cos(90/2)=0.707
	z2=fDetectorBulletRadius*(1.-0.707);
	h2=h0-z2;
	rDZI2=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z2)*(fDetectorBulletRadius-z2));
	rDZE2=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-z2)*(fDetectorBulletRadius-z2));


    //z2v, the position where the line with angle with vertical 2*90/4, sin(90/2)=0.707, cos(90/2)=0.707
    //touches internal part DL
	z2v=z2+fDetectorDeadLayer*0.707;
	h2v=h0-z2v;
	rDZI2v=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z2v)*(fDetectorBulletRadius-z2v));
	rDZE2v=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-z2v)*(fDetectorBulletRadius-z2v));


	//z3, angle with vertical 3*90/4, sin(3*90/4)=0.924, cos(3*90/4)=0.383
	z3=fDetectorBulletRadius*(1.-0.382);
	h3=h0-z3;
	rDZI3=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z3)*(fDetectorBulletRadius-z3));
	rDZE3=rDZE0+TMath::Sqrt((fDetectorBulletRadius*fDetectorBulletRadius)-(fDetectorBulletRadius-z3)*(fDetectorBulletRadius-z3));


    //z3v, the position where the line with angle with vertical 3*90/4, sin(3*90/2)=0.924, cos(3*90/4)=0.383
    //touches internal part DL
	z3v=z3+fDetectorDeadLayer*0.383;
	h3v=h0-z3v;
	rDZI3v=rDZE0+TMath::Sqrt((fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer)-(fDetectorBulletRadius-z3v)*(fDetectorBulletRadius-z3v));
	rDZE3v=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-z3v)*(fDetectorBulletRadius-z3v));

	//z4, angle with vertical 90
	z4=fDetectorBulletRadius;
	h4=h0-z4;
	rDZI4=fDetectorDiameter/2.-fDetectorDeadLayer;
	rDZE4=fDetectorDiameter/2.;

	//z5, position where detector hole starts
	h5=-h0+fDetectorHoleLength;

	//z6, next position a r distance under the previous
	h6=h5-fDetectorHoleBulletRadius;
	rDI=fDetectorHoleDiameter/2.;

    //z7, position where crystal finishes
    h7=-h0;

    //Order 1 new: v<z1 normal order
    //Order 2 new: v=z1 normal order
    //Order 3 new: v>z1, v<z2
    //Order 4 new: v=z2
    //Order 5 new: v>z2, v<z3
    //Order 6 new: v=v3
    //Order 7 new: v>v3, v<v4
    //order 8 new: v=v4
    //Order 9 new: v>v4, v<22. mm

    G4Polycone * solidDetDZ = nullptr;
    G4Polycone * solidDet = nullptr;
    if (fDetectorDeadLayer>0. && fDetectorDeadLayer<z1)
    {
        G4int numZPlanesDZ = 13;
        G4int numZPlanesD = 11;
        hv=h0-fDetectorDeadLayer;
        hvv=hv-0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

        const G4double zPlaneDZ[] = {h0,hvv,hv,h1,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
        const G4double rInnerDZ[] = {z0,z0,rDZIv,rDZI1,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
        const G4double rOuterDZ[] = {rDZE0,rDZEv,rDZEv,rDZE1,rDZE1v,rDZE2,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

        const G4double zPlaneD[] = {hv,h1,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
        const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,z0,z0,rDI,rDI};
        const G4double rOuterD[] = {rDEv,rDZI1,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order1!"<<G4endl;
    }
    else if (fDetectorDeadLayer==z1)
    {
        G4int numZPlanesDZ = 12;
        G4int numZPlanesD = 10;
		hv=h0-fDetectorDeadLayer;
		hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

        const G4double zPlaneDZ[] = {h0,hvv,hv,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
        const G4double rInnerDZ[] = {z0,z0,rDZIv,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
        const G4double rOuterDZ[] = {rDZE0,rDZEv,rDZEv,rDZE1v,rDZE2,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

        const G4double zPlaneD[] = {hv,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
        const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,z0,rDI,rDI};
        const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order2!"<<G4endl;
    }
	else if (fDetectorDeadLayer>z1 && fDetectorDeadLayer<z2)
    {
		hv=h0-fDetectorDeadLayer;
		hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;
        if (z1v<z2)
        {
			G4int numZPlanesDZ = 13;
			G4int numZPlanesD = 10;
			const G4double zPlaneDZ[] = {h0,h1,hvv,hv,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
			const G4double rInnerDZ[] = {z0,z0,z0,rDZIv,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZEv,rDZEv,rDZE1v,rDZE2,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

			const G4double zPlaneD[] = {hv,h1v,h2,h2v,h3,h3v,h4,h5,h6,h7};
			const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,z0,rDI,rDI};
			const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
			solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
		}
		else if (z1v>z2)
        {
			G4int numZPlanesDZ = 13;
			G4int numZPlanesD = 10;
			const G4double zPlaneDZ[] = {h0,h1,hvv,hv,h2,h1v,h2v,h3,h3v,h4,h5,h6,h7};
			const G4double rInnerDZ[] = {z0,z0,z0,rDZIv,rDZI2,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZEv,rDZEv,rDZE2,rDZE1v,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

			const G4double zPlaneD[] = {hv,h2,h1v,h2v,h3,h3v,h4,h5,h6,h7};
			const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,z0,rDI,rDI};
			const G4double rOuterD[] = {rDEv,rDZI2,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
			solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
		}
		else if (z1v==z2)
        {
			G4int numZPlanesDZ = 12;
			G4int numZPlanesD = 9;

			const G4double zPlaneDZ[] = {h0,h1,hvv,hv,h2,h3,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerDZ[] = {z0,z0,z0,rDZIv,rDZI2,rDZI3,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZEv,rDZEv,rDZE2,rDZE3,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

			const G4double zPlaneD[] = {hv,h2,h3,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,rDI,rDI};
			const G4double rOuterD[] = {rDEv,rDZI2,rDZI3,rDZI2v,rDZIv,rDZI4,rDZI4,rDZI4,rDZI4};
			solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
			solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
		}
        G4cout<<"Order3!"<<G4endl;
    }
    else if (fDetectorDeadLayer==z2)
    {

		hv=h0-fDetectorDeadLayer;
		hvv=hv+0.000000001*mm;
		rDZIv=rDZE0;
		rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
		rDIv=z0;
		rDEv=rDZIv;
		G4int numZPlanesDZ = 12;
		G4int numZPlanesD = 9;

		const G4double zPlaneDZ[] = {h0,h1,hvv,hv,h1v,h2v,h3,h3v,h4,h5,h6,h7};
		const G4double rInnerDZ[] = {z0,z0,z0,rDZIv,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZEv,rDZEv,rDZE1v,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

		const G4double zPlaneD[] = {hv,h1v,h2v,h3,h3v,h4,h5,h6,h7};
		const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,rDI,rDI};
		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
		solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
		G4cout<<"Order4!"<<G4endl;
    }
    else if (fDetectorDeadLayer>z2 && fDetectorDeadLayer<z3)
    {
        hv=h0-fDetectorDeadLayer;
	    hvv=hv+0.000000001*mm;
    	rDZIv=rDZE0;
	    rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
    	rDIv=z0;
	    rDEv=rDZIv;
    	if (z1v<z3 && z2v<z3)
        {
            G4int numZPlanesDZ = 13;
    		G4int numZPlanesD = 9;

	    	const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h1v,h2v,h3,h3v,h4,h5,h6,h7};
		    const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
    		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE1v,rDZE2v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

	    	const G4double zPlaneD[] = {hv,h1v,h2v,h3,h3v,h4,h5,h6,h7};
		    const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,rDI,rDI};
    		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

	    	solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
		    solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        }
        else if (z1v<z3 && z2v==z3)
        {
            G4int numZPlanesDZ = 12;
    		G4int numZPlanesD = 8;

	    	const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h1v,h3,h3v,h4,h5,h6,h7};
		    const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI1v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
    		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE1v,rDZE3,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

	    	const G4double zPlaneD[] = {hv,h1v,h3,h3v,h4,h5,h6,h7};
		    const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,rDI,rDI};
    		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI3,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

		    solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
	    	solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        }
        else if (z1v<z3 && z2v>z3)
        {
		    G4int numZPlanesDZ = 13;
    		G4int numZPlanesD = 9;

	    	const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h1v,h3,h2v,h3v,h4,h5,h6,h7};
		    const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI1v,rDZI3,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
    		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE1v,rDZE3,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

	    	const G4double zPlaneD[] = {hv,h1v,h3,h2v,h3v,h4,h5,h6,h7};
		    const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,rDI,rDI};
    		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI3,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

            solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
		    solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
    	}
        else if (z1v==z3 && z2v>z3)
        {
            G4int numZPlanesDZ = 12;
			G4int numZPlanesD = 8;

			const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h3,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI3,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE3,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

			const G4double zPlaneD[] = {hv,h3,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,rDI,rDI};
			const G4double rOuterD[] = {rDEv,rDZI3,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

            solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
			solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);

        }
		else if (z1v>z3 && z2v>z3)
        {
            G4int numZPlanesDZ = 13;
			G4int numZPlanesD = 9;

			const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h3,h1v,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI3,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
			const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE3,rDZE1v,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

			const G4double zPlaneD[] = {hv,h3,h1v,h2v,h3v,h4,h5,h6,h7};
			const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,z0,rDI,rDI};
			const G4double rOuterD[] = {rDEv,rDZI3,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

            solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
			solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);

        }
        G4cout<<"Order5!"<<G4endl;
    }
    else if (fDetectorDeadLayer==z3)
    {
        G4int numZPlanesDZ = 12;
		G4int numZPlanesD = 8;
		hv=h0-fDetectorDeadLayer;
		hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

   		const G4double zPlaneDZ[] = {h0,h1,h2,hvv,hv,h1v,h2v,h3v,h4,h5,h6,h7};
   		const G4double rInnerDZ[] = {z0,z0,z0,z0,rDZIv,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZEv,rDZEv,rDZE1v,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

		const G4double zPlaneD[] = {hv,h1v,h2v,h3v,h4,h5,h6,h7};
		const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,rDI,rDI};
		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order6!"<<G4endl;
    }
    else if (fDetectorDeadLayer>z3 && fDetectorDeadLayer<z4)
    {
        G4int numZPlanesDZ = 13;
        G4int numZPlanesD = 8;
		hv=h0-fDetectorDeadLayer;
		hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

		const G4double zPlaneDZ[] = {h0,h1,h2,h3,hvv,hv,h1v,h2v,h3v,h4,h5,h6,h7};
		const G4double rInnerDZ[] = {z0,z0,z0,z0,z0,rDZIv,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};
		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZE3,rDZEv,rDZEv,rDZE1v,rDZE2v,rDZE3v,rDZE4,rDZE4,rDZE4,rDZE4};

		const G4double zPlaneD[] = {hv,h1v,h2v,h3v,h4,h5,h6,h7};
		const G4double rInnerD[] = {rDIv,z0,z0,z0,z0,z0,rDI,rDI};
		const G4double rOuterD[] = {rDEv,rDZI1v,rDZI2v,rDZI3v,rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order7!"<<G4endl;
    }
     else if (fDetectorDeadLayer==z4)
     {
        G4int numZPlanesDZ = 9;
        G4int numZPlanesD = 4;
        hv=h0-fDetectorDeadLayer;
        hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

		const G4double zPlaneDZ[] = {h0,h1,h2,h3,hvv,hv,h5,h6,h7};
		const G4double rInnerDZ[] = {z0,z0,z0,z0,z0,rDZI4,rDZI4,rDZI4,rDZI4};
		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZE3,rDZE4,rDZE4,rDZE4,rDZE4,rDZE4};

		const G4double zPlaneD[] = {hv,h5,h6,h7};
		const G4double rInnerD[] = {rDIv,z0,rDI,rDI};
		const G4double rOuterD[] = {rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order9!"<<G4endl;
    }
    else if (fDetectorDeadLayer>z4)
    {
        G4int numZPlanesDZ = 10;
        G4int numZPlanesD = 4;
        hv=h0-fDetectorDeadLayer;
        hvv=hv+0.000000001*mm;
        rDZIv=rDZE0;
        rDZEv=rDZE0+TMath::Sqrt(fDetectorBulletRadius*fDetectorBulletRadius-(fDetectorBulletRadius-fDetectorDeadLayer)*(fDetectorBulletRadius-fDetectorDeadLayer));
        rDIv=z0;
        rDEv=rDZIv;

		const G4double zPlaneDZ[] = {h0,h1,h2,h3,h4,hvv,hv,h5,h6,h7};
		const G4double rInnerDZ[] = {z0,z0,z0,z0,z0,z0,rDZI4,rDZI4,rDZI4,rDZI4};
		const G4double rOuterDZ[] = {rDZE0,rDZE1,rDZE2,rDZE3,rDZE4,rDZE4,rDZE4,rDZE4,rDZE4,rDZE4};

		const G4double zPlaneD[] = {hv,h5,h6,h7};
		const G4double rInnerD[] = {rDIv,z0,rDI,rDI};
		const G4double rOuterD[] = {rDZI4,rDZI4,rDZI4,rDZI4};

		solidDetDZ = new G4Polycone("solidDetDZ",phiStart,phiTotal,numZPlanesDZ,zPlaneDZ,rInnerDZ,rOuterDZ);
        solidDet = new G4Polycone("solidDet",phiStart,phiTotal,numZPlanesD,zPlaneD,rInnerD,rOuterD);
        G4cout<<"Order10!"<<G4endl;
    }

	phiX=0.*deg;
    phiY=0.*deg;
    phiZ=0.*deg;
    G4RotationMatrix rdet;
    rdet.rotateX(phiX);
    rdet.rotateY(phiY);
    rdet.rotateZ(phiZ);

    G4ThreeVector tdetDZ = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorLength/2.));
    _logicDetectorDeadLayer = new G4LogicalVolume(solidDetDZ, _detectorMatter, "DetectorDeadLayer", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rdet,tdetDZ),
            _logicDetectorDeadLayer,
            "DetectorDeadLayer",
            logicVacCryo,
            false,
            checkOverlaps);
    _logicDetector = new G4LogicalVolume(solidDet, _detectorMatter, "Detector", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rdet,tdetDZ),
            _logicDetector,
            "Detector",
            logicVacCryo,
            false,
            checkOverlaps);


	// #################### Detector holder
	G4Tubs* solidHolderH = new G4Tubs("HolderH", fHolderInternalDiameter/2., fHolderExternalDiameter/2.,
									  (fHolderLength-fHolderBottomWidth)/2., 0, twopi);
	G4Tubs* solidHolderNH = new G4Tubs("HolderNH", 0, fHolderExternalDiameter/2., fHolderBottomWidth/2., 0, twopi);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rhold;
    rhold.rotateX(phiX);
    rhold.rotateY(phiY);
    rhold.rotateZ(phiZ);

	G4ThreeVector thold1 = G4ThreeVector(0, 0, (fVacCryoLength/2.-fCapWindowWidth
		-fDistanceDetectorWindowCap-fDetectorOverHolder-(fHolderLength-fHolderBottomWidth)/2.));
	G4ThreeVector thold2 = G4ThreeVector(0, 0,(fVacCryoLength/2.-fCapWindowWidth
		-fDistanceDetectorWindowCap-fDetectorOverHolder-fHolderLength+fHolderBottomWidth/2.));
    auto logicHolder1 =
        new G4LogicalVolume(solidHolderH, _holderMatter, "Holder1", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rhold,thold1),
            logicHolder1,
            "Holder1",
            logicVacCryo,
            false,
            checkOverlaps);
	auto logicHolder2 =
        new G4LogicalVolume(solidHolderNH, _holderMatter, "Holder2", 0, 0, 0);
	new G4PVPlacement(
            G4Transform3D(rhold,thold2),
            logicHolder2,
            "Holder2",
            logicVacCryo,
            false,
            checkOverlaps);


    // #################### Pb thin layer
	//Pb thin layer around holder inner part (modified 22/11/2013)
	//5 cylinder sectors: one 60x0.3x60 mm3, four 22x0.3x10 mm3
	//the angle of the sector theta=2*L^2/R^2, L=lenght of sector, R=external detector radius
	//the big sector on one side, the 4 small ones on the other side, 2 up and 2 down.
	G4Tubs *solidLayerThinPb1 =
        new G4Tubs("solidPbLayerThin1",
                fDetectorDiameter/2.,
                (fDetectorDiameter/2.+fLayerThinPbWidth),
                fLayerThinPbL1/2.,
                0,
                ((2*fLayerThinPbL1*fLayerThinPbL1)/(fDetectorDiameter/2.*fDetectorDiameter/2.))/2.);
	G4Tubs *solidLayerThinPb2 =
        new G4Tubs("solidPbLayerThin2",
                fDetectorDiameter/2.,
                (fDetectorDiameter/2.+fLayerThinPbWidth),
                fLayerThinPbL3/2.,
		        0,
                ((2*fLayerThinPbL2*fLayerThinPbL2)/(fDetectorDiameter/2.*fDetectorDiameter/2.))/2.);

	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=180.*deg;
	G4RotationMatrix rpbl1;
    rpbl1.rotateX(phiX);
    rpbl1.rotateY(phiY);
    rpbl1.rotateZ(phiZ);
    G4ThreeVector tpbl1 = G4ThreeVector(0.,0.,
		(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-fLayerThinPbL1/2.));
    G4LogicalVolume * logicLayerThinPb1 =
        new G4LogicalVolume(solidLayerThinPb1, _shieldPbMatter, "LayerThinPb1", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl1,tpbl1),
            logicLayerThinPb1,
            "LayerThinPb1",
            logicVacCryo,
            false,
            checkOverlaps);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpbl2;
    rpbl2.rotateX(phiX);
    rpbl2.rotateY(phiY);
    rpbl2.rotateZ(phiZ);
    G4ThreeVector tpbl2 =
        G4ThreeVector(0.,0.,
                (fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-fLayerThinPbL3/2.));
    G4LogicalVolume * logicLayerThinPb2 =
        new G4LogicalVolume(solidLayerThinPb2, _shieldPbMatter, "LayerThinPb2", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl2,tpbl2),
            logicLayerThinPb2,
            "LayerThinPb2",
            logicVacCryo,
            false,
            checkOverlaps);


	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpbl3;
    rpbl3.rotateX(phiX);
    rpbl3.rotateY(phiY);
    rpbl3.rotateZ(phiZ);
    G4ThreeVector tpbl3 =
        G4ThreeVector(0.,
                0.,
                (fVacCryoLength/2.-fCapWindowWidth-\
                 fDistanceDetectorWindowCap-fDetectorOverHolder-9.*fLayerThinPbL3/2.));
    auto logicLayerThinPb3 =
        new G4LogicalVolume(solidLayerThinPb2, _shieldPbMatter, "LayerThinPb3", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl3,tpbl3),
            logicLayerThinPb3,
            "LayerThinPb3",
            logicVacCryo,
            false,
            checkOverlaps);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl4;
    rpbl4.rotateX(phiX);
    rpbl4.rotateY(phiY);
    rpbl4.rotateZ(phiZ);
    G4ThreeVector tpbl4 =
        G4ThreeVector(0.,
                0.,
                (fVacCryoLength/2.-fCapWindowWidth-\
                   fDistanceDetectorWindowCap-\
                   fDetectorOverHolder-fLayerThinPbL3/2.));
    auto logicLayerThinPb4 = new G4LogicalVolume(solidLayerThinPb2, _shieldPbMatter, "LayerThinPb4", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl4,tpbl4),
            logicLayerThinPb4,
            "LayerThinPb4",
            logicVacCryo,
            false,
            checkOverlaps);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl5;
    rpbl5.rotateX(phiX);
    rpbl5.rotateY(phiY);
    rpbl5.rotateZ(phiZ);
    G4ThreeVector tpbl5 =
        G4ThreeVector(0.,
                0.,
                (fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-9*fLayerThinPbL3/2.));
    auto logicLayerThinPb5 =
        new G4LogicalVolume(solidLayerThinPb2, _shieldPbMatter, "LayerThinPb5", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl5,tpbl5),
            logicLayerThinPb5,
            "LayerThinPb5",
            logicVacCryo,
            false,
            checkOverlaps);


	// #################### Pb layer
	G4Tubs* solidLayerPb =
        new G4Tubs("solidPbLayer",
                fLayerPbInternalDiameter/2.,
                fLayerPbExternalDiameter/2.,
				fLayerPbLength/2., 0, twopi);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl;
    rpbl.rotateX(phiX);
    rpbl.rotateY(phiY);
    rpbl.rotateZ(phiZ);
    G4ThreeVector tpbl =
        G4ThreeVector(0.,
                0.,
                -(fHolderLength-fHolderBottomWidth)/2.+fLayerPbLength/2.);
	auto logicLayerPb =
        new G4LogicalVolume(solidLayerPb, _layerPbMatter, "LayerPb", 0, 0, 0);
    new G4PVPlacement(
            G4Transform3D(rpbl,tpbl),
            logicLayerPb,
            "LayerPb",
            logicHolder1,
            false,
            checkOverlaps);


    // #################### Target (sample)
    G4Box* solidTar1 = new G4Box("solidTar1", _targetLx/2., _targetLy/2., _targetLz/2.);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rm1;
    rm1.rotateX(phiX);
    rm1.rotateY(phiY);
    rm1.rotateZ(phiZ);
	G4ThreeVector tar1 = G4ThreeVector(0.,0.,_targetLz/2.+9.9*mm);//!!!!check the distance sample-endcap and propagate it to all .mac files
	_logicTarget = new G4LogicalVolume(solidTar1,_tarMatter,"Target1",0,0,0);
	// NCM remove target from the world, to check 60Co -->
    new G4PVPlacement(
            G4Transform3D(rm1,tar1),
            _logicTarget,
            "Target1",
            _logicWorld,
            false,
            checkOverlaps);



    //------------------------------------------------
    // Sensitive detectors
    //------------------------------------------------
    //
    //  G4SDManager* SDman = G4SDManager::GetSDMpointer();
    //
    // G4String detectortargetSDname = "exrdm/DetectorTargetSD";
	// exrdmDetectorSD* aDetectorSD = new exrdmDetectorSD( detectorTargetSDname );
	// SDman->AddNewDetector( aDetectorSD );
	// logicTarget->SetSensitiveDetector( aDetectorSD );
	// _logicDetector->SetSensitiveDetector( aDetectorSD );
	//
	//-------------------------------------------------
	// regions
	//
	// if(targetRegion) delete targetRegion;
	// if(detectorRegion) delete detectorRegion;
	// if(targetRegion) delete targetRegion;
	_targetRegion1 = new G4Region("Target1");
	_targetRegion1->AddRootLogicalVolume(_logicTarget);

    //if(detectorRegion) delete detectorRegion;
	_detectorRegion   = new G4Region("Detector");
	_detectorRegion->AddRootLogicalVolume(_logicDetector);
	_detectorDeadLayerRegion = new G4Region("DetectorDeadLayer");
	_detectorDeadLayerRegion->AddRootLogicalVolume(_logicDetectorDeadLayer);

	//if(capRegion) delete capRegion;
	_capRegion = new G4Region("Cap");
	_capRegion->AddRootLogicalVolume(logicCap1);
	_capRegion->AddRootLogicalVolume(logicCap2);
	_capRegion->AddRootLogicalVolume(logicCap3);

    //if(shieldPbRegion) delete shieldPbRegion;
	_shieldPbRegion = new G4Region("ShieldPb");
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield1);
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield2);
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield3);
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield4);
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield5);
	_shieldPbRegion->AddRootLogicalVolume(logicPbShield6);

	//if(shieldCuRegion) delete shieldCuRegion;
	_shieldCuRegion = new G4Region("ShieldCu");
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield1);
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield2);
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield3);
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield4);
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield5);
	_shieldCuRegion->AddRootLogicalVolume(logicCuShield6);

    //supportRegion = new G4Region("Support");
	//supportRegion->AddRootLogicalVolume(logicSupport);

    //if(holderRegion) delete holderRegion;
	_holderRegion = new G4Region("Holder");
	_holderRegion->AddRootLogicalVolume(logicHolder1);
	_holderRegion->AddRootLogicalVolume(logicHolder2);

    //if(layerPbRegion) delete layerPbRegion;
	_layerPbRegion = new G4Region("LayerPb");
	_layerPbRegion->AddRootLogicalVolume(logicLayerPb);
	_layerThinPbRegion = new G4Region("LayerThinPb");
	_layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb1);
	_layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb2);
	_layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb3);
	_layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb4);
	_layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb5);


	//--------- Visualization attributes -------------------------------
	_logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
    auto vis_att= new G4VisAttributes(G4Colour(1.0,1.0,1.0));//white
	_logicTarget->SetVisAttributes(vis_att);
    _visAttrs.push_back(vis_att);
	//_logicTarget->SetVisAttributes(G4VisAttributes::Invisible);
	vis_att= new G4VisAttributes(G4Colour(1.0,0.,1.0));//magenta
	_logicDetector->SetVisAttributes(vis_att);
    _visAttrs.push_back(vis_att);
	//_logicDetector->SetVisAttributes(G4VisAttributes::Invisible);
	vis_att=new G4VisAttributes(G4Color(1.0,0.,0.));//red
	_logicDetectorDeadLayer->SetVisAttributes(vis_att);
    _visAttrs.push_back(vis_att);
	//_logicDetectorDeadLayer->SetVisAttributes(G4VisAttributes::Invisible);
	vis_att = new G4VisAttributes(G4Colour(0.,1.0,1.0));//blue
	logicCap1->SetVisAttributes(vis_att);
	//logicCap1->SetVisAttributes(G4VisAttributes::Invisible);
	logicCap2->SetVisAttributes(vis_att);
	//logicCap2->SetVisAttributes(G4VisAttributes::Invisible);
	logicCap3->SetVisAttributes(vis_att);
	logicHolder1->SetVisAttributes(vis_att);
	//logicHolder1->SetVisAttributes(G4VisAttributes::Invisible);
	logicHolder2->SetVisAttributes(vis_att);
	//logicHolder2->SetVisAttributes(G4VisAttributes::Invisible);
    _visAttrs.push_back(vis_att);
	//logicCap3->SetVisAttributes(G4VisAttributes::Invisible);
	vis_att = new G4VisAttributes(G4Colour(0.,0.,1.0));//dark blue
	logicPbShield1->SetVisAttributes(vis_att);
	//logicPbShield1->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield2->SetVisAttributes(vis_att);
	//logicPbShield2->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield3->SetVisAttributes(vis_att);
	//logicPbShield3->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield4->SetVisAttributes(vis_att);
	//logicPbShield4->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield5->SetVisAttributes(vis_att);
	//logicPbShield5->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield6->SetVisAttributes(vis_att);
	//logicPbShield6->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield7->SetVisAttributes(vis_att);
    //logicPbShield7->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield8->SetVisAttributes(vis_att);
    //logicPbShield8->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield9->SetVisAttributes(vis_att);
    //logicPbShield9->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield10->SetVisAttributes(vis_att);
	//logicLayerPb->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb1->SetVisAttributes(vis_att);
	//logicLayerThinPb1->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb2->SetVisAttributes(vis_att);
	//logicLayerThinPb2->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb3->SetVisAttributes(vis_att);
	//logicLayerThinPb3->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb4->SetVisAttributes(vis_att);
	//logicLayerThinPb4->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb5->SetVisAttributes(vis_att);
	//logicLayerThinPb5->SetVisAttributes(G4VisAttributes::Invisible);
    _visAttrs.push_back(vis_att);
	//logicPbShield10->SetVisAttributes(G4VisAttributes::Invisible);
	vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.));//yellow
	logicCuShield1->SetVisAttributes(vis_att);
	//logicCuShield1->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield2->SetVisAttributes(vis_att);
	//logicCuShield2->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield3->SetVisAttributes(vis_att);
	//logicCuShield3->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield4->SetVisAttributes(vis_att);
	//logicCuShield4->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield5->SetVisAttributes(vis_att);
	//logicCuShield5->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield6->SetVisAttributes(vis_att);
    _visAttrs.push_back(vis_att);
	//logicCuShield6->SetVisAttributes(G4VisAttributes::Invisible);
	logicVacCryo->SetVisAttributes(G4VisAttributes::Invisible);//logicVaCryo
	//logicVacCryo->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));//white


  return physiWorld;
}

//////////////////////////////////////////////////////////
//  SENSITIVE DETECTORS IMPLEMENTATION
/////////////////////////////////////////////////////////
void gebicDetectorConstruction::ConstructSDandField()
{
   auto sdManager = G4SDManager::GetSDMpointer();

    auto HPGe = new gebicSensitiveDetector("/HPGe");
    sdManager->AddNewDetector(HPGe);
    _logicDetector->SetSensitiveDetector(HPGe);

}


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

gebicDetectorConstruction::gebicDetectorConstruction()
:logicWorld(0),  physiWorld(0),
 logicTarget1(0), physiTarget1(0),
 _logicDetector(0), physiDetector(0),
 logicDetectorDeadLayer(0), physiDetectorDeadLayer(0),
 logicCap1(0), physiCap1(0),
 logicCap2(0), physiCap2(0),
 logicCap3(0), physiCap3(0),
 logicVacCryo(0), physiVacCryo(0),
 logicHolder1(0), physiHolder1(0),
 logicHolder2(0), physiHolder2(0),
 logicLayerThinPb1(0),physiLayerThinPb1(0),
 logicLayerThinPb2(0),physiLayerThinPb2(0),
 logicLayerThinPb3(0),physiLayerThinPb3(0),
 logicLayerThinPb4(0),physiLayerThinPb4(0),
 logicLayerThinPb5(0),physiLayerThinPb5(0),
 logicLayerPb(0), physiLayerPb(0),
 logicPbShield1(0),physiPbShield1(0),
 logicPbShield2(0),physiPbShield2(0),
 logicPbShield3(0),physiPbShield3(0),
 logicPbShield4(0),physiPbShield4(0),
 logicPbShield5(0),physiPbShield5(0),
 logicPbShield6(0),physiPbShield6(0),
 logicPbShield7(0),physiPbShield7(0),
 logicPbShield8(0),physiPbShield8(0),
 logicPbShield9(0),physiPbShield9(0),
 logicPbShield10(0),physiPbShield10(0),
 logicCuShield1(0),physiCuShield1(0),
 logicCuShield2(0),physiCuShield2(0),
 logicCuShield3(0),physiCuShield3(0),
 logicCuShield4(0),physiCuShield4(0),
 logicCuShield5(0),physiCuShield5(0),
 logicCuShield6(0),physiCuShield6(0),
 DefaultMatter(0), TarMatter(0), DetectorMatter(0), CapMatter(0),
 VacCryoMatter(0), HolderMatter(0), LayerPbMatter(0), ShieldPbMatter(0), ShieldCuMatter(0)
{
    //detectorMessenger = new gebicDetectorMessenger(this);
    DefineMaterials();

	//Dimensions of the World (G4Box)
    fWorldDiameter	=  900.0*mm;
	//fWorldLength  = 950.0*mm;
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
    fL1 = 150.0*mm;//x
    fL2 = 50.0*mm;//y
    fL3 = 25.0*mm;//z

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
    //delete detectorMessenger;
}

void gebicDetectorConstruction::DefineMaterials()
{
    G4UnitDefinition::BuildUnitsTable();

	//Material definition
	G4String name,symbol;

	G4NistManager* nistman = G4NistManager::Instance();
	nistman->SetVerbose(1);

	DefaultMatter = nistman->FindOrBuildMaterial("G4_AIR");

    TarMatter = nistman->FindOrBuildMaterial("G4_Pb");

	ShieldPbMatter = nistman->FindOrBuildMaterial("G4_Pb");
	ShieldCuMatter = nistman->FindOrBuildMaterial("G4_Cu");
	DetectorMatter = nistman->FindOrBuildMaterial("G4_Ge");

	CapMatter = nistman->FindOrBuildMaterial("G4_Al");
	HolderMatter = nistman->FindOrBuildMaterial("G4_Al");
	LayerPbMatter = ShieldPbMatter;
	//SupportMatter = nistman->FindOrBuildMaterial("G4_POLYPROPYLENE");
	VacCryoMatter = nistman->FindOrBuildMaterial("G4_Galactic");
}



G4VPhysicalVolume* gebicDetectorConstruction::Construct()
{
	// ##################### World
	G4Box* solidWorld= new G4Box("world", fWorldDiameter/2., fWorldDiameter/2., fWorldLength/2.);
	logicWorld= new G4LogicalVolume(solidWorld, DefaultMatter, "World", 0, 0, 0);
	physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

	// ##################### Pb shield
	G4Box* basePbShield = new G4Box("basePbShield", fShieldPbDim1/2., fShieldPbDim1/2., fShieldPbDim4/2.);
	G4Box* wallPbShield = new G4Box("wall1PbShield", fShieldPbDim2/2., fShieldPbDim4/2., fShieldPbDim3/2.);
    G4Box* doorPbShield = new G4Box("doorPbShield", fShieldPbDim5/2., fShieldPbDim5/2., fShieldPbDim4/2.);
    G4Box* walltopPbShield = new G4Box("walltopPbShield", (fShieldPbDim5+fShieldPbDim7)/2., fShieldPbDim7/2., fShieldPbDim6/2.);

	G4double phiX=0.*deg;
	G4double phiY=0.*deg;
	G4double phiZ=0.*deg;

	G4RotationMatrix rpb1;
    rpb1.rotateX(phiX);
    rpb1.rotateY(phiY);
    rpb1.rotateZ(phiZ);

	//G4ThreeVector tpb1 = G4ThreeVector(0.,0.,(-fWorldLength/2.+fShieldPbDim4/2.));
    G4ThreeVector tpb1 = G4ThreeVector(0.,0.,(-fDistanceEndcapFloor-fShieldCuDim4-fShieldPbDim4/2.));
	logicPbShield1 = new G4LogicalVolume(basePbShield,ShieldPbMatter,"PbShield1",0,0,0);//base Pb
	physiPbShield1 = new G4PVPlacement(G4Transform3D(rpb1,tpb1),logicPbShield1,"PbShield1",logicWorld,false,0);//base Pb

	//G4ThreeVector tpb2 = G4ThreeVector(0.,0.,(fWorldLength/2.-fShieldPbDim4/2.));
    G4ThreeVector tpb2 = G4ThreeVector(0.,0.,(fDistanceEndcapCeiling+fShieldCuDim4+fShieldPbDim4/2.));
	logicPbShield2 = new G4LogicalVolume(doorPbShield,ShieldPbMatter,"PbShield2",0,0,0);//door Pb
	physiPbShield2 = new G4PVPlacement(G4Transform3D(rpb1,tpb2),logicPbShield2,"PbShield2",logicWorld,false,0);//top Pb


	//G4ThreeVector tpb3 = G4ThreeVector(fShieldPbDim4/2.,(-fWorldDiameter/2.+fShieldPbDim4/2.),-fWorldLength/2.+fShieldPbDim4+fShieldPbDim3/2.);
    G4ThreeVector tpb3 = G4ThreeVector(fShieldPbDim4/2.,(-fWorldDiameter/2.+fShieldPbDim4/2.),(-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	logicPbShield3 = new G4LogicalVolume(wallPbShield,ShieldPbMatter,"PbShield3",0,0,0);//front wall Pb
	physiPbShield3 = new G4PVPlacement(G4Transform3D(rpb1,tpb3),logicPbShield3,"PbShield3",logicWorld,false,0);//front wall Pb


	//G4ThreeVector tpb4 = G4ThreeVector(-fShieldPbDim4/2.,(fWorldDiameter/2.-fShieldPbDim4/2.),-fWorldLength/2.+fShieldPbDim4+fShieldPbDim3/2.);
    G4ThreeVector tpb4 = G4ThreeVector(-fShieldPbDim4/2.,(fWorldDiameter/2.-fShieldPbDim4/2.),(-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	logicPbShield4 = new G4LogicalVolume(wallPbShield,ShieldPbMatter,"PbShield4",0,0,0);//back wall Pb
	physiPbShield4 = new G4PVPlacement(G4Transform3D(rpb1,tpb4),logicPbShield4,"PbShield4",logicWorld,false,0);//back wall Pb

	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpb2;
    rpb2.rotateX(phiX);
    rpb2.rotateY(phiY);
    rpb2.rotateZ(phiZ);
	//G4ThreeVector tpb5 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4/2.),-fShieldPbDim4/2.,-fWorldLength/2.+fShieldPbDim4+fShieldPbDim3/2.);
    G4ThreeVector tpb5 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4/2.),-fShieldPbDim4/2.,(-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	logicPbShield5 = new G4LogicalVolume(wallPbShield,ShieldPbMatter,"PbShield5",0,0,0);//left wall Pb
	physiPbShield5 = new G4PVPlacement(G4Transform3D(rpb2,tpb5),logicPbShield5,"PbShield5",logicWorld,false,0);//left wall Pb

	//G4ThreeVector tpb6 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4/2.),fShieldPbDim4/2.,-fWorldLength/2.+fShieldPbDim4+fShieldPbDim3/2.);
    G4ThreeVector tpb6 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4/2.),fShieldPbDim4/2.,(-fDistanceEndcapFloor-fShieldCuDim4+fShieldPbDim3/2.));
	logicPbShield6 = new G4LogicalVolume(wallPbShield,ShieldPbMatter,"PbShield6",0,0,0);//right wall Pb
	physiPbShield6 = new G4PVPlacement(G4Transform3D(rpb2,tpb6),logicPbShield6,"PbShield6",logicWorld,false,0);//right wall Pb


    //G4ThreeVector tpb7 = G4ThreeVector(fShieldPbDim4/2.+25.0*mm,(-fWorldDiameter/2.+fShieldPbDim7/2.),fWorldLength/2.-fShieldPbDim6/2.);
    G4ThreeVector tpb7 = G4ThreeVector((fShieldPbDim4/2.+25.0*mm),(-fWorldDiameter/2.+fShieldPbDim7/2.),(fDistanceEndcapCeiling+fShieldPbDim6/2.));
    logicPbShield7 = new G4LogicalVolume(walltopPbShield,ShieldPbMatter,"PbShield7",0,0,0);//front top wall Pb
    physiPbShield7 = new G4PVPlacement(G4Transform3D(rpb1,tpb7),logicPbShield7,"PbShield7",logicWorld,false,0);//front wall top Pb


    //G4ThreeVector tpb8 = G4ThreeVector(-fShieldPbDim4/2.-25.0*mm,(fWorldDiameter/2.-fShieldPbDim7/2.),fWorldLength/2.-fShieldPbDim6/2.);
    G4ThreeVector tpb8 = G4ThreeVector((-fShieldPbDim4/2.-25.0*mm),(fWorldDiameter/2.-fShieldPbDim7/2.),(fDistanceEndcapCeiling+fShieldPbDim6/2.));
    logicPbShield8 = new G4LogicalVolume(walltopPbShield,ShieldPbMatter,"PbShield8",0,0,0);//back wall top Pb
    physiPbShield8 = new G4PVPlacement(G4Transform3D(rpb1,tpb8),logicPbShield8,"PbShield8",logicWorld,false,0);//back wall top Pb


    //G4ThreeVector tpb9 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim7/2.),-fShieldPbDim4/2.-25.0*mm,fWorldLength/2.-fShieldPbDim6/2.);
    G4ThreeVector tpb9 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim7/2.),(-fShieldPbDim4/2.-25.0*mm),(fDistanceEndcapCeiling+fShieldPbDim6/2.));
    logicPbShield9 = new G4LogicalVolume(walltopPbShield,ShieldPbMatter,"PbShield9",0,0,0);//left wall top Pb
    physiPbShield9 = new G4PVPlacement(G4Transform3D(rpb2,tpb9),logicPbShield9,"PbShield9",logicWorld,false,0);//left wall top Pb

    //G4ThreeVector tpb10 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim7/2.),fShieldPbDim4/2.+25.0*mm,fWorldLength/2.-fShieldPbDim6/2.);
    G4ThreeVector tpb10 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim7/2.),(fShieldPbDim4/2.+25.0*mm),(fDistanceEndcapCeiling+fShieldPbDim6/2.));
    logicPbShield10 = new G4LogicalVolume(walltopPbShield,ShieldPbMatter,"PbShield10",0,0,0);//right wall top Pb
    physiPbShield10 = new G4PVPlacement(G4Transform3D(rpb2,tpb10),logicPbShield10,"PbShield10",logicWorld,false,0);//right wall top Pb


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
	//G4ThreeVector tcu1 = G4ThreeVector(0.,0.,(-fWorldLength/2.+fShieldPbDim4+fShieldCuDim4/2.));
    G4ThreeVector tcu1 = G4ThreeVector(0.,0.,(-fDistanceEndcapFloor-fShieldCuDim4/2.));
	logicCuShield1 = new G4LogicalVolume(baseCuShield,ShieldCuMatter,"CuShield1",0,0,0);//base Cu
	physiCuShield1 = new G4PVPlacement(G4Transform3D(rcu1,tcu1),logicCuShield1,"CuShield1",logicWorld,false,0);//base Cu

	//G4ThreeVector tcu2 = G4ThreeVector(0.,0.,(fWorldLength/2.-fShieldPbDim4-fShieldCuDim4/2.));
    G4ThreeVector tcu2 = G4ThreeVector(0.,0.,(fDistanceEndcapCeiling+fShieldCuDim4/2.));
	logicCuShield2 = new G4LogicalVolume(doorCuShield,ShieldCuMatter,"CuShield2",0,0,0);//top Cu
	//NCM remove CuShield2 (TAPA) from the world, to check 60Co ----> physiCuShield2 = new G4PVPlacement(G4Transform3D(rcu1,tcu2),logicCuShield2,"CuShield2",logicWorld,false,0);//top Cu


	//G4ThreeVector tcu3 = G4ThreeVector(fShieldCuDim4/2.,(-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),0.);
    G4ThreeVector tcu3 = G4ThreeVector(fShieldCuDim4/2.,(-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),(-fDistanceEndcapFloor+fShieldCuDim3/2.));
	logicCuShield3 = new G4LogicalVolume(wallCuShield,ShieldCuMatter,"CuShield3",0,0,0);//front wall Cu
	physiCuShield3 = new G4PVPlacement(G4Transform3D(rcu1,tcu3),logicCuShield3,"CuShield3",logicWorld,false,0);//front wall Cu


	//G4ThreeVector tcu4 = G4ThreeVector(-fShieldCuDim4/2.,(fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),0.);
    G4ThreeVector tcu4 = G4ThreeVector(-fShieldCuDim4/2.,(fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),(-fDistanceEndcapFloor+fShieldCuDim3/2.));
	logicCuShield4 = new G4LogicalVolume(wallCuShield,ShieldCuMatter,"CuShield4",0,0,0);//back wall Cu
	physiCuShield4 = new G4PVPlacement(G4Transform3D(rcu1,tcu4),logicCuShield4,"CuShield4",logicWorld,false,0);//back wall Cu


	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rcu2;
    rcu2.rotateX(phiX);
    rcu2.rotateY(phiY);
    rcu2.rotateZ(phiZ);
	//G4ThreeVector tcu5 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),-fShieldCuDim4/2.,0.);
    G4ThreeVector tcu5 = G4ThreeVector((-fWorldDiameter/2.+fShieldPbDim4+fShieldCuDim4/2.),-fShieldCuDim4/2.,(-fDistanceEndcapFloor+fShieldCuDim3/2.));
	logicCuShield5 = new G4LogicalVolume(wallCuShield,ShieldCuMatter,"CuShield5",0,0,0);//left wall Cu
	physiCuShield5 = new G4PVPlacement(G4Transform3D(rcu2,tcu5),logicCuShield5,"CuShield5",logicWorld,false,0);//left wall Cu

	//G4ThreeVector tcu6 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),fShieldCuDim4/2.,0.);
    G4ThreeVector tcu6 = G4ThreeVector((fWorldDiameter/2.-fShieldPbDim4-fShieldCuDim4/2.),fShieldCuDim4/2.,(-fDistanceEndcapFloor+fShieldCuDim3/2.));
	logicCuShield6 = new G4LogicalVolume(wallCuShield,ShieldCuMatter,"CuShield6",0,0,0);//right wall Cu
	physiCuShield6 = new G4PVPlacement(G4Transform3D(rcu2,tcu6),logicCuShield6,"CuShield6",logicWorld,false,0.);//right wall Cu


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
	logicVacCryo = new G4LogicalVolume(solidVacCryo, VacCryoMatter, "VacCryo", 0, 0, 0);
	physiVacCryo = new G4PVPlacement(G4Transform3D(rvc,tvc), logicVacCryo, "VacCryo", logicWorld, false, 0);

	// ##################### Cap
	G4double fCapWidth2 = (fCapExternalDiameter-fCapInternalDiameter)/2.-fCapWindowWidth;
	G4Tubs* solidCap1 = new G4Tubs("solidCap1", 0, fCapInternalDiameter/2., fCapWindowWidth/2., 0, twopi);//window part of cap -- upper part
	G4Tubs* solidCap2 = new G4Tubs("solidCap2", fCapWindowDiameter/2., fCapInternalDiameter/2., fCapWidth2/2., 0, twopi);//window part of cap -- hole part
	G4Tubs* solidCap3 = new G4Tubs("solidCap3",fCapInternalDiameter/2.,fCapExternalDiameter/2.,fCapLength/2.,0.,twopi);//cap body
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rcap;
    rcap.rotateX(phiX);
    rcap.rotateY(phiY);
    rcap.rotateZ(phiZ);
    G4ThreeVector tcap1 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth/2.));
    logicCap1 = new G4LogicalVolume(solidCap1, CapMatter, "Cap1", 0, 0, 0);
	physiCap1 = new G4PVPlacement(G4Transform3D(rcap,tcap1), logicCap1, "Cap1", logicVacCryo, false, 0);
	G4ThreeVector tcap2 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fCapWidth2/2.));
    logicCap2 = new G4LogicalVolume(solidCap2, CapMatter, "Cap2", 0, 0, 0);
	physiCap2 = new G4PVPlacement(G4Transform3D(rcap,tcap2), logicCap2, "Cap2", logicVacCryo, false, 0);
	G4ThreeVector tcap3 = G4ThreeVector(0.,0.,0.);
    logicCap3 = new G4LogicalVolume(solidCap3, CapMatter, "Cap3", 0, 0, 0);
	physiCap3 = new G4PVPlacement(G4Transform3D(rcap,tcap3), logicCap3, "Cap3", logicVacCryo, false, 0);


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
    logicDetectorDeadLayer = new G4LogicalVolume(solidDetDZ, DetectorMatter, "DetectorDeadLayer", 0, 0, 0);
    physiDetectorDeadLayer = new G4PVPlacement(G4Transform3D(rdet,tdetDZ), logicDetectorDeadLayer, "DetectorDeadLayer", logicVacCryo, false, 0);
    _logicDetector = new G4LogicalVolume(solidDet, DetectorMatter, "Detector", 0, 0, 0);
    // XXX TO BE REMOVE with Geant4.10
    this->ConstructSDandField();
    // --> XXX TO BE REMOVE with Geant4.10
	physiDetector = new G4PVPlacement(G4Transform3D(rdet,tdetDZ), _logicDetector, "Detector", logicVacCryo, false, 0);


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
	logicHolder1 = new G4LogicalVolume(solidHolderH, HolderMatter, "Holder1", 0, 0, 0);
	physiHolder1 = new G4PVPlacement(G4Transform3D(rhold,thold1), logicHolder1, "Holder1", logicVacCryo, false, 0);
	logicHolder2 = new G4LogicalVolume(solidHolderNH, HolderMatter, "Holder2", 0, 0, 0);
	physiHolder2 = new G4PVPlacement(G4Transform3D(rhold,thold2), logicHolder2, "Holder2", logicVacCryo, false, 0);


    // #################### Pb thin layer
	//Pb thin layer around holder inner part (modified 22/11/2013)
	//5 cylinder sectors: one 60x0.3x60 mm3, four 22x0.3x10 mm3
	//the angle of the sector theta=2*L^2/R^2, L=lenght of sector, R=external detector radius
	//the big sector on one side, the 4 small ones on the other side, 2 up and 2 down.
	G4Tubs *solidLayerThinPb1 = new G4Tubs("solidPbLayerThin1",fDetectorDiameter/2.,(fDetectorDiameter/2.+fLayerThinPbWidth),fLayerThinPbL1/2.,
		0,((2*fLayerThinPbL1*fLayerThinPbL1)/(fDetectorDiameter/2.*fDetectorDiameter/2.))/2.);
	G4Tubs *solidLayerThinPb2 = new G4Tubs("solidPbLayerThin2",fDetectorDiameter/2.,(fDetectorDiameter/2.+fLayerThinPbWidth),fLayerThinPbL3/2.,
		0,((2*fLayerThinPbL2*fLayerThinPbL2)/(fDetectorDiameter/2.*fDetectorDiameter/2.))/2.);

	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=180.*deg;
	G4RotationMatrix rpbl1;
    rpbl1.rotateX(phiX);
    rpbl1.rotateY(phiY);
    rpbl1.rotateZ(phiZ);
    G4ThreeVector tpbl1 = G4ThreeVector(0.,0.,
		(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-fLayerThinPbL1/2.));
    logicLayerThinPb1 = new G4LogicalVolume(solidLayerThinPb1, ShieldPbMatter, "LayerThinPb1", 0, 0, 0);
    physiLayerThinPb1 = new G4PVPlacement(G4Transform3D(rpbl1,tpbl1), logicLayerThinPb1, "LayerThinPb1", logicVacCryo, false, 0);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpbl2;
    rpbl2.rotateX(phiX);
    rpbl2.rotateY(phiY);
    rpbl2.rotateZ(phiZ);
    G4ThreeVector tpbl2 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-fLayerThinPbL3/2.));
    logicLayerThinPb2 = new G4LogicalVolume(solidLayerThinPb2, ShieldPbMatter, "LayerThinPb2", 0, 0, 0);
    physiLayerThinPb2 = new G4PVPlacement(G4Transform3D(rpbl2,tpbl2), logicLayerThinPb2, "LayerThinPb2", logicVacCryo, false, 0);


	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=90.*deg;
	G4RotationMatrix rpbl3;
    rpbl3.rotateX(phiX);
    rpbl3.rotateY(phiY);
    rpbl3.rotateZ(phiZ);
    G4ThreeVector tpbl3 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-9.*fLayerThinPbL3/2.));
    logicLayerThinPb3 = new G4LogicalVolume(solidLayerThinPb2, ShieldPbMatter, "LayerThinPb3", 0, 0, 0);
    physiLayerThinPb3 = new G4PVPlacement(G4Transform3D(rpbl3,tpbl3), logicLayerThinPb3, "LayerThinPb3", logicVacCryo, false, 0);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl4;
    rpbl4.rotateX(phiX);
    rpbl4.rotateY(phiY);
    rpbl4.rotateZ(phiZ);
    G4ThreeVector tpbl4 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-fLayerThinPbL3/2.));
    logicLayerThinPb4 = new G4LogicalVolume(solidLayerThinPb2, ShieldPbMatter, "LayerThinPb4", 0, 0, 0);
    physiLayerThinPb4 = new G4PVPlacement(G4Transform3D(rpbl4,tpbl4), logicLayerThinPb4, "LayerThinPb4", logicVacCryo, false, 0);

    phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl5;
    rpbl5.rotateX(phiX);
    rpbl5.rotateY(phiY);
    rpbl5.rotateZ(phiZ);
    G4ThreeVector tpbl5 = G4ThreeVector(0.,0.,(fVacCryoLength/2.-fCapWindowWidth-fDistanceDetectorWindowCap-fDetectorOverHolder-9*fLayerThinPbL3/2.));
    logicLayerThinPb5 = new G4LogicalVolume(solidLayerThinPb2, ShieldPbMatter, "LayerThinPb5", 0, 0, 0);
    physiLayerThinPb5 = new G4PVPlacement(G4Transform3D(rpbl5,tpbl5), logicLayerThinPb5, "LayerThinPb5", logicVacCryo, false, 0);


	// #################### Pb layer
	G4Tubs* solidLayerPb = new G4Tubs("solidPbLayer", fLayerPbInternalDiameter/2., fLayerPbExternalDiameter/2.,
									  fLayerPbLength/2., 0, twopi);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rpbl;
    rpbl.rotateX(phiX);
    rpbl.rotateY(phiY);
    rpbl.rotateZ(phiZ);
    G4ThreeVector tpbl = G4ThreeVector(0., 0., -(fHolderLength-fHolderBottomWidth)/2.+fLayerPbLength/2.);
	logicLayerPb = new G4LogicalVolume(solidLayerPb, LayerPbMatter, "LayerPb", 0, 0, 0);
	physiLayerPb = new G4PVPlacement(G4Transform3D(rpbl,tpbl), logicLayerPb, "LayerPb", logicHolder1, false, 0);


    // #################### Target (sample)
    G4Box* solidTar1 = new G4Box("solidTar1", fL1/2., fL2/2., fL3/2.);
	phiX=0.*deg;
	phiY=0.*deg;
	phiZ=0.*deg;
	G4RotationMatrix rm1;
    rm1.rotateX(phiX);
    rm1.rotateY(phiY);
    rm1.rotateZ(phiZ);
	G4ThreeVector tar1 = G4ThreeVector(0.,0.,fL3/2.+9.9*mm);//!!!!check the distance sample-endcap and propagate it to all .mac files
	logicTarget1 = new G4LogicalVolume(solidTar1,TarMatter,"Target1",0,0,0);
	// NCM remove target from the world, to check 60Co --> physiTarget1 = new G4PVPlacement(G4Transform3D(rm1,tar1),logicTarget1,"Target1",logicWorld,false,0);



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
	targetRegion1 = new G4Region("Target1");
	targetRegion1->AddRootLogicalVolume(logicTarget1);

    //if(detectorRegion) delete detectorRegion;
	detectorRegion   = new G4Region("Detector");
	detectorRegion->AddRootLogicalVolume(_logicDetector);
	detectorDeadLayerRegion = new G4Region("DetectorDeadLayer");
	detectorDeadLayerRegion->AddRootLogicalVolume(logicDetectorDeadLayer);

	//if(capRegion) delete capRegion;
	capRegion = new G4Region("Cap");
	capRegion->AddRootLogicalVolume(logicCap1);
	capRegion->AddRootLogicalVolume(logicCap2);
	capRegion->AddRootLogicalVolume(logicCap3);

    //if(shieldPbRegion) delete shieldPbRegion;
	shieldPbRegion = new G4Region("ShieldPb");
	shieldPbRegion->AddRootLogicalVolume(logicPbShield1);
	shieldPbRegion->AddRootLogicalVolume(logicPbShield2);
	shieldPbRegion->AddRootLogicalVolume(logicPbShield3);
	shieldPbRegion->AddRootLogicalVolume(logicPbShield4);
	shieldPbRegion->AddRootLogicalVolume(logicPbShield5);
	shieldPbRegion->AddRootLogicalVolume(logicPbShield6);

	//if(shieldCuRegion) delete shieldCuRegion;
	shieldCuRegion = new G4Region("ShieldCu");
	shieldCuRegion->AddRootLogicalVolume(logicCuShield1);
	shieldCuRegion->AddRootLogicalVolume(logicCuShield2);
	shieldCuRegion->AddRootLogicalVolume(logicCuShield3);
	shieldCuRegion->AddRootLogicalVolume(logicCuShield4);
	shieldCuRegion->AddRootLogicalVolume(logicCuShield5);
	shieldCuRegion->AddRootLogicalVolume(logicCuShield6);

    //supportRegion = new G4Region("Support");
	//supportRegion->AddRootLogicalVolume(logicSupport);

    //if(holderRegion) delete holderRegion;
	holderRegion = new G4Region("Holder");
	holderRegion->AddRootLogicalVolume(logicHolder1);
	holderRegion->AddRootLogicalVolume(logicHolder2);

    //if(layerPbRegion) delete layerPbRegion;
	layerPbRegion = new G4Region("LayerPb");
	layerPbRegion->AddRootLogicalVolume(logicLayerPb);
	layerThinPbRegion = new G4Region("LayerThinPb");
	layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb1);
	layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb2);
	layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb3);
	layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb4);
	layerThinPbRegion->AddRootLogicalVolume(logicLayerThinPb5);


	//--------- Visualization attributes -------------------------------
	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* TargetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));//white
	logicTarget1->SetVisAttributes(TargetVisAtt);
	//logicTarget1->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* DetectorVisAtt = new G4VisAttributes(G4Colour(1.0,0.,1.0));//magenta
	_logicDetector->SetVisAttributes(DetectorVisAtt);
	//_logicDetector->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* DeadVisAtt = new G4VisAttributes(G4Color(1.0,0.,0.));//red
	logicDetectorDeadLayer->SetVisAttributes(DeadVisAtt);
	//logicDetectorDeadLayer->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* CapVisAtt = new G4VisAttributes(G4Colour(0.,1.0,1.0));//blue
	logicCap1->SetVisAttributes(CapVisAtt);
	//logicCap1->SetVisAttributes(G4VisAttributes::Invisible);
	logicCap2->SetVisAttributes(CapVisAtt);
	//logicCap2->SetVisAttributes(G4VisAttributes::Invisible);
	logicCap3->SetVisAttributes(CapVisAtt);
	//logicCap3->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.,0.,1.0));//dark blue
	logicPbShield1->SetVisAttributes(LeadVisAtt);
	//logicPbShield1->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield2->SetVisAttributes(LeadVisAtt);
	//logicPbShield2->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield3->SetVisAttributes(LeadVisAtt);
	//logicPbShield3->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield4->SetVisAttributes(LeadVisAtt);
	//logicPbShield4->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield5->SetVisAttributes(LeadVisAtt);
	//logicPbShield5->SetVisAttributes(G4VisAttributes::Invisible);
	logicPbShield6->SetVisAttributes(LeadVisAtt);
	//logicPbShield6->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield7->SetVisAttributes(LeadVisAtt);
    //logicPbShield7->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield8->SetVisAttributes(LeadVisAtt);
    //logicPbShield8->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield9->SetVisAttributes(LeadVisAtt);
    //logicPbShield9->SetVisAttributes(G4VisAttributes::Invisible);
    logicPbShield10->SetVisAttributes(LeadVisAtt);
	//logicPbShield10->SetVisAttributes(G4VisAttributes::Invisible);
	G4VisAttributes* CopperVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.));//yellow
	logicCuShield1->SetVisAttributes(CopperVisAtt);
	//logicCuShield1->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield2->SetVisAttributes(CopperVisAtt);
	//logicCuShield2->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield3->SetVisAttributes(CopperVisAtt);
	//logicCuShield3->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield4->SetVisAttributes(CopperVisAtt);
	//logicCuShield4->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield5->SetVisAttributes(CopperVisAtt);
	//logicCuShield5->SetVisAttributes(G4VisAttributes::Invisible);
	logicCuShield6->SetVisAttributes(CopperVisAtt);
	//logicCuShield6->SetVisAttributes(G4VisAttributes::Invisible);
	logicVacCryo->SetVisAttributes(G4VisAttributes::Invisible);//logicVaCryo
	//logicVacCryo->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));//white
	logicHolder1->SetVisAttributes(CapVisAtt);
	//logicHolder1->SetVisAttributes(G4VisAttributes::Invisible);
	logicHolder2->SetVisAttributes(CapVisAtt);
	//logicHolder2->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerPb->SetVisAttributes(LeadVisAtt);
	//logicLayerPb->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb1->SetVisAttributes(LeadVisAtt);
	//logicLayerThinPb1->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb2->SetVisAttributes(LeadVisAtt);
	//logicLayerThinPb2->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb3->SetVisAttributes(LeadVisAtt);
	//logicLayerThinPb3->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb4->SetVisAttributes(LeadVisAtt);
	//logicLayerThinPb4->SetVisAttributes(G4VisAttributes::Invisible);
	logicLayerThinPb5->SetVisAttributes(LeadVisAtt);
	//logicLayerThinPb5->SetVisAttributes(G4VisAttributes::Invisible);

    //------------ set the incident position ------
    //
    // get the pointer to the User Interface manager
    //
    // G4UImanager* UI = G4UImanager::GetUIpointer();
    //    UI->ApplyCommand("/run/verbose 1");
    //        UI->ApplyCommand("/event/verbose 2");
    //        UI->ApplyCommand("/tracking/verbose 1");

    //G4double zpos = -fWorldLength/2.;
    //  G4double zpos = 0.;
    //  G4String command = "/gps/pos/centre ";
    //  std::ostringstream os;
    //  os << zpos ;
    //  G4String xs = os.str();
    //
    // UI->ApplyCommand(command+"0. 0. "+xs+" mm");
    // UI->ApplyCommand("/gps/pos/type Point");
	// UI->ApplyCommand("/gps/pos/shape Cylinder");
    // UI->ApplyCommand("/gps/position ");
    // UI->ApplyCommand(command+"0. 0. "+xs+" mm");
    // UI->ApplyCommand("/gps/particle proton");
    // UI->ApplyCommand("/gps/direction 0 0 1");
    // UI->ApplyCommand("/gps/ion 100 MeV");
    // UI->ApplyCommand("/gps/direction 0 0 1");
    // UI->ApplyCommand("/gps/energy 100 MeV");
    //

  return physiWorld;
}

void gebicDetectorConstruction::ConstructSDandField()
{
    // sensitive detectors -----------------------------------------------------
    auto sdManager = G4SDManager::GetSDMpointer();

    auto HPGe = new gebicSensitiveDetector("/HPGe");
    sdManager->AddNewDetector(HPGe);
    _logicDetector->SetSensitiveDetector(HPGe);

}


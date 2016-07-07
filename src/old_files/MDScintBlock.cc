#include "MDScintBlock.hh"

#include "G4NistManager.hh"
#include "G4Colour.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"

MDScintBlock::MDScintBlock(const G4ThreeVector &size, G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *pMotherLogical,
		 					G4bool pMany, G4int pCopyNo):
							G4PVPlacement(pRot,tlate,
							//Temp logical volume must be created here
							new G4LogicalVolume(new G4Box("temp",1,1,1),G4Material::GetMaterial("Vacuum"),"temp",0,0,0),
							"Scintillator",pMotherLogical,pMany,pCopyNo):
							G4PVPlacement(pRot,tlate,
							//Temp logical volume must be created here
							new G4LogicalVolume(new G4Box("temp",1,1,1),
							G4Material::GetMaterial("Vacuum"),
							"temp",0,0,0),
							"housing",pMotherLogical,pMany,pCopyNo)

{
	
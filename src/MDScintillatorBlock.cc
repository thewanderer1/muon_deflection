/*
 * MDScintillatorBlock.cc
 *
 *  Created on: Jul 20, 2016
 *      Author: shantam
 */

#include "MDScintillatorBlock.hh"

#include "G4RunManager.hh"
#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "MDScintSD.hh"
#include "G4SDManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Box.hh"

bool first = true;

MDScintillatorBlock::MDScintillatorBlock(G4RotationMatrix *pRot, const G4ThreeVector &tlate, const G4ThreeVector & dimensions, G4LogicalVolume *pMotherLogical, G4bool pMany, G4int  pCopyNo,MDDetectorConstruction* detector):
G4PVPlacement(pRot,tlate,
			new G4LogicalVolume(new G4Box("temp",1,1,1),
		                                         G4Material::GetMaterial("Vacuum"),
		                                         "temp",0,0,0),
			"scintillator",pMotherLogical,pMany,pCopyNo),
xdim(dimensions[0]),
ydim(dimensions[1]),
zdim(dimensions[2])
{
	G4PVPlacement* physscint = new G4PVPlacement(0,G4ThreeVector(0,0,0),detector->logicscint,"scint_block",detector->logichousing,false,0);

	SetLogicalVolume(detector->logichousing);

	G4OpticalSurface* scintWrap = new G4OpticalSurface("ScintWrap");
	new G4LogicalBorderSurface("ScintWrap", physscint,
                               this,
                               scintWrap);

	scintWrap->SetModel(glisur);
	scintWrap->SetFinish(polished);
	G4double pp[] = {2.0*eV,7.0*eV,7.14*eV};
	const G4int num = sizeof(pp)/sizeof(G4double);
	G4double reflectivity[] = {1., 1.};
	G4double efficiency[] = {0.0, 0.0};
    G4MaterialPropertiesTable* scintWrapProperty
       = new G4MaterialPropertiesTable();

    scintWrapProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
    scintWrapProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
    scintWrap->SetMaterialPropertiesTable(scintWrapProperty);
}

MDScintillatorBlock::~MDScintillatorBlock() {
	// TODO Auto-generated destructor stub
}


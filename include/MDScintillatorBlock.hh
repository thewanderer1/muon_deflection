/*
 * MDScintillatorBlock.hh
 *
 *  Created on: Jul 20, 2016
 *      Author: shantam
 */

#ifndef INCLUDE_MDSCINTILLATORBLOCK_HH_
#define INCLUDE_MDSCINTILLATORBLOCK_HH_

#include <G4PVPlacement.hh>
#include "G4Material.hh"
#include "MDDetectorConstruction.hh"

class MDScintillatorBlock: public G4PVPlacement {
public:
	MDScintillatorBlock(G4RotationMatrix *pRot, const G4ThreeVector &tlate, const G4ThreeVector & dimensions, G4LogicalVolume *pMotherLogical, G4bool pMany, G4int  pCopyNo,MDDetectorConstruction*);
	virtual ~MDScintillatorBlock();

private:
	G4double xdim;
	G4double ydim;
	G4double zdim;
	void SetSurfProps();
	G4PVPlacement* physscint;
};

#endif /* INCLUDE_MDSCINTILLATORBLOCK_HH_ */

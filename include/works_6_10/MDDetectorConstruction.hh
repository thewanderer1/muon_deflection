

#ifndef MDDetectorConstruction_h
#define MDDetectorConstruction_h


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"

class MDDetectorConstruction: public G4VUserDetectorConstruction
{
public:
	MDDetectorConstruction(int nummodules,G4ThreeVector dim,int stripsperlayer,G4ThreeVector init_pos); // x dim has to be divisble by ydim!! and y dim must equal z dim in the vector dim
	
	virtual ~MDDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

protected:
    G4LogicalVolume*  fScoringVolume;

private:
	void SurfaceProperties();
	G4Material* GetScintillatorMaterial();
	int nummodules;
	G4ThreeVector dim;
	int stripsperlayer;
	G4ThreeVector init_pos;	
};
 

#endif
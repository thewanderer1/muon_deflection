

#ifndef MDDetectorConstruction_h
#define MDDetectorConstruction_h


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"

class MDDetectorConstruction: public G4VUserDetectorConstruction
{
	friend class MDScintillatorBlock;
public:
	MDDetectorConstruction(int nummodules,G4ThreeVector dim,std::vector<G4ThreeVector>,std::vector<G4ThreeVector>,std::vector<G4ThreeVector>,std::vector<std::string>,std::vector<std::string>); // x dim has to be divisble by ydim!! and y dim must equal z dim in the vector dim
	
	virtual ~MDDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

	static G4Material* GetVaccuumMaterial();
	static G4Material* GetScintillatorMaterial();


protected:
    G4LogicalVolume*  fScoringVolume;

private:
	void SurfaceProperties();
	static G4Material *vacuum;
	int nummodules;
	G4ThreeVector dim;
	
	void SetScintillators();
	G4VPhysicalVolume* physWorld;
	std::vector<G4ThreeVector> positions;
	std::vector<G4ThreeVector> densepositions;
	std::vector<G4ThreeVector> densesize;
	std::vector<std::string> densetypes;
	std::vector<std::string> densematerials;
	double xdim;
	double ydim;
	double zdim;
	int numstrips;

	G4LogicalVolume* logicWorld; 

	G4LogicalVolume *logichousing;
	G4LogicalVolume *logicscint;
	G4Box *scint_box;
	G4Box *housing_box;
};
 

#endif

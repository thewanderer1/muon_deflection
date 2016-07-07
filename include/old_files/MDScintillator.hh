#ifndef MD_SCINTILLATOR_HH
#define MD_SCINTILLATOR_HH


#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"

class MDScintillator : public G4PVPlacement
{
public:
	MDScintillator(const G4ThreeVector &size, G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *pMotherLogical,
		 G4bool pMany, G4int pCopyNo, MDDetectorConstruction *c); //NOTE: there is a 12 mev/cm loss for muons traveling through lead:https://pdg.web.cern.ch/pdg/2013/AtomicNuclearProperties/adndt.pdf
	virtual ~MDScintillator();
private:
	void  VisAttributes();
	void  SetSurfaceProperties();
	G4Material* GetScintillatorMaterial();

	G4LogicalVolume *scint;
	G4LogicalVolume *housing;
	G4LogicalVolume *pmt
	G4LogicalVolume *photocath	
	
};




#endif // MD_LEAD_SHIELD_HH


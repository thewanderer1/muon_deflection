#ifndef MD_LEAD_SHIELD_HH
#define MD_LEAD_SHIELD_HH

#include "MDDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"

class MDLeadShield : public G4PVPlacement
{
public:
	MDLeadShield(G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *pMotherLogical,
		 G4bool pMany, G4int pCopyNo, MDDetectorConstruction *c); //NOTE: there is a 12 mev/cm loss for muons traveling through lead:https://pdg.web.cern.ch/pdg/2013/AtomicNuclearProperties/adndt.pdf
	virtual ~MDLeadShield();
private:
	void  SetVisAttributes();
	void  SetSurfaceProperties();	
	
};




#endif // MD_LEAD_SHIELD_HH


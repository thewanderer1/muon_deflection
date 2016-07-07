#include "G4PVPlacement.hh"


class MDScintUnit: public G4PVPlacement
{
public:
	MDScintUnit(const G4ThreeVector &tlate)
	virtual ~MDScintUnit();
	G4LogicalVolume * GetSingleBlockLogical();

private:
	G4LogicalVolume* singleblock;
	static int blockcopyno;
}
/*
 * MDHit.hh
 *
 *  Created on: Jun 16, 2016
 *      Author: shantam
 */

#ifndef INCLUDE_MDHIT_HH_
#define INCLUDE_MDHIT_HH_

#include "G4VHit.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include <iostream>

class MDHit: public G4VHit {
public:
	MDHit(G4double,G4double,G4double,G4int);
	virtual ~MDHit();



	G4int getTrackId() const ;

	G4double getX() const ;

	G4double getY() const ;


	G4double getZ() const ;

	const G4ThreeVector& getActualxhitpt() const;
	void setActualxhitpt(const G4ThreeVector& actualxhitpt);
	const G4ThreeVector& getActualyhitpt() const;
	void setActualyhitpt(const G4ThreeVector& actualyhitpt);

	bool writeAsCSV;

	std::ofstream& operator>>(std::ofstream& of);
	const G4ThreeVector& getEntryp() const;
	void setEntryp(const G4ThreeVector& entryp);
	const G4ThreeVector& getExitp() const;
	void setExitp(const G4ThreeVector& exitp);
	const G4ThreeVector& getMiddlep() const;
	void setMiddlep(const G4ThreeVector& middlep);
	const G4ThreeVector& getActualexitpoint() const;
	void setActualexitpoint(const G4ThreeVector& actualexitpoint);

private:
	G4ThreeVector entryp;
	G4ThreeVector middlep;
	G4ThreeVector exitp;
	G4double x;
	G4double y;
	G4double z;
	G4int trackID;
	G4ThreeVector actualyhitpt;
	G4ThreeVector actualxhitpt;
	G4ThreeVector actualexitpoint;

//put exact positions in as well
};
typedef G4THitsCollection<MDHit> MDHitsCollection;
#endif /* INCLUDE_MDHIT_HH_ */

/*
 * MDHit.cc
 *
 *  Created on: Jun 16, 2016
 *      Author: shantam
 */

#include "MDHit.hh"
#include "G4ThreeVector.hh"
#include <iostream>
#include <fstream>
#include "globals.hh"
#include "G4SystemOfUnits.hh"

MDHit::MDHit(G4double xval,G4double yval,G4double zval,G4int tid):x(xval),y(yval),z(zval),trackID(tid),writeAsCSV(true)
{

}

MDHit::~MDHit() {
	// TODO Auto-generated destructor stub
}


G4int MDHit::getTrackId() const {
	return trackID;
}

G4double MDHit::getX() const {
	return x;
}



G4double MDHit::getY() const {
	return y;
}



G4double MDHit::getZ() const {
	return z;
}

const G4ThreeVector& MDHit::getActualxhitpt() const {
	return actualxhitpt;
}

void MDHit::setActualxhitpt(const G4ThreeVector& actualxhitpt) {
	this->actualxhitpt = actualxhitpt;
}

const G4ThreeVector& MDHit::getActualyhitpt() const {
	return actualyhitpt;
}

void MDHit::setActualyhitpt(const G4ThreeVector& actualyhitpt) {
	this->actualyhitpt = actualyhitpt;
}

std::ofstream& MDHit::operator>>(std::ofstream& of)
{
	//of<<<<initalKE<<','<<finalKE<<','<<currentKE<<','<<x<<','<<y<<','<<z<<','<<actualyhitpt[0]<<','<<actualyhitpt[1]<<','<<actualyhitpt[2]<<','<<actualxhitpt[0]<<','<<actualxhitpt[1]<<','<<actualxhitpt[2]<<trackID<<
	if(writeAsCSV)
	{
		of<<entryp[0]<<','<<entryp[1]<<','<<entryp[2]<<','
				<<middlep[0]<<','<<middlep[1]<<','<<middlep[2]<<','
				<<exitp[0]<<','<<exitp[1]<<','<<exitp[2]<<','<<x<<','<<y<<','<<z<<','<<actualyhitpt[0]<<','<<actualyhitpt[1]<<','<<actualyhitpt[2]<<','<<actualxhitpt[0]<<','<<actualxhitpt[1]<<','<<actualxhitpt[2]<<','<<actualyhitpt[0]<<','<<actualyhitpt[1]<<','<<actualyhitpt[2]<<','<<actualexitpoint[0]<<','<<actualexitpoint[1]<<','<<actualexitpoint[2]<<','<<"mu-"<<G4endl;
	}
	return of;
}

const G4ThreeVector& MDHit::getEntryp() const {
	return entryp;
}

void MDHit::setEntryp(const G4ThreeVector& entryp) {
	this->entryp = entryp;
}

const G4ThreeVector& MDHit::getExitp() const {
	return exitp;
}

void MDHit::setExitp(const G4ThreeVector& exitp) {
	this->exitp = exitp;
}

const G4ThreeVector& MDHit::getMiddlep() const {
	return middlep;
}

void MDHit::setMiddlep(const G4ThreeVector& middlep) {
	this->middlep = middlep;
}

const G4ThreeVector& MDHit::getActualexitpoint() const {
	return actualexitpoint;
}

void MDHit::setActualexitpoint(const G4ThreeVector& actualexitpoint) {
	this->actualexitpoint = actualexitpoint;
}

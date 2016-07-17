#ifndef INCLUDE_MDSCINT_HH_
#define INCLUDE_MDSCINT_HH_

#include <vector>
#include <fstream>

#include "MDHit.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


struct HalfHitX{
	G4double x;
	G4double zofmodule;//z of the strip
	G4ThreeVector exitp;
	G4int trackID;
	G4ThreeVector exitpoint;

 // this is BAD, use some way to uniquely identify which strip was hit!!!
	HalfHitX(G4double xval, G4double zval, const G4ThreeVector& p,const G4ThreeVector& point, G4int tid):x(xval),exitpoint(point),zofmodule(zval),exitp(p),trackID(tid){}
};

struct HalfHitY{
	//G4double zvalofprevxscinthit;
	G4double y;
	G4double z;
	G4ThreeVector entryp;
	G4int trackID;
	bool accountedFor;
	G4ThreeVector yhitpt;

	HalfHitY(G4double yval, G4double zval, const G4ThreeVector& p,G4int trackid, const G4ThreeVector hitpt):y(yval),z(zval),entryp(p),trackID(trackid),accountedFor(false),yhitpt(hitpt)
	{}
};

struct PhotonEngs
{
	G4double trackid;
	G4double x;
	G4double y;
	G4double z;
	G4double energy;
	PhotonEngs(G4int tid, const G4VTouchable* gvt, G4double eng): trackid(tid),x(gvt->GetTranslation()[0]),y(gvt->GetTranslation()[1]),z(gvt->GetTranslation()[2]),energy(eng)
	{}
};

class MDScintSD : public G4VSensitiveDetector
{
	
public:
	MDScintSD(G4String name);
	virtual ~MDScintSD();
	virtual void Initialize(G4HCofThisEvent*HCE);
	virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
	virtual void EndOfEvent(G4HCofThisEvent*HCE);


	static std::vector<PhotonEngs> pngs;

private:
	MDHitsCollection * hitsCollection;
	G4int collectionID;
	std::vector<HalfHitY> halfhits;
	bool InHalfHits(G4Step*);
	std::ofstream debugfile;


//look at the pre step point and the post step point
//if the prestep point is part of a scintillator and so is the post step point and their locations are seperated by xdim, then get the 
};
#endif

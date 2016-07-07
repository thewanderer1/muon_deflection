#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include <random>
#include <ctime>
#include <chrono>


class MDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		//MDPrimaryGeneratorAction(const G4ThreeVector& c1,const G4ThreeVector& c2,const G4ThreeVector& c3,const G4ThreeVector& c4);
		MDPrimaryGeneratorAction();
		virtual ~MDPrimaryGeneratorAction();
       	virtual void GeneratePrimaries(G4Event*);	


    private:
 	G4ParticleDefinition* muon;
 	G4ParticleGun* fParticleGun;
 	G4ParticleTable* particleTable;
 	G4ThreeVector GetRandMomentumVector(const G4ThreeVector& p);
 	G4ThreeVector GetRandPos();
	 std::default_random_engine generator;
	 std::uniform_real_distribution<double> urd;

	 G4ThreeVector corner1;
	 G4ThreeVector corner2;
	 G4ThreeVector corner3;
	 G4ThreeVector corner4;

 };

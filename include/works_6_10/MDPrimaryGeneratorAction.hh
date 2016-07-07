#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

class MDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		MDPrimaryGeneratorAction();
		virtual ~MDPrimaryGeneratorAction();
       	virtual void GeneratePrimaries(G4Event*);	
    private:
 	G4ParticleDefinition* muon;
 	G4ParticleGun* fParticleGun;

 };

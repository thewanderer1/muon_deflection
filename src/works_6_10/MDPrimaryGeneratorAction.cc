#include "MDPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MDPrimaryGeneratorAction::MDPrimaryGeneratorAction() :
 G4VUserPrimaryGeneratorAction(), muon(0)
 {
 	fParticleGun  = new G4ParticleGun(5);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
 	muon = particleTable->FindParticle(particleName="mu+");
 	fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,9*m));
 	fParticleGun->SetParticleDefinition(muon);
 	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
 	fParticleGun->SetParticleEnergy(1000*MeV);


 }	

 void MDPrimaryGeneratorAction::GeneratePrimaries(G4Event* e)
 {
 	fParticleGun->GeneratePrimaryVertex(e);
 }

 MDPrimaryGeneratorAction::~MDPrimaryGeneratorAction()
 {
 	delete fParticleGun;
 }
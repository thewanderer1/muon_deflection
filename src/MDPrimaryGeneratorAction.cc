#include "MDPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include <random>
#include <ctime>
#include <chrono>

//this should be passed the g4threevectors of the four corners of the bottommost module
//corner 1 should be +x,+y , 2 should be -x,+y
/*
MDPrimaryGeneratorAction::MDPrimaryGeneratorAction(const G4ThreeVector& c1,const G4ThreeVector& c2,const G4ThreeVector& c3,const G4ThreeVector& c4) :
 G4VUserPrimaryGeneratorAction(), muon(0),particleTable(0), generator(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())),urd(std::uniform_real_distribution<double>(-1,1)),
 corner1(c1),corner2(c2),corner3(c3),corner4(c4)
 {
 	fParticleGun  = new G4ParticleGun(1);
    
    particleTable = G4ParticleTable::GetParticleTable();
 }	
 */
MDPrimaryGeneratorAction::MDPrimaryGeneratorAction() :
 G4VUserPrimaryGeneratorAction(), muon(0),particleTable(0), generator(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())),urd(std::uniform_real_distribution<double>(-1,1))
{
 	fParticleGun  = new G4ParticleGun(1);

    particleTable = G4ParticleTable::GetParticleTable();
 }
 void MDPrimaryGeneratorAction::GeneratePrimaries(G4Event* e)
 {
	 G4String particleName;
	 muon = particleTable->FindParticle(particleName="mu-");


  	fParticleGun->SetParticleDefinition(muon);

  	fParticleGun->SetParticleEnergy(1000*MeV);

 	G4ThreeVector position = GetRandPos();
 	G4ThreeVector momentum = GetRandMomentumVector(position);
  	fParticleGun->SetParticlePosition(position);
  	fParticleGun->SetParticleMomentumDirection(momentum);
   	fParticleGun->GeneratePrimaryVertex(e);

;
 }

 MDPrimaryGeneratorAction::~MDPrimaryGeneratorAction()
 {
 	delete fParticleGun;
 	delete particleTable;
 }

 G4ThreeVector MDPrimaryGeneratorAction::GetRandMomentumVector(const G4ThreeVector& p)
 {
	 /*
	 G4ThreeVector position = p;

	 G4ThreeVector temp = p;

	 G4ThreeVector limit1 = temp -= corner1;
	 temp = p;
	 G4ThreeVector limit2 = temp -= corner2;
	 temp=p;

	 G4ThreeVector limit3 = temp -= corner3;
	 temp = p;
	 G4ThreeVector limit4 = temp -= corner4;
	 temp = p;
*/
	 double xval,yval,zval;
	 double length;

	 do
	 {
		 xval = urd(generator);
		 yval = urd(generator);
		 zval = urd(generator);
		 length = sqrt(xval*xval+yval*yval+zval*zval);
		 xval = xval/length;
		 yval = yval/length;
		 zval = zval/length;
	 }while(length > 1 || zval > -.8);



	 return G4ThreeVector(xval,yval,zval);
 }
 G4ThreeVector MDPrimaryGeneratorAction::GetRandPos()
{
	 double xval,yval;
	 xval = urd(generator)*2;
	 yval = urd(generator)*2;
	// return G4ThreeVector(0,0,5*m); //not getting rand pos rn
	 return G4ThreeVector(xval*m,yval*m,5*m);
}

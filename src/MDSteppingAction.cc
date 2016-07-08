/*
 * MDSteppingAction.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: shantam
 */

#include "MDSteppingAction.hh"
#include "MDDetectorConstruction.hh"
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Box.hh"

std::vector<HalfHitX> MDSteppingAction::hfx;

MDSteppingAction::MDSteppingAction():particleTable(G4ParticleTable::GetParticleTable()) , muon(particleTable->FindParticle(particleName="mu-")){
	// TODO Auto-generated constructor stub

}

MDSteppingAction::~MDSteppingAction() {
	// TODO Auto-generated destructor stub
}

void MDSteppingAction::UserSteppingAction(const G4Step* fullstep)
{
	//if the post step is a vaccumm and the prestep is a rotated scintillator, then look up the pos and length of the scintillator
	//add it to the half hit y with the z val of the OTHER, scinillator, and the trackID



	if((*(fullstep->GetTrack()->GetParticleDefinition()) == *muon) && ((*(fullstep->GetPostStepPoint()->GetMaterial()) ==  *(MDDetectorConstruction::GetVaccuumMaterial())) &&
			(*(fullstep->GetPreStepPoint()->GetMaterial()) == *(MDDetectorConstruction::GetScintillatorMaterial()))))
	{
		G4StepPoint *prestep = fullstep->GetPreStepPoint();
		G4StepPoint *poststep = fullstep->GetPostStepPoint();
		//first calculate the zval - DEPRICATED

		G4double blockzval = prestep->GetTouchable()->GetTranslation()[2];

		G4Box* scintpiece = dynamic_cast<G4Box*>(prestep->GetPhysicalVolume()->GetLogicalVolume()->GetSolid());

		if(!scintpiece)
		{
			G4cout<<"unable to cast the scint block as a G4box, exiting"<<G4endl;
			exit(1);
		}

		G4double zposofmodule = blockzval + scintpiece->GetZHalfLength();
		MDSteppingAction::hfx.push_back(HalfHitX(prestep->GetTouchable()->GetTranslation()[0],zposofmodule,poststep->GetMomentum(),poststep->GetPosition(),fullstep->GetTrack()->GetTrackID()));
		G4cout<<"done: "<<poststep->GetMomentum()[0]<<" x: "<<prestep->GetTouchable()->GetTranslation()[0]<<" z: "<<zposofmodule<<G4endl;
	}
}

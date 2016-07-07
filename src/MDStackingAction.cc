/*
 * MDStackingAction.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: shantam
 */

#include "MDStackingAction.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4ParticleTable.hh"
#include "MDScintSD.hh"

MDStackingAction::MDStackingAction() {
	// TODO Auto-generated constructor stub

}

MDStackingAction::~MDStackingAction() {
	// TODO Auto-generated destructor stub
}

G4ClassificationOfNewTrack MDStackingAction::ClassifyNewTrack(const G4Track* trk) {


	if(*(trk->GetDefinition()) == *(G4OpticalPhoton::OpticalPhotonDefinition()))
	{
		//add to photon engs here

		//the following should only be used for debuggin (very slow)
		/*
		for(std::vector<PhotonEngs>::iterator it = MDScintSD::pngs.begin(); it != MDScintSD::pngs.end(); ++it)
		{
			if(trk->GetTrackID() == it->trackid)
			{
				G4cout<<"error!!"<<G4endl;
				exit(1);
			}
		}
		*/
		double wavelength = 3e8*4.135e-15/(trk->GetKineticEnergy()*1e6)*1e9;
		if(wavelength < 300 || wavelength > 700)
			return fKill;
		MDScintSD::pngs.push_back(PhotonEngs(trk->GetTrackID(),trk->GetTouchable(),trk->GetKineticEnergy()));

		return fKill;
	}
	else
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		G4ParticleDefinition* muon = particleTable->FindParticle(particleName="mu-");
		G4ParticleDefinition* elec = particleTable->FindParticle(particleName="e-");
		if(!((*(trk->GetParticleDefinition()) == *muon) || (*(trk->GetParticleDefinition()) == *elec)))
			G4cout<<trk->GetDefinition()->GetParticleName()<<G4endl;
		return fUrgent;
	}

	
}

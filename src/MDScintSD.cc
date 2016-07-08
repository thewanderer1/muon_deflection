/*
 * MDScintSD.cc
 *
 *  Created on: Jun 16, 2016
 *      Author: shantam
 */
#include "MDScintSD.hh"
#include "MDHit.hh"
#include "MDSteppingAction.hh"
#include "G4ParticleTable.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"

std::vector<PhotonEngs> MDScintSD::pngs;
MDScintSD::MDScintSD(G4String name):G4VSensitiveDetector(name),hitsCollection(0),collectionID(-1),halfhits(),debugfile("debug.txt")
{
	collectionName.insert("hitsCollection");

}
MDScintSD::~MDScintSD()
{

}
void MDScintSD::Initialize(G4HCofThisEvent*HCE)
{
	hitsCollection = new MDHitsCollection(SensitiveDetectorName,collectionName[0]);
	if(collectionID<0)
	{
		collectionID = GetCollectionID(0);
	}
	HCE->AddHitsCollection(collectionID,hitsCollection);
}
G4bool MDScintSD::ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist)// a particle has to be at the pre step point to be detected
{
	//debuginfo

	//check if muon
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* muon = particleTable->FindParticle(particleName="mu-");
	G4ParticleDefinition* e = particleTable->FindParticle(particleName="e-");

	G4StepPoint* preStep = aStep->GetPreStepPoint();

	if((*(aStep->GetTrack()->GetParticleDefinition()) == *muon))
	{

		const G4RotationMatrix *grm = preStep->GetTouchable()->GetRotation();
		const G4ThreeVector trans = preStep->GetTouchable()->GetTranslation();

		G4RotationMatrix* rotated = new G4RotationMatrix();
		rotated->rotateZ(90*deg);

		#ifdef DEBUG
		debugfile<<trans[0]<<","<<trans[1]<<","<<trans[2]<<G4endl;
		debugfile<<"xval: "<<preStep->GetPosition()[0]<<" yval: "<<preStep->GetPosition()[1]<<" zval: "<<preStep->GetPosition()[2]
						  <<" Track id: "<<aStep->GetTrack()->GetTrackID()<<" is rotated: "<<(*grm == *rotated)<<G4endl;
		#endif
	//what is going on is that the muon is taking multiple steps through the scintillator and at each step a hit is being triggered
		//so when it takes a second step in a rotated scint, it is exiting

		if(*grm == *rotated)//if the volume is rotated 90, ie it is a rotated scint
		{

			//check if the hit is already in first

			for(int i = 0; i < hitsCollection->GetSize(); ++i)
			{
				if((*hitsCollection)[i]->getTrackId() == aStep->GetTrack()->GetTrackID())
				{
					G4Box *scintpiece = dynamic_cast<G4Box*>(preStep->GetPhysicalVolume()->GetLogicalVolume()->GetSolid());
					if(!scintpiece)
					{
						G4cout<<"error"<<G4endl;
					}

					if((*hitsCollection)[i]->getX() == trans[0] && (*hitsCollection)[i]->getZ() == (trans[2] + scintpiece->GetZHalfLength()))
					{
						debugfile<<"returning"<<G4endl;
						return false;

					}
				}
			}

			debugfile<<"pt a"<<G4endl;

			bool halfhitfound = false;
			for(std::vector<HalfHitY>::iterator it = halfhits.begin(); it < halfhits.end() && !halfhitfound; ++it)
			{
				if(it->trackID == aStep->GetTrack()->GetTrackID() && !(it->accountedFor))
				{

					MDHit *mdh = new MDHit(trans[0],it->y,preStep->GetPosition()[2],aStep->GetTrack()->GetTrackID());

					mdh->setEntryp(it->entryp);
					mdh->setMiddlep(preStep->GetMomentum());
					mdh->setActualyhitpt(it->yhitpt);
					mdh->setActualxhitpt(preStep->GetPosition());
					hitsCollection->insert(mdh);
					it->accountedFor = true;
					halfhitfound = true;
		#ifdef DEBUG
					debugfile<<"hit added: "<<"x: "<<trans[0]<<" y: "<<it->y<<" z: "<<preStep->GetPosition()[2]<<
							 " trackid: "<<aStep->GetTrack()->GetTrackID()<<G4endl;
		#endif
				}
			}
			if(!halfhitfound)
			{
				bool error = true;
				for(int i = 0; i < (hitsCollection->GetSize()) && error; ++i)
				{
					G4Box *scintpiece = dynamic_cast<G4Box*>(preStep->GetPhysicalVolume()->GetLogicalVolume()->GetSolid());
					if(!scintpiece)
					{
						G4cout<<"error"<<G4endl;
						exit(1);
					}
					G4double zdim = scintpiece->GetZHalfLength();
					if(((*hitsCollection)[i]->getZ()-zdim) == trans[2] && aStep->GetTrack()->GetTrackID() == (*hitsCollection)[i]->getTrackId())
						error = false;
				}
				if(error)
				{
					std::cout<<"detector error"<<std::endl;
					G4cout<<aStep->GetTrack()->GetParticleDefinition()->GetParticleName()<<G4endl;

					for(std::vector<HalfHitY>::iterator it = halfhits.begin(); it < halfhits.end(); ++it)
					{
						G4cout<<"halfits contents"<<G4endl;
					//G4cout<<"xval: "<<it->x<<" track id: "<<it->trackID<<" zval: "<<it->z<<" acc for: "<<it->accountedFor<<G4endl;
					}

					G4cout<<"current particle contents"<<G4endl;
					G4cout<<"xval: "<<preStep->GetPosition()[0]<<" yval: "<<preStep->GetPosition()[1]<<" zval: "<<preStep->GetPosition()[2]
					  <<" Track id: "<<aStep->GetTrack()->GetTrackID()<<G4endl;


					exit(1);
				}
			}
		}
		else//if it isn't rotated 90, ie an unrotated scint
		{

			if(!InHalfHits(aStep))
			{
				halfhits.push_back(HalfHitY(trans[1],trans[2],preStep->GetMomentum(),aStep->GetTrack()->GetTrackID(),preStep->GetPosition()));
	#ifdef DEBUG
				debugfile<<"half hit added"<<" y: "<<trans[1]<<" z: "<<trans[2]<<" trackid: "<<aStep->GetTrack()->GetTrackID()<<G4endl;
	#endif
			}
		}
		delete rotated;
		return true;
	}

	return false;

}
void MDScintSD::EndOfEvent(G4HCofThisEvent*HCE)
{
	//here, look into half y hits and the half x hits and then calculate the delta ke and add that into the hits

	for(std::vector<HalfHitX>::iterator yt = MDSteppingAction::hfx.begin(); yt < MDSteppingAction::hfx.end(); ++yt)
	{
		for(int i = 0; i < hitsCollection->GetSize(); ++i)
		{
			if((*hitsCollection)[i]->getZ() == (yt->zofmodule))
			{
				(*hitsCollection)[i]->setExitp(yt->exitp);
				(*hitsCollection)[i]->setActualexitpoint(yt->exitpoint);
				//G4cout<<"stored: "<<yt->exitp[0]<<G4endl;
			}
		}
	}

	std::ofstream ofs("photoninfo.csv",std::ofstream::app);
	std::vector<G4ThreeVector> positions;
	std::vector<int> count;
	std::vector<double> sumofwavelengths;
	

	for(std::vector<PhotonEngs>::iterator it = pngs.begin(); it != pngs.end(); ++it)
	{
		G4ThreeVector p(it->x,it->y,it->z);
		double wavelength = 3e8*4.135e-15/(it->energy*1e6)*1e9;


		bool found = false;
		for(int i = 0; i < positions.size(); i++)
		{

			if(positions[i] == p)
			{
				count[i]++;
				sumofwavelengths[i] += wavelength;
				found = true;
				break;
			}
		}
		if(!found)
		{
			positions.push_back(p);
			count.push_back(1);
			sumofwavelengths.push_back(wavelength);
			//int i;
			//ofs<<"hi"<<G4endl;
		}
	}
	G4int eid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	for(int i = 0; i < positions.size(); i++)
	{
		ofs<<eid<<","<<positions[i][0]<<','<<positions[i][1]<<','<<positions[i][2]<<','<<count[i]<<','<<sumofwavelengths[i]/count[i]<<G4endl;
	}
	ofs.close();
	//clear the static vars
	pngs = std::vector<PhotonEngs>();
	MDSteppingAction::hfx = std::vector<HalfHitX>();
	halfhits = std::vector<HalfHitY> ();


}

bool MDScintSD::InHalfHits(G4Step* step)
{
	int trackid = step->GetTrack()->GetTrackID();

	for(std::vector<HalfHitY>::iterator it = halfhits.begin(); it < halfhits.end(); ++it)
	{
		if(it->trackID == trackid && !(it->accountedFor))
			return true;
	}
	return false;
}

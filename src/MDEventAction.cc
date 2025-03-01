/*
 * MDEventAction.cc
 *
 *  Created on: Jun 20, 2016
 *      Author: shantam
 */


#include "MDEventAction.hh"
#include "MDScintSD.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4UserEventAction.hh"
#include "RunNumber.hh"

MDEventAction::MDEventAction():G4UserEventAction() {


}

MDEventAction::~MDEventAction() {
	// TODO Auto-generated destructor stub
}

void MDEventAction::EndOfEventAction(const G4Event* evt) {

	std::string flname = "RunNumber_";
	flname = flname + std::to_string(RunNumber) + std::string("_runinfo.csv");

	std::ofstream ofs(flname,std::ofstream::app);

	static int CHCID = -1;

	if(CHCID<0)
	{
		CHCID = G4SDManager::GetSDMpointer()->GetCollectionID("hitsCollection");
	}

	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	MDHitsCollection* hitscoll = 0;
	if(HCE)
	{
		hitscoll = dynamic_cast<MDHitsCollection*>((HCE->GetHC(CHCID)));
	}

	if(hitscoll)
	{
		G4cout<<"there are "<<hitscoll->GetSize()<<" entries"<<", event num: "<<evt->GetEventID()<<G4endl;
		for(int i = 0; i < hitscoll->GetSize(); ++i)
		{
			/*
			G4cout<<"x: "<<(*hitscoll)[i]->getX()<<" y: "<<(*hitscoll)[i]->getY()<<" z: "<<(*hitscoll)[i]->getZ()<<
					" init ke:"<<(*hitscoll)[i]->getInitalKe()<<
					" current ke:"<<(*hitscoll)[i]->getCurrentKe()<<
					" final ke:"<<(*hitscoll)[i]->getFinalKe()<<
					" track id: "<<(*hitscoll)[i]->getTrackId()<<G4endl;
			*/
			ofs<<evt->GetEventID()<<',';
			*((*hitscoll)[i])>>ofs;
		}
	}

	ofs.close();


}

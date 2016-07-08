/*
 * MDSteppingAction.h
 *
 *  Created on: Jun 16, 2016
 *      Author: shantam
 */

#ifndef MDSTEPPINGACTION_H_
#define MDSTEPPINGACTION_H_

#include "G4UserSteppingAction.hh"
#include <vector>
#include "MDScintSD.hh"


class MDSteppingAction: public G4UserSteppingAction {
public:
	MDSteppingAction();
	virtual ~MDSteppingAction();

	virtual void UserSteppingAction(const G4Step*);

	static std::vector<HalfHitX> hfx;

private:
	G4ParticleTable* particleTable;
	G4String particleName;
	G4ParticleDefinition* muon;
};



#endif /* MDSTEPPINGACTION_H_ */

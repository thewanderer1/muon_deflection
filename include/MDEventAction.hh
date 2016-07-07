/*
 * MDEventAction.hh
 *
 *  Created on: Jun 20, 2016
 *      Author: shantam
 */

#ifndef MDEVENTACTION_HH_
#define MDEVENTACTION_HH_

#include "G4EventManager.hh"
#include "G4UserEventAction.hh"

class MDEventAction: public G4UserEventAction {
public:
	MDEventAction();
	virtual ~MDEventAction();
	virtual void EndOfEventAction(const G4Event*);
};

#endif /* MDEVENTACTION_HH_ */

/*
 * MDStackingAction.hh
 *
 *  Created on: Jun 22, 2016
 *      Author: shantam
 */

#ifndef SRC_MDSTACKINGACTION_HH_
#define SRC_MDSTACKINGACTION_HH_

#include "G4UserStackingAction.hh"


class MDStackingAction: public G4UserStackingAction {
public:
	MDStackingAction();
	virtual ~MDStackingAction();
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
};

#endif /* SRC_MDSTACKINGACTION_HH_ */

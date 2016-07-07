/*
 * MDTrackingAction.hh
 *
 *  Created on: Jun 21, 2016
 *      Author: shantam
 */

#ifndef MDTRACKINGACTION_HH_
#define MDTRACKINGACTION_HH_

#include <G4UserTrackingAction.hh>

class MDTrackingAction: public G4UserTrackingAction {
public:
	MDTrackingAction();
	virtual ~MDTrackingAction();
	virtual void PostUserTrackingAction(const G4Track * aTrack);

};

#endif /* MDTRACKINGACTION_HH_ */

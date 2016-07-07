#ifndef MD_PHYSICS_LIST_HH
#define MD_PHYSICS_LIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class MDPhysicsList: public G4VModularPhysicsList
{
  public:

    MDPhysicsList();
    virtual ~MDPhysicsList();

  public:

    // SetCuts()
    virtual void SetCuts();

};


#endif // MD_PHYSICS_LIST_HH

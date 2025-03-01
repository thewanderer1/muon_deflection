#ifndef GENERAL_PHYSICS_HH
#define GENERAL_PHYSICS_HH

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

class GeneralPhysics : public G4VPhysicsConstructor
{
  public:

    GeneralPhysics(const G4String& name = "general");
    virtual ~GeneralPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

};

#endif // GENERAL_PHYSICS_HH

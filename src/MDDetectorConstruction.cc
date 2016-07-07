#include "MDDetectorConstruction.hh"


#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "MDScintSD.hh"
#include "G4SDManager.hh"
#include <cassert>

G4Material *MDDetectorConstruction::vacuum = new G4Material("Vacuum",1.,1.01*g/mole,
	                            universe_mean_density,kStateGas,0.1*kelvin,
	                             1.e-19*pascal);





MDDetectorConstruction::~MDDetectorConstruction()
{}

MDDetectorConstruction::MDDetectorConstruction(int nummodules,G4ThreeVector dim,std::vector<G4ThreeVector> p):
  nummodules(nummodules),//still needs to be implemented
  dim(dim),
  positions(p)
{


}

G4VPhysicalVolume* MDDetectorConstruction::Construct()
{


	 //create the world
  xdim = dim[0];
  ydim = dim[1];
  zdim = dim[2];

  assert(ydim==zdim); //Ensure that the basic assumptions are met here
  assert(xdim<10*m);
  assert(ydim<10*m);
  assert(zdim<10*m);

  
  numstrips = (int) xdim/ydim;


//create the scintillator strips and the world
	G4Box *worldsolid = new G4Box("World",5*m, 5*m, 5*m);
	G4Box *scint = new G4Box("scintillator",                       
       xdim/2, ydim/2, zdim/2);




	logicWorld =              
    		new G4LogicalVolume(worldsolid,          
            MDDetectorConstruction::GetVaccuumMaterial(),
            "World");  

	logicScint =                         
    		new G4LogicalVolume(scint,MDDetectorConstruction::GetScintillatorMaterial(),"scintillator");//create the logical volume for the scintillator

  G4VisAttributes *gva = new G4VisAttributes(G4Colour::Green());

  logicScint->SetVisAttributes(gva);
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4SDManager* SDMan = G4SDManager::GetSDMpointer();

  MDScintSD *mds = new MDScintSD("/mydet/scint1");

  SDMan->AddNewDetector(mds);
  logicScint->SetSensitiveDetector(mds);

   	physWorld = 
    	new G4PVPlacement(0,                     
        	              G4ThreeVector(),       
            	          logicWorld,            
                	      "World",               
                    	  0,                     
                      	false,                 
                      		0); //create the physical world object

  SetScintillators();

    

    return physWorld;

}

G4Material* MDDetectorConstruction::GetScintillatorMaterial()
{ 
  G4NistManager* mat = G4NistManager::Instance();
  G4Material *pstyrene = mat->FindOrBuildMaterial("G4_POLYSTYRENE");//the base material is polysterene


  //here, we define all of the scintillation constants
  pstyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  G4double wls_Energy[] = {1.77*eV,2.48*eV, 3.2 *eV,4.13*eV};//energy of the photons
  const G4int wlsnum = sizeof(wls_Energy)/sizeof(G4double);

  G4double rIndexPstyrene[]={ 1.5, 1.5, 1.5, 1.5};
  G4double scintilFast[]={1.00, 1.00, 1.00, 1.00};//fast scintillation?
  G4double absorption1[]={2.*cm, 2.*cm, 2.*cm, 2.*cm};//absorbtion length
  G4MaterialPropertiesTable *s_mt = new G4MaterialPropertiesTable();
  s_mt->AddProperty("RINDEX",wls_Energy,rIndexPstyrene,wlsnum);
  s_mt->AddProperty("ABSLENGTH",wls_Energy,absorption1,wlsnum);
  s_mt->AddProperty("FASTCOMPONENT",wls_Energy, scintilFast,wlsnum);
  s_mt->AddConstProperty("SCINTILLATIONYIELD",10./keV);
  s_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  s_mt->AddConstProperty("FASTTIMECONSTANT", 10.*ns); //add yield ratio of 1
  pstyrene->SetMaterialPropertiesTable(s_mt);
  return pstyrene;
}

void MDDetectorConstruction::SetScintillators()
{

//rather than create a whole another class, a series of for loops is used to place the scintillators
  //this is due to the fact that it will be much easier to recover the position in this way using the 
  //built in geant4 systems
  for(int j = 0; j<nummodules; ++j)//master for loop that runs through the number of times the modules should be placed
  {
    G4ThreeVector currpos = positions[j];

    for(int i = 0; i < numstrips; i++)//place first layer
    {

      G4VPhysicalVolume* physScint = 
        new G4PVPlacement(0,                     //no rotation
                          G4ThreeVector(0+currpos[0],(xdim)/2-i*ydim-ydim/2+currpos[1],ydim/2+currpos[2]),       //at (0,0,0)
                          logicScint,            //its logical volume
                          "scintillator",               //its name
                          logicWorld,                     //its mother  volume
                          false,i);  
    }
    for(int i = 0; i < numstrips; i++)//place second layer
    {
      G4RotationMatrix* rmx = new G4RotationMatrix();
      rmx->rotateZ(90*deg);
      G4VPhysicalVolume* physScint = 
      new G4PVPlacement(rmx,                     //no rotation
                        G4ThreeVector((xdim)/2-i*ydim-ydim/2+currpos[0],0+currpos[1],-ydim/2+currpos[2]),       //at (0,0,0)
                        logicScint,            //its logical volume
                        "scintillator",               //its name
                        logicWorld,                     //its mother  volume
                        false,i);
    }

    //now make the block on the negative side
    for(int i = 0; i < numstrips; i++)
    {

      G4VPhysicalVolume* physScint = 
        new G4PVPlacement(0,                     //no rotation
                          G4ThreeVector(0+currpos[0],(xdim)/2-i*ydim-ydim/2+currpos[1],ydim/2-currpos[2]),       //at (0,0,0)
                          logicScint,            //its logical volume
                          "scintillator",               //its name
                          logicWorld,                     //its mother  volume
                          false,i);  
    }
    for(int i = 0; i < numstrips; i++)
    {
      G4RotationMatrix* rmx = new G4RotationMatrix();
      rmx->rotateZ(90*deg);
      G4VPhysicalVolume* physScint = 
      new G4PVPlacement(rmx,                     //no rotation
                        G4ThreeVector((xdim)/2-i*ydim-ydim/2+currpos[0],0+currpos[1],-ydim/2-currpos[2]),       //at (0,0,0)
                        logicScint,            //its logical volume
                        "scintillator",               //its name
                        logicWorld,                     //its mother  volume
                        false,i);
    }

  }

}

G4Material* MDDetectorConstruction::GetVaccuumMaterial()
{

	G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};

	const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
	G4double vacuum_RIND[]={1.,1.,1.}; //define the refractive index

	G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
	vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);//add the refractive index into the list of material properties
	vacuum->SetMaterialPropertiesTable(vacuum_mt);
	return vacuum;
}

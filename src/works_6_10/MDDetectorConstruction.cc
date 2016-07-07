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
MDDetectorConstruction::~MDDetectorConstruction()
{}

MDDetectorConstruction::MDDetectorConstruction(int nummodules,G4ThreeVector dim,int stripsperlayer,G4ThreeVector init_pos):
  nummodules(nummodules),//still needs to be implemented
  dim(dim),
  stripsperlayer(stripsperlayer),//not sure how to implement
  init_pos(init_pos)
{}

G4VPhysicalVolume* MDDetectorConstruction::Construct()
{


	 //create the world
  double xdim = dim[0];
  double ydim = dim[1];
  double zdim = dim[2];

  assert(ydim==zdim);
  assert(xdim<10*m);
  assert(ydim<10*m);
  assert(zdim<10*m);



	G4Box *worldsolid = new G4Box("World",                       //its name
       10*m, 10*m, 10*m);
	G4Box *scint = new G4Box("scintillator",                       //its name
       xdim/2, ydim/2, zdim/2);

  int tempstrips = (int) xdim/ydim;

  G4Material *vacuum = new G4Material("Vacuum",1.,1.01*g/mole,
                          universe_mean_density,kStateGas,0.1*kelvin,
                           1.e-19*pascal);
  G4double vacuum_Energy[]={2.0*eV,7.0*eV,7.14*eV};
  const G4int vacnum = sizeof(vacuum_Energy)/sizeof(G4double);
     G4double vacuum_RIND[]={1.,1.,1.};
  G4MaterialPropertiesTable *vacuum_mt = new G4MaterialPropertiesTable();
  vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND,vacnum);
  vacuum->SetMaterialPropertiesTable(vacuum_mt);

	G4LogicalVolume* logicWorld =              
    		new G4LogicalVolume(worldsolid,          //its solid
            vacuum,           //its material
            "World");  

	G4LogicalVolume* logicScint =                         
    		new G4LogicalVolume(scint,GetScintillatorMaterial(),"scintillator");

  G4VisAttributes *gva = new G4VisAttributes(G4Colour::Yellow());

  logicScint->SetVisAttributes(gva);
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);


   	G4VPhysicalVolume* physWorld = 
    	new G4PVPlacement(0,                     //no rotation
        	              G4ThreeVector(),       //at (0,0,0)
            	          logicWorld,            //its logical volume
                	      "World",               //its name
                    	  0,                     //its mother  volume
                      	false,                 //no boolean operation
                      		0); 


  for(int i = 0; i < tempstrips; i++)
  {

   	G4VPhysicalVolume* physScint = 
    	new G4PVPlacement(0,                     //no rotation
        	              G4ThreeVector(0,(xdim)/2-i*ydim-ydim/2,ydim/2+init_pos[2]),       //at (0,0,0)
            	          logicScint,            //its logical volume
                	      "scintillator",               //its name
                    	  logicWorld,                     //its mother  volume
                      	false,i);  
  }
  for(int i = 0; i < tempstrips; i++)
  {
    G4RotationMatrix* rmx = new G4RotationMatrix();
    rmx->rotateZ(90*deg);
    G4VPhysicalVolume* physScint = 
    new G4PVPlacement(rmx,                     //no rotation
                      G4ThreeVector((xdim)/2-i*ydim-ydim/2,0,-ydim/2+init_pos[2]),       //at (0,0,0)
                      logicScint,            //its logical volume
                      "scintillator",               //its name
                      logicWorld,                     //its mother  volume
                      false,i);
  }
    return physWorld;

}

G4Material* MDDetectorConstruction::GetScintillatorMaterial()
{ 
  G4NistManager* mat = G4NistManager::Instance();
  G4Material *pstyrene = mat->FindOrBuildMaterial("G4_POLYSTYRENE");


  pstyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  G4double wls_Energy[] = {1.77*eV,2.48*eV, 3.2 *eV,4.13*eV};
  const G4int wlsnum = sizeof(wls_Energy)/sizeof(G4double);

  G4double rIndexPstyrene[]={ 1.5, 1.5, 1.5, 1.5};
  G4double scintilFast[]={0.00, 0.00, 1.00, 1.00};
  G4double absorption1[]={2.*cm, 2.*cm, 2.*cm, 2.*cm};
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
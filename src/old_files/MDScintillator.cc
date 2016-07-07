#include "MDScintillator.hh"

#include "G4NistManager.hh"
#include "G4Colour.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"

MDScintillator::MDScintillator(const G4ThreeVector &size, G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *pMotherLogical,
		 					G4bool pMany, G4int pCopyNo):
							G4PVPlacement(pRot,tlate,
							//Temp logical volume must be created here
							new G4LogicalVolume(new G4Box("temp",1,1,1),G4Material::GetMaterial("Vacuum"),"temp",0,0,0),
							"Scintillator",pMotherLogical,pMany,pCopyNo):
							G4PVPlacement(pRot,tlate,
							//Temp logical volume must be created here
							new G4LogicalVolume(new G4Box("temp",1,1,1),
							G4Material::GetMaterial("Vacuum"),
							"temp",0,0,0),
							"housing",pMotherLogical,pMany,pCopyNo)

{
	//extract the size data
	double size_x = size[0];
	double size_y = size[1];
	double size_z = size[2];
	double pmtlen = 3

	//build the housing and the scintillator
	GBox *housing_box = new G4Box("housing_box",size_x*cm,size_y*cm,size_z*cm); //note that the sizes are in cm
	GBox *scint_box = now GBox("scint_box",(size_x-0.025)*cm,(size_y-.05)*cm,(size_z-.05*cm));
	scint = new G4LogicalVolume(scint_box,GetScintillatorMaterial(),"scint_log");
	housing = new G4LogicalVolume(housing_box,G4Material::GetMaterial("Pb"),"housing_log");

	new G4PVPlacement(0,G4ThreeVector(-0.025*cm,0,0),scint,"scintillator",housing,false,0);//place the scintillator in the housing

	//build the pmt
	//build the glass tube first
	GTubs *glass_tub = new GTubs("glass_tub",2*cm,2.5*cm,pmtlen*cm,0,360*deg); //this needs to be changed
	GTubs *photocath_box = new GTubs("photocath",0,2*cm,1*cm,0,360*deg);

	pmt = new G4LogicalVolume(glass_tub,G4Material::GetMaterial("Glass"),"tube_log");
	photocath = new G4LogicalVolume(photocath_box,G4Material::GetMaterial("Al"),"photocath_log");
	
	//place the photo cath in the glass tube

	new G4PVPlacement(0,G4ThreeVector(0,0,-1*pmtlen*cm),photocath,"photocath",pmt,false,0);

	//place the pmt on the box

	G4RotationMatrix* rmx = new G4RotationMatrix();
	rmx->rotateX(-90*deg);
	new G4PVPlacement(rmx,G4ThreeVector((-size_x/2-pmtlen/2)*cm),pmt,"pmt",housing,false,0); //-sizex/2-pmtlen/2

	//set the visible attributes and the surface properties
	SetVisAttributes();
	SetSurfaceProperties();

	SetLogicalVolume(housing);
}

void MDScintillator::VisAttributes()
{
	housing->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
	scint->SetVisAttributes(new G4VisAttributes(G4Colour::White()));
	pmt->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
	photocath->SetVisAttributes(new G4VisAttributes(G4Colour::Brown()));

}

void MDScintillator::SetSurfaceProperties()
{
	G4double ephoton[] = {1.77*eV,2.48*eV,4.13*eV,}; //after some research it seems like the property vector is linearly interpolated according
													  // to the photon energies, ie, the x value is the photon energy, the y val is the 
	const G4int num = sizeof(ephoton)/sizeof(G4double);

	G4double reflectivity[] = {1,1,1};
	G4double efficiency[] = {0,0,0};

	G4MaterialPropertiesTable* housingskin = new G4MaterialPropertiesTable();

	housingskin->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
	housingskin->AddProperty("EFFICIENCY", ephoton, efficiency, num); //transmission efficiency
	
	G4OpticalSurface* OpScintHousingSurface =
	new G4OpticalSurface("HousingSurface",unified,polished,dielectric_metal);

	OpScintHousingSurface->SetMaterialPropertiesTable(housingskin);

	G4double photocath_EFF[]={1.,1.,1}; //Enables 'detection' of photons

	G4double photocath_ReR[]={1.92,1.92,1.92};
	G4double photocath_ImR[]={1.69,1.69,1.69};
	G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
	photocath_mt->AddProperty("EFFICIENCY",ephoton,photocath_EFF,num);
	photocath_mt->AddProperty("REALRINDEX",ephoton,photocath_ReR,num);
	photocath_mt->AddProperty("IMAGINARYRINDEX",ephoton,photocath_ImR,num);
	G4OpticalSurface* photocath_opsurf=
		new G4OpticalSurface("photocath_opsurf",glisur,polished,
		dielectric_metal);
	photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

	new G4LogicalSkinSurface("housing_surf",housing,
                            OpScintHousingSurface);
	new G4LogicalSkinSurface("photcath_surf",photocath,photocath_opsurf);

	//add a glass surface
}

G4Material* MDScintillator::GetScintillatorMaterial()
{	
	G4NistManager* mat = G4NistManager::Instance();
	G4Material *pstyrene = mat->FindOrBuildMaterial("G4_POLYSTYRENE");
	delete mat;

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
	s_mt->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
	pstyrene->SetMaterialPropertiesTable(s_mt);
	return pstyrene
}
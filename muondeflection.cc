/* 

NOTES ON HOW TO FORMAT THE INPUT DOCUMENT

This is a csv style document. On one line, there should just be the variable name, followed by the value (no space).
The next variable should be on the next line and so on.

The other variables can be in any order, but must all be before the start of the position variables
The position variable should have x,y and z coordinates for each one of the modules you want to place.
The following is an example of what an initialization file would look like. The text in parentheses
is just comments and should not be included:

stripXYZ,10,1,1       (xyz dimension of a single strip)
numModules,2          (number of modules above the unknown object. The total number of modules will be 2x this number)
posXYZ,0,0,0          (position coordinates for each slab)
posXYZ,0,0,5

The position coordinates should only be for the modules above the unknown (positive Z). The program will mirror these coordinates for the
modules below (negative Z).

The position should be based on the geant4 coordinate system positions. This means that the exact 3-D center of the module will be located
at the points you specify.

ALL UNITS ARE IN CENTEMETERS

ENSURE THAT THE LONG DIMENSION FOR THE STRIP IS THE X DIMENSION

*/








#include <iostream>
#include <fstream>
#include <cassert>
#include <string>


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UIExecutive.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//#undef G4VIS_USE

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "MDPhysicsList.hh"
#include "MDPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "MDSteppingAction.hh"
#include "MDEventAction.hh"
#include "MDStackingAction.hh"
#include <fstream>
#include "RunNumber.hh"

#include "MDDetectorConstruction.hh"

void ReadFileAndCreateDetectorGeometry(std::ifstream& file,std::ifstream&);

G4RunManager* runManager = new G4RunManager; //create the default run manager

int RunNumber = 0;

int main(int argc, char** argv)
{
	std::ifstream rdrn("run_number.txt");
	int runnum = 0;
	std::string strrun;
	if(rdrn)//exits
	{
		rdrn>>strrun;
		runnum = std::stoi(strrun);
		rdrn.close();
		std::ofstream rdrno("run_number.txt");
		runnum++;
		rdrno<<(runnum);
		rdrno.close();
	}
	else
	{
		//create one
		rdrn.close();
		std::ofstream rdrno("run_number.txt");
		rdrno<<0;
		rdrno.close();
		//runnum = 0;
	}
	RunNumber = runnum;
	std::string flname = "RunNumber_";
	flname = flname + std::to_string(RunNumber) + std::string("_photoninfo.csv");
	std::ofstream ofs(flname);
	ofs.close(); //clear the file
	flname = "RunNumber_";
	flname = flname + std::to_string(RunNumber) + std::string("_runinfo.csv");
	std::ofstream k(flname);
	k.close();
	RunNumber = runnum;

  G4UImanager* UI = G4UImanager::GetUIpointer();//create the ui system


  assert(argc == 4);//ensure that there is one and only one input (config file name)

  //read the file here

  std::ifstream file(argv[2]);
  std::ifstream denseblocksfile(argv[3]);

  //read in the number of events here

  std::vector<std::string> firstline;

  std::string line;
  std::string entry;

  std::getline(file,line);

  std::stringstream strstrm(line);

  while(std::getline(strstrm,entry,','))
  {
    firstline.push_back(entry);
  }

  if(firstline[0] != "numEvents")
  {
	  G4cout<<"error reading file: numEvents not found"<<G4endl;
	  exit(1);
  }

  int numberofevents = std::stoi(firstline[1]);

  //now create the detector geometry

  G4cout<<"hit"<<G4endl;

  ReadFileAndCreateDetectorGeometry(file,denseblocksfile);


  #ifdef G4VIS_USE
    // visualization manager
    G4VisManager* visManager = new G4VisExecutive;

    visManager->Initialize();
  #endif

  //create and set physics lists
  G4VModularPhysicsList* physicsList = new MDPhysicsList();
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);


  // set mandatory user action classMDPrimaryGeneratorAction
  // this is the what the simulation does, ie launching particles etc
  runManager->SetUserAction(new MDPrimaryGeneratorAction());
  runManager->SetUserAction(new MDSteppingAction());
  runManager->SetUserAction(new MDEventAction());
  runManager->SetUserAction(new MDStackingAction());
 // runManager->SetUserAction(new MDTrackingAction());



  // initialize G4 kernel
  runManager->Initialize();


 // G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance(); //create the vis manager

 // UI->ApplyCommand("/run/verbose 1");
  //UI->ApplyCommand("/vis/open OGL 1200x1200-0+0");
 // UI->ApplyCommand("/vis/viewer/create OGLIQt");
  //UI->ApplyCommand("/vis/drawVolume");
  //UI->ApplyCommand("/event/verbose 1");
  //UI->ApplyCommand("/tracking/verbose 1");



#ifdef G4VIS_USE
  UI->ApplyCommand("/control/execute init_vis.mac"); //set up visuialization
#endif


  G4UIExecutive* ui = 0;

//dont use the UI, go ahead an run 10,000 events
  if(std::string(argv[1]) == "-ui")
  {
   ui = new G4UIExecutive(argc, argv);
   ui->SessionStart(); //create the UI
  }
  else if(std::string(argv[1]) == "-run")
  {
   runManager->BeamOn(numberofevents);
  }
    //delete ui;
#ifdef G4VIS_USE
    delete visManager;
#endif

  // job termination
  delete runManager;
  return 0;
}

void ReadFileAndCreateDetectorGeometry(std::ifstream& file, std::ifstream& denseblocks)
{
  std::vector<std::string> fileinfo;
  std::vector<std::vector<std::string>> densefileinfo;
  std::string line;
  std::string entry;
  //storing the vars in any order is is fine as long as they are first

  while(std::getline(file,line))//store the contents of the file delemited by a return or a comma
  {
    std::stringstream strstrm(line);
    while(std::getline(strstrm,entry,','))
    {
      fileinfo.push_back(entry);
    }
  }

  while(std::getline(denseblocks,line))//store the contents of the file delemited by a return or a comma
  {
    std::stringstream strstrm(line);
    std::vector<std::string> bits;
    while(std::getline(strstrm,entry,','))
    {
      bits.push_back(entry);
    }
    densefileinfo.push_back(bits);
  }


  double stripx, stripy, stripz;
  std::vector<G4ThreeVector> positions;
  int nummodules;

  bool done[2] = {};

  G4ThreeVector currentposition;

//here, we parse through the vector that contains the contents of the file
  for(int i = 0; i < fileinfo.size(); ++i)
  {

    if(fileinfo[i] == "stripXYZ") //look for the strip XYZ input parameter
    {
      if(!done[0])
      {
        stripx = std::stoi(fileinfo[i+1]);
        stripy = std::stoi(fileinfo[i+2]);
        stripz = std::stoi(fileinfo[i+3]);

        done[0] = true;
      }
      else
      {
        std::cout<<"stripXYZ is repeated"<<std::endl;
        exit(1);
      }
    }
    else if(fileinfo[i] == "numModules")
    {
      if(!done[1])
      {
        nummodules = std::stoi(fileinfo[i+1]);
        done[1] = true;
      }
      else
      {
        std::cout<<"nummodules is repeated"<<std::endl;
        exit(1);
      }
    }
    else if((((((((((fileinfo[i] == "posXYZ1")||fileinfo[i] == "posXYZ2")||fileinfo[i] == "posXYZ3")||fileinfo[i] == "posXYZ4")||fileinfo[i] == "posXYZ5")||fileinfo[i] == "posXYZ6")||fileinfo[i] == "posXYZ7")||fileinfo[i] == "posXYZ8")||fileinfo[i] == "posXYZ9"))
    {
      currentposition[0] = std::stoi(fileinfo[i+1])*cm;
      currentposition[1] = std::stoi(fileinfo[i+2])*cm;
      currentposition[2] = std::stoi(fileinfo[i+3])*cm;
      positions.push_back(currentposition);
    }
  }

    //check to make sure all vars were loaded

  for(int j = 0; j < 2; ++j)
  {
    if(!done[j])
    {
      std::cout<<j<<std::endl;
      std::cout<<"not all vars loaded"<<std::endl;
      exit(1);
    }
  }

  if (positions.size() != nummodules)
  {
    std::cout<<"number of position inputs does not match the number of modules"<<std::endl;
    exit(1);
  }

	std::vector<G4ThreeVector> densepositions;
	std::vector<G4ThreeVector> densesize;
	std::vector<std::string> densetypes;
	std::vector<std::string> densematerials;

  for(std::vector<std::vector<std::string>>::iterator it = densefileinfo.begin(); it != densefileinfo.end(); ++it)
  {
	  //G4cout<<"hit"<<G4endl;
	  if(((*it).empty()))
		  break;
	  if(((*it)[0].at(0)) == '#')
		  continue;

	  if((*it)[0] == "rectangle")
	  {
		  densematerials.push_back((*it)[1]);
		  double sizex = std::stod((*it)[2]);
		  double sizey = std::stod((*it)[3]);
		  double sizez = std::stod((*it)[4]);
		  densesize.push_back(G4ThreeVector(sizex*cm,sizey*cm,sizez*cm));
		  double posx = std::stod((*it)[5]);
		  double posy = std::stod((*it)[6]);
		  double posz = std::stod((*it)[7]);
		  densepositions.push_back(G4ThreeVector(posx*cm,posy*cm,posz*cm));
		  densetypes.push_back(std::string("rectangle"));
	  }
	  else if((*it)[0] == "cylinder")
	  {
		  densematerials.push_back((*it)[1]);
		  double anglerotated = std::stod((*it)[2]);
		  double outerrad = std::stod((*it)[3]);
		  double height = std::stod((*it)[4]);
		  densesize.push_back(G4ThreeVector(anglerotated*deg,outerrad*cm,height*cm));
		  double posx = std::stod((*it)[5]);
		  double posy = std::stod((*it)[6]);
		  double posz = std::stod((*it)[7]);
		  densepositions.push_back(G4ThreeVector(posx*cm,posy*cm,posz*cm));
		  densetypes.push_back(std::string("cylinder"));
	  }
	  else
	  {
		  G4cout<<"error in reading the blocks file"<<G4endl;
		  exit(1);
	  }
  }

  std::cout<<"vars loaded"<<std::endl;

  assert((stripx/stripy-(int) (stripx/stripy)) == 0); //check if the number of strips is a whole number

  //G4cout<<"hit"<<G4endl;

  runManager->SetUserInitialization(new MDDetectorConstruction(nummodules,G4ThreeVector(stripx*cm,stripy*cm,stripz*cm),positions,densepositions,densesize,densetypes,densematerials));


}

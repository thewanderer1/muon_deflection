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

#include "MDDetectorConstruction.hh"

void ReadFileAndCreateDetectorGeometry(std::ifstream& file);

G4RunManager* runManager = new G4RunManager; //create the default run manager

int main(int argc, char** argv)
{

	std::ofstream ofs("runinfo.csv");
	ofs.close(); //clear the file
	std::ofstream k("photoninfo.csv");
	k.close();

  G4UImanager* UI = G4UImanager::GetUIpointer();//create the ui system


  assert(argc == 2);//ensure that there is one and only one input (config file name)

  //read the file here

  std::ifstream file(argv[1]);

  ReadFileAndCreateDetectorGeometry(file);

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
  //still need to make photons invisible

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

    ui = new G4UIExecutive(argc, argv);
    ui->SessionStart(); //create the UI


    delete ui;
#ifdef G4VIS_USE
    delete visManager;
#endif

  // job termination
  delete runManager;
  return 0;
}

void ReadFileAndCreateDetectorGeometry(std::ifstream& file)
{
  std::vector<std::string> fileinfo;
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

  std::cout<<"vars loaded"<<std::endl;

  assert((stripx/stripy-(int) (stripx/stripy)) == 0); //check if the number of strips is a whole number


  runManager->SetUserInitialization(new MDDetectorConstruction(nummodules,G4ThreeVector(stripx*cm,stripy*cm,stripz*cm),positions));


}

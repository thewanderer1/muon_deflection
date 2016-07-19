
#include <string>

#include <iostream>
#include <fstream>
#include <TMath.h>
#include <TGraph2D.h>
#include <TRandom2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TH1.h>
#include <Math/Functor.h>
#include <TPolyLine3D.h>
#include <Math/Vector3D.h>
#include <Fit/Fitter.h>
#include "LineFitter.h"
#define PI 3.14159265

int Plot2DShapes(const int runnumber)
{
	std::string flname = "RunNumber_";
	flname = flname + std::to_string(runnumber) + std::string("_fitdata.csv");

	ifstream file(flname);

	vector<vector<string>> wholefile;
	std::string line;
	std::string entry;
	bool once = true;
	//storing the vars in any order is is fine as long as they are first
	cout<<"hi"<<endl;

	while(std::getline(file,line))//store the contents of the file delemited by a return or a comma
	{
		vector<string> linebits;

		std::stringstream strstrm(line);
		while(std::getline(strstrm,entry,','))
		{
			linebits.push_back(entry);
		}

		wholefile.push_back(linebits);
	}

	TH2D *values = new TH2D("values","values",50,-250,250,50,-250,250);
	TH2D *count = new TH2D("count","count",50,-250,250,50,-250,250);

	for (std::vector<vector<string>>::iterator i = wholefile.begin(); i != wholefile.end(); ++i)
	{
		int xbin = (int) ((stod((*i)[2]) + 251)/(500./ (double) 50.));
		int ybin = (int) ((stod((*i)[3]) + 251)/(500./ (double) 50.));
		cout<<stod((*i)[3])<<','<<stod((*i)[2])<<endl;

		values->SetBinContent(xbin,ybin,values->GetBinContent(xbin,ybin) + stod((*i)[1]));

		count->SetBinContent(xbin,ybin,count->GetBinContent(xbin,ybin) + 1);
	}

	values->Divide(count);

	values->Draw("surf3");

	file.close();
	return 0;

}
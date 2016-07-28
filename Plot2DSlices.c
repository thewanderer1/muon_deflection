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

int Plot2DSlices(const int runnumber,const vector<double> zvalues)
{
	std::string flname = "RunNumber_";
	flname = flname + std::to_string(runnumber) + std::string("_fitdata.csv");

	TH1D *zhist = new TH1D("zhist","Z frequency",100,-101,101);
	TH2D *extraval = new TH2D("values","remainder",50,-250,250,50,-250,250);
	TH2D *extracount = new TH2D("values","values",50,-250,250,50,-250,250);


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

	int num = zvalues.size();
	vector<TH2D*> values;
	vector<TH2D*> counts;

	for(int i = 0; i < num; ++i)
	{
		values.push_back(new TH2D("values",(std::string("values ")+to_string(zvalues[i])).c_str(),50,-250,250,50,-250,250));
		counts.push_back(new TH2D("count","count",50,-250,250,50,-250,250));
	}


	for (std::vector<vector<string>>::iterator it = wholefile.begin(); it != wholefile.end(); ++it)
	{
		int xbin = (int) ((stod((*it)[2]) + 250)/(500./ (double) 50.)) + 1; // add one more bin to ensure we get all the data
		int ybin = (int) ((stod((*it)[3]) + 250)/(500./ (double) 50.)) + 1;
		cout<<stod((*it)[3])<<','<<stod((*it)[2])<<endl;
		double zval = stod((*it)[4]);
		if((zval > -100 && zval < 100) && stod((*it)[1]) > 0.6)
			zhist->Fill(zval);

		bool found = false;
		
		for(int i = 0; i < num; ++i)
		{
			if((zval > (zvalues[i] - 10) && zval < (zvalues[i] + 10)) && stod((*it)[1]) > 0.6)
			{
				values[i]->SetBinContent(xbin,ybin,values[i]->GetBinContent(xbin,ybin) + stod((*it)[1]));
				counts[i]->SetBinContent(xbin,ybin,counts[i]->GetBinContent(xbin,ybin) + 1);
				found = true;
			}
		}
		if(!found && stod((*it)[1]) > 0.6)
		{
			extraval->SetBinContent(xbin,ybin,extraval->GetBinContent(xbin,ybin) + stod((*it)[1]));
			extracount->SetBinContent(xbin,ybin,extracount->GetBinContent(xbin,ybin) + 1);
		}
	}
	new TCanvas("c100");

	zhist->Draw();

	extraval->Divide(extracount);
	new TCanvas("c200");
	extraval->Draw("surf3");

	for(int i = 0; i < num; i++)
	{
		new TCanvas((std::string("c")+to_string(i)).c_str());
		values[i]->Divide(counts[i]);
		
		values[i]->Draw("surf3");
	}


	return 0;

}


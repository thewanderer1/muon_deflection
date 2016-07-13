//read in the entire file as a vector of vectors - a vector for each line
//loop thru the lines and extract hit info
//if the hits go all the way thru, fit the lines, calulate the pts and angle and store it

//print it to file

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


const double* Fit(TGraph2D *tg);

void Analyze()
{
	std::ifstream ifs("runinfo.csv",std::ifstream::in);
	if(!ifs)
		std::cout<<"error";

	vector<vector<string>> wholefile;
	vector<Event> evtlist;

	std::string line;
	std::string entry;
	bool once = true;
	//storing the vars in any order is is fine as long as they are first
	//cout<<"hi"<<endl;

	while(std::getline(ifs,line))//store the contents of the file delemited by a return or a comma
	{
		vector<string> linebits;

		std::stringstream strstrm(line);
		while(std::getline(strstrm,entry,','))
		{
			linebits.push_back(entry);
		}

		wholefile.push_back(linebits);
	}
	bool reading = true;
	double x,y,z;
	bool seen = false;
	int i1 = 0;
	int i2 = 0;
	TGraph2D *top = new TGraph2D();
	TGraph2D *bot = new TGraph2D();
	for (int i = 0; i < wholefile.size(); ++i)
	{
		x = stoi(wholefile[i][9]) / ((double) 10.);
		y = stoi(wholefile[i][10]) / ((double) 10.);
		z = stoi(wholefile[i][11]) / ((double) 10.);

		if (z == -300)
		{
			bot->SetPoint(i2,x,y,z);

			i1 = 0;
			i2 = 0;
			const double* topparams = Fit(top);
			const double* bottomparams = Fit(bot);

			delete top;
			delete bot;

			top = new TGraph2D();
			bot = new TGraph2D();

			if(!(topparams && bottomparams))
			{
				cout<<"failed!!!"<<endl;
				continue;
				cout<<i<<endl;
				exit(1);
			}

			XYZVector x1(topparams[3],topparams[4],topparams[5]);
			XYZVector u1 = x1.Unit();
			XYZVector x2(bottomparams[3],bottomparams[4],bottomparams[5]);
			XYZVector u2 = x2.Unit();

			double dp = u1.Dot(u2);

			Event e;

			e.val = dp;
			//add closest point here
			evtlist.push_back(e);
		}
		else if(z == 300)
		{
			//reset the loop

			delete top;
			delete bot;

			top = new TGraph2D();
			bot = new TGraph2D();

			i1 = 0;
			i2 = 0;

			top->SetPoint(i1,x,y,z);
			++i1;
		}

		if(z > 0)
		{
			top->SetPoint(i1,x,y,z);
			++i1;
		}
		else
		{
			bot->SetPoint(i2,x,y,z);
			++i2;
		}

	}

	ofstream fitinfo("fitdata.csv");
	for (std::vector<Event>::iterator i = evtlist.begin(); i != evtlist.end(); ++i)
	{
		fitinfo<<i->val<<","<<acos(i->val)  * 180.0 / PI<<endl;
	}
	fitinfo.close();
}

const double* Fit(TGraph2D *tg)
{
	LineFit lf(tg);

	ROOT::Fit::Fitter  fitter;

	#ifdef __CINT__
	   ROOT::Math::Functor fcn(&lf,6,"SumDistance2");

	#else
	   ROOT::Math::Functor fcn(lf,6);
	#endif

    double paramstart[6] = {-10.,-10.,-10.,-10.,-10.,-10.};


   	fitter.SetFCN(fcn,paramstart);

   	for (int i = 0; i < 6; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

   	bool ok = fitter.FitFCN();

   	if (!ok) {
   		
   	   Error("line3Dfit","Line3D Fit failed");
   	  return 0;
   	}

   	const ROOT::Fit::FitResult & result = fitter.Result();

   	std::cout << "Total final distance square " << result.MinFcnValue() << std::endl;
   	result.Print(std::cout);
   	const double * parFit = result.GetParams();
   	return parFit;
}
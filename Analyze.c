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

double* topparams = new double(6);
double* bottomparams = new double(6);



void Fit(TGraph2D *tg, int num);

void Plot(TGraph2D *t1, TGraph2D *t2)
{
	TCanvas *c3 = new TCanvas("c3");
	c3->cd();
	TH2D *h2 = new TH2D("first","points",6000,-300,300,6000,-300,300);
	h2->SetMinimum(-400);
	h2->SetMaximum(400);

	t1->SetHistogram(h2);
	t1->SetMarkerStyle(34);
	t1->SetMarkerSize(1);
	t1->SetMarkerColor(2);
	t1->Draw("P");

	TCanvas *two =new TCanvas("c2");
	two->cd();
	//t2->SetHistogram(h2);
	t2->SetMarkerStyle(34);
	t2->SetMarkerSize(1);
	t2->SetMarkerColor(2);
	t2->Draw("P");
}

void Analyze(const int runnumber)
{	
	TGraph2D *impactpoints = new TGraph2D();
	TH1D *h1 = new TH1D("distr","Distribution of deflection angles",200,0,10);
	TH1D *impactdists = new TH1D("impact dists","Distribution of impact Distances",500,0,5);
	//rebin the last if using different materials
	int impactpointscounter = 0;
	std::string flname = "RunNumber_";
	flname = flname + std::to_string(runnumber) + std::string("_runinfo.csv");
	std::ifstream ifs(flname,std::ifstream::in);
	if(!ifs.is_open())
	{
		cout<<"error: file not found"<<endl;
		return;
	}
	flname = "RunNumber_";
	flname = flname + std::to_string(runnumber) +std::string("_fitdata.csv");
	ofstream fitinfo(flname);
	vector<vector<string>> wholefile;
	vector<Event> evtlist;

	std::string line;
	std::string entry;
	bool once = true;
	//storing the vars in any order is is fine as long as they are first
	cout<<"hi"<<endl;

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
	cout<<"hi2"<<endl;
	bool reading = true;
	double x,y,z;
	bool seen = false;
	int i1 = 0;
	int i2 = 0;
	TGraph2D *top = new TGraph2D();
	TGraph2D *bot = new TGraph2D();
	int currenteventnum;
	int nexteventnum;
	for (int i = 0; i < wholefile.size(); ++i)
	{
		x = stoi(wholefile[i][10]) / ((double) 10.);
		y = stoi(wholefile[i][11]) / ((double) 10.);
		z = stoi(wholefile[i][12]) / ((double) 10.);
		currenteventnum = stoi(wholefile[i][0]);
		if( (i+1) == wholefile.size())
			nexteventnum = -1;
		else
			nexteventnum = stoi(wholefile[i+1][0]);

		if (currenteventnum != nexteventnum && z == -300)
		{
			bot->SetPoint(i2,x,y,z);
			//cout<<x<<','<<y<<','<<z<<endl;
			//cout<<"fit"<<endl;

			i1 = 0;
			i2 = 0;
			Fit(top,1);
			Fit(bot,2);

			/*
			if(!(topparams && bottomparams))
			{
				cout<<"failed!!!"<<endl;
				continue;
				cout<<i<<endl;
				//exit(1);
			}*/

				//algo from here : https://en.wikipedia.org/wiki/Skew_lines#Nearest_Points
			

			XYZVector d1(topparams[3],topparams[4],topparams[5]);

			XYZVector d2(bottomparams[3],bottomparams[4],bottomparams[5]);
			//double dp = d1.Dot(d2);


			XYZVector p1(topparams[0],topparams[1],topparams[2]);
			XYZVector p2(bottomparams[0],bottomparams[1],bottomparams[2]);

			XYZVector n1 = d1.Cross(d2);
			XYZVector n2 = d2.Cross(n1);
			XYZVector n3 = d1.Cross(n1);
			XYZVector c2 = p2 + (((p1-p2).Dot(n3))/(d2.Dot(n3)))*(d2);
			XYZVector c1 = p1 + (((p2-p1).Dot(n2))/(d1.Dot(n2)))*(d1);

			XYZVector impactpt((c1.x()+c2.x())/2,(c1.y()+c2.y())/2,(c1.z()+c2.z())/2);
			XYZVector u1 = d1.Unit();
			XYZVector u2 = d2.Unit();

			double dp = fabs(u1.Dot(u2));

			//fitinfo<<abs(dp)<<','<<acos(abs(dp)) * 180 / PI<<','<<(xd1+xd2)/2<<','<<(yd1+yd2)/2<<','<<(zd1+zd2)/2<<','<<endl;

			/*if(abs(dp) < 0.9)
			{
				cout<<"error!!!"<<endl;
				cout<<topparams[3]<<','<<topparams[4]<<','<<topparams[5]<<endl;
				cout<<bottomparams[3]<<','<<bottomparams[4]<<','<<bottomparams[5]<<endl;
				//cout<<x1.x()<<','<<x1.y()<<','<<x1.z()<<endl;
				//cout<<x2.x()<<','<<x2.y()<<','<<x2.z()<<endl;
				//Plot(top,bot);
				return;
			}*/


			//Event e(dp,impactpt);
			//add closest point here
			//evtlist.push_back(e);

			if(((impactpt.z() < 100 && impactpt.z() > -100) && (impactpt.x() < 250 && impactpt.x() > -250))&&(impactpt.y() < 250 && impactpt.y() > -250))
			{
				if((sqrt((impactpt-c1).Mag2())*2) <0.5)// && (acos(dp)*180/PI) > 2.5)
				{
					impactpoints->SetPoint(impactpointscounter,impactpt.x(),impactpt.y(),impactpt.z());
					impactpointscounter++;
					h1->Fill(acos(dp)*180/PI);
				}
			}

			fitinfo<<dp<<','<<acos(dp)*180/PI<<','<<impactpt.x()<<','<<impactpt.y()<<','<<impactpt.z()<<','<<c1.x()<<','<<c1.y()<<','<<c1.z()<<','<<c2.x()<<','<<c2.y()<<','<<c2.z()<<','<<sqrt(impactpt.Mag2())*2<<endl;
			
			impactdists->Fill(sqrt((impactpt-c1).Mag2())*2);

			delete top;
			delete bot;

			top = new TGraph2D();
			bot = new TGraph2D();
		}
		else if(currenteventnum != nexteventnum)
		{
			//reset the loop
			//cout<<x<<','<<y<<','<<z<<endl;
			//cout<<"reset"<<endl;

			delete top;
			delete bot;

			top = new TGraph2D();
			bot = new TGraph2D();

			i1 = 0;
			i2 = 0;
		}
		else if(z > 0)
		{
			top->SetPoint(i1,x,y,z);
			//cout<<x<<','<<y<<','<<z<<endl;
			++i1;
		}
		else if(z < 0)
		{
			bot->SetPoint(i2,x,y,z);
			//cout<<x<<','<<y<<','<<z<<endl;
			++i2;
		}

	}


	
	impactpoints->SetMarkerStyle(34);
	impactpoints->SetMarkerSize(1);
	impactpoints->SetMarkerColor(2);
	impactpoints->Draw("P");

	fitinfo.close();
	TCanvas *c3 = new TCanvas("c3");
	h1->Draw();
	TCanvas *c4 = new TCanvas("c4");
	impactdists->Draw();
}

void Fit(TGraph2D *tg, int num)
{
	LineFit lf(tg);

	ROOT::Fit::Fitter  fitter;

	#ifdef __CINT__
	   ROOT::Math::Functor fcn(&lf,6,"SumDistance2");

	#else
	   ROOT::Math::Functor fcn(lf,6);
	#endif

    

    double* x,*y,*z;
    x = tg->GetX();
    y = tg->GetY();
    z = tg->GetZ();

    int lastpt = tg->GetN() - 1;

    XYZVector firstpt(x[0],y[0],z[0]);
    
    XYZVector secondpt(x[lastpt],y[lastpt],z[lastpt]);

    XYZVector dir = secondpt - firstpt; 

    double paramstart[6] = {secondpt.x(),secondpt.y(),secondpt.z(),dir.x(),dir.y(),dir.z()};

   	fitter.SetFCN(fcn,paramstart);

   	for (int i = 0; i < 6; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

   	bool ok = fitter.FitFCN();

   	if (!ok) {
   		
   	   //Error("line3Dfit","Line3D Fit failed");
   	  cout<<"trying again"<<endl;

   	  XYZVector dir2(-dir.x(),-dir.y(),-dir.z());

   	  double paramstart2[6] = {secondpt.x(),secondpt.y(),secondpt.z(),dir2.x(),dir2.y(),dir2.z()};

   	  fitter.SetFCN(fcn,paramstart2);

   	  for (int i = 0; i < 6; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

   	  bool ok2 = fitter.FitFCN();
   	  
   		if(!ok2)
   		{
   			cout<<"fialed"<<endl;
   			cout<<firstpt.x()<<','<<firstpt.y()<<','<<firstpt.z()<<endl;
   			cout<<secondpt.x()<<','<<secondpt.y()<<','<<secondpt.z()<<endl;
   			exit(1);
   		}

   	}

   	const ROOT::Fit::FitResult & result = fitter.Result();

   	std::cout << "Total final distance square " << result.MinFcnValue() << std::endl;
   	if(result.MinFcnValue() > 12)
   	{
   		cout<<"error, high min val"<<endl;
   		exit(1);
   	}
   	result.Print(std::cout);
   	const double * parFit = result.GetParams();

   	if(num == 1)
   	{
   		for(int i = 0; i<6; i++)
   		{
   			topparams[i] = parFit[i];
   		}
   	}
   	else
   	{
   		for(int i = 0; i<6; i++)
   		{
   			bottomparams[i] = parFit[i];
   		}
   	}

}
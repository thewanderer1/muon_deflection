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



int DrawTest()
{

	std::ifstream ifs("runinfo.csv",std::ifstream::in);
	if(!ifs)
		std::cout<<"error";

/*	TCanvas* c1 = new TCanvas("c2");
   Double_t w = gPad->GetWw()*gPad->GetAbsWNDC();
   Double_t h = gPad->GetWh()*gPad->GetAbsHNDC();
   Double_t xmin = -150;
   Double_t xmax =  150;
   Double_t ymin = -150;
   Double_t ymax = ((xmax-xmin)*h/w)-150;
   c1->SetFixedAspectRatio();
   c1->Range(xmin,ymin,xmax,ymax); */

	TGraph2D *dt = new TGraph2D();
	TGraph2D *dt2 = new TGraph2D();

	int i1 = 0;
	int i2 = 0;

	int col = 1;

	double x,y,z;


	std::string line;
	std::string entry;
	bool once = true;
	//storing the vars in any order is is fine as long as they are first
	//cout<<"hi"<<endl;

	while(std::getline(ifs,line))//store the contents of the file delemited by a return or a comma
	{
		std::cout<<"hi"<<std::endl;
	  std::stringstream strstrm(line);
	  while(std::getline(strstrm,entry,','))
	  {
	  	if(col == 10)
	  	{
	  		x = std::stoi(entry)/(double) 10.;
	  		cout<<x<<endl;
	  	}
	  	else if(col == 11)
	  	{
	  		y = std::stoi(entry)/(double) 10.;
			cout<<y<<endl;
	  	}
	  	else if(col == 12)
	  	{
	  		z = std::stoi(entry)/(double) 10.;
			cout<<z<<endl;
	  		
	  	}

	  	col++;
	  }
	  cout<<"point1"<<endl;
	  
	  if((!once) && z == 300)
	  	break;
	if(z == 300)
	  			once = false;
	  if(z > 0)
	  {
	  	cout<<i1<<endl;
	  	dt->SetPoint(i1,x,y,z);
	  	i1++;
	  }
	  else
	  {
	  	cout<<i2<<endl;
	  	dt2->SetPoint(i2,x,y,z);
	  	i2++;
	  }
	  col = 1;
	  cout<<"point2"<<endl;

	}
	TH2D *h2 = new TH2D("first","points",6000,-300,300,6000,-300,300);
	h2->SetMinimum(-400);
	h2->SetMaximum(400);
	//h2->GetZaxis()->SetRangeUser(-400,400);
	dt->SetHistogram(h2);
	dt->SetMarkerStyle(34);
	dt->SetMarkerSize(1);
	dt->SetMarkerColor(2);
	dt2->SetHistogram(h2);
	dt2->SetMarkerStyle(34);
	dt2->SetMarkerSize(1);
	dt2->SetMarkerColor(2);

	//dt->Draw("P");

	dt2->Draw("P");
	
	
	//return 0;
	LineFit lf(dt);
	LineFit lf2(dt2);

	ROOT::Fit::Fitter  fitter;

	#ifdef __CINT__
	   ROOT::Math::Functor fcn(&lf,6,"SumDistance2");
	   ROOT::Math::Functor fcn2(&lf2,6,"SumDistance22");
	#else
	   ROOT::Math::Functor fcn(lf,6);
	   ROOT::Math::Functor fcn2(lf2,6);
	#endif

	double paramstart[6] = {-10.,-10.,-10.,-10.,-10.,-10.};

	int n = 10000;
/*

	fitter.SetFCN(fcn,paramstart);

	for (int i = 0; i < 4; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

	bool ok = fitter.FitFCN();

	if (!ok) {
	   Error("line3Dfit","Line3D Fit failed");
	   return 1;
	}

	const ROOT::Fit::FitResult & result = fitter.Result();

	std::cout << "Total final distance square " << result.MinFcnValue() << std::endl;
	result.Print(std::cout);

	//dt->Draw("P");
	dt2->Draw("P");

	//gr->Draw("p0");

	// get fit parameters
	
	const double * parFit = result.GetParams();

	
	double t0 = 50;
	double dtl = 250;
	TPolyLine3D *l = new TPolyLine3D(n);
	for (int i = 0; i <n;++i) {
	   double t = t0+ dtl*i/n;
	   double x,y,z;
	   line1(t,parFit,x,y,z);
	   //cout<<x<<','<<y<<','<<z<<endl;
	   l->SetPoint(i,x,y,z);
	}
	l->SetLineColor(kRed);
	//h2->Draw();
	//l->Draw("same");
	*/

	fitter.SetFCN(fcn2,paramstart);

	for (int i = 0; i < 4; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

	bool ok1 = fitter.FitFCN();

	if (!ok1) {
	   Error("line3Dfit","Line3D Fit failed");
	   return 1;
	}

	const ROOT::Fit::FitResult & result2 = fitter.Result();
	parFit = result2.GetParams();

	std::cout << "Total final distance square " << result2.MinFcnValue() << std::endl;
	result2.Print(std::cout);
	t0 = -200;
	dtl = 200;
	TPolyLine3D *l2 = new TPolyLine3D(n);
	for (int i = 0; i <n;++i) {
	   double t = t0+ dtl*i/n;
	   double x,y,z;
	   line1(t,parFit,x,y,z);
	   //cout<<x<<','<<y<<','<<z<<endl;
	   l2->SetPoint(i,x,y,z);
	}
	l2->SetLineColor(kRed);
	l2->Draw("same");


	


/*	TBox *tb = new TBox(10,10,20,20);
	tb->SetFillColor(2);
	tb->Draw("same");
	return 0;*/


	return 0;
}

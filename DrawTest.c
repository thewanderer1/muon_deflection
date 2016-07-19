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

	std::ifstream ifs("testsim.csv",std::ifstream::in);
	if(!ifs)
		std::cout<<"error";

	/*TCanvas* c1 = new TCanvas("c1");/*
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
	  	if(col == 11)
	  	{
	  		x = std::stoi(entry)/(double) 10.;
	  		cout<<x<<endl;
	  	}
	  	else if(col == 12)
	  	{
	  		y = std::stoi(entry)/(double) 10.;
			cout<<y<<endl;
	  	}
	  	else if(col == 13)
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

	//dt2->Draw("P");
	
	
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

	//double paramstart[6] = {-10.,-10.,-10.,-10.,-10.,-10.};

	double* x1,*y1,*z1;
	x1 = dt->GetX();
	y1 = dt->GetY();
	z1 = dt->GetZ();

	int lastpt = dt->GetN() - 1;

	XYZVector firstpt(x1[0],y1[0],z1[0]);
	
	XYZVector secondpt(x1[lastpt],y1[lastpt],z1[lastpt]);

	XYZVector dir = secondpt - firstpt; 

	double paramstart[6] = {secondpt.x(),secondpt.y(),secondpt.z(),dir.x(),dir.y(),dir.z()};

	int n = 10000;



	fitter.SetFCN(fcn,paramstart);

	for (int i = 0; i < 6; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

	bool ok = fitter.FitFCN();

	if (!ok) {
	   Error("line3Dfit","Line3D Fit failed");
	   return 1;
	}

	const ROOT::Fit::FitResult & result = fitter.Result();

	std::cout << "Total final distance square " << result.MinFcnValue() << std::endl;
	result.Print(std::cout);

	dt->Draw("P");
	//dt2->DrawClone("P same");
	//c1->Modified();
	//c1->Update();

	//gr->Draw("p0");

	// get fit parameters
	
	double * parFit1  = new double(6);
	const double* fitparms = result.GetParams();

	for (int i = 0; i < 6; ++i)
	{
		parFit1[i] = fitparms[i];
	}

	
	double t0 = 0;
	double dtl = -250;
	TPolyLine3D *l = new TPolyLine3D(n);
	for (int i = 0; i <n;++i) {
	   double t = t0+ dtl*i/n;
	   double x,y,z;
	   line1(t,parFit1,x,y,z);
	   //cout<<x<<','<<y<<','<<z<<endl;
	   l->SetPoint(i,x,y,z);
	}
	l->SetLineColor(kRed);
	//h2->Draw();
	l->Draw("same");

	double* x2,*y2,*z2;
	x2 = dt2->GetX();
	y2= dt2->GetY();
	z2 = dt2->GetZ();

	int lastpt2 = dt2->GetN() - 1;

	XYZVector firstpt2(x2[0],y2[0],z2[0]);
	
	XYZVector secondpt2(x2[lastpt],y2[lastpt],z2[lastpt]);

	XYZVector dir2 = secondpt2 - firstpt2; 

	double paramstart2[6] = {secondpt2.x(),secondpt2.y(),secondpt2.z(),dir2.x(),dir2.y(),dir2.z()};

	

	fitter.SetFCN(fcn2,paramstart2);

	for (int i = 0; i < 6; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);

	bool ok1 = fitter.FitFCN();

	if (!ok1) {
	   Error("line3Dfit","Line3D Fit failed");
	   return 1;
	}

	const ROOT::Fit::FitResult & result2 = fitter.Result();
	double * parFit2  = new double(6);
	fitparms = result2.GetParams();

	for (int i = 0; i < 6; ++i)
	{
		parFit2[i] = fitparms[i];
	}

	std::cout << "Total final distance square " << result2.MinFcnValue() << std::endl;
	result2.Print(std::cout);
	t0 = -200;
	dtl = 200;
	TPolyLine3D *l2 = new TPolyLine3D(n);
	for (int i = 0; i <n;++i) {
	   double t = t0+ dtl*i/n;
	   double x,y,z;
	   line1(t,parFit2,x,y,z);
	   //cout<<x<<','<<y<<','<<z<<endl;
	   l2->SetPoint(i,x,y,z);
	}
	l2->SetLineColor(kRed);
	//l2->Draw("same");
/*
	ROOT::Fit::Fitter  fitter2;

	ImpactPointFit ipf(parFit1,parFit2);

	#ifdef __CINT__
	   ROOT::Math::Functor ipffcn(&ifp,2,"ImpactPointFit");
	#else
	   ROOT::Math::Functor ipffcn(ipf,2);
	#endif	

	double sparams[2] = {-100,100};

	fitter2.SetFCN(ipffcn,sparams);

	for (int i = 0; i < 2; ++i) fitter2.Config().ParSettings(i).SetStepSize(0.01);

	bool fin = fitter2.FitFCN();

	if (!fin)
	{
		cout<<"failed"<<endl;
		return 1;
	}*/
/*
	const ROOT::Fit::FitResult & resultipf = fitter2.Result();

	std::cout << "Total final distance square " << resultipf.MinFcnValue() << std::endl;
	resultipf.Print(std::cout);

	const double *ipfparams = resultipf.GetParams();

//	cout<<ipfparams[0]<<','<<ipfparams[1]<<endl;
*/
/*	TBox *tb = new TBox(10,10,20,20);
	tb->SetFillColor(2);
	tb->Draw("same");
	return 0;*/

	TPolyMarker3D *tpd = new TPolyMarker3D();
	//double x,y,z;
	//line1(ipfparams[0],parFit1,x,y,z);
//	tpd->SetPoint(0,x,y,z);
//	line1(ipfparams[1],parFit2,x,y,z);
	//tpd->SetPoint(1,x,y,z);

	//cout<<x<<','<<y<<','<<z<<','<<endl;

	XYZVector p1(parFit1[0],parFit1[1],parFit1[2]);
	XYZVector p2(parFit2[0],parFit2[1],parFit2[2]);

	XYZVector d1(parFit1[3],parFit1[4],parFit1[5]);
	XYZVector d2(parFit2[3],parFit2[4],parFit2[5]);

	XYZVector n1 = d1.Cross(d2);
	XYZVector n2 = d2.Cross(n1);
	XYZVector n3 = d1.Cross(n1);
	XYZVector c2 = p2 + (((p1-p2).Dot(n3))/(d2.Dot(n3)))*(d2);
	XYZVector c1 = p1 + (((p2-p1).Dot(n2))/(d1.Dot(n2)))*(d1);
	
	cout<<c1.x()<<','<<c1.y()<<','<<c1.z()<<','<<endl;
	cout<<c2.x()<<','<<c2.y()<<','<<c2.z()<<','<<endl;

	XYZVector impactpt((c1.x()+c2.x())/2,(c1.y()+c2.y())/2,(c1.z()+c2.z())/2);
	cout<<impactpt.x()<<','<<impactpt.y()<<','<<impactpt.z()<<','<<endl;

	double ax = parFit1[0];
	double bx = parFit1[3];
	double ay = parFit1[1];
	double by = parFit1[4];

	double c = parFit1[2];
	double d = parFit1[5];

	double px = parFit2[0];
	double qx = parFit2[3];
	double py = parFit2[1];
	double qy = parFit2[4];

	double n = parFit2[2];
	double m = parFit2[5];


	double xzcoeffsvals[4] = {parFit1[5],parFit2[5],-parFit1[3],-parFit2[3]};
	double xzsolsvals[2] = {parFit1[5]*parFit1[0] - parFit1[3]*parFit1[2],parFit2[5]*parFit2[0] - parFit2[3]*parFit2[2]};
	TMatrixD xzcoeffs(2,2,xzcoeffsvals,0);
	TMatrixD xzsols(2,1,xzsolsvals,0);

	xzcoeffs = xzcoeffs.Invert();

	xzcoeffs =* xzsolsvals;

	double *solns = xzcoeffs.GetMatrixArray();

	cout<<solns[1]<<','<<solns[2]<<endl;

	return 0;
}

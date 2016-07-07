#include <string>

#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TGraph2D.h>
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

	int i = 0;

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
	  if((!once) && z == 300)
	  	break;
	if(z == 300)
	  			once = false;
	  dt->SetPoint(i,x,y,z);
	  i++;
	  col = 1;

	}

	TH2D *h2 = new TH2D("first","points",6000,-300,300,6000,-300,300);
	dt->SetHistogram(h2);
	dt->SetMarkerStyle(34);
	dt->SetMarkerSize(3);
	dt->SetMarkerColor(2);
	
	dt->Draw("P");



/*	TBox *tb = new TBox(10,10,20,20);
	tb->SetFillColor(2);
	tb->Draw("same");
	return 0;*/


	return 0;
}

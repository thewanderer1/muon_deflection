#include <fstream>
#include <string>
#include <vector>

struct Point{
	Point(double ix, double iy, double iz): x(ix), y(iy), z(iz)
	{}
	double x;
	double y;
	double z;

};


TGraph *xz = new TGraph();
TGraph *yz = new TGraph();
  TGraph *xz2 = new TGraph();
  TGraph *yz2 = new TGraph();

void ReadFileAndCreateGeometry(std::ifstream& file,TCanvas* c1, TCanvas* c2)
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
    int numstrips;
  std::vector<Point> positions;
  int nummodules;

  bool done[2] = {};

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
      Point p(std::stoi(fileinfo[i+1]),std::stoi(fileinfo[i+2]),std::stoi(fileinfo[i+3]));
      positions.push_back(p);
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

  numstrips = stripx/stripz;
  assert(numstrips % 2 == 0);

  if(numstrips % 2 == 0)
  {
  	//this means that there are an even number of modules
  	//draw the bottom module
  	for(std::vector<Point>::iterator it = positions.begin(); it != positions.end(); ++it)
  	{
  		//draw the positive module
  		double topx,topy,bottomx,bottomy;
/*
  		topy = it->z;
  		bottomy = it->z - stripz;
  		topx = it->x - ((double) stripx)/2.;
  		bottomx = it->x + ((double) stripx)/2.;

  		TBox *side = new TBox(topx,topy,bottomx,bottomy);

  		topy = -1*it->z;
  		bottomy = -1*(it->z) - stripz;
  		topx = it->x - ((double) stripx)/2.;
  		bottomx = it->x + ((double) stripx)/2.;

  		TBox *side2 = new TBox(topx,topy,bottomx,bottomy);
  		//TBox *side = new TBox();
  		//side->SetBBoxCenter(TPoint(it->x,it->z - stripz/2.));
  		side->SetFillColor(0);
  		side->SetLineColor(1);
  		side->SetLineWidth(1);
  		side->SetLineStyle(1);
  		side->Draw("l");

  		side2->SetFillColor(0);
  		side2->SetLineColor(1);
  		side2->SetLineWidth(1);
  		side2->SetLineStyle(1);
  		side2->Draw("l");
*/
  		//cout<<topx<<","<<topy<<","<<bottomx<<","<<bottomy<<","<<endl;x
  		c1->cd();
  		for(int i = 0; i < numstrips; i++)
  		{
  			int index = i - numstrips/2;
  			topy = it->z + stripz;
  			bottomy = it->z;
  			topx = index*stripy+it->x;
  			bottomx = (index + 1)*stripy + it->x;
  			TBox *top = new TBox(topx,topy,bottomx,bottomy);
  			cout<<topx<<","<<topy<<","<<bottomx<<","<<bottomy<<","<<endl;


  			bool filled = false;

  			for(int i = 0; i < xz->GetN(); i++)
  			{
  				double x,y;
  				xz->GetPoint(i,x,y);
  				if(bottomy == y && (bottomx-(stripy/2.)) == x)
  					filled = true;
  			}

  			if(filled)
  				top->SetFillColor(2);
  			else
  				top->SetFillColor(0);
  			top->SetLineColor(1);
  			top->SetLineWidth(1);
  			top->SetLineStyle(1);
  			top->Draw("l");

  			topy = -1*(it->z) + stripz;
  			bottomy = -1*(it->z);
  			TBox *top2 = new TBox(topx,topy,bottomx,bottomy);

  			bool filled2 = false;

  			for(int i = 0; i < xz->GetN(); i++)
  			{
  				double x,y;
  				xz->GetPoint(i,x,y);
  				if(bottomy == y && (bottomx-(stripy/2.)) == x)
  					filled2 = true;
  			}

  			if(filled2)
  				top2->SetFillColor(2);
  			else
  				top2->SetFillColor(0);
  			top2->SetLineColor(1);
  			top2->SetLineWidth(1);
  			top2->SetLineStyle(1);
  			top2->Draw("l");

  		}
  		c2->cd();
  		for(int i = 0; i < numstrips; i++)
  		{
  			int index = i - numstrips/2;
  			topy = it->z - stripz;
  			bottomy = it->z;
  			topx = index*stripy+it->x;
  			bottomx = (index + 1)*stripy + it->x;
  			TBox *top = new TBox(topx,topy,bottomx,bottomy);
  			cout<<topx<<","<<topy<<","<<bottomx<<","<<bottomy<<","<<endl;
  			bool filled = false;

  			for(int i = 0; i < yz->GetN(); i++)
  			{
  				double x,y;
  				yz->GetPoint(i,x,y);
  				if(bottomy == y && (bottomx-(stripy/2.)) == x)
  					filled = true;
  			}

  			if(filled)
  				top->SetFillColor(2);
  			else
  				top->SetFillColor(0);
  			top->SetLineColor(1);
  			top->SetLineWidth(1);
  			top->SetLineStyle(1);
  			top->Draw("l");

  			topy = -1*(it->z) - stripz;
  			bottomy = -1*(it->z);
  			TBox *top2 = new TBox(topx,topy,bottomx,bottomy);

  			bool filled2 = false;

  			for(int i = 0; i < yz->GetN(); i++)
  			{
  				double x,y;
  				yz->GetPoint(i,x,y);
  				if(bottomy == y && (bottomx-(stripy/2.)) == x)
  					filled2 = true;
  			}

  			if(filled2)
  				top2->SetFillColor(2);
  			else
  				top2->SetFillColor(0);
  			top2->SetLineColor(1);
  			top2->SetLineWidth(1);
  			top2->SetLineStyle(1);
  			top2->Draw("l");

  		}
  	}
  }
}


int CreateGeom(const char* filename)
{
	//first read and draw the block positions
	//this is the z/x plane

	ifstream f(filename);

	
	TCanvas *c1 = new TCanvas("c1");


	TH1F *base = new TH1F("base","X-Z Projection of the Detector Geometry",100,-270,270);
	base->GetYaxis()->SetRangeUser(-320,320);
	TH1F *base2 = new TH1F("base2","Y-Z Projection of the Detector Geometry",100,-270,270);
	base2->GetYaxis()->SetRangeUser(-320,320);

	
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


	int i = 0;
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
			//cout<<y<<endl;
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
  if(z < 0)
  {
    xz2->SetPoint(i2,x,z);
    yz2->SetPoint(i2,y,z);
    i2++;
  }
  else{
	  xz->SetPoint(i,x,z);
	  yz->SetPoint(i,y,z);
    i++;
  }
	  //i++;
	  col = 1;

	}
  xz2->SetHistogram(base);
  yz2->SetHistogram(base);

  xz2->SetHistogram(base);
  xz2->SetMarkerStyle(34);
  xz2->SetMarkerSize(1);
  xz2->SetMarkerColor(4);

	base->GetYaxis()->SetTitle("z axis");
	base->GetXaxis()->SetTitle("x axis");
	base->Draw();
	xz->SetHistogram(base);
	xz->SetMarkerStyle(34);
	xz->SetMarkerSize(1);
	xz->SetMarkerColor(4);
	
	xz->Draw("P");
  xz2->Draw("PSAME");



	TCanvas *c2 = new TCanvas("c2");

	base2->GetYaxis()->SetTitle("z axis");
	base2->GetXaxis()->SetTitle("y axis");
	base2->Draw();
	yz->SetHistogram(base);
	yz->SetMarkerStyle(34);
	yz->SetMarkerSize(1);
	yz->SetMarkerColor(4);

  yz2->SetHistogram(base);
  yz2->SetMarkerStyle(34);
  yz2->SetMarkerSize(1);
  yz2->SetMarkerColor(4);
	
	yz->Draw("P");
  yz2->Draw("P");
	
	ReadFileAndCreateGeometry(f,c1,c2);


  TF1 *f1=new TF1("f1","x++1",-300,300);
  TF1 *f2=new TF1("f2","x++1",-300,300);
  TF1 *f3=new TF1("f3","x++1",-300,300);
  TF1 *f4=new TF1("f4","x++1",-300,300);

  xz->Fit(f1);
  xz2->Fit(f2);
  yz->Fit(f3);
  yz2->Fit(f4);

  
	/*
	TGraph *base = new TGraph(1000);
	base->SetPoint(1,200,0);
	base->SetPoint(1,-200,0);
	
	base->Draw();
	base->GetYaxis()->SetRangeUser(-200,200);
	base->GetXaxis()->SetRangeUser(-200,200);
	base->Draw();
	*/




	return 0;
}
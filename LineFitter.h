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

using namespace ROOT::Math;

void line1(double t, const double *p, double &x, double &y, double &z) {
   // a parameteric line is define from 6 parameters but 4 are independent
   // x0,y0,z0,z1,y1,z1 which are the coordinates of two points on the line
   // can choose z0 = 0 if line not parallel to x-y plane and z1 = 1;
   x = p[0] + p[3]*t;
   y = p[1] + p[4]*t;
   z = p[2] + p[5]*t;
}


struct ImpactPointFit{
   const double* linea;
   const double* lineb;

   ImpactPointFit(const double* a, const double *b): lineb(b), linea(a)
   {}

   double distance(double t1, double t2)
   {
      double x1,y1,z1,x2,y2,z2;
      line1(t1,linea,x1,y1,z1);
      line1(t2,lineb,x2,y2,z2);
      return sqrt((x1-x2)*(x1-x2)+(y2-y1)*(y2-y1)+(z1-z2)*(z1-z2));
   }

   double operator()(const double* pars)
   {
      return distance(pars[0],pars[1]);
   }
};

struct Event{
      double val; //in degrees
      XYZVector closestpt;
      Event(double df, const XYZVector& xz): val(df), closestpt(xz)
      {}
      Event()
      {}
};


struct LineFit{
	TGraph2D * fGraph;
	bool first;

   LineFit(TGraph2D * g) : fGraph(g), first(true) {}

   const double* parameters;

   // implementation of the function to be minimized
   double operator() (const double * par) {
   	  parameters = par;
      assert(fGraph    != 0);
      double * x = fGraph->GetX();
      double * y = fGraph->GetY();
      double * z = fGraph->GetZ();
      int npoints = fGraph->GetN();
      double sum = 0;
      for (int i  = 0; i < npoints; ++i) {
         double d = distance2(x[i],y[i],z[i]);
         sum += d;
#ifdef DEBUG
         if (first) std::cout << "point " << i << "\t"
            << x[i] << "\t"
            << y[i] << "\t"
            << z[i] << "\t"
            << std::sqrt(d) << std::endl;
#endif
      }
      if (first)
         std::cout << "Total Initial distance square = " << sum << std::endl;
      first = false;
      return sum;
   }

   double distance2(double x,double y,double z)
   {

   		XYZVector xp(x,y,z);
   	   XYZVector x0(parameters[0],parameters[1],parameters[2]);
   	   XYZVector x1(parameters[3], parameters[4],parameters[5]);
   	   XYZVector u = x1.Unit();
   	   double d2 = ((xp-x0).Cross(u)).Mag2();
   	   return d2;
   }
};

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include<unistd.h>
#include<cstdio>

using namespace std;


#include "random.h"
int seed[4];
Random rnd;

bool print=false; // mi serve per stampare tutta la media a blocchi della soluzione finale 


class Psi {

    public:

        Psi(double s = 1., double em = 1.) {
            sigma = s;
            mu = em;
            h = 1.;
            m = 1.;
        }
        
        double Eval(double x){
            return exp( - pow((x - mu), 2)/(2*sigma*sigma) ) + exp( - pow((x + mu), 2 )/(2*sigma*sigma) );
        }

        double Evalmod2(double x){
            return pow( (exp( - pow((x - mu), 2)/(2*sigma*sigma) ) + exp( - pow((x + mu), 2 )/(2*sigma*sigma) ) )  , 2);
        }

        double EvalIntegrand(double x){
            return h*h/(2*m*pow(sigma,4)) * (sigma*sigma - mu*mu - x*x + 2*x*mu*tanh(x*mu/(sigma*sigma))) +
                (pow(x,4) - 5./2.*x*x);  
        }

    public:
        double sigma, mu;
        double h; 
        double m;
       

};


class Ints{

  public:

    Ints(double I, double error){
      m_I=I;
      m_errorI=error;
    };
    
    double m_I; 
    double m_errorI;

};







//parameters, observables
const int m_props=1000;
int n_props, ie; //in questo programma faccio la stima solo della E

double walker[m_props];

// averages
double blk_av[m_props], blk_norm, accepted, attempted,acceptedSA,attemptedSA;
double glob_av[m_props], glob_av2[m_props];
double  stima_e;
double err_e;
double e_def;

// simulation
double temp;
double tempINI,sigmaINI,muINI;
int nstepSA, nblk,nstepT,nstepINT;
double x=1; // variabile che estraggo per il calcolo dell' integrale



double delta,deltaSA;
double beta;
const int wd=12;
vector<Ints> myI;

//pigreco
const double pi=3.1415927;

//functions
void Input(void);
void Reset(int);
void Accumulate(void);
void Averages(int);
void Move1(Psi &f); //qui usero Metropolis per il calcolo dell'integrale
void Move2(Psi &f); //qui usero Metropolis per il SA
void Measure(Psi &f);
double Error(double,double,int);
Ints Integral(double,double);






void Input(void)
{
  ifstream ReadInput, Primes, Seed;

  

//Read seed for random numbers
  int p1, p2;
  Primes.open("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

  Seed.open("seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> tempINI;
  ReadInput >> sigmaINI;
  ReadInput >> muINI;
  ReadInput >> delta;
  ReadInput >> deltaSA;
  ReadInput >> nblk;
  ReadInput >> nstepINT;
  ReadInput >> nstepSA;
  ReadInput >> nstepT;

  cout<<"INPUT"<<endl;
  cout << tempINI<<endl;
  cout << sigmaINI<<endl;
  cout << muINI<<endl;
  cout << delta<<endl;
  cout << deltaSA<<endl;
  cout << nblk<<endl;
  cout << nstepINT<<endl;
  cout << nstepSA<<endl;
  cout << nstepT<<endl;
  cout << "..................."<<endl;


  ReadInput.close();

  n_props=1;
  ie=0;

  return;
}


double Error(double sum, double sum2, int iblk)
{
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}





void Move1(Psi &f)
{
    ofstream isto;
    double xold,xnew;
    xold=x;
    xnew= xold+delta*(rnd.Rannyu()-0.5);
    
    //Metropolis test

      double A = min(1. , f.Evalmod2(xnew)/f.Evalmod2(xold));

      double r = rnd.Rannyu();

      if( r < A ){
        //Update
        x=xnew;
        accepted = accepted + 1.0;
      } else {
        x=xold;
      }
      attempted = attempted + 1.0;
    
      
  
  return;
}

// mi calcola l' integrale con Metropolis e l' errore
Ints Integral(Psi &f){
    for(int iblk=1; iblk <= nblk; iblk++) {         

        Reset(iblk);   //Reset block averages
        for(int istep=1; istep <=nstepINT; istep++){ 
                    
            Move1(f);
            Measure(f);
            Accumulate();    //Update block averages
                
        }
        Averages(iblk);  
    }

    Ints I(e_def,err_e);

    return I;



};



void Move2(Psi &f)
{
    double sigmaold,sigmanew,muold,munew;

    muold=f.mu;
    sigmaold=f.sigma;
    
    sigmanew= sigmaold+deltaSA*(rnd.Rannyu()-0.5);
    munew= muold+deltaSA*(rnd.Rannyu()-0.5);

    Psi psi_aux(sigmanew,munew);

    Ints Inew=Integral(psi_aux);

    Ints Iold=myI.back(); // prendo l'ultimo integrale
    
    //Metropolis test

    double p = exp(beta*(Iold.m_I-Inew.m_I));
      if(p >= rnd.Rannyu()){
        //Update
        f.sigma=sigmanew;
        f.mu=munew;
        myI.push_back(Inew);
        acceptedSA = acceptedSA + 1.0;
      } else {
        myI.push_back(Iold);
      }
      attemptedSA = attemptedSA + 1.0;
    
  
  return;
}





void Measure(Psi &f) //Properties measurement
{
  double i;

  i=f.EvalIntegrand(x);

  walker[ie] = i;  
  
  return;
}


void Reset(int iblk) //Reset block averages
{
   
   if(iblk == 1)
   {
       for(int i=0; i<n_props; ++i)
       {
           glob_av[i] = 0;
           glob_av2[i] = 0;
       }
   }

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = 0;
   }
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}


void Accumulate(void) //Update block averages
{

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = blk_av[i] + walker[i];
   }
   blk_norm = blk_norm + 1.0;
}


void Averages(int iblk) //Print results for current block
{
    
  ofstream eI; 
  const int wd=12;
    
    //cout << "Block number " << iblk << endl;
    //cout << "Acceptance rate " << accepted/attempted << endl << endl;
    
    
    

    stima_e = blk_av[ie]/blk_norm; 
    
    glob_av[ie] += stima_e;
    glob_av2[ie] += stima_e*stima_e;
    

    if(iblk==nblk){

      e_def=glob_av[ie]/(double)nblk;
      err_e=Error(glob_av[ie],glob_av2[ie],iblk);
      //eI.open("output_I.dat",ios::app);
      //eI << setw(wd) << iblk <<  setw(wd) << stima_e << setw(wd) << glob_av[ie]/(double)iblk << setw(wd) << err_e << endl;
    }

    if(print){
      e_def=glob_av[ie]/(double)nblk;
      err_e=Error(glob_av[ie],glob_av2[ie],iblk);
      eI.open("output_I.dat",ios::app);
      eI << setw(wd) << iblk <<  setw(wd) << stima_e << setw(wd) << glob_av[ie]/(double)iblk << setw(wd) << err_e << endl;
    
      eI.close();
    }

    


    
}




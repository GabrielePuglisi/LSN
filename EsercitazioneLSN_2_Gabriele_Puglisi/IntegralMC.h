/***********************************************************
////////////////////////////////////////////////////////////
            Versione di LSN con generatore nuovo

////////////////////////////////////////////////////////////
***********************************************************/










#ifndef __INTEGRALMC_H__
#define __INTEGRALMC_H__

#include <iostream>
#include<cmath>
#include "FunzioneBase.h" // funzione base contiene anche le funzioni con vettori per tanto hai bisogno di VectorOperations
//#include "RandomGen.h"
#include "random.h"
#include"VectorOperations.h" // giusto per essere sicuri di non dimenticarlo


using namespace std;

/*Versione Tnds
class IntegraleMC {
    public:
        IntegraleMC(unsigned int seed =1){
            m_gen=new RandomGen(seed); //costruisco il generatore di numeri casuali
            m_error=0;
        }
        ~IntegraleMC(){
            delete m_gen;
        }

        // metodi della media per funzioni da R a R e da Rn a R
        double IntegraleMedia(double a, double b, const FunzioneBase& f, unsigned int npunti);
		double IntegraleMedia(vector<double> a, vector<double> b, const FunzioneScalareBase& f, unsigned int npunti);

        double IntegraleHM(double a,double b,const FunzioneBase& f,double fmax, unsigned int npunti);

    private:
        RandomGen * m_gen;
        double m_error;
		
		
    
};



double IntegraleMC::IntegraleMedia(double a, double b, const FunzioneBase& f, unsigned int npunti){

	
    double fx;
    double I=0;

    
    for(int k=0; k <npunti; k++){
        fx=f.Eval(m_gen->Unif(a,b));
        I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx;
    }


   
    return I*(b-a);


}
double IntegraleMC::IntegraleMedia(vector<double> a, vector<double> b, const FunzioneScalareBase& f, unsigned int npunti){

    double fx;
    double I=0;
	vector<double> x(2);
	

    
    for(int k=0; k <npunti; k++){
		x[0]=m_gen->Unif(a[0],a[1]);
		x[1]=m_gen->Unif(b[0],b[1]);

        fx=f.Eval(x);
        I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx;
    }

	double r=(b[1]-b[0])*(a[1]-a[0]);
   
    return I*r;


}

double IntegraleMC::IntegraleHM(double a, double b, const FunzioneBase& f,double fmax,unsigned int npunti){
	
	

	double fx;
	double y;
	double n=0; // punti dentro l'area

	for(int i=0; i<npunti;i++){
		fx=f.Eval(m_gen->Unif(a,b));
		y=m_gen->Unif(0.,fmax);
		if(y<fx){
			n++;
		}
	}

	return (b-a)*fmax*static_cast<double>(n)/static_cast<double>(npunti);
	

}

*/

////Versione LSN////

class IntegraleMC {
public:

    IntegraleMC(): rnd(){
        int seed[4];
        int p1, p2;
        ifstream Primes("Primes");
        if (Primes.is_open()){
            Primes >> p1 >> p2 ;
        } else {
            cerr << "PROBLEM: Unable to open Primes" << endl;
        }
        Primes.close();

        ifstream input("seed.in");
        string property;
        if (input.is_open()){
            while ( !input.eof() ){
                input >> property;
                if( property == "RANDOMSEED" ){
                    input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                    rnd.SetRandom(seed,p1,p2);
                }
            }
            input.close();
        } else {
            cerr << "PROBLEM: Unable to open seed.in" << endl;
        }
    }


    ~IntegraleMC(){
        //delete rnd;
    }
    

   
    
    // metodi della media per funzioni da R a R e da Rn a R
    double IntegraleMedia(double a, double b, const FunzioneBase& f, unsigned int npunti);


    //metodo con distribuzione a piacere, va passata l'inversa della cumulativa

    double IntegraleMedia(double a, double b, const FunzioneBase& f,const FunzioneBase &p, unsigned int npunti);
    //double IntegraleMedia(vector<double> a, vector<double> b, const FunzioneScalareBase& f, unsigned int npunti);

    //double IntegraleHM(double a,double b,const FunzioneBase& f,double fmax, unsigned int npunti);
    //metodo con distribuzione a piacere e accept reject
    //double IntegraleMediaAR(double a, double b, const FunzioneBase& f, const FunzioneBase &p,const double pmax, const double N, unsigned int npunti);
    double GetVar(){return var;}
private:
    Random rnd;
    double var;
};





#endif // __INTEGRALMC_H__





#include"IntegralMC.h"

using namespace std;


double IntegraleMC::IntegraleMedia(double a, double b, const FunzioneBase& f, unsigned int npunti){

	
    double fx[npunti];
    double I=0;
    var=0;
    
    for(unsigned int k=0; k <npunti; k++){
        fx[k]=f.Eval(rnd.Rannyu(a,b));
        I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx[k];
    }

    for(unsigned int k=0; k <npunti; k++){
        fx[k]=pow((fx[k]-I),2);
        var=static_cast<double>(k)/static_cast<double>(k+1)*var + 1./static_cast<double>(k+1)*fx[k];
    }

   
    return I*(b-a);


}
// qui a e b sono gli estremi dell'inversa
double IntegraleMC::IntegraleMedia(double a, double b, const FunzioneBase& f,const FunzioneBase &p, unsigned int npunti){
    double fx[npunti];
    double I=0;
    var=0;
    
    for(unsigned int k=0; k <npunti; k++){
        fx[k]=f.Eval(p.Eval(rnd.Rannyu(a,b)));
        I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx[k];
    }

    for(unsigned int k=0; k <npunti; k++){
        fx[k]=pow((fx[k]-I),2);
        var=static_cast<double>(k)/static_cast<double>(k+1)*var + 1./static_cast<double>(k+1)*fx[k];
    }
    

    


   
    return I;
}



/*
double IntegraleMC::IntegraleMediaAR(double a, double b, const FunzioneBase& f, const FunzioneBase &p,const double pmax, const double N, unsigned int npunti){
    double fx;
    double I=0;

    vector <double> r(npunti); // vettore variabili aleatorie

    //metodo AR
    for(unsigned int k=0; k <npunti; k++){
       double x=rnd.Rannyu(a,b);
       double y=rnd.Rannyu(0,pmax);
       if()
    }

    for(unsigned int k=0; k <npunti; k++){
        fx=f.Eval(rnd.Rannyu(a,b));
        I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx;
    }


   
    return I*N;
}*/


#include<iostream>
#include<fstream>
#include<string>
#include"random.h"

#include<armadillo>
#include<cmath>


#include"LSN3.h"

using namespace arma;

double error(const vec &sum_prog,const vec &sum_prog2, int n){
   
   double error;
   if (n==0)
      error=0;
   else
      error=sqrt((sum_prog2[n]-pow(sum_prog[n],2))/n);  // media dei quadrati meno il quadrato delle medie

   return error;
}


int main(){

    //// devo campionare il moto browniano geometrico di S(t) a un tempo T grande, plottero' la media progressidrift=var=1;
    double risk=0.1; // parametri di della S(t) GBM
    double sigma=0.25;

    double K=100;

    double S_0=100;

    double S_T;

    double T=1;

    

    int M=10000; //numero di numeri generati
    int N=100; //numero di blocchi

    int L=M/N; // lunghezza del singolo blocco

    //call

    vec av1_C(N,arma::fill::zeros);  //vettore contenente le N medie degli N blocchi 
    vec av2_C(N,arma::fill::zeros);  //vettore contenente il quadrato delle N medie degli N blocchi 

    //put
    vec av1_P(N,arma::fill::zeros);  //vettore contenente le N medie degli N blocchi 
    vec av2_P(N,arma::fill::zeros);  //vettore contenente il quadrato delle N medie degli N blocchi 

    GBM s(risk,sigma,S_0);
    
    for(int i=0; i<N; i++){

        double sumC=0;
        double sumP=0;
        for(int j=0; j<L; j++)
        {
            S_T=s.S_direct(T);       //S_0*exp((risk-0.5*sigma*sigma)*T+sigma*r[i*L+j]);

            sumC+=exp(-risk*T)*Call(S_T,K);    // sommo gli elementi del singolo i-esimo blocco // uso la soluzione di S(t)
            sumP+=exp(-risk*T)*Put(S_T,K);                              

        }
       
        av1_C[i]=(sumC/L);
        av2_C[i]=(av1_C[i]*av1_C[i]);
        av1_P[i]=(sumP/L);
        av2_P[i]=(av1_P[i]*av1_P[i]);
   }
   
    vec sum_prog_C(N,arma::fill::zeros);  
    vec sum_prog2_C(N,arma::fill::zeros); 

    vec error_prog_C(N,arma::fill::zeros); 

    vec sum_prog_P(N,arma::fill::zeros);  
    vec sum_prog2_P(N,arma::fill::zeros); 

    vec error_prog_P(N,arma::fill::zeros); 

    for(int i=0;i<N;i++){
      
      for(int j=0; j<i+1;j++)
        {
            sum_prog_C[i]+=av1_C[j];
            sum_prog2_C[i]+=av2_C[j];

            sum_prog_P[i]+=av1_P[j];
            sum_prog2_P[i]+=av2_P[j];
        }
      sum_prog_C[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2_C[i]/=(double)(i+1); // media dei quadrati
      error_prog_C[i]=(error(sum_prog_C,sum_prog2_C,i)); // errore sul valore medio

      sum_prog_P[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2_P[i]/=(double)(i+1); // media dei quadrati
      error_prog_P[i]=(error(sum_prog_P,sum_prog2_P,i)); // errore sul valore medio
   
    }
    std::ofstream output;
    output.open("call_direct.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog_C[i]<<"\t"<<error_prog_C[i]<<endl;
    }
    output.close();

    output.open("put_direct.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog_P[i]<<"\t"<<error_prog_P[i]<<endl;
    }
    output.close();


    //rifaccio in modo discreto

    
    s=GBM(risk,sigma,S_0);

    int NN=100; // numeri di passi con cui arrivo da 0 a T
   

   for(int i=0; i<N; i++){

        double sumC=0;
        double sumP=0;
        for(int j=0; j<L; j++)
        {
            // va da 0 a T in modo discreto in 100 passi
            double t=0;
            S_T=S_0;
            while(t<T){
                S_T=s.S_discretized(t,t+T/NN,S_T);
                t+=T/(double)NN;
            }
            //cout<<S_T<<" discreto"<<endl;
            sumC+=exp(-risk*T)*Call(S_T,K);    // sommo gli elementi del singolo i-esimo blocco // uso la soluzione di S(t)
            sumP+=exp(-risk*T)*Put(S_T,K);                              
            
        }
        av1_C[i]=(sumC/L);
        av2_C[i]=(av1_C[i]*av1_C[i]);
        av1_P[i]=(sumP/L);
        av2_P[i]=(av1_P[i]*av1_P[i]);
   }

    sum_prog_C.zeros();
    sum_prog_P.zeros();

    sum_prog2_C.zeros();
    
    sum_prog2_P.zeros();

   for(int i=0;i<N;i++){
      
      for(int j=0; j<i+1;j++)
        {
            sum_prog_C[i]+=av1_C[j];
            sum_prog2_C[i]+=av2_C[j];

            sum_prog_P[i]+=av1_P[j];
            sum_prog2_P[i]+=av2_P[j];
        }
      sum_prog_C[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2_C[i]/=(double)(i+1); // media dei quadrati
      error_prog_C[i]=(error(sum_prog_C,sum_prog2_C,i)); // errore sul valore medio

      sum_prog_P[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2_P[i]/=(double)(i+1); // media dei quadrati
      error_prog_P[i]=(error(sum_prog_P,sum_prog2_P,i)); // errore sul valore medio
   
    }

   output.open("call_discretized.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog_C[i]<<"\t"<<error_prog_C[i]<<endl;
    }
    output.close();

    output.open("put_discretized.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog_P[i]<<"\t"<<error_prog_P[i]<<endl;
    }
    output.close();
    
    return 0;


}

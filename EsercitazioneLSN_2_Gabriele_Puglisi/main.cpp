
//programma per il calcolo di un integrale unidimensionale con il metodo di Monte Carlo 





#include<iostream>
#include<fstream>
#include<string>
#include"random.h"

#include<vector>
#include<cmath>


#include"FunzioneBase.h"
#include"IntegralMC.h" 


using namespace std;

double error(const vector<double> &sum_prog,const vector<double> &sum_prog2, int n){
   
   double error;
   if (n==0)
      error=0;
   else
      error=sqrt((sum_prog2[n]-pow(sum_prog[n],2))/n);  // media dei quadrati meno il quadrato delle medie

   return error;
}



//funzione da integrare

//funzione F(x) // (0,pi/4) e' l' inversa della cumulativa

class ES2_LSN_Fx : public FunzioneBase{
	public:
	double Eval (double x) const override{
		return 1-sqrt(1-x);
	};
};


class ES2_LSN_gx : public FunzioneBase{
	public:
	double Eval (double x) const override{
		return M_PI_2*cos(M_PI_2*x)/(2-2*x);
	};
};

//funzione g(x)



int main (int argc, char *argv[]){

    ES2_LSN f;
    ES2_LSN_gx g;
    ES2_LSN_Fx p;
    IntegraleMC MC;

    //////genero 10^4 punti e calcolo la media dell'integrale per N=100 blocchi
    int M=10000; //numero di numeri generati
    int N=100; //numero di blocchi

    int L=M/N; // lunghezza del singolo blocco

    vector<double> av1(N);  //vettore contenente le N medie degli N blocchi
    vector<double> av2(N);  //vettore contenente il quadrato delle N medie degli N blocchi

    vector<double> var1(N);  //vettore contenente le N varianze degli N blocchi
    vector<double> var2(N);  //vettore contenente il quadrato delle N varianze degli N blocchi

    


    for(int i=0; i<N; i++){

        av1[i]=MC.IntegraleMedia(0,1,f,L);
        av2[i]=av1[i]*av1[i];
        var1[i]=MC.GetVar();
        var2[i]=var1[i]*var1[i];
        
           
    }

    //ora che ho N valori medi devo calcolare la media e la varianza(del valore medio) 
    //al variare del numero di valori medi che prendo
   
    vector<double> sum_prog(N);
    vector<double> sum_prog2(N);
    vector<double> error_prog(N);

    for(int i=0;i<N;i++){
      sum_prog[i]=(0);
      sum_prog2[i]=(0);
      for(int j=0; j<i+1;j++)
        {
            sum_prog[i]+=av1[j];
            sum_prog2[i]+=av2[j];
        }
      sum_prog[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2[i]/=(double)(i+1); // media dei quadrati
      error_prog[i]=(error(sum_prog,sum_prog2,i)); // errore sul valore medio
   
    }

    //stampo le medie in funzione di i tra (1 e N) e l'errore associato

    ofstream output;
    output.open("mean1.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();


   //calcolo la varianza sigmaI

    for(int i=0;i<N;i++){
      sum_prog[i]=(0);
      sum_prog2[i]=(0);
      for(int j=0; j<i+1;j++)
        {
            sum_prog[i]+=var1[j];
            sum_prog2[i]+=var2[j];
        }
      sum_prog[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2[i]/=(double)(i+1); // media dei quadrati
      error_prog[i]=(error(sum_prog,sum_prog2,i)); // errore sul valore medio
   
    }

  
    output.open("var1.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();


  //rifaccio con una distribuzione presa dallo sviluppo al primo ordine dell'integranda in x=1
  for(int i=0; i<N; i++){

        av1[i]=MC.IntegraleMedia(0,1,g,p,L);
        av2[i]=av1[i]*av1[i];
        var1[i]=MC.GetVar();
        var2[i]=var1[i]*var1[i];
           
    }

  for(int i=0;i<N;i++){
      sum_prog[i]=(0);
      sum_prog2[i]=(0);
      for(int j=0; j<i+1;j++)
        {
            sum_prog[i]+=av1[j];
            sum_prog2[i]+=av2[j];
        }
      sum_prog[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2[i]/=(double)(i+1); // media dei quadrati
      error_prog[i]=(error(sum_prog,sum_prog2,i)); // errore sul valore medio
   
    }

    //stampo le medie in funzione di i tra (1 e N) e l'errore associato

    
    output.open("mean2.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();


   //calcolo la varianza sigmaI

    for(int i=0;i<N;i++){
      sum_prog[i]=(0);
      sum_prog2[i]=(0);
      for(int j=0; j<i+1;j++)
        {
            sum_prog[i]+=var1[j];
            sum_prog2[i]+=var2[j];
        }
      sum_prog[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2[i]/=(double)(i+1); // media dei quadrati
      error_prog[i]=(error(sum_prog,sum_prog2,i)); // errore sul valore medio
   
    }

  
    output.open("var2.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();

  

    return 0;
}
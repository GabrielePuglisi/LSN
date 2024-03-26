#include<iostream>
#include<fstream>
#include<string>
#include"random.h"

#include<vector>
#include<cmath>
#include<unistd.h>

using namespace std;


//funzione per l'incertezza del valore medio di un osservabile

double error(const vector<double> &sum_prog,const vector<double> &sum_prog2, int n){
   
   double error;
   if (n==0)
      error=0;
   else
      error=sqrt((sum_prog2[n]-pow(sum_prog[n],2))/n);  // media dei quadrati meno il quadrato delle medie

   return error;
}

class Buffon{
   private:
      double m_L; // deve poco piu piccolo di 1
      Random rnd;
   public:
      Buffon(double L=0.9) : rnd(){
         int seed[4];
         int p1, p2;
         ifstream Primes("Primes");
         if (Primes.is_open()){
            Primes >> p1 >> p2 ;
         } else cerr << "PROBLEM: Unable to open Primes" << endl;
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
         } else cerr << "PROBLEM: Unable to open seed.in" << endl;

         if(L>=1 or L<0.1)
            L=0.9;
            

          m_L=L;
          //cout<<"sto usando m_l="<<m_L<<endl;
         

      }

      int Try(){
         double p1=rnd.Rannyu(0,1);
         double theta=rnd.UnifTheta(); //estrae un angolo tra 0 e pi senza conoscere pi
         //cout<<theta<<endl;
         //sleep(1);
         double p2=p1+m_L*sin(theta);
         if(p2>1 or p2<0)
            return 1;
         else 
            return 0;

      }


};



vector<double> frequency(const vector<double> &x, int M){

   vector<double> freq(M,0); // vettore contenente le frequenze inizializzato a zero

   for(int i=0; i<(int)x.size(); i++){

      int k=x[i]*M; // versione migliorata di quello sotto

      freq[k]++;

      /*
      for(int j=0; j<M;j++){
         if(x[i]>=(double) j/M and x[i]<(double)(j+1)/M)
            freq[j]++;
         }
      */
   }


   return freq;
}

double TestChi(const vector<double> O,double E){ // valori osservati e valore atteso
   double chi=0;
   

   for(int i=0; i<(int)(O.size()); i++){
      chi+=pow((O[i]-E),2)/(E);
   }

   return chi;
}
 
int main (int argc, char *argv[]){

    Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
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
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;


    int M=10000; //numero di numeri generati
    int N=100; //numero di blocchi

    int L=M/N; // lunghezza del singolo blocco

    vector<double> av1;  //vettore contenente le N medie degli N blocchi
    vector<double> av2;  //vettore contenente il quadrato delle N medie degli N blocchi

    vector<double> r;   //numeri in [0,1)
    for(int i=0; i < M ; i++){
        r.push_back(rnd.Rannyu());
    }

    for(int i=0; i<N; i++){

        double sum=0;
        for(int j=0; j<L; j++)
        {
            sum+=r[i*L+j];    // sommo gli elementi del singolo i-esimo blocco

        }
        av1.push_back(sum/L);
        av2.push_back(av1[i]*av1[i]);
        
           
    }

    //ora che ho N valori medi devo calcolare la media e la varianza 
    //al variare del numero di valori medi che prendo
   
    vector<double> sum_prog;
    vector<double> sum_prog2;
    vector<double> error_prog;

    for(int i=0;i<N;i++){
      sum_prog.push_back(0);
      sum_prog2.push_back(0);
      for(int j=0; j<i+1;j++)
        {
            sum_prog[i]+=av1[j];
            sum_prog2[i]+=av2[j];
        }
      sum_prog[i]/=(double)(i+1); //media dell'osservabile
      sum_prog2[i]/=(double)(i+1); // media dei quadrati
      error_prog.push_back(error(sum_prog,sum_prog2,i)); // errore sul valore medio
   
    }

    //stampo le medie in funzione di i tra (1 e N) e l'errore associato

    ofstream output;
    output.open("mean.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();


   //ripeto tutta la procedura per il calcolo della varianza

    


   for(int i=0; i<N; i++){

        double sum=0;
        for(int j=0; j<L; j++)
        {
            sum+=pow((r[i*L+j]-0.5),2);    // sommo gli elementi del singolo i-esimo blocco 
                                          //(qui voglio l'incertezza dell'i-esimo blocco)

        }
        av1[i]=(sum/L);
        av2[i]=(av1[i]*av1[i]);
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
   
   output.open("var.data");
    for(int i=0; i<N; i++){
      output<<(i+1)*L<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();




   //////////////////////////////////////////////
   //Test di Chi quadro//
   //////////////////////////////////////////////

   // effettuo il test 100 volte

   double MM=100; //numero di sottointervali in cui divido (0,1)
   double E=M/MM;
   cout<<E;
   vector<double> chi(100);
   for(int i=0;i<100; i++ ){

      chi[i]=TestChi(frequency(r,MM),E); // M e' il numero di lanci e MM il numero di sotto intervalli
      //nuovi numeri in [0,1)
      for(int j=0; j < M ; j++){
         r[j]=(rnd.Rannyu());   
      }
   }

   // stampo i 100 valori di chi quadrato
   output.open("chi.data");
    for(int i=0; i<100; i++){
      output<<chi[i]<<endl;
    }
   output.close();


   ///// Test Limite Centrale Distribuzione Dado uniforme
   //plotto M=10^4 valori medi
   // per 4 casisitiche N=1,2,10,100

   vector<double> Sn(4);
   // stampo i 100 valori di chi quadrato

   ofstream output1,output2,output3;
   output.open("dice1.data");
   output1.open("dice2.data");
   output2.open("dice10.data");
   output3.open("dice100.data");
  

   for(int i=0; i<M; i++){
      //estraggo 100 numeri



      for(int j=0; j<N;j++){
         r[j]=floor(rnd.Rannyu(1,7));
      }
      
      Sn={0,0,0,0};

      //metto in un vettore la media Sn  
      Sn[0]=r[0];

      output<<Sn[0]<<endl;

      for(int j=0; j<2;j++){
         Sn[1]+=r[j];
      }

      Sn[1]/=(double)(2);
      output1<<Sn[1]<<endl;

       for(int j=0; j<10;j++){
         Sn[2]+=r[j];
      }
      Sn[2]/=(double)(10);
      output2<<Sn[2]<<endl;

       for(int j=0; j<100;j++){
         Sn[3]+=r[j];
      }
      Sn[3]/=(double)(100);
      output3<<Sn[3]<<endl;
      


   }
    output.close();
    output1.close();
    output2.close();
    output3.close();


   ///// Test Limite Centrale Distribuzione Esponenziale
   //plotto M=10^4 valori medi
   // per 4 casisitiche N=1,2,10,100

   
   // stampo i 100 valori di chi quadrato

  
   output.open("exp1.data");
   output1.open("exp2.data");
   output2.open("exp10.data");
   output3.open("exp100.data");
   
  

   for(int i=0; i<M; i++){
      //estraggo 100 numeri
      for(int j=0; j<N;j++){
         r[j]=(rnd.Exp(1));
      }
      

      //metto in un vettore la media Sn 

      Sn={0,0,0,0};

      Sn[0]=r[0];

      output<<Sn[0]<<endl;

      for(int j=0; j<2;j++){
         Sn[1]+=r[j];
      }

      Sn[1]/=(double)(2);
      output1<<Sn[1]<<endl;

       for(int j=0; j<10;j++){
         Sn[2]+=r[j];
      }
      Sn[2]/=(double)(10);
      output2<<Sn[2]<<endl;

       for(int j=0; j<100;j++){
         Sn[3]+=r[j];
      }
      Sn[3]/=(double)(100);
      output3<<Sn[3]<<endl;
      

   }
    output.close();
    output1.close();
    output2.close();
    output3.close();
    


   output.open("cauchy1.data");
   output1.open("cauchy2.data");
   output2.open("cauchy10.data");
   output3.open("cauchy100.data");
   
  

   for(int i=0; i<M; i++){
      //estraggo 100 numeri
      for(int j=0; j<N;j++){
         r[j]=(rnd.Cauchy(0,1));
      }
      

      //metto in un vettore la media Sn 

      Sn={0,0,0,0};

      Sn[0]=r[0];

      output<<Sn[0]<<endl;

      for(int j=0; j<2;j++){
         Sn[1]+=r[j];
      }

      Sn[1]/=(double)(2);
      output1<<Sn[1]<<endl;

       for(int j=0; j<10;j++){
         Sn[2]+=r[j];
      }
      Sn[2]/=(double)(10);
      output2<<Sn[2]<<endl;

       for(int j=0; j<100;j++){
         Sn[3]+=r[j];
      }
      Sn[3]/=(double)(100);
      output3<<Sn[3]<<endl;
      

   }
    output.close();
    output1.close();
    output2.close();
    output3.close();
    

   ////// calcolo pi greco 2L/Pd

   

   double l=0.7;
   M=1000000;
   N=100;
   L=M/N;

   Buffon buffo(l);




   //faccio M lanci segnando con 1  l'esito positivo e con 0 quello negativo
   vector<int> tries(M);

   for(int i=0;i<M;i++){
      tries[i]=buffo.Try();
      //cout<<tries[i]<<endl;
      //sleep(1);

    
   }

   // faccio N misure di pi

   for(int i=0; i<N; i++){

      double accepted=0;
     for(int j=0; j<L; j++) //ricroda L qui e' il numero di elementi per blocco
     {
         accepted+=tries[i*L+j];    // sommo gli elementi del singolo i-esimo blocco
         
     }
     double Pn=1.*accepted/L;
     
     av1[i]=(2.*(l)/(Pn));
     av2[i]=(av1[i]*av1[i]);
     
           
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
   
   output.open("buffon.data");
    for(int i=0; i<N; i++){
      output<<(i+1)<<"\t"<<sum_prog[i]<<"\t"<<error_prog[i]<<endl;
    }
   output.close();



   
   

   





   rnd.SaveSeed();
   return 0;
}
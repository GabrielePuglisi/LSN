#include<iostream>
#include<fstream>
#include<string>
#include"random.h"

#include<vector>
#include<cmath>

#include<armadillo>


///Random-Walk 3D discreto e continuo 


double error(const double &sum_prog,const double &sum_prog2, int n){
   
   double error;
   if (n==0)
      error=0;
   else
      error=sqrt((sum_prog2-pow(sum_prog,2))/n);  // media dei quadrati meno il quadrato delle medie

   return error;
}

using namespace arma;
int main(){


    //// genero M random walk di 100 passi, divido in 100 blocchi; 
    //// in ogni blocco calcolo il valore medio di r(i)^2 e ne faccio la radice. Dopo calcolo la media delle radici calcolata sui 100 blocchi
    ///questo mi completa il singolo random walk


    // a me interessa mappare il valore medio di sqrt{r(i)^2} al variare di i


    int M=10000; // numero di volte che simulo il random walk
    int P=100;  // numero di passi per singolo random walk

    int N=100; // numero di blocchi di rw su cui faro' la media

    int L=M/N;

    vec rw(3,arma::fill::zeros); //vettore per singolo random walk, contiene le 3 coordinate spaziali, parte dall'origine

    vec r_i(P,arma::fill::zeros); // vettore sui cui andro' a caricare la media delle distanze al qadrato r(i) per M/N rw

    vec av(P,arma::fill::zeros);   // vettoree sui andro' a caricare la media della radice di r_i, media sugli N blocchi
    vec av2(P,arma::fill::zeros);  //vettore contenente la media dei quadrati
    int u; // variabile che usero' per decidere l'asse
    int v; // variabile che usero' per decidere il verso

    //generatore numeri casuali, le dirazioni saranno scelte uniformemente 

    Random rnd; 
    int seed[4];
        int p1, p2;
        std::ifstream Primes("Primes");
        if (Primes.is_open()){
            Primes >> p1 >> p2 ;
        } else {
            std::cerr << "PROBLEM: Unable to open Primes" << endl;
        }
        Primes.close();

        std::ifstream input("seed.in");
        std::string property;
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
    


   
    std::ofstream output;
    output.open("rw_discreto.data");


    //I=static_cast<double>(k)/static_cast<double>(k+1)*I + 1./static_cast<double>(k+1)*fx[k];
    
    for(int k=0;k<N;k++){  // ciclo sui blocchi
        // all'inizio di ogni blocco azzero r_i
        r_i.zeros();
        for(int i=0;i<(L);i++){ // ciclo sul blocco, qui dentro c'e' un random walk
            rw.zeros(); // parto sempre dall' origine
            for(int j=0; j<P; j++){ //i  100 passi del rw
                u=rnd.Rannyu(0,1)*3; // questo numero sara o 0 o 1 o 2 
                v=rnd.Rannyu(0,1)*2; // questo sara o 0 o 1
                if(v==0)
                    rw[u]--;
                if(v==1)
                    rw[u]++;
                r_i[j]=static_cast<double>(k*L+i)/static_cast<double>(k*L+i+1)*r_i[j] + 1./static_cast<double>(k*L+i+1)*(arma::dot(rw,rw));
                av[j]=static_cast<double>(k)/static_cast<double>(k+1)*av[j] + 1./static_cast<double>(k+1)*sqrt(r_i[j]);
                // mi serve anche la media quadra sui vettori per il calcolo dell'errore
                av2[j]=static_cast<double>(k)/static_cast<double>(k+1)*av2[j] + 1./static_cast<double>(k+1)*(r_i[j]);
            }
            
        }// alla fine di ogni blocco r_i[j] conterra' la media
        // con queste medie faccio la media progressiva della radice sugli n blocchi
        
    }
    // calcolo la barra di errore per ogni passo i
    vec err_i(P);
    for(int i=0;i<P;i++){
        err_i[i]=error(av[i],av2[i],N); // passo il valore medio, il valore medio dei quadrati, e il numero di blocchi su cui hai mediato
    }
    
    // stampo i risultati in funzione del passo i
    output<<0<<"\t"<<0<<"\t"<<0<<endl;
    for(int i=0; i<P;i++){
        output<<i+1<<"\t"<<av[i]<<"\t"<<err_i[i]<<endl;
    }
    output.close();



    ////rifaccio estranedo in modo continuo su un angolo solido, in questo esercizio mi basta estrarre unif theta e phi

    double theta,phi;

    output.open("rw_continuo.data");

    av.zeros();
    av2.zeros();


    for(int k=0;k<N;k++){  // ciclo sui blocchi
        // all'inizio di ogni blocco azzero r_i
        r_i.zeros();
        for(int i=0;i<(L);i++){ // ciclo sul blocco, qui dentro c'e' un random walk
            rw.zeros(); // parto sempre dall' origine
            for(int j=0; j<P; j++){ //i  100 passi del rw
                theta=rnd.SinX();
                phi=rnd.Rannyu(0,2*M_PI);
                rw[0]+=sin(theta)*cos(phi); //x
                rw[1]+=sin(theta)*sin(phi); //y
                rw[2]+=cos(theta); //z
                r_i[j]=static_cast<double>(k*L+i)/static_cast<double>(k*L+i+1)*r_i[j] + 1./static_cast<double>(k*L+i+1)*(arma::dot(rw,rw));
                av[j]=static_cast<double>(k)/static_cast<double>(k+1)*av[j] + 1./static_cast<double>(k+1)*sqrt(r_i[j]);
                // mi serve anche la media quadra sui vettori per il calcolo dell'errore
                av2[j]=static_cast<double>(k)/static_cast<double>(k+1)*av2[j] + 1./static_cast<double>(k+1)*(r_i[j]);
            }
            
        }// alla fine di ogni blocco r_i[j] conterra' la media
        // con queste medie faccio la media progressiva della radice sugli n blocchi
        
    }
  


    // calcolo la barra di errore per ogni passo i
    
    for(int i=0;i<P;i++){
        err_i[i]=error(av[i],av2[i],N); // passo il valore medio, il valore medio dei quadrati, e il numero di blocchi su cui hai mediato
    }
    
    // stampo i risultati in funzione del passo i
    output<<0<<"\t"<<0<<"\t"<<0<<endl;
    for(int i=0; i<P;i++){
        output<<i+1<<"\t"<<av[i]<<"\t"<<err_i[i]<<endl;
    }
    output.close();



    ///// faccio un plot per vedere che in questo modo i punti sono distributi uniformemente su una sfera unitaria

    vec point(3);
    output.open("sfera_giusta.dat");
    

    for(int i=0;i<1000;i++){
        theta=rnd.SinX();
        phi=rnd.Rannyu(0,2*M_PI);
        point[0]=sin(theta)*cos(phi); //x
        point[1]=sin(theta)*sin(phi); //y
        point[2]=cos(theta); //z

        

        output<<point[0]<<"\t"<<point[1]<<"\t"<<point[2]<<endl;
    }

    output.close();

    output.open("sfera_sbagliata.dat");
    

    for(int i=0;i<1000;i++){
        theta=rnd.Rannyu(0,M_PI);
        phi=rnd.Rannyu(0,2*M_PI);
        point[0]=sin(theta)*cos(phi); //x
        point[1]=sin(theta)*sin(phi); //y
        point[2]=cos(theta); //z

        

        output<<point[0]<<"\t"<<point[1]<<"\t"<<point[2]<<endl;
    }

    output.close();
    return 0;


    return 0;
}



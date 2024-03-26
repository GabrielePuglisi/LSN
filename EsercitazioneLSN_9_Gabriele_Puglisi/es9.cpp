#include<iostream>

#include"GAS.h"
#include"random.h"
#include<fstream>
#include<iomanip>

using namespace std;



void initRand(Random &rnd){

    ifstream Primes("Primes");

    //Read seed for random numbers
    int p1, p2;
    Primes >> p1 >> p2 ;
    Primes.close();

    //if(restart) Seed.open("seed.out");
    //else Seed.open("seed.in");

    int seed[4];

    ifstream Seed("seed.in");
    Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
    rnd.SetRandom(seed,p1,p2);
    Seed.close();

}

int main(int argc, char *argv[]){

    unsigned int N=stoi(argv[1]); // numero citta
    unsigned int a=stoi(argv[2]); // disposizione, circolare se a=0, quadrata altrimenti
    unsigned int M=stoi(argv[3]); // numero cromosomi
    unsigned int nsteps=stoi(argv[4]);
    double pcrossover=stod(argv[5]); // probabilita di riproduzione
    double pmutation=stod(argv[6]); // probabilita di mutare
    initRand(rnd);



    vector<Posizione> map=Mappa(N,a);
    // la stampo su un file
    ofstream MappaData;
    string type;
    if(a==0)
        type="circle";
        
    else
        type="square";
        

    MappaData.open("map_"+type+".data");
    for(unsigned int i=0; i<map.size();i++){
        MappaData<<map[i].GetX()<<setw(12)<<map[i].GetY()<<endl;
    }

    Population pop(map,M,pcrossover,pmutation);

    ofstream bestpathLength, bestpath,input;

    if(pcrossover==0){
        bestpathLength.open("bestlength_"+type+"_onlymutation.data");
        bestpath.open("best_onlymutation_"+type+".data"); 
        input.open("input_onlymutation_"+type+".data");  
    }else{
        bestpathLength.open("bestlength_"+type+".data");
        bestpath.open("best_"+type+".data");
        input.open("input_"+type+".data");   
    }

    

    for( unsigned int i=0; i<nsteps; i++){
        pop.Move();
        cout<<"Mossa fatta"<<endl;
        for(unsigned int i=0; i< M; i++)
            bestpathLength<<setw(12)<<pop.m_pop[i].L2;
            
        bestpathLength<<endl;
        
    }

    
    
    for(unsigned int i=0; i<N; i++){
        bestpath<<setw(12)<<pop.m_pop[0].m_p[i];
    }

    

    input<<N<<endl<<M<<endl<<nsteps<<endl<<pcrossover<<endl<<pmutation<<endl;

    
    
    
    




    return 0;
}
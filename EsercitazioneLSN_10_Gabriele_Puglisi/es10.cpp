#include<iostream>

#include"GAS.h"
#include"random.h"
#include<fstream>
#include<iomanip>


#include <mpi.h>

using namespace std;



void initRand(Random &rnd, int rank=0){

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
    seed[3]+=rank;
    rnd.SetRandom(seed,p1,p2);
    Seed.close();

}

int main(int argc, char *argv[]){


    MPI_Init(&argc, &argv);

    // variabili comuni

     ifstream MappaFile;
     MappaFile.open("American_capitals.dat");

    if (!MappaFile.is_open()) {
        cerr << "Impossibile aprire il file." << endl;
        return 1;
    }

    double x, y;
    vector<Posizione> Mappa;

    while (MappaFile >> x >> y) {
        Mappa.push_back(Posizione(x, y));
    }

    MappaFile.close();

    

    int N=Mappa.size();
    int M=1000; // numero cromosomi
    int nsteps=1000;
    double pcrossover=0.9; // probabilita di riproduzione
    double pmutation=0.05; // probabilita di mutare
    int mig=0;
    int Nmig=nsteps*0.20;

     string migr;

    if(mig==0){
        migr="";
    }else
        migr="migration";

    // DIVISIONE IN NODI

	 int size;
	 int rank;
	 
	 MPI_Comm_size(MPI_COMM_WORLD, &size);
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	 cout <<endl<< "Sono il nodo " << rank << " di " << size << endl;

    
    //Inizializzazione generatore di numeri casuali. Uno condiviso, gli altri per singolo processo.
    Random rnd;
    initRand(rnd,rank);

    Random shared_rnd;
    initRand(shared_rnd,0); // con questo faro le migrazioni


    Population pop(rnd,Mappa,M,pcrossover,pmutation);

    ofstream bestpathLength,input;

   


    bestpathLength.open("bestlength"+migr+"_"+to_string(rank)+".data");
    
    input.open("input.data");  
    input<<N<<endl<<M<<endl<<nsteps<<endl<<pcrossover<<endl<<pmutation<<endl<<Nmig<<endl;

    

    for(int i=0; i<nsteps; i++){
        if(i%Nmig==0 and mig!=0){
        // faccio la migrazione
        int j=shared_rnd.Rannyu(1,size);

        int sendind_rank=rank;
        int recev_rank=(rank+j)%size;
        cout<<"Sono il nodo "<<sendind_rank<<"e invio al nodo "<<recev_rank<<endl;

        vector<int> send=pop.m_pop[0].m_p;
        vector<int> receive(N);


        MPI_Sendrecv(send.data(), N, MPI_INT, recev_rank, 0,                                                   //Invio e ricezione
                 receive.data(), N, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        
        
        cout<<"Sono il nodo "<<sendind_rank<<"e ho terminato lo scambio"<<endl;

        pop.m_pop[M-1].m_p=receive;
        pop.calcL(pop.m_pop[M-1]);
        pop.m_pop[M-1].CheckPath();
        sort(pop.m_pop.begin(), pop.m_pop.end(), [](const chromosome& a, const chromosome& b) {
            return a.L2 < b.L2;
             });

        


        
        }
        pop.Move();
        for(int i=0; i< M; i++)
            bestpathLength<<setw(12)<<pop.m_pop[i].L2;
            
        bestpathLength<<endl;
        
    }
    

    //raccolgo il migliore tra tutti i nodi

    //chromosome best_local=pop.m_pop[0];
    //vector<chromosome> all_chromosomes(size);

    vector<int> send(N);
    vector<int> receive(size * N);

    // Riempimento di send con il miglior individuo dal nodo corrente
    for (int j = 0; j < N; ++j) {
        send[j] = pop.m_pop[0].m_p[j];
    }

    // Utilizzo di MPI_Gather per raccogliere i cromosomi migliori da tutti i nodi
    MPI_Gather(send.data(), N, MPI_INT, receive.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    // Sul nodo 0, elabora i cromosomi raccolti
    if (rank == 0) {
        vector<chromosome> all_best(size);

        ofstream bestpath;
        ofstream bestlength;
        bestlength.open("bestlength"+migr+".data");
        bestpath.open("best"+migr+".data");

        // Riempimento di all_best con i cromosomi ricevuti
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < N; ++j) {
                all_best[i].m_p.push_back(receive[i * N + j]);
            }
            all_best[i].CheckPath();
            pop.calcL(all_best[i]);
        }

        // Ordinamento di all_best
        sort(all_best.begin(), all_best.end(), [](const chromosome& a, const chromosome& b) {
            return a.L2 < b.L2;
        });

        for(int i=0; i< all_best.size();i++){
            cout<<"\t"<<all_best[i].L2;
        }

        // Elaborazione del miglior cromosoma
        chromosome best = all_best[0];
        best.CheckPath();
        

    
        for (int i = 0; i < N; i++) {
            bestpath << setw(12) << best.m_p[i];
        }

        bestlength<<best.L2;

        bestpath.close();

        bestlength.close();
        }


    
    bestpathLength.close(); 

    MPI_Finalize();
    



    return 0;
}
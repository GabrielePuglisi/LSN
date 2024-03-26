#pragma once 

#include<iostream>
#include<fstream>
#include<string>
#include"random.h"
#include<algorithm>
#include<iomanip>


#include<vector>
#include<armadillo>
#include<cmath>

using namespace std;


//Random numbers
#include "random.h"
int seed[4];
Random rnd;







class Posizione {
public:
    Posizione();
    Posizione(double x, double y);
    ~Posizione() {}

    double Distanza(const Posizione &p) const;

    double GetX() const;
    double GetY() const;

    void SetX(double x);
    void SetY(double y);

private:
    double m_x, m_y;
};

Posizione::Posizione() {
    m_x = 0;
    m_y = 0;
}

Posizione::Posizione(double x, double y) {
    m_x = x;
    m_y = y;
}


double Posizione::GetX() const {
    return m_x;
}

double Posizione::GetY() const {
    return m_y;
}

void Posizione::SetX(double x) {
    m_x = x;
}

void Posizione::SetY(double y) {
    m_y = y;
}

double Posizione::Distanza(const Posizione &p) const {
    return sqrt(pow(GetX() - p.GetX(), 2) + pow(GetY() - p.GetY(), 2));
}



vector<Posizione> Mappa(int N=10, int a=0){ //n numero citta' , a parametro per decidere se avere una circ o un quadrato

    vector<Posizione> map(N);

    if(a==0){
        //genero citta' su una circoferenza 
        for(int i=0; i< N; i++){
            double angle=rnd.Rannyu(0,2*M_PI);
            map[i].SetX(cos(angle));
            map[i].SetY(sin(angle));
        }

    }else{
         for(int i=0; i< N; i++){
            map[i].SetX(rnd.Rannyu(0,1));
            map[i].SetY(rnd.Rannyu(0,1));
        }
    }

    return map;

}


class chromosome{

    public:

    chromosome(){;};

    vector<int> m_p; //questo vettore contiene solo gli indici associati alle singole citta'
    double L2=0; // L del chromosome

    

    void add(int n){
        m_p.push_back(n);
    };


    bool CheckPath() {
    if (m_p[0] != 0) {
        cerr << "Errore nella funzione check: 0 non e' la prima citta'" << endl;
        return false;
    }

    vector<bool> visited(m_p.size(), false);

    for (int i = 0; i < m_p.size(); i++) {
        if (visited[m_p[i]]) {
            cerr << "Errore nella funzione check: citta' ripetute" << endl;
            return false;
        }
        visited[m_p[i]] = true;
    }

    return true;
    }

    void SetPath(vector<int> p){
            for(int i=0; i < m_p.size(); i++){
                m_p[i] = p[i];
            }
        }

    
    int pbc(int icity){
    //Per non modificare la città di partenza ci spostiamo a quella immediatamente successiva
    if(icity == 0) {
        return 1;
    }

    //Se l'indice della città in cui ci troviamo è superiore al numero di città disponibili, lo diminuiamo fino a rientrare nel giusto range [1, m_N]
    while(icity >= m_p.size()){
        icity -= (m_p.size()- 1);
    }

    return icity;
}




};



class Population{

    public:

    Population(Random r,const vector<Posizione> map,int  M,double crossover, double mutation){  // il costruttore prende le citta' e il numero di cromosomi da creare
        m_map=map;
        m_M=M;
        rnd=r;
        pmutation=mutation;
        pcrossover=crossover;
        m_N=m_map.size();
        m_pop.resize(m_M);
        Print();
        for(int j=0;j<m_M;j++){
            for(int i=0;i<m_N;i++){
                m_pop[j].add(i);
                
            }
            
        }
        // ora che ho M cromosomi tutti del tipo [0,1....N-1] gli faccio mutare tutti N volte
        // preservando due regole 
        // 1) zero come primo elemento
        // 2) nessuna citta ripetuta
        for(int j=0;j<m_M;j++){
            for(int i=0;i<m_N;i++){
                double p=1; // cosi la mutazione e' assicurata
                Mutation(j,p);
                
            
                
            }
            
            
        }
        //riordino
        sort(m_pop.begin(), m_pop.end(), [](const chromosome& a, const chromosome& b) {
        return a.L2 < b.L2;});

        
        
    }

    vector<Posizione> m_map; // le mie citta'

    vector <chromosome> m_pop; // la mia popolazione di n^2 percorsi di n elementi

    

    int m_M;
    int m_N;
    Random rnd;

    double c=4; //esponente per il selection
    double pcrossover,pmutation;
    
    void Print(){
        cout<<"GAS con:"<<endl
            <<"N citta'= "<<m_N<<endl
            <<"M cromosomi= "<<m_M<<endl
            <<"Probabilita di crossover= "<<pcrossover<<endl
            <<"Probabilita di mutazione= "<<pmutation<<endl;
    }

    
    void Move(){

        //all'inizio di ogni generazione ogni individuo della meta' peggiore puo' mutare

        for (int i = m_M/2; i < m_M; i++)
        {
            Mutation(i,pmutation);
            
        }
        

        //ordino la popolazione prima di procedere
        // Ordinamento del vettore in base al parametro degli oggetti utilizzando una lambda function
        sort(m_pop.begin(), m_pop.end(), [](const chromosome& a, const chromosome& b) {
        return a.L2 < b.L2;});

        for(int i=0;i<m_N;i++){
            // seleziono due percorsi
            int a=Selection();
            int b=Selection();
            // mi riproduco con una probabilita' pcrossover
            double p=rnd.Rannyu(0,1);
            if(p<pcrossover){
                Crossover(a,b);
            }else{
                m_pop.push_back(m_pop[a]);
                Mutation(m_pop.size()-1,pmutation);
                
                m_pop.push_back(m_pop[b]);
                Mutation(m_pop.size()-1,pmutation);
              
                // se non si sono riprodotti lavoro sui genitori
            }

        }

          
        //ora riordino e cancello la seconda meta
        sort(m_pop.begin(), m_pop.end(), [](const chromosome& a, const chromosome& b) {
            return a.L2 <b.L2;});
        m_pop.resize(m_M);
        
    }


    int Selection(){
        double r=rnd.Rannyu();
    
        return (int)m_M*pow(r,c);
    }
    



    void calcL (chromosome &p){
        p.L2=0;
        for(int i=1; i<m_N;i++){
            p.L2+=m_map[p.m_p[i]].Distanza(m_map[p.m_p[(i-1)]]);
        }

        // con questa piccola accortezza dovrebbe tornare indietro alla prima citta'
        p.L2+=m_map[p.m_p.back()].Distanza(m_map[p.m_p[(0)]]);
    }

    void Mutation(int a,double p){
        double p1=rnd.Rannyu(0,1);
        if(p1<p)
            PairPermutation(a);

        double p2=rnd.Rannyu(0,1);
        if(p2<p)
            Shift(a);

        double p3=rnd.Rannyu(0,1);
        if(p3<p)
            Permutation(a);

        double p4=rnd.Rannyu(0,1);
        if(p4<p)
            Inversion(a);
    //in questa funzione tutte le 4 mutazioni avranno probabilita p di verificarsi
    }
    // qui invece scrivo le 4 mutazioni singolarmente in modo da poterle forzare per fare dei test
    

    void PairPermutation(int a){

        //permuto la citta i-esima con la j-esima 
        // la citta 0 deve rimanere sempre la prima
    

        int j=(rnd.Rannyu(1,m_N));
        int i=(rnd.Rannyu(1,m_N));
        
        while(i==j){
            i=(rnd.Rannyu(1,m_N));
        }
        int temp=m_pop[a].m_p[i];
        m_pop[a].m_p[i]=m_pop[a].m_p[j];
        m_pop[a].m_p[j]=temp;
        
        if(m_pop[a].CheckPath()==false){
            cout<<"Errore nel PairPermutation"<<endl;
            cout<<m_pop[a].m_p[0]<<endl;
            exit(-1);
        }
        calcL(m_pop[a]);
    }

    void Shift(int a){

    int n = floor(rnd.Rannyu(1, m_N)); //Numero di posizioni di cui shiftare la città
    int m = floor(rnd.Rannyu(1, m_N)); //Numero di città da shiftare
    int start = floor(rnd.Rannyu(1, m_N)); //Città da cui partire con la mutazione

    //Ciclo esterno sul numero di posizioni di cui shiftare la città
    for(int i = 0; i < n; i++){
        //Ciclo interno sul numero di città da shiftare
            for(int j = m - 1; j >= 0; j--){
                //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 5, 2, 3], 
                //n = 2, m = 2, start = 1, start + i + j = 2 => [1, 2, 3, 4, 5] -> [1, 2, 4, 3, 5], j --
                //start + i + j = 1 => [1, 2, 4, 3, 5] -> [1, 4, 2, 3, 5], i++
                //start + i + j = 3 => [1, 4, 2, 3, 5] -> [1, 4, 2, 5, 3], j --
                //start + i + j = 2 => [1, 4, 2, 3, 5] -> [1, 4, 5, 2, 3]

                //Città in posizione start + i + j scambiata con la città successiva, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_N]
                swap(m_pop[a].m_p[m_pop[a].pbc(start + i + j)], m_pop[a].m_p[m_pop[a].pbc(start + i + j + 1)]);
            }
        }
        if(m_pop[a].CheckPath()==false){
            cout<<"Errore nel PairPermutation"<<endl;
            cout<<m_pop[a].m_p[0]<<endl;
            exit(-1);
        }
        calcL(m_pop[a]);
    }

    //Terzo esempio di mutazione: si scambiano le posizioni di m città contigue con quelle di altre m città contigue, controllando che m < N/2    
    void Permutation(int a){   

        int start =  floor(rnd.Rannyu(1, m_N)); //Città da cui partire con la mutazione
        int m = floor(rnd.Rannyu(2, m_N/2.)); //Numero di città da permutare

        //Ciclo sul numero di città da permutare
        for(int i = 0; i < m; i++){
            //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 5, 2, 3], m = 2, start = 1, start + i = 1, start + i + m = 3, i++, start + i = 2, start + i + m = 4
            //Città in posizione start + i scambiata con la città in posizione start + i + m, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_N]
            swap(m_pop[a].m_p[m_pop[a].pbc(start + i)], m_pop[a].m_p[m_pop[a].pbc(start + i + m)]);
        }

        if(m_pop[a].CheckPath()==false){
            cout<<"Errore nel PairPermutation"<<endl;
            cout<<m_pop[a].m_p[0]<<endl;
            exit(-1);
        }
        calcL(m_pop[a]);
    }
    //Quarto esempio di mutazione: si inverte l'ordine in cui le città appaiono nel percorso, per m città con m <= N
    void Inversion(int a){

        int m = floor(rnd.Rannyu(2, m_N)); //Numero di città da invertire
        int start = floor(rnd.Rannyu(1, m_N)); //Città da cui partire con la mutazione

        //Ciclo esterno sul numero di città da invertire, dimezzato, poiché scambiano le posizioni reciproche delle città
        for (int i = 0; i < m/2; i++) {
            //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 3, 2, 5], m = 2, start = 1, start + i = 1, start + m - i = 3
            //Città in posizione start + i scambiata con la città in posizione start + m - i, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_N]
            swap(m_pop[a].m_p[m_pop[a].pbc(start + i)], m_pop[a].m_p[m_pop[a].pbc(start + m - i)]);
        }

        if(m_pop[a].CheckPath()==false){
            cout<<"Errore nel PairPermutation"<<endl;
            cout<<m_pop[a].m_p[0]<<endl;
            exit(-1);
        }
        calcL(m_pop[a]);
    }

    
    void Crossover(int a, int b) {  
    // passo gli indici dei genitori


        int count1 = 0;
        int count2 = 0;

        chromosome child_1=m_pop[a];
        chromosome child_2=m_pop[b];

        int cut = floor(rnd.Rannyu(1., m_N - 1));      //random cut index

        vector<int> path_1 = child_1.m_p;
        vector<int> path_2 = child_2.m_p;

        path_1.resize(cut);             //Paths stay the same until cut point
        path_2.resize(cut);

        // Crossover

        for(int i = 0; i < m_N; i ++){    // Iterate over cities in the parent routes

            count1 = 0;
            count2 = 0;
            // Check if the city from selected2 already exists in child1's route
            for(int j = 0; j < path_1.size() ; j++){
                if( m_pop[b].m_p[i] == path_1[j] ){
                    count1 ++;
                    break;
                }
            }
            // If city doesn't exist, add it to child1's route
            if (count1 == 0 ) path_1.push_back(m_pop[b].m_p[i]);

            // Check if the city from selected1 already exists in child2's route
            for(int j = 0; j < path_2.size(); j++){
                if( m_pop[a].m_p[i] == path_2[j] ){
                    count2 ++;
                    break;
                }
            }
            // If city doesn't exist, add it to child2's route
            if (count2 == 0 ) path_2.push_back(m_pop[a].m_p[i]);
        }

        child_1.SetPath(path_1);
        child_2.SetPath(path_2);

        if (!child_1.CheckPath()) {
            cout << "Errore nel crossover" << endl;
            exit(-1);
        }
        calcL(child_1);

    
        if (!child_1.CheckPath()) {
            cout << "Errore nel crossover" << endl;
            exit(-1);
        }
        calcL(child_2);

        m_pop.push_back(child_1);
        m_pop.push_back(child_2);

    }

   
   
    
};




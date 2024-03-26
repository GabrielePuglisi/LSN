
#include<iostream>
#include<fstream>
#include<string>
#include"random.h"

#include<armadillo>
#include<cmath>

using namespace arma;

double Call(const double &S,const double &k){
    if(S>k)
        return S-k;
    
    else
        return 0;
}

double Put(const double &S,const double &k){
    if(S<k)
        return k-S;
    else
        return 0;
}



class GBM{
    public:
    GBM(const double &risk=0.1, const double &sigma=1, const double &S_0=0 ): rnd(){
        int seed[4];
        int p1, p2;
        std::ifstream Primes("Primes");
        if (Primes.is_open()){
            Primes >> p1 >> p2 ;
        } else {
            cerr << "PROBLEM: Unable to open Primes" << endl;
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

        m_risk=risk;
        m_sigma=sigma;
        m_S_0=S_0;

    }


    ~GBM(){}

    double S_direct(const double &T){ // passo da t=0 a t=T
        S_t=m_S_0*exp((m_risk-0.5*m_sigma*m_sigma)*T+m_sigma*rnd.Gauss(0,sqrt(T)));
        
        return S_t;
    }

    double S_discretized(const double &t1,const double &t2,const double &S_t1){ // passo da t=ti a t=ti+1

        double m_t1,m_t2;

        if(t2>=t1){
            m_t1=t1;
            m_t2=t2;
        }else{
            m_t1=t2;
            m_t2=t1;
        }

        

        S_t=S_t1*exp((m_risk-0.5*m_sigma*m_sigma)*(m_t2-m_t1)+m_sigma*rnd.Gauss(0,1)*sqrt(m_t2-m_t1));
        //cout<<S_t<<endl;
        return S_t;
    }


    private:
        Random rnd;
        double S_t;
        double m_sigma;
        double m_risk;
        double m_S_0;
};
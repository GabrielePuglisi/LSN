#include"es8.h"


int main(int argc, char* argv[]){

    Random rnd;
    
    Input();

    Psi psi(sigmaINI,muINI);              // creo la psi con i parametri inizial
       // vettori di I con i loro errori

    
    
   

    //calcolo primo integrale

    myI.push_back(Integral(psi));
    cout<<myI.back().m_I;
    
    ofstream param, ints;
    param.open("parameters.data");
    ints.open("ints.data");
    //cosi ho il primo integrale con i parametri iniziali

    //stampo i valori iniziali
    param << setw(wd)<< 0 << setw(wd) << psi.sigma<< setw(wd) << psi.sigma <<endl;
    //stampo anche l'integrale
    ints << setw(wd)<< 0 << setw(wd) << myI.back().m_I<< setw(wd) << myI.back().m_errorI <<endl;

    //ciclo sulle temperature
    temp=tempINI;
    for(int i=0; i<nstepT; i++){
        // per ogni temperatura faccio un SA quindi ne vedo l'efficenza
        attemptedSA=0;
        acceptedSA=0;
        beta=1/temp;
        for(int j=0; j<nstepSA;j++){
            if(i==nstepT-1 and j==nstepSA-1)
                print=true;
            Move2(psi);
            param << setw(wd)<< (i)*nstepSA+(j+1) << setw(wd) << psi.sigma<< setw(wd) << psi.mu <<endl;
            //stampo anche l'integrale
            ints << setw(wd)<< (i)*nstepSA+(j+1) << setw(wd) << myI.back().m_I<< setw(wd) << myI.back().m_errorI <<endl;
        }
        cout << "SA with temp=" << temp << endl;
        cout << "Acceptance rate " << acceptedSA/attemptedSA << endl << endl;
        // abbasso la temperatura
        temp*=0.8;
    }

    //plotto la distribuzione finale

    ofstream isto;
    x=1;
    isto.open("isto.dat",ios::app);
    for(int i=0; i<100000;i++){
        Move1(psi);
        isto<<x<<endl;
    }

    isto.close();
    


    return 0;
}

 
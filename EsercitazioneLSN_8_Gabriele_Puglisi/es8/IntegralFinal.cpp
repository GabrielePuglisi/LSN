#include"es8.h"
#include"random.h"


void Averages2(int iblk) //Print results for current block
{
    
   ofstream eI; 
   const int wd=12;
    
   cout << "Block number " << iblk << endl;
   cout << "Acceptance rate " << accepted/attempted << endl << endl;
    
    
    

    stima_e = blk_av[ie]/blk_norm; 
    
    glob_av[ie] += stima_e;
    glob_av2[ie] += stima_e*stima_e;
    

    

    e_def=glob_av[ie]/(double)nblk;
    err_e=Error(glob_av[ie],glob_av2[ie],iblk);
    eI.open("output_I.dat",ios::app);
    eI << setw(wd) << iblk <<  setw(wd) << stima_e << setw(wd) << glob_av[ie]/(double)iblk << setw(wd) << err_e << endl;
    


    eI.close();
}



// mi calcola l' integrale con Metropolis e l' errore
Ints Integral2(Psi &f){
    for(int iblk=1; iblk <= nblk; iblk++) {         

        Reset(iblk);   //Reset block averages
        for(int istep=1; istep <=nstepINT; istep++){ 
                    
            Move1(f);
            Measure(f);
            Accumulate();    //Update block averages
                
        }
        Averages2(iblk);  
    }

    Ints I(e_def,err_e);

    return I;



};




using namespace std;



int main(){

    ifstream ReadInput, Primes, Seed;

  

  //Read seed for random numbers
  int p1, p2;
  Primes.open("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

  Seed.open("seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();


  Psi f(0.60,0.84);

  nstepINT=1000;
  nblk=100;

  Ints MyInt=Integral(f);
  return 0;
}
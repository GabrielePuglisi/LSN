// ===============================================================================                  //
// A library defining possible operations among std::vector 
//
// ===============================================================================                                                                                               

#ifndef __VectorOperations_h__
#define __VectorOperations_h__

#include <iostream>
#include <vector>
#include <algorithm>  // se se vogliono usare algoritmi STL
#include <numeric>    // per inner_product
#include <iomanip>
#include <fstream>
#include<cmath>
using namespace std;

// ===============================================================================               
// somma di due vettori : somma componente per componente
// ===============================================================================                                                                                               
template <typename T> std::vector<T> operator+(const std::vector<T> &a, const std::vector<T> &b) {
  
  // below different ways to check if vectors have the same size 

  // assert(a.size() == b.size());  
  //  if ( a.size() != b.size() ) throw "Trying to sum vectors with different size" ;  
  if ( a.size() != b.size() ) { 
    cout << "Trying to sum vectors with different size, exiting" << endl ; 
    exit(-1); 
  } ; 
  
  std::vector<T> result(a.size());
  
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = a[i] + b[i];    
  
  // Alternativamente si puo' usare l'algoritmo transform della STL
  //
  //  std::transform(a.begin(), a.end(), b.begin(), result.begin() , std::plus<T>()); 
  
  return result;
  
}

// ===============================================================================          
// differenza di due vettori componente per componente
// [ preferisco re-implementarlo perche' si fanno meno operazioni rispetto result = a + (-1.*b) ]
// ===============================================================================                                                                                                
template <typename T> std::vector<T> operator-(const std::vector<T> &a, const std::vector<T> &b) {
  
  // below different ways to check if vectors have the same size                                                                                                             
  // assert(a.size() == b.size());                                                                                                                                           
  // if ( a.size() != b.size() ) throw "Trying to sum vectors with different size" ;                                                                                             
  if ( a.size() != b.size() ) {
    cout << "Trying to sum vectors with different size, exiting" << endl ;
    exit(-1);
  } ;

  std::vector<T> result(a.size());
  
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = a[i] - b[i];    
  
  // Alternativamente si puo' usare l'algoritmo transform della STL
  //
  //    std::transform(a.begin(), a.end(), b.begin(), result.begin() , std::minus<T>()); 
  
  return result;
  
}

// ===============================================================================  
// prodotto scalare tra due vettori 
// ===============================================================================

template <typename T> T operator*(const std::vector<T> &a, const std::vector<T> &b) {

  // below different ways to check if vectors have the same size                                                                                                             
  // assert(a.size() == b.size());                                                                                                                                          
  // if ( a.size() != b.size() ) throw "Trying to sum vectors with different size" ;                                                                                         
  if ( a.size() != b.size() ) {
    cout << "Trying to sum vectors with different size, exiting" << endl ;
    exit(-1);
  } ;

  T sum = 0 ;
  for (int i = 0; i < static_cast<int>(a.size()); i++) sum += a[i] * b[i];  
  
  // Alternativamente si puo' usare l'algoritmo inner_product della STL
  //
  // sum = std::inner_product(std::begin(a), std::end(a), std::begin(b), 1.0);
  
  return sum;
  
}

// ===============================================================================
// prodotto tra uno scalare e un vettore
// ===============================================================================

template <typename T> std::vector<T> operator*( T c , const std::vector<T> &a) {
  
  std::vector<T> result(a.size());
  
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = c * a[i];  
  
  // Alternativamente si puo' usare l'algoritmo inner product
  //
  //     std::transform(std::begin(a), std::end(a), std::begin(result), [&c](T x){ return x * c; } );
  
  return result;
  
}

// ===============================================================================
// prodotto tra un vettore e uno scalare 
// ===============================================================================

template <typename T> std::vector<T> operator*( const std::vector<T> &a , T c) {
  
  std::vector<T> result(a.size());
  
for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = c * a[i];  
  
  // oppure il ciclo for puo' essere sostituito da ( ~ stesso numero di operazioni con il
  // move constructor del vector altrimenti sarebbe meno efficiente )
  // 
  // result = c * a ;
  
  // Alternativamente si puo' usare l'algoritmo transform della STL con una lambda function
  //    
  //    std::transform(std::begin(a), std::end(a), std::begin(result), [&c](T x){ return x * c; } );
  
  return result;
  
}

// ===============================================================================
// divisione tra un vettore e uno scalare 
// ===============================================================================

template <typename T> std::vector<T> operator/( const std::vector<T> &a , T c) {
  
  std::vector<T> result(a.size());
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = a[i] / c ;  
  
  // oppure il ciclo for puo' essere sostituito da
  
  // double fact = 1./c
  // result = a * fact ;
  
  // Alternativamente si puo' usare l'algoritmo transform della STL con una lambda function
  //    
  //    std::transform(std::begin(a), std::end(a), std::begin(result), [&c](T x){ return x / c; } );
  
  return result;
  
}

// ===============================================================================
// somma ad a un vettore b e il risultato viene messo in a
// ===============================================================================

template <typename T> std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b) {

  // below different ways to check if vectors have the same size                                                                                                             
  // assert(a.size() == b.size());                                                                                                                                           
  // if ( a.size() != b.size() ) throw "Trying to sum vectors with different size" ;                                                                                         
  if ( a.size() != b.size() ) {
    cout << "Trying to sum vectors with different size, exiting" << endl ;
    exit(-1);
  } ;
  
  for (int i = 0; i < static_cast<int>(a.size()); i++) a[i] += b[i];    
  
  // Alternativamente si puo' usare l'algoritmo transform della STL
  //
  //    std::transform(a.begin(), a.end(), b.begin(), a.begin() , std::plus<T>()); 
  
  return a;
}

// ===============================================================================
// sottrai ad a un vettore b e il risultato viene messo in a
// ===============================================================================

template <typename T> std::vector<T>& operator-=(std::vector<T>& a, const std::vector<T>& b) {
  
  // below different ways to check if vectors have the same size                                                                                                             
  // assert(a.size() == b.size());                                                                                                                                           
  // if ( a.size() != b.size() ) throw "Trying to sum vectors with different size" ;                                                                                         
  if ( a.size() != b.size() ) {
    cout << "Trying to sum vectors with different size, exiting" << endl ;
    exit(-1);
  } ;
  
  for (int i = 0; i < static_cast<int>(a.size()); i++) a[i] -= b[i];    
  
  // Alternativamente si puo' usare l'algoritmo transform della STL
  //
  //    std::transform(a.begin(), a.end(), b.begin(), a.begin() , std::minus<T>()); 
  
  return a;
}

// ===============================================================================
// Possiamo usare funzioni generiche che agiscono sui vettori
// ===============================================================================
  
  // metodo comodo per stampare il vettore
  
/*template <typename T> void Print( const std::vector<T> &v ) {
  
  //std::cout << "Printing vector" << std::endl;
  for ( auto it = v.begin() ; it != v.end() ; it++ ) std::cout << *it << " " ;
  std::cout << std::endl;
 // std::cout << "End of printing vector" << std::endl;
  
};*/
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> void Print( const vector<T> & v ) {
  for (int i=0; i<v.size(); i++) { 
        cout<< setw(10) << v[i];
		if((i+1)%10==0) cout<<endl;
  }
};

template <typename T> void Print( const vector<T> & v  , const char* nomefile ) {
 
	ofstream out;
	out.open(nomefile);

	
	if(out.good()){

		for(int i=0; i < v.size(); i++){
			out<< setw(10) << v[i];
			if((i+1)%10==0) out<<endl;
		}

		out.close();
	}else{
		cerr<<"Errore apertura file"<<endl;
		exit(-3);

	
}



};

// =====================================================================

template <typename T> vector<T> Read( unsigned int N , const char* nomefile ) {

  ifstream in;

	vector<T> v;

	in.open(nomefile);

	if(in.fail()){
		cerr<<"Errore apertura file lettura"<<endl;
		exit(-2);
	}
	T appo;
	

	for(int i=0; i < N; i++){
			in >> appo;
			v.push_back(appo);
	}

	in.close();

	return v;

  
};

template <typename T> vector<T> ReadAll( const char* nomefile ) {

  ifstream in;

	vector<T> v;

	in.open(nomefile);

	if(in.fail()){
		cerr<<"Errore apertura file lettura"<<endl;
		exit(-2);
	}
	T appo;
	
	//in >> appo;
	// v.push_back(appo);

	while(in>> appo){
			 // devo inizializzare appo=0;
			//in >> appo;
			v.push_back(appo);
	}

	in.close();

	return v;
 
};

// =====================================================================

template <typename T> double CalcolaMedia( const vector<T> & v) {

	T media=0;

  for(int i=0; i < v.size(); i++ ){
    media+=v[i];
  }
  return media/v.size();

};

template <typename T> double CalcolaMediana( vector<T> v ) {

	sort( v.begin(), v.end() ) ;  // Use the STL sort !!!

  T mediana;

	
  if(v.size()%2==0){
    mediana= (v[(v.size()/2)-1] + v[(v.size()/2)])/2;
  }else{
    mediana=v[(v.size()/2)];
  }

	return mediana;

};

template <typename T> double CalcolaVarianza( const vector<T> & v) {

  T media=CalcolaMedia<T>(v);
  T var=0;

  for(T x: v ){
    var+=pow((x-media),2);
  }

  return var/(v.size()-1);

};
// =====================================================================





/*
template <typename T> class linearVector : public std::vector<T> {
  
public :

  linearVector<T> ( ) : std::vector<T>(){} ;
  linearVector<T> ( int size ) : std::vector<T>(size){} ;  

  linearVector<T> operator+( const linearVector<T> &a ) {
    
    assert(a.size() == this->size());
    
    linearVector<T> result(a.size()) ;
    for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = a[i] + (*this)[i];
    
    return result;
    
  }
  
};
*/

#endif //  __VectorOperations_h__

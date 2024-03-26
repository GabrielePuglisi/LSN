#ifndef __FunzioneBase_h__
#define __FunzioneBase_h__

#include "VectorOperations.h"
#include <cmath>
#include<vector>

using namespace std;

class FunzioneBase {

  public:

  virtual double Eval (double x) const= 0;
	
  virtual ~FunzioneBase() {;};

};

class Seno : public FunzioneBase{
	public:
	double Eval (double x) const override{
		return sin(x);
	};
};


class Parabola : public FunzioneBase {

public:

  Parabola () {m_a=0.; m_b=0.; m_c=0.;}
  Parabola (double a, double b, double c) {m_a=a; m_b=b; m_c=c;}
  ~Parabola () {}
  double Eval (double x) const override {return m_a*x*x + m_b*x + m_c;}
  void SetA (double a) {m_a=a;}
  double GetA () const {return m_a;}
  void SetB (double b) {m_b=b;}
  double GetB () const {return m_b;}
  void SetC (double c) {m_c=c;};
  double GetC () const {return m_c;}

private:

  double m_a, m_b, m_c;

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ===============================================================
// Classe astratta, restituisce la derivata nel punto x al tempo t
// ===============================================================

class FunzioneVettorialeBase {

public:
  virtual vector<double> Eval(double t, const vector<double> & x) const = 0;
};

// ===============================================================
// Caso fisico concreto
// ===============================================================

class OscillatoreArmonico : public FunzioneVettorialeBase {

public:

  OscillatoreArmonico(double omega0) { m_omega0 = omega0; };

  vector<double> Eval(double t, const vector<double> &x) const override {

		// devo restituire la derivata di (x,v) tramite la legge dell'OscillatoreArmonico a=-omega*x

		vector<double> dxdt { x[1], -pow(m_omega0,2)*x[0]};

		return dxdt;

  }

private:
  double m_omega0;
};


class Pendolo : public FunzioneVettorialeBase {

public:

  Pendolo(double g, double l) { m_g=g; m_l=l; };

  vector<double> Eval(double t, const vector<double> &x) const override {

		// devo restituire la derivata di (theta, theta_punto) tramite la legge del pendolo semplice 

		vector<double> dxdt { x[1], -m_g/m_l*sin(x[0])};  

		return dxdt;

  }

private:
  double m_g;
  double m_l;
};


class OscillatoreArmonicoSF : public FunzioneVettorialeBase {

public:

  OscillatoreArmonicoSF(double omega0,double gamma,double omega, double F=1, double m=1) { m_omega0 = omega0; m_gamma=gamma; m_omega=omega;  m_F=F; m_m=m;};

  vector<double> Eval(double t, const vector<double> &x) const override {

		// devo restituire la derivata di (x,v) tramite la legge dell'OscillatoreArmonico a=-omega*x

		vector<double> dxdt { x[1], -pow(m_omega0,2)*x[0] - m_gamma*x[1] +m_F/m_m*sin(m_omega*t)};

		return dxdt;

  }

private:
  double m_omega0;
  double m_gamma;
  double m_omega;
  double m_F;
  double m_m;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FunzioneScalareBase {

public:
  virtual double Eval( const vector<double> & x) const = 0;
};

// ===============================================================
// Caso fisico concreto
// ===============================================================

class Esercizio10_3 : public FunzioneScalareBase {

public:


  double Eval(const vector<double> &x) const override {
      return (5./(x[0]*x[0])*cos(4*x[1])*sin(7*x[0]) + 10);
		
  }


};

///////////Simulazione Numerica, usero' funzioni lambda

class ES2_LSN : public FunzioneBase{
	public:
	double Eval (double x) const override{
		return M_PI_2*cos(M_PI_2*x);
	};
};




#endif // __FunzioneVettorialeBase_h__

//____________________________________________________________________________
/*!

\class     Rnd

\brief     Singleton to keep instances of random number generators used in the
           analysis and control their seed numbers.
           Currently, five TRandom3 objects are instantiated: three for use in 
           toy MC generation (generating: systematic tweaks, poisson fluctuations 
           and oscillation params) one for use in fitters (eg generating random 
           initial points etc) and one for all other uses.
           By default, the seed is set to 0: the seed is automatically computed 
           via a TUUID object and is guaranteed to be unique.
           Using specific seed numbers can be usefull in some instances such as,
           for example, if one wants to repeat the same set of N systematic tweaks 
           for all oscillation parameters bins etc.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef __gastpc_rnd__
#define __gastpc_rnd__

#include <iostream>

#include <TRandom3.h>

using std::ostream;

class Rnd {

public:
  static Rnd * Instance();

  // default seed is 0
  void SetSeeds(UInt_t toymcstat = 0, UInt_t toymcsyst = 0, UInt_t toymcosc = 0, UInt_t fitter = 0, UInt_t other = 0);

  TRandom3 & RndToyMCstat  (void) { return fRndToyMCstat;  }
  TRandom3 & RndToyMCsyst  (void) { return fRndToyMCsyst;  }
  TRandom3 & RndToyMCosc   (void) { return fRndToyMCosc;   }
  TRandom3 & RndFitter     (void) { return fRndFitter;     }
  TRandom3 & RndOther      (void) { return fRndOther;      }

  void Print(ostream & stream) const;
  friend ostream & operator << (ostream & stream, const Rnd & rnd);

private:

  Rnd();
  Rnd(const Rnd &) {}
 ~Rnd();

  TRandom3 fRndToyMCstat;
  TRandom3 fRndToyMCsyst;
  TRandom3 fRndToyMCosc;
  TRandom3 fRndFitter;
  TRandom3 fRndOther;

  UInt_t fSeedToyMCstat;
  UInt_t fSeedToyMCsyst;
  UInt_t fSeedToyMCosc;
  UInt_t fSeedFitter;
  UInt_t fSeedOther;

  static Rnd * fInstance;

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (Rnd::fInstance !=0) {
            delete Rnd::fInstance;
            Rnd::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
};

ostream & operator<< (ostream & stream, const Rnd & plst);

#endif // __gastpc_rnd__

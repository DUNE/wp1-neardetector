//____________________________________________________________________________
/*!

\class    Units

\brief    Class for units

\author   Yordan Karadzhov <Yordan.Karadzhov \at cern.ch>
	  University of Geneva

\created  Sep 2012
\last update sep 2012

*/
//____________________________________________________________________________
#include "Units.hh"

double stringToUnitFactor(const std::string& str) {
  if (str == "") return 1.;
  else if (str == "millimeter"      ) return  CLHEP::millimeter;
  else if (str == "millimeter2"     ) return  CLHEP::millimeter2;
  else if (str == "millimeter3"     ) return  CLHEP::millimeter3;
  else if (str == "centimeter"      ) return  CLHEP::centimeter;
  else if (str == "centimeter2"     ) return  CLHEP::centimeter2;
  else if (str == "centimeter3"     ) return  CLHEP::centimeter3;
  else if (str == "meter"           ) return  CLHEP::meter;
  else if (str == "meter2"          ) return  CLHEP::meter2;
  else if (str == "meter3"          ) return  CLHEP::meter3;
  else if (str == "kilometer"       ) return  CLHEP::kilometer;
  else if (str == "kilometer2"      ) return  CLHEP::kilometer2;
  else if (str == "kilometer3"      ) return  CLHEP::kilometer3;
  else if (str == "parsec"          ) return  CLHEP::parsec;
  else if (str == "micrometer"      ) return  CLHEP::micrometer;
  else if (str == "nanometer"       ) return  CLHEP::nanometer;
  else if (str == "angstrom"        ) return  CLHEP::angstrom;
  else if (str == "fermi"           ) return  CLHEP::fermi;
  else if (str == "barn"            ) return  CLHEP::barn;
  else if (str == "millibarn"       ) return  CLHEP::millibarn;
  else if (str == "microbarn"       ) return  CLHEP::microbarn;
  else if (str == "nanobarn"        ) return  CLHEP::nanobarn;
  else if (str == "picobarn"        ) return  CLHEP::picobarn;
  else if (str == "mm"              ) return  CLHEP::mm;
  else if (str == "um"              ) return  CLHEP::um;
  else if (str == "nm"              ) return  CLHEP::nm;
  else if (str == "mm2"             ) return  CLHEP::mm2;
  else if (str == "mm3"             ) return  CLHEP::mm3;
  else if (str == "cm"              ) return  CLHEP::cm;
  else if (str == "cm2"             ) return  CLHEP::cm2;
  else if (str == "cm3"             ) return  CLHEP::cm3;
  else if (str == "m"               ) return  CLHEP::m;
  else if (str == "m2"              ) return  CLHEP::m2;
  else if (str == "m3"              ) return  CLHEP::m3;
  else if (str == "km"              ) return  CLHEP::km;
  else if (str == "km2"             ) return  CLHEP::km2;
  else if (str == "km3"             ) return  CLHEP::km3;
  else if (str == "pc"              ) return  CLHEP::pc;
  else if (str == "radian"          ) return  CLHEP::radian;
  else if (str == "milliradian"     ) return  CLHEP::milliradian;
  else if (str == "degree"          ) return  CLHEP::degree;
  else if (str == "steradian"       ) return  CLHEP::steradian;
  else if (str == "rad"             ) return  CLHEP::rad;
  else if (str == "mrad"            ) return  CLHEP::mrad;
  else if (str == "sr"              ) return  CLHEP::sr;
  else if (str == "deg"             ) return  CLHEP::deg;
  else if (str == "nanosecond"      ) return  CLHEP::nanosecond;
  else if (str == "second"          ) return  CLHEP::second;
  else if (str == "millisecond"     ) return  CLHEP::millisecond;
  else if (str == "microsecond"     ) return  CLHEP::microsecond;
  else if (str == "picosecond"      ) return  CLHEP::picosecond;
  else if (str == "hertz"           ) return  CLHEP::hertz;
  else if (str == "kilohertz"       ) return  CLHEP::kilohertz;
  else if (str == "megahertz"       ) return  CLHEP::megahertz;
  else if (str == "ns"              ) return  CLHEP::ns;
  else if (str == "s"               ) return  CLHEP::s;
  else if (str == "ms"              ) return  CLHEP::ms;
  else if (str == "eplus"           ) return  CLHEP::eplus;
  else if (str == "e_SI"            ) return  CLHEP::e_SI;
  else if (str == "coulomb"         ) return  CLHEP::coulomb;
  else if (str == "megaelectronvolt") return  CLHEP::megaelectronvolt;
  else if (str == "electronvolt"    ) return  CLHEP::electronvolt;
  else if (str == "kiloelectronvolt") return  CLHEP::kiloelectronvolt;
  else if (str == "gigaelectronvolt") return  CLHEP::gigaelectronvolt;
  else if (str == "teraelectronvolt") return  CLHEP::teraelectronvolt;
  else if (str == "petaelectronvolt") return  CLHEP::petaelectronvolt;
  else if (str == "joule"           ) return  CLHEP::joule;
  else if (str == "MeV"             ) return  CLHEP::MeV;
  else if (str == "eV"              ) return  CLHEP::eV;
  else if (str == "keV"             ) return  CLHEP::keV;
  else if (str == "GeV"             ) return  CLHEP::GeV;
  else if (str == "TeV"             ) return  CLHEP::TeV;
  else if (str == "PeV"             ) return  CLHEP::PeV;
  else if (str == "kilogram"        ) return  CLHEP::kilogram;
  else if (str == "gram"            ) return  CLHEP::gram;
  else if (str == "milligram"       ) return  CLHEP::milligram;
  else if (str == "kg"              ) return  CLHEP::kg;
  else if (str == "g"               ) return  CLHEP::g;
  else if (str == "mg"              ) return  CLHEP::mg;
  else if (str == "watt"            ) return  CLHEP::watt;
  else if (str == "newton"          ) return  CLHEP::newton;
  else if (str == "pascal"          ) return  CLHEP::hep_pascal;
  else if (str == "bar"             ) return  CLHEP::bar;
  else if (str == "atmosphere"      ) return  CLHEP::atmosphere;
  else if (str == "ampere"          ) return  CLHEP::ampere;
  else if (str == "milliampere"     ) return  CLHEP::milliampere;
  else if (str == "microampere"     ) return  CLHEP::microampere;
  else if (str == "nanoampere"      ) return  CLHEP::nanoampere;
  else if (str == "megavolt"        ) return  CLHEP::megavolt;
  else if (str == "kilovolt"        ) return  CLHEP::kilovolt;
  else if (str == "volt"            ) return  CLHEP::volt;
  else if (str == "ohm"             ) return  CLHEP::ohm;
  else if (str == "farad"           ) return  CLHEP::farad;
  else if (str == "millifarad"      ) return  CLHEP::millifarad;
  else if (str == "microfarad"      ) return  CLHEP::microfarad;
  else if (str == "nanofarad"       ) return  CLHEP::nanofarad;
  else if (str == "picofarad"       ) return  CLHEP::picofarad;
  else if (str == "weber"           ) return  CLHEP::weber;
  else if (str == "tesla"           ) return  CLHEP::tesla;
  else if (str == "gauss"           ) return  CLHEP::gauss;
  else if (str == "kilogauss"       ) return  CLHEP::kilogauss;
  else if (str == "henry"           ) return  CLHEP::henry;
  else if (str == "kelvin"          ) return  CLHEP::kelvin;
  else if (str == "mole"            ) return  CLHEP::mole;
  else if (str == "becquerel"       ) return  CLHEP::becquerel;
  else if (str == "curie"           ) return  CLHEP::curie;
  else if (str == "gray"            ) return  CLHEP::gray;
  else if (str == "kilogray"        ) return  CLHEP::kilogray;
  else if (str == "milligray"       ) return  CLHEP::milligray;
  else if (str == "microgray"       ) return  CLHEP::microgray;
  else if (str == "candela"         ) return  CLHEP::candela;
  else if (str == "lumen"           ) return  CLHEP::lumen;
  else if (str == "lux"             ) return  CLHEP::lux;
  else if (str == "perCent"         ) return  CLHEP::perCent;
  else if (str == "perThousand"     ) return  CLHEP::perThousand;
  else if (str == "perMillion"      ) return  CLHEP::perMillion;
  else {
    std::cout << "Unknown units '" << str << "'" << std::endl;
    exit(1);
  }
}

double stringToUnit(const std::string& str) {
  size_t pos;
  pos = str.rfind("*");
  int mul = (pos == str.npos)?-1:pos;
  pos = str.rfind("/");
  int div = (pos == str.npos)?-1:pos;

  if (mul == div)
    return stringToUnitFactor(str);
  else if (mul > div)
    return stringToUnit(str.substr(0, mul)) * stringToUnitFactor(str.substr(mul+1));
  else //if (div > mul)
    return stringToUnit(str.substr(0, div)) / stringToUnitFactor(str.substr(div+1));
}

/* $Source: /cvsroot/psrchive/psrchive/Util/genutil/MJD.h,v $
   $Revision: 1.10 $
   $Date: 1999/12/21 04:42:21 $
   $Author: straten $ */

#ifndef __MJD_H
#define __MJD_H

#include <stdio.h>
#include <string>
#include "environ.h"
#include "utc.h"
#ifdef MPI
#include "mpi.h"
#endif
#include "psr_cpp.h"

class MJD {

 private:
  int    days;
  int    secs;
  double fracsec;

 public:
  static int verbose;
  MJD () { days=0;secs=0;fracsec=0.0; };
  MJD (int dd, int ss, double fs);       // Create and verify an MJD
  MJD (int intday, double fracday);

  // construct from a string of the form "50312.4569"
  MJD (const char* mjdstring) {
    if (Construct (mjdstring) < 0)
      throw ("MJD::MJD(char*) construct error");
  };

  // some standard C time formats
  MJD (time_t time);               // returned by time()
  MJD (const struct tm& greg);     // returned by gmtime()
  MJD (const struct timeval& tp);  // returned by gettimeofday()

  // simple little struct invented in S2 days
  MJD (const utc_t& utc);

  MJD (double yy,double ss, double fs);

  // long double (from Sun) used by CPSR
  MJD (float128 mjd);

  // parses a string of the form 51298.45034 ish
  int Construct (const char* mjdstr);

  // constructs an MJD from the unix time_t
  int Construct (time_t time);

  // constructs an MJD from the unix struct tm
  int Construct (const struct tm& greg);

  // another UNIX time standard
  int Construct (const struct timeval& tp);

  // constructs an MJD from the home-grown utc_t
  int Construct (const utc_t& utc);

  // constructs from a BAT (binary atomic time), ie MJD in microseconds
  // stored in two 32 bit unsigned integers --- as returned by the AT clock
  int Construct (unsigned long long bat);

  double in_seconds() const;
  double in_days()    const;
  double in_minutes() const;

  // cast into other forms
  int UTC       (utc_t* utc, double* fsec=NULL) const;
  int gregorian (struct tm* gregdate, double* fsec=NULL) const;

  MJD & operator = (const MJD &);
  MJD & operator += (const MJD &);
  MJD & operator -= (const MJD &);
  MJD & operator += (const double &);
  MJD & operator -= (const double &);
  MJD & operator *= (const double &);
  MJD & operator /= (const double &);
  const friend MJD operator + (const MJD &, const MJD &);
  const friend MJD operator - (const MJD &, const MJD &);
  const friend MJD operator + (const MJD &, double);  // Add seconds to an MJD
  const friend MJD operator - (const MJD &, double);  // Take seconds from MJD
  const friend MJD operator * (const MJD &, double);  
  const friend MJD operator / (const MJD &, double);  
  friend int operator > (const MJD &, const MJD &);
  friend int operator < (const MJD &, const MJD &);
  friend int operator >= (const MJD &, const MJD &);
  friend int operator <= (const MJD &, const MJD &);
  friend int operator == (const MJD &, const MJD &);
  friend int operator != (const MJD &, const MJD &);

  // function to return plotable value to xyplot template class
  float plotval() const { return float (in_days()); };
  
  // These bits are useful for tempo
  int    intday()  const;                // To access the integer day
  double fracday() const;                // To access fractional day
  int    get_secs() const {return(secs);};
  double get_fracsec() const {return(fracsec);};

  // return LST in hours (longitude given in degrees)
  double LST (float longitude) const;

  string printdays (unsigned precision) const;

  int print (FILE *stream);
  int println (FILE *stream);
  char * printall()    const;
  char * printdays()   const;
  char * printhhmmss() const;
  char * printfs()     const;
  char * strtempo() const; 

  // returns a string formatted as in 'strftime()'
  char* datestr (char* dstr, int len, const char* format) const;

#ifdef MPI
  friend int mpiPack_size (const MJD&, MPI_Comm comm, int* size);
  friend int mpiPack   (const MJD&, void* outbuf, int outcount, 
			int* position, MPI_Comm comm);
  friend int mpiUnpack (void* inbuf, int insize, int* position, 
			MJD*, MPI_Comm comm);
#endif

};

inline ostream& operator<< (ostream& ostr, const MJD& sz) 
{ return ostr << sz.printdays(8); }

#endif  /* not __MJD_H defined */


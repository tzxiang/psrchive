//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/TwoBitStats.h,v $
   $Revision: 1.2 $
   $Date: 2003/10/08 14:17:38 $
   $Author: straten $ */

#ifndef __TwoBitStats_h
#define __TwoBitStats_h

#include "Pulsar/Archive.h"

namespace Pulsar {

  //! Stores histograms of two-bit low-voltage states
  /*! The TwoBitStats class contains histograms of the number of
    low-voltage states encountered in samples of a given length.
    Refer to the TwoBitCorrection class in baseband/dsp for further
    details.
  */

  class TwoBitStats : public Pulsar::Archive::Extension {

  public:
    
    //! Default constructor
    TwoBitStats ();

    //! Copy constructor
    TwoBitStats (const TwoBitStats& extension);

    //! Operator =
    const TwoBitStats& operator= (const TwoBitStats& extension);

    //! Clone method
    TwoBitStats* clone () const { return new TwoBitStats( *this ); }

    //! Set the number of samples per estimate and number of digitizers
    void resize (unsigned nsample, unsigned ndig);

    //! Get the number of digitizers
    unsigned get_ndig () const;

    //! Get the number of time samples used to estimate undigitized power
    unsigned get_nsample () const;

    //! Set the sampling threshold as a fraction of the noise power
    void set_threshold (float threshold);
    
    //! Get the sampling threshold as a fraction of the noise power
    float get_threshold () const { return threshold; }

    //! Set the cut off power used for impulsive interference excision
    void set_cutoff_sigma (float cutoff_sigma);
    
    //! Get the cut off power for impulsive interference excision
    float get_cutoff_sigma () const { return cutoff_sigma; }

    //! Get the specified histogram
    const vector<float>& get_histogram (unsigned idig) const;

    //! Set the specified histogram
    void set_histogram (const vector<float>&, unsigned idig);

    //! Set all histogram data to zero
    void zero ();

  protected:

    //! Number of digitizers
    unsigned ndig;

    //! Number of samples used to estimate undigitized power
    unsigned nsample;

    //! Sampling threshold as a fraction of the noise power
    float threshold;

    //! Cut off power used for impulsive interference excision
    float cutoff_sigma;

    //! Low-voltage state count histograms
    vector< vector<float> > histogram;

    //! Throw an exception if idig out of range
    void range_check (unsigned idig, const char* method) const;

  };
 
}

#endif

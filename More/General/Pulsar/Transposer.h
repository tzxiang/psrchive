//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/Transposer.h,v $
   $Revision: 1.3 $
   $Date: 2004/09/16 09:54:15 $
   $Author: straten $ */

#ifndef __Pulsar_Transposer_h
#define __Pulsar_Transposer_h

#include "Reference.h"
#include "Types.h"

namespace Pulsar {
  
  class Archive;
  class Integration;
  class Profile;
  class Dimensions;

  //! Provides access to transposed Archive data
  class Transposer {

  public:

    //! Default constructor
    Transposer (const Archive* archive = 0);

    //! Destructor
    ~Transposer ();

    //! Set the Archive from which data will be read
    void set_Archive (const Pulsar::Archive* archive);

    //! Set the specified dimension
    void set_dim (unsigned idim, Signal::Dimension dim);

    //! Get the size of the specified dimension
    unsigned get_ndim (unsigned idim);

    //! Get the stride
    Dimensions get_stride () const;

    //! Returns amplitude data; ordered according to the specified dimension
    void get_amps (vector<float>& amps) const;

    //! Fill the array according to the dimensions specified by stride
    void get_amps (const Integration*, float*, const Dimensions& stride) const;

    //! Fill the array according to the dimensions specified by stride, jbin
    void get_amps (const Profile*, float*, unsigned jbin) const;

    //! verbosity flag
    bool verbose;

  protected:

    void range_check (unsigned idim, const char* method) const;

    Reference::To<const Archive> archive;
    Signal::Dimension dim[4];

  };

  //! Provides enumerated access to the dimensions of an Archive
  class Dimensions {

  public:

    //! Default constructor
    Dimensions (const Pulsar::Archive* archive = 0);
    
    //! Get the size of the specified dimension
    unsigned get_ndim (Signal::Dimension axis) const;
    
  protected:

    friend class Transposer;

    //! Set the size of the specified dimension
    void set_ndim (Signal::Dimension axis, unsigned ndim);

    //! Set the size of the remaining dimension
    void set_if_zero (unsigned ndim);

    unsigned nsub;
    unsigned npol;
    unsigned nchan;
    unsigned nbin;

  };

}

#endif

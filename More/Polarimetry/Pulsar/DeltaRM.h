//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/DeltaRM.h,v $
   $Revision: 1.6 $
   $Date: 2007/09/24 11:29:17 $
   $Author: straten $ */

#ifndef __Pulsar_DeltaRM_h
#define __Pulsar_DeltaRM_h

#include "Reference.h"
#include "Estimate.h"

namespace Pulsar {

  class Archive;

  //! Refines an RM estimate using two halves of the band
  class DeltaRM  {

  public:

    //! Default constructor
    DeltaRM ();

    //! Destructor
    ~DeltaRM ();

    //! Set the rotation measure (initial guess)
    void set_rotation_measure (const Estimate<double>& rm)
    { rotation_measure = rm; }

    //! Get the rotation measure 
    Estimate<double> get_rotation_measure () const
    { return rotation_measure; }

    //! Get the number of phase bins used in last call to refine
    unsigned get_used_bins () { return used_bins; }

    //! Set the archive from which to derive the refined rotation measure
    void set_data (Archive*);

    //! Refine the rotation measure estimate
    void refine ();

    //! Set the cutoff threshold in units of the baseline noise
    void set_threshold (float t) { threshold = t; }
    float get_threshold () const { return threshold; }

  protected:

    float threshold;
    unsigned used_bins;

    //! The rotation measure
    Estimate<double> rotation_measure;

    //! The archive from which the rotation measure will be refined
    Reference::To<Archive> data;

  };

}

#endif

//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/CalibratorPlotter.h,v $
   $Revision: 1.7 $
   $Date: 2003/10/27 12:20:13 $
   $Author: straten $ */

#ifndef __CalibratorPlotter_H
#define __CalibratorPlotter_H

#include "Calibrator.h"

namespace Pulsar {

  //! Pure virtual base class of Pulsar::Calibrator plotters
  class CalibratorPlotter : public Reference::Able {
    
  public:
    //! Verbosity flag
    static bool verbose;

    //! Default constructor
    CalibratorPlotter ();
    
    //! Destructor
    virtual ~CalibratorPlotter ();

    //! Plot the Pulsar::Calibrator
    virtual void plot (const Calibrator* calibrator);

    //! Plot the Pulsar::Calibrator
    virtual void plot (const Calibrator::Info* info, unsigned nchan,
		       double centre_frequency, double bandwidth);

    //! Number of panels (vertical)
    unsigned npanel;

    //! Fraction of total plotting space placed between panels
    float between_panels;

    //! Use colour in plots
    bool use_colour;

  };

}

#endif

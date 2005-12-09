//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/CoherencyMeasurementSet.h,v $
   $Revision: 1.2 $
   $Date: 2005/12/09 16:41:06 $
   $Author: straten $ */

#ifndef __Calibration_CoherencyMeasurementSet_H
#define __Calibration_CoherencyMeasurementSet_H

#include "Calibration/CoherencyMeasurement.h"
#include "MEAL/Argument.h"
#include "Reference.h"

namespace Calibration {

  //! A CoherencyMeasurement set and their coordinates
  class CoherencyMeasurementSet : public std::vector<CoherencyMeasurement> {

  public:

    //! Default constructor
    CoherencyMeasurementSet (unsigned transformation_index = 0);

    //! Destructor
    ~CoherencyMeasurementSet ();

    //! Set the transformation through which the measurements are made
    void set_transformation_index (unsigned index);

    //! Set the transformation through which the measurements are made
    unsigned get_transformation_index () const;

    //! Add an independent variable
    void add_coordinate (MEAL::Argument::Value* abscissa);

    //! Apply the independent variables
    void set_coordinates () const;

  protected:

    //! Index of the transformation through which the measurements are made
    unsigned transformation_index;

    //! The coordinates of the measurement
    std::vector< Reference::To<MEAL::Argument::Value> > coordinates;

  };

}

#endif

//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/ReceptionModelAxisPlotter.h,v $
   $Revision: 1.4 $
   $Date: 2005/12/09 16:41:06 $
   $Author: straten $ */

#ifndef __Calibration_ReceptionModelAxisPlotter_H
#define __Calibration_ReceptionModelAxisPlotter_H

#include "Calibration/ReceptionModelPlotter.h"
#include "MEAL/Axis.h"
#include "Pauli.h"

#include <cpgplot.h>

namespace Calibration {

  //! Plots model Stokes parameters as a function of parallactic angle
  template<class Type>
  class ReceptionModelAxisPlotter : public ReceptionModelPlotter
  {

  public:

    ReceptionModelAxisPlotter () { axis = 0; npt = 100; }

    //! Set the Axis<Type> to which Parallactic model is connected
    void set_axis (MEAL::Axis<Type>* _axis) { axis = _axis; }

    //! Set the number of points in the plot
    void set_npt (unsigned _npt) { npt = _npt; }

    //! Set the minimum value on the axis
    void set_min (Type _min) { min = _min; }

    //! Set the maximum value on the axis
    void set_max (Type _max) { max = _max; }

    //! Plot the model in the current configuration
    void plot_model ();

  protected:

    //! The Axis<Type> to which Parallactic model is connected
    MEAL::Axis<Type>* axis;

    //! The number of points in the plot
    unsigned npt;

    //! The minimum value on the axis
    Type min;

    //! The maximum value on the axis
    Type max;

  };

}

/*! The Parallactic model must be connected to the specified Axis<Type> */
template<class Type>
void Calibration::ReceptionModelAxisPlotter<Type>::plot_model ()
{
  Type step = (max - min) / (npt - 1);

  model->set_input_index (isource);

  for (unsigned ipt=0; ipt<npt; ipt++) {

    Type index = min + step * ipt;

    axis->set_value (index);

    float pa = parallactic->get_param(0) * 180.0/M_PI;
    Stokes<double> stokes = coherency( model->evaluate () );

    if (ipt == 0)
      cpgmove (pa, stokes[ipol]);
    else
      cpgdraw (pa, stokes[ipol]);
    
  }
}


#endif


//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/Plotting/Pulsar/MultiPhase.h,v $
   $Revision: 1.9 $
   $Date: 2006/03/16 17:07:17 $
   $Author: straten $ */

#ifndef __Pulsar_MultiPhase_h
#define __Pulsar_MultiPhase_h

#include "Pulsar/MultiPlot.h"
#include "Pulsar/PhaseScale.h"

namespace Pulsar {

  //! Plots multiple viewports with pulse phase along the shared x-axis
  class MultiPhase : public MultiPlot {

  public:

    //! Default constructor
    MultiPhase ();

    //! Text interface to the MultiPhase class
    class Interface : public TextInterface::To<MultiPhase> {
    public:
      Interface (MultiPhase* = 0);
    };

    //! Get the text interface to the configuration attributes
    TextInterface::Class* get_interface ()
    { return new Interface (this); }

    //! Manage a plot
    void manage (const std::string& name, Plot* plot);

    //! Get the scale
    PhaseScale* get_scale ();

  };

}

#endif

//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/Gain.h,v $
   $Revision: 1.4 $
   $Date: 2005/04/06 20:23:36 $
   $Author: straten $ */

#ifndef __MEAL_Gain_H
#define __MEAL_Gain_H

#include "MEAL/Complex2.h"
#include "MEAL/Parameters.h"

namespace MEAL {

  //! A gain transformation
  class Gain : public Complex2 {

  public:

    //! Default constructor
    Gain ();

    // ///////////////////////////////////////////////////////////////////
    //
    // Function implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  protected:

    //! Calculate the Jones matrix and its gradient
    void calculate (Jones<double>& result, std::vector< Jones<double> >*);
   
  private:

    //! Parameter policy
    Parameters parameters;

  };

}

#endif

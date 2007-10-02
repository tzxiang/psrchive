//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/MEAL/MEAL/FunctionPolicy.h,v $
   $Revision: 1.5 $
   $Date: 2007/10/02 05:42:48 $
   $Author: straten $ */

#ifndef __MEAL_FunctionPolicy_H
#define __MEAL_FunctionPolicy_H

#include "ReferenceAble.h"

namespace MEAL {

  class Function;

  //! Abstract base class of Function policies
  class FunctionPolicy : public Reference::Able {

  public:

    //! Default constructor
    FunctionPolicy (Function* _context = 0) { context = _context; }

    //! Virtual destructor
    virtual ~FunctionPolicy () { }

  protected:

    //! Return the context to derived classes
    Function* get_context () const { return context; }

  private:

    //! The context
    Function* context;

  };

}

#endif

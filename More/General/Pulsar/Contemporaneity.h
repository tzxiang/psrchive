//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/Contemporaneity.h,v $
   $Revision: 1.1 $
   $Date: 2009/11/19 11:52:31 $
   $Author: straten $ */

#ifndef _Pulsar_Contemporaneity_H
#define _Pulsar_Contemporaneity_H

#include "Pulsar/Algorithm.h"
#include "Pulsar/Predictor.h"

namespace Pulsar {
  
  class Archive;
  class Integration;

  //! Evaluates the contemporaneity of Integrations from two Archives
  class Contemporaneity : public Algorithm 
  {
    
  public:
    
    //! Default constructor
    Contemporaneity ();
    
    //! Retrieve any additional information that may be required
    virtual void set_archives (const Archive*, const Archive*)=0;

    //! Return a fraction between 0 (no overlap) and 1 (complete overlap) 
    virtual double evaluate (const Integration*, const Integration*)=0;

    //! Contemporaneity at the pulsar, as determined by the phase model
    class AtPulsar;

    //! Contemporaneity at the Earth, as determined by the MJD
    class AtEarth;
  };

  class Contemporaneity::AtPulsar : public Contemporaneity
  {
  public:

    //! Retrieve any additional information that may be required
    void set_archives (const Archive*, const Archive*);

    //! Return a fraction between 0 (no overlap) and 1 (complete overlap) 
    double evaluate (const Integration*, const Integration*);

  protected:

    Reference::To<const Predictor> predA, predB;
  };

  class Contemporaneity::AtEarth : public Contemporaneity
  {
  public:

    //! Retrieve any additional information that may be required
    void set_archives (const Archive*, const Archive*);

    //! Return a fraction between 0 (no overlap) and 1 (complete overlap) 
    double evaluate (const Integration*, const Integration*);
  };
}

#endif

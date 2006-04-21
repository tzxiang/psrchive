//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2003 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Classes/Pulsar/IntegrationManager.h,v $
   $Revision: 1.13 $
   $Date: 2006/04/21 04:20:09 $
   $Author: straten $ */

#ifndef __Pulsar_IntegrationManager_h
#define __Pulsar_IntegrationManager_h

#include "Pulsar/Container.h"

namespace Pulsar {

  class Integration;

  //! Manages a vector of Integration instances
  /*! This pure virtual base class implements the storage and manipulation
    of a vector of Pulsar::Integration objects. */
  class IntegrationManager : public Container {

  public:

    //! A verbosity flag that can be set for debugging purposes
    static unsigned verbose;

    //! null constructor
    IntegrationManager ();

    //! copy constructor
    IntegrationManager (const IntegrationManager& archive);

    //! operator =
    IntegrationManager& operator = (const IntegrationManager& archive);

    //! destructor
    virtual ~IntegrationManager ();
    
    //! Return pointer to the specified Integration
    Integration* get_Integration (unsigned subint);

    //! Return a pointer to the last Integration
    Integration* get_last_Integration ();

    //! Return a pointer to the first Integration
    Integration* get_first_Integration ();
    
    //! Return const pointer to the specified Integration
    const Integration* get_Integration (unsigned subint) const;
    
    //! Return a const pointer to the last Integration
    const Integration* get_last_Integration () const;
    
    //! Return a const pointer to the first Integration
    const Integration* get_first_Integration () const;
    
    //! Construct new Integration instance
    virtual Integration* new_Integration (Integration* copy_this = 0) = 0;

    //! Load new Integration instance
    virtual Integration* load_Integration (unsigned isubint) = 0;

    //! Get the number of sub-integrations stored in the file
    /*! This attribute may be set only through IntegrationManager::resize */
    virtual unsigned get_nsubint () const = 0;

    //! Provides access to protected and private methods of IntegrationManager
    class Expert;

    //! Provide access to the expert interface
    Expert* expert ();

  protected:

    friend class TimeSortedOrder;
    friend class Expert;

    //! Set the number of sub-integrations
    /*! Called by IntegrationManager methods to update child attribute */
    virtual void set_nsubint (unsigned num_sub) = 0;

    //! Resize the Integration vector
    virtual void resize (unsigned nsubint, bool instances=true);

    //! Append new_Integration copies of Integration objects to self
    void append (const IntegrationManager* more_subints);

    //! Directly append the Integration instance (no copy)
    void manage (Integration* integration);

  private:

    //! The Integration vector
    /*!
      Access to Integrations must be made through the
      Pulsar::IntegrationManager::get_Integration method
    */
    std::vector< Reference::To<Integration> > subints;

    //! Expert interface
    Reference::To<Expert> expert_interface;

  };

}

#endif

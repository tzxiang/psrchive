//-*-C++-*-
                                                                                
/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/PeriastronOrder.h,v $
   $Revision: 1.1 $
   $Date: 2003/12/29 10:43:20 $
   $Author: ahotan $ */
                                                                                
#ifndef __PeriastronOrder_h
#define __PeriastronOrder_h
                                                                                
#include "Pulsar/Archive.h"
#include "Pulsar/Integration.h"
#include "Pulsar/IntegrationOrder.h"
                                                                            
namespace Pulsar {
  
  /*! This PeriastronOrder class provides a means for developers to
    re-index the way Integrations are stored. It is a virtual base class
    that must be inherited by developers who wish to define a new way to
    index the Integrations in their Archive */
  
  class PeriastronOrder : public Pulsar::IntegrationOrder {
                                                                                
  public:
    
    // These are the standard Archive::Extension methods:

    //! Default constructor
    PeriastronOrder ();
    
    //! Destructor
    ~PeriastronOrder ();
    
    //! Copy constructor
    PeriastronOrder (const PeriastronOrder& extension);
    
    //! Operator =
    const PeriastronOrder& operator= (const PeriastronOrder& extension);
    
    //! Clone method
    IntegrationOrder* clone () const;
    
    void organise (Archive*);
    void append   (Archive*, const Archive*);    
    void combine  (Archive*, unsigned);

  };                                                                            
  
}
                                                                                
#endif

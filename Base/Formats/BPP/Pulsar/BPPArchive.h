//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2007 by Paul Demorest
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __BPPArchive_h
#define __BPPArchive_h

#include "Pulsar/BasicArchive.h"
#include "Pulsar/Agent.h"

// BPP header struct
#include <bpp_header.h>

namespace Pulsar {

  //! Loads and unloads BPP Pulsar archives
  /*! Class for loading Berkeley Pulsar Processor (BPP) data files.
    Based on ExampleArchive class.  Probably will not have unloading
    functionality ever. */
  class BPPArchive : public BasicArchive {

  public:
    
    //! Default constructor
    BPPArchive ();

    //! Copy constructor
    BPPArchive (const BPPArchive& archive);

    //! Destructor
    ~BPPArchive ();

    //! Assignment operator
    const BPPArchive& operator = (const BPPArchive& archive);
    
    //! Base copy constructor
    BPPArchive (const Archive& archive);

    //! Base extraction constructor
    BPPArchive (const Archive&, const std::vector<unsigned>& subint);

    //! Copy all of the class attributes and the selected Integration data
    void copy (const Archive& archive, const std::vector<unsigned>& subints);

    //! Return a new copy-constructed BPPArchive instance
    BPPArchive* clone () const;

    //! Return a new extraction-constructed BPPArchive instance
    BPPArchive* extract (const std::vector<unsigned>& subints) const;
    
  protected:

    //! Correct the RFs array from the file.
    void fix_orig_rfs();

    //! Convert raw header data to big endian
    void hdr_to_big_endian ();

    //! Load the BPP header information from filename
    virtual void load_header (const char* filename);

    //! Get integer MJD out of the BPP header.
    int get_mjd_from_hdr();

    //! Load the specified Integration from filename, returning new instance
    virtual Integration*
    load_Integration (const char* filename, unsigned subint);

    //! Unload the BPPArchive (header and Integration data) to filename.
    //! Not implemented.
    virtual void unload_file (const char* filename) const;

    // Advocates the use of the BPPArchive plugin
    class Agent;

    //! Enable Advocate template class to access protected Agent class
    friend class Archive::Advocate<BPPArchive>;

  private:

    //! The raw BPP header struct
    bpp_header hdr;

    //! The original RFs array from the file.  NOTE: Don't use these
    //! unless orig_rfs_corrected has been set.
    double *orig_rfs;
    int orig_rfs_corrected;

    //! Initialize all values to null
    void init ();

  };
 

  // Advocates the use of the BPPArchive plugin
  class BPPArchive::Agent : public Archive::Advocate<BPPArchive> {
    
  public:
    
    Agent () { } 
    
    //! Advocate the use of BPPArchive to interpret filename
    bool advocate (const char* filename);
    
    //! Return the name of the BPPArchive plugin
    std::string get_name () { return "BPPArchive"; }
    
    //! Return description of this plugin
    std::string get_description ();
    
  };


}

#endif
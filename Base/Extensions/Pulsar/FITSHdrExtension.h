//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/FITSHdrExtension.h,v $
   $Revision: 1.4 $
   $Date: 2003/10/08 14:17:38 $
   $Author: straten $ */

#ifndef __FITSHdrExtension_h
#define __FITSHdrExtension_h

#include "Pulsar/Archive.h"

namespace Pulsar {
  
  //! FITSArchive specific seader extensions
  
  class FITSHdrExtension : public Pulsar::Archive::Extension {
    
  public:
    
    //! Default constructor
    FITSHdrExtension ();

    //! Copy constructor
    FITSHdrExtension (const FITSHdrExtension& extension);

    //! Operator =
    const FITSHdrExtension& operator= (const FITSHdrExtension& extension);
    
    //! Destructor
    ~FITSHdrExtension ();

    //! Clone method
    FITSHdrExtension* clone () const { return new FITSHdrExtension( *this ); }

    //! Return two strings based on coordmode attribute
    void get_coord_string (const sky_coord& coordinates,
			   string& coord1, string& coord2) const;


    //////////////////////////////////////////////////////////////////////
    
    //! Header start time (as opposed to subint start time)
    MJD start_time;
    
    //! Pulsar FITS header version information
    string hdrver;
    
    //! File creation date
    string creation_date;

    //! Coordinate mode (J2000, Gal, Ecliptic, AZEL, HADEC)
    string coordmode;
    
    //! Track mode (TRACK, SCANGC, SCANLAT)
    string trk_mode;

    //! Start UT date (YYYY-MM-DD)
    string stt_date;

    //! Start UT (hh:mm:ss)
    string stt_time;

    //! Start LST
    double stt_lst;
    
  };
  
}

#endif


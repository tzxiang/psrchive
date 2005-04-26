//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/genutil/Attic/Telescope.h,v $
   $Revision: 1.4 $
   $Date: 2005/04/26 13:07:29 $
   $Author: straten $ */

#ifndef __Telescope_h
#define __Telescope_h

//! Tempo telescope codes
namespace Telescope {

  const char Parkes = '7';
  const char ATCA = '2';
  const char Tidbinbilla = '6';
  const char Arecibo = '3';
  const char Hobart = '4';
  const char GreenBank = '1';
  const char WSRT = 'i';

  //! Convert a telescope name to a code
  char code (const char* telescope_name);

  //! Convert a telecope code to a name
  const char* name (char code);

}

#endif

/* $Source: /cvsroot/psrchive/psrchive/Util/genutil/coord_parse.h,v $
   $Revision: 1.1 $
   $Date: 2004/11/30 16:06:19 $
   $Author: straten $ */

#ifndef __COORD_H
#define __COORD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************
   str2coord -
	converts a string with RA and DEC in their human notations:
	RA in time-measure (1h = 15 deg)
	DEC in sexagesimal angular measure
	into an ra and dec in radians
   ******************************************************************** */
  int str2coord (double *ra, double *dec, const char* coordstr);
  int str2ra  (double *ra,  const char* rastr);
  int str2dec (double *dec, const char* decstr);
  
  int coord2str (char* coordstring, unsigned strlen,
		 double ra, double dec, unsigned places);

  int ra2str (char* rastr, unsigned rastrlen, double ra, unsigned places);
  int dec2str (char* decstr, unsigned decstrlen, double dec, unsigned places);

#ifdef __cplusplus
	   }
#endif

#endif

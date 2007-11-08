//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Util/fitsutil/psrfitsio.h,v $
   $Revision: 1.8 $
   $Date: 2007/11/08 03:34:45 $
   $Author: nopeer $ */

#ifndef __psrfitsio_h
#define __psrfitsio_h

#include "FITSError.h"
#include "fitsutil.h"

#include <fitsio.h>

#include <string>
#include <vector>

//! Remove any existing rows from the current binary table
void psrfits_clean_rows (fitsfile*);

//! Empty template class requires specialization 
template<typename T> struct FITS_traits { };
  
//! Template specialization for double
template<> struct FITS_traits<double> {
  static inline int datatype() { return TDOUBLE; }
};

//! Template specialization for float
template<> struct FITS_traits<float> {
  static inline int datatype() { return TFLOAT; }
  static inline float null () { return fits_nullfloat; }
};

//! Template specialization for int
template<> struct FITS_traits<int> {
  static inline int datatype() { return TINT; }
};

//! Template specialization for long
template<> struct FITS_traits<long> {
  static inline int datatype() { return TLONG; }
};

//! Calls fits_update_key; throws a FITSError exception if status != 0
template<typename T>
void psrfits_update_key (fitsfile* fptr, const char* name, T data)
{
  // no comment
  char* comment = 0;
  // status
  int status = 0;
  
  fits_update_key (fptr, FITS_traits<T>::datatype(),
		   const_cast<char*>(name), &data,
		   comment, &status);
  
  if (status)
    throw FITSError (status, "psrfits_update_key", name);
}

//! Specialization for string
void psrfits_update_key (fitsfile* fptr, const char* name,
			 const std::string& data);

//! Specialization for C string constants
void psrfits_update_key (fitsfile* fptr, const char* name, const char* data);

//! Worker function does not handle status
template<typename T>
void psrfits_read_key_work (fitsfile* fptr, const char* name, T* data, 
			    int* status)
{
  // no comment
  char* comment = 0;
  
  fits_read_key (fptr, FITS_traits<T>::datatype(), 
		 const_cast<char*>(name), data,
		 comment, status);
}

//! Specialization for string
void psrfits_read_key_work (fitsfile* fptr, const char* name, std::string*,
			    int* status);

//! Calls fits_read_key; throws a FITSError exception if status != 0
template<typename T>
void psrfits_read_key (fitsfile* fptr, const char* name, T* data)
{
  // status
  int status = 0;
  psrfits_read_key_work (fptr, name, data, &status);
  if (status)
    throw FITSError (status, "psrfits_read_key", name);
}

//! Calls fits_read_key; sets data to default if status != 0
template<typename T>
void psrfits_read_key (fitsfile* fptr, const char* name, T* data,
		       T dfault, bool verbose = false)
{
  // status
  int status = 0;
  psrfits_read_key_work (fptr, name, data, &status);
  if (status) {
    if (verbose) {
      FITSError error (status, "psrfits_read_key", name);
      std::cerr << error.get_message() << std::endl;
      std::cerr << "psrfits_read_key: using default="<< dfault <<std::endl;
    }
    *data = dfault;
  }
}


template<typename T>
void psrfits_write_col (fitsfile* fptr, const char* name, std::vector<T>& data,
		       int row = 1)
{
  //
  // Get the number of the named column

  int colnum = 0;
  int status = 0;

  fits_get_colnum (fptr, CASEINSEN, const_cast<char*>(name), &colnum, &status);

  fits_modify_vector_len (fptr, colnum, data.size(), &status);

  fits_write_col (fptr, FITS_traits<T>::datatype(),
		  colnum, row,
		  1, data.size(),
		  &(data[0]), &status);

  if (status)
    throw FITSError (status, "psrfits_write_col(vector<T>)", name);
}

template<typename T>
    void psrfits_write_col( fitsfile *fptr, const char *name, T &data, int row = 1 )
{
  int colnum = 0;
  int status = 0;
  
  fits_get_colnum (fptr, CASEINSEN, const_cast<char*>(name), &colnum, &status );
  
  fits_write_col (fptr, FITS_traits<T>::datatype(),
		  colnum, row,
		  1, 1,
		  &data, &status);

  if (status)
    throw FITSError (status, "psrfits_write_col(T)", name);
}

template<typename T>
void psrfits_read_col (fitsfile* fptr, const char* name, std::vector<T>& data,
		       int row = 1, T null = FITS_traits<T>::null())
{
  //
  // Get the number of the named column

  int colnum = 0;
  int status = 0;

  fits_get_colnum (fptr, CASEINSEN, const_cast<char*>(name), &colnum, &status);

  int anynul = 0;
  fits_read_col (fptr, FITS_traits<T>::datatype(),
		 colnum, row,
		 1, data.size(),
		 &null, &(data[0]),
		 &anynul, &status);

  if (status)
    throw FITSError (status, "psrfits_read_col", name);
}


template< typename T >
    void psrfits_read_col ( fitsfile *fptr, const char *name, T *data, int row = 1, T null = FITS_traits<T>::null )
{ 
  int colnum = 0;
  int status = 0;

  fits_get_colnum (fptr, CASEINSEN, const_cast<char*>(name), &colnum, &status);
  
  int anynul = 0;
  fits_read_col( fptr, FITS_traits<T>::datatype(),
		 colnum, row,
		 1, 1, &null, data, 
		 &anynul, &status );
  
  if( status )
    throw  FITSError ( status, "psrfits_read_col", name );
}

#endif

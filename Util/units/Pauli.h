//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/Util/units/Pauli.h,v $
   $Revision: 1.10 $
   $Date: 2003/12/26 11:27:08 $
   $Author: straten $ */

#ifndef __Pauli_H
#define __Pauli_H

#include "Jones.h"
#include "Quaternion.h"

#include <vector>

// convert Hermitian Quaternion to Jones matrix
template<typename T>
const Jones<T> convert (const Quaternion<complex<T>,Hermitian>& q)
{
  return Jones<T> (q.s0+q.s1, q.s2-ci(q.s3),
		   q.s2+ci(q.s3), q.s0-q.s1);
}

// convert Unitary Quaternion to Jones matrix
template<typename T>
const Jones<T> convert (const Quaternion<complex<T>,Unitary>& q)
{
  return Jones<T> (q.s0+ci(q.s1), q.s3+ci(q.s2),
		   -q.s3+ci(q.s2), q.s0-ci(q.s1));
}

// convert Hermitian Quaternion to Jones matrix
template<typename T>
const Jones<T> convert (const Quaternion<T,Hermitian>& q)
{
  return Jones<T> (q.s0+q.s1, q.s2-ci(q.s3),
		   q.s2+ci(q.s3), q.s0-q.s1);
}

// convert Unitary Quaternion to Jones matrix
template<typename T>
const Jones<T> convert (const Quaternion<T,Unitary>& q)
{
  return Jones<T> (q.s0+ci(q.s1), q.s3+ci(q.s2),
		   -q.s3+ci(q.s2), q.s0-ci(q.s1));
}

// convert coherency vector to Jones matrix
template<typename T>
const Jones<T> convert (const vector<T>& c)
{
  if (c.size() != 4)
    throw Error (InvalidParam, "Jones<T> convert (vector<T>)",
		 "vector.size=%d != 4", c.size());

  return Jones<T> (c[0], complex<T> (c[2], -c[3]),
		   complex<T> (c[2], c[3]), c[1]);
}

// convert Jones matrix to Hermitian Biquaternion
template<typename T>
const Quaternion<complex<T>, Hermitian> convert (const Jones<T>& j)
{
  return Quaternion<complex<T>, Hermitian>
    ( T(0.5) * (j.j11 + j.j22),
      T(0.5) * (j.j11 - j.j22),
      T(0.5) * (j.j12 + j.j21),
      T(0.5) * ci (j.j12 - j.j21) );
}

// convert Jones matrix to Hermitian Biquaternion
template<typename T>
const Quaternion<complex<T>, Unitary> unitary (const Jones<T>& j)
{
  return Quaternion<complex<T>, Unitary>
    ( T(0.5) *     (j.j11 + j.j22),
      T(-0.5) * ci (j.j11 - j.j22),
      T(-0.5) * ci (j.j12 + j.j21),
      T(0.5) *     (j.j12 - j.j21) );
}

// convert Jones matrix to Hermitian and Unitary Quaternion
template<typename T>
void polar (complex<T>& d, Quaternion<T, Hermitian>& h,
	    Quaternion<T, Unitary>& u, Jones<T> j)
{
  // make j unimodular
  d = sqrt (det(j));
  j /= d;

  // calculate the hermitian
  h = sqrt( real( convert( j*herm(j) ) ) );

  // remove the hermitian component from j
  j = inv(convert(h)) * j;

  // take the unitary component out of j
  u = real ( unitary (j) );
}


// multiply a Jones matrix by a Quaternion
template<typename T, typename U, Basis B>
const Jones<T> operator * (const Jones<T>& j, const Quaternion<U,B>& q)
{
  return j * convert(q);
}

// multiply a Jones matrix by a Quaternion
template<typename T, typename U, Basis B>
const Jones<T> operator * (const Quaternion<T,B>& q, const Jones<U>& j)
{
  return convert(q) * j;
}

// multiply Quaternions from different Basis
template<typename T, typename U, Basis A, Basis B>
const Jones<T> operator * (const Quaternion<T,A>& q, const Quaternion<U,B>& u)
{
  return convert(q) * convert(u);
}


#endif

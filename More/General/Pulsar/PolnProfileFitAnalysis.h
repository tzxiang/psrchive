//-*-C++-*-

/* $Source: /cvsroot/psrchive/psrchive/More/General/Pulsar/Attic/PolnProfileFitAnalysis.h,v $
   $Revision: 1.5 $
   $Date: 2005/09/14 12:04:49 $
   $Author: straten $ */

#ifndef __Pulsar_PolnProfileFitAnalysis_h
#define __Pulsar_PolnProfileFitAnalysis_h

#include "Pulsar/PolnProfileFit.h"
#include "Jones.h"

#include <vector>

namespace Pulsar {

  //! Analysis of the matrix template matching algorithm
  class PolnProfileFitAnalysis {

  public:

    //! Set the PolnProfileFit algorithm to be analysed
    void set_fit (PolnProfileFit*);

    //! Get the normalized arrival time error
    Estimate<double> get_normalized_error () const;

    //! Get the multiple correlation between phase shift and Jones parameters
    Estimate<double> get_multiple_correlation () const;

  protected:

    //! The normalized arrival time error
    Estimate<double> normalized_error;

    //! The multiple correlation between phase shift and Jones parameters
    Estimate<double> multiple_correlation;

    //! The PolnProfileFit algorithm to be analysed
    Reference::To<PolnProfileFit> fit;

    //! The partial derivative of the model gradient wrt Re[S_k]
    Jones<double> delgradient_delS (unsigned index, unsigned k) const;

    //! The partial derivative of the multiple correlation squared wrt S_k
    double delR2_varphiJ_delS (Matrix<8,8,double>& delC_delS);

    //! The partial derivative of the covariance matrix wrt Re[S_k] and Im[S_k]
    void delC_delS( Matrix<8,8,double>& delC_delSre,
		    Matrix<8,8,double>& delC_delSim, unsigned k ) const;

    //! Get the curvature matrix
    void get_curvature (Matrix<8,8,double>& curvature);

    //! Set the model up to evaluate the specified harmonic
    void set_harmonic (unsigned index);

    Jones<double> model_result;
    std::vector< Jones<double> > model_gradient;

    Jones<double> xform_result;
    std::vector< Jones<double> > xform_gradient;

    Jones<double> phase_result;
    std::vector< Jones<double> > phase_gradient;

    //! the covariance matrix
    Matrix<8,8,double> covariance;

    //! the covariances between the Jones matrix parameters
    Matrix <7,7,double> C_JJ;

    //! the inverse of the covariances between the Jones matrix parameters
    Matrix <7,7,double> inv_C_JJ;

    //! the covariances between the phase shift and each Jones matrix parameter
    Vector <7,double> C_varphiJ;
    
    //! the variance of the unconditional phase shift
    double c_varphi;

    //! the multiple correlation squared
    double R2_varphiJ;

    double mean_variance;

    double efac;

  };


  //! Analysis of the scalar template matching algorithm
  class ScalarProfileFitAnalysis {

  public:

    //! Set the PolnProfileFit that refers to the standard
    void set_fit (PolnProfileFit*);

    //! Get the curvature matrix
    void get_curvature (Matrix<2,2,double>& curvature);

    //! The partial derivative of the covariance matrix wrt Re[S_0] and Im[S_0]
    void delC_delS ( Matrix<2,2,double>& delC_delSre,
		     Matrix<2,2,double>& delC_delSim,
		     unsigned index ) const;

  protected:

    //! The PolnProfileFit algorithm to be analysed
    Reference::To<PolnProfileFit> fit;

    Matrix<2,2,double> covariance;
    double mean_variance;
  };

}

#endif

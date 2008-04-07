//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/More/Polarimetry/Pulsar/ReceptionModel.h,v $
   $Revision: 1.13 $
   $Date: 2008/04/07 00:38:18 $
   $Author: straten $ */

#ifndef __ReceptionModel_H
#define __ReceptionModel_H

#include "Pulsar/MeasurementEquation.h"
#include "MEAL/SumRule.h"
#include "Estimate.h"

namespace Calibration {

  class CoherencyMeasurementSet;

  //! Models a set of transformations and source polarizations
  /*! This class models the output polarization of an arbitrary number of
    both input polarization states and signal paths.  Each signal path
    is represented by a MeasurementEquation. */

  class ReceptionModel : public MeasurementEquation {

  public:

    //! null constructor
    ReceptionModel ();

    //! destructor
    ~ReceptionModel ();

    //! Additional input, \f$\rho_i,k\f$, where \f$i\f$=transformation_index
    void add_to_output (unsigned transformation_index, Complex2* state);

    //! Add a transformation, \f$J_M\f$, where \f$M\f$ = get_num_transformation
    void add_transformation (Complex2* state = 0);

    // ///////////////////////////////////////////////////////////////////
    //
    // Add data to the model
    //
    // ///////////////////////////////////////////////////////////////////

    //! Checks that each CoherencyMeasurement has a valid source_index
    virtual void add_data (CoherencyMeasurementSet& data);

    //! Get the number of CoherencyMeasurementSet
    unsigned get_ndata () const;

    //! Get the specified CoherencyMeasurementSet
    const CoherencyMeasurementSet& get_data (unsigned idata) const;

    //! Delete all data
    void delete_data ();

    // ///////////////////////////////////////////////////////////////////
    //
    // Add conditions the model
    //
    // ///////////////////////////////////////////////////////////////////

    //! Add a convergence condition
    void add_convergence_condition( Functor< bool(ReceptionModel*) > );

    //! Add an acceptance condition
    void add_acceptance_condition( Functor< bool(ReceptionModel*) > );

    // ///////////////////////////////////////////////////////////////////
    //
    // Fit the model
    //
    // ///////////////////////////////////////////////////////////////////

    //! solve_work should report the reduced chisq on completion
    static bool report_chisq;

    //! Solve the measurement equations by least squares minimization
    void solve ();

    //! Set the maximum number of iterations in fit algorithm
    void set_fit_maximum_iterations (unsigned maximum_iterations);

    //! Set the convergence threshold
    void set_fit_convergence_threshold (float delta_chi_squared,
					bool exact_solution = false);


    //! Set the reduced chi-squared above which the fit is considered bad
    void set_fit_maximum_reduced (float maximum_reduced_chi_squared);

    //! Set the fit report flag
    void set_fit_report (bool flag = true) { fit_report = flag; }

    //! Set the verbosity during solve
    void set_fit_debug (bool flag = true) { fit_debug = flag; }

    //! Get the covariance matrix of the last fit
    void get_fit_covariance (std::vector< std::vector<double> >&) const;

    //! The number of iterations in last call to solve method
    unsigned get_fit_iterations () const;

    //! The chi-squared in last call to solve method
    float get_fit_chisq () const;

    //! The number of free parameters in last call to solve method
    unsigned get_fit_nfree () const;

    //! Return true if the solve method has been called
    bool get_fit_solved () const;

    //! Copy the fitted parameters from another model
    void copy_fit (const ReceptionModel*);

    // ///////////////////////////////////////////////////////////////////
    //
    // OptimizedModel implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Returns \f$ \rho^\prime \f$ and its gradient
    void calculate (Jones<double>& result, std::vector<Jones<double> >* grad);

    // ///////////////////////////////////////////////////////////////////
    //
    // Model implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Return the name of the class
    std::string get_name () const;

  protected:

    //! The maximum number of iterations in during fit
    unsigned maximum_iterations;

    //! The convergence threshold;
    float convergence_threshold;

    //! Additional convergence conditions
    std::vector< Functor< bool(ReceptionModel*) > > convergence_condition;

    //! Additional acceptance conditions
    std::vector< Functor< bool(ReceptionModel*) > > acceptance_condition;

    //! The maximum reduced chi-squared allowed
    float maximum_reduced;

    //! Flag set when measurements have no experimental error (simulation)
    bool exact_solution;

    //! The number of iterations in last call to solve method
    unsigned iterations;

    //! The best chi-squared in last call to solve method
    float best_chisq;

    //! The number of free parameters in last call to solve method
    unsigned nfree;

    //! The fit debug mode
    bool fit_debug;

    //! When set, the solve method will report parameter names, free parameters, etc.
    bool fit_report;

    //! Ensure that idata <= get_ndata()
    void range_check (unsigned idata, const char* method) const;

    //! The output for a given signal path
    std::vector< MEAL::SumRule<MEAL::Complex2> > output;

 private:

    //! Observations used to constrain the measurement equations
    std::vector<CoherencyMeasurementSet> data;

    //! Used in multi-threaded solution of measurement equations
    static void* solve_thread (void* instance);

    //! Wait to add to_solve as an active thread
    static void solve_wait (ReceptionModel* to_solve);

    bool top_calculate;

    //! The covariance matrix set after fitting
    std::vector< std::vector<double> > covariance;

    bool is_solved;
  };


}

#endif


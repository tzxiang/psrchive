//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/psrchive/psrchive/Base/Extensions/Pulsar/Receiver.h,v $
   $Revision: 1.16 $
   $Date: 2006/04/16 13:27:26 $
   $Author: straten $ */

#ifndef __ReceiverExtension_h
#define __ReceiverExtension_h

#include "Pulsar/Archive.h"
#include "Jones.h"
#include "Stokes.h"
#include "Angle.h"

namespace Pulsar {
  
  //! Contains information about the receiver and receiver platform

  class Receiver : public Archive::Extension {

  public:
    
    //! Construct a new Receiver instance from a file
    static Receiver* load (const string& filename);

    //! Construct a new Receiver instance from a string
    static Receiver* new_Receiver (const string& text);

    //! Default constructor
    Receiver ();

    //! Copy constructor
    Receiver (const Receiver& extension);

    //! Operator =
    const Receiver& operator= (const Receiver& extension);

    //! Destructor
    ~Receiver ();

    //! Clone method
    Receiver* clone () const { return new Receiver(*this); }

    //! Parses the value of a Receiver attribute from a string
    void parse (string text);

    //! Different modes of feed rotational tracking
    enum Tracking {
      //! Constant feed angle
      Feed,
      //! Constant Celestial position angle
      Celestial,
      //! Constant Galactic position angle
      Galactic
    };

    //! Get the tracking mode of the receiver platform
    Tracking get_tracking_mode () const { return tracking_mode; }
    //! Set the tracking mode of the receiver platform
    void set_tracking_mode (Tracking mode) { tracking_mode = mode; }

    //! Get the position angle tracked by the receiver
    Angle get_tracking_angle () const { return tracking_angle; }
    //! Get the position angle tracked by the receiver
    void set_tracking_angle (const Angle& angle) { tracking_angle = angle; }

    //! Get the name of the receiver
    const string get_name () const { return name; }
    //! Set the name of the receiver
    void set_name (const string& _name) { name = _name; }

    //! Get the basis of the feed receptors
    Signal::Basis get_basis () const { return state->get_basis(); }
    //! Set the basis of the feed receptors
    void set_basis (Signal::Basis basis);

    /** @name General orthogonal basis interface
     *  These parameters describe the configuration of a receiver with
     *  ideal receptors.
     */
    //@{

    //! Get the orientation of the basis about the line of sight
    Angle get_orientation () const { return state->get_orientation(); }
    //! Set the orientation of the basis about the line of sight
    void set_orientation (const Angle& celestial_position_angle);

    //! Get the hand of the basis
    Signal::Hand get_hand () const { return state->get_hand(); }
    //! Set the hand of the basis
    void set_hand (Signal::Hand);
 
    //! Get the phase of the reference source
    Angle get_reference_source_phase () const
    { return state->get_reference_source_phase(); }
    //! Set the phase of the reference source
    void set_reference_source_phase (const Angle& phase);

    //@}

    /** @name Basis-dependent interface
     *  The interpretation of these parameters depends upon the basis.
     */
    //@{

    //! Get the orientation of the equal in-phase electric field vector
    Angle get_field_orientation () const;
    //! Set the orientation of the equal in-phase electric field vector
    void set_field_orientation (const Angle& celestial_position_angle);

    //@}

    /** @name Linear basis-specific interface
     *  These parameters describe the configuration of a receiver with
     *  ideal, linearly polarized receptors.
     */
    //@{

    //! Get the offset of the feed X axis with respect to the platform zero
    Angle get_X_offset () const;
    //! Set the offset of the feed X axis with respect to the platform zero
    void set_X_offset (const Angle& offset);

    //! Get the offset of the feed Y axis from its nominal value
    Angle get_Y_offset () const;
    //! Set the offset of the feed Y axis from its nominal value
    void set_Y_offset (const Angle& offset);

    //! Get the offset of the feed calibrator axis from its nominal value
    Angle get_calibrator_offset () const;
    //! Set the offset of the feed calibrator axis from its nominal value
    void set_calibrator_offset (const Angle& offset);

    //@}

    //! Get the flag set when the offset of the feed has been corrected
    bool get_feed_corrected () const { return feed_corrected; }
    //! Set the flag set when the offset of the feed has been corrected
    void set_feed_corrected (bool val) { feed_corrected = val; }

    //! Get if platform to sky transformation has been corrected
    bool get_platform_corrected () const { return platform_corrected; }
    //! Set when platform to sky transformation has been corrected
    void set_platform_corrected (bool val) { platform_corrected = val; }

    //! Get the attenuator, Poln A
    float get_atten_a () const { return atten_a; }
    //! Set the attenuator, Poln A
    void set_atten_a (float atten) { atten_a = atten; }

    //! Get the attenuator, Poln B
    float get_atten_b () const { return atten_b; }
    //! Set the attenuator, Poln B
    void set_atten_b (float atten) { atten_b = atten; }

    //! Return true if the receiver configurations match
    bool match (const Receiver* receiver, string& reason) const;

    //! Return a string that describes the tracking mode
    string get_tracking_mode_string() const;

    //! Return the feed correction matrix
    Jones<double> get_transformation () const;

    //! Return the Stokes parameters of the reference source
    Stokes<double> get_reference_source () const;

    //! The internal state of the Receiver Extension class
    class State : public Reference::Able {
    public:

      //! Get the basis of the feed receptors
      virtual Signal::Basis get_basis () const = 0;

      //! Get the hand of the basis
      virtual Signal::Hand get_hand () const = 0;

      //! Get the orientation of the basis about the line of sight
      virtual Angle get_orientation () const = 0;


      //! Get the phase of the reference source
      virtual Angle get_reference_source_phase () const = 0;

      //! Copy from another State
      virtual void copy (const State*) = 0;

    };

    class Native;
    class Linear;
    class Field;

  protected:

    template<class StateType> StateType* get() const;

    //! Name of the receiver
    string name;

    //! State of the receiver
    Reference::To<State> state;

    //! The tracking mode of the receiver platform
    Tracking tracking_mode;

    //! The rotation angle tracked by the feed
    /*! The interpretation of this angle depends on the mode */
    Angle tracking_angle;

    //! Flag set when the offset of the feed has been corrected
    /*! This flag should be set when the offset of the feed X and Y
      axes and any rotation of the feed (tracking_angle) with respect to
      the platform zero have been corrected. */
    bool feed_corrected;

    //! Flag set when platform to sky transformation has been corrected
    /*! For a horizon mounted antenna, this flag should be set when
      the rotation of the platform zero, known as the vertical angle or
      parallactic angle, has been corrected. */
    bool platform_corrected;

    //! Attenuator, Poln A
    /*! The software currently does nothing with this value */
    float atten_a;

    //! Attenuator, Poln B
    /*! The software currently does nothing with this value */
    float atten_b;

    //! Get the tranformation arising from the handedness
    Jones<double> get_hand_transformation () const;

  };

}

#endif

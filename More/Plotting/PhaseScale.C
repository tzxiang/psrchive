/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/PhaseScale.h"
#include "Pulsar/Archive.h"
#include "Pulsar/Integration.h"

using namespace std;

Pulsar::PhaseScale::PhaseScale ()
{
  units = Turns;
  origin_norm = 0;
}

void Pulsar::PhaseScale::init (const Archive* data)
{
  float scale = get_scale (data);

  float min = origin_norm;
  float max = 1 + origin_norm;

  min *= scale;
  max *= scale;

  set_minmax (min, max);
}

void Pulsar::PhaseScale::get_indeces (const Archive* data, 
				      unsigned& min, unsigned& max) const
{
  bool cyclic = true;
  PlotScale::get_indeces (data->get_nbin(), min, max, cyclic);
}


void Pulsar::PhaseScale::get_ordinates (const Archive* data,
					std::vector<float>& axis) const
{
  float scale = 1.0;
 
  if (data != 0) {
    axis.resize (data->get_nbin());
    scale = get_scale (data);
  }

  for (unsigned ibin = 0; ibin < axis.size(); ibin++)
    axis[ibin] = scale * (origin_norm + (float(ibin) + 0.5) / axis.size());
}


float Pulsar::PhaseScale::get_scale (const Archive* data) const
{
  if (units == Milliseconds && data != 0)
    return data->get_Integration(0)->get_folding_period() * 1e3;

  else if (units == Radians)
    return 2.0 * M_PI;

  else if (units == Degrees)
    return 360.0;

  else if (units == Bins)
    return data->get_nbin();

  else
    return 1.0;
}

//! Get the default label for the x axis
std::string Pulsar::PhaseScale::get_label () const
{
  switch (units) {
  case Turns: return "Pulse Phase";
  case Degrees: return "Phase (deg.)";
  case Radians: return "Phase (rad.)";
  case Milliseconds: return "Time (ms)";
  case Bins: return "Bin Number";
  default: return "Unknown";
  }
}

std::ostream& Pulsar::operator << (std::ostream& os, PhaseScale::Units units)
{
  switch (units) {
  case PhaseScale::Turns:
    return os << "turn";
  case PhaseScale::Degrees:
    return os << "deg";
  case PhaseScale::Radians:
    return os << "rad";
  case PhaseScale::Milliseconds:
    return os << "ms";
  case PhaseScale::Bins:
    return os << "bin";
  default:
    return os << "unknown";
  }
}

std::istream& Pulsar::operator >> (std::istream& is, PhaseScale::Units& units)
{
  std::streampos pos = is.tellg();
  std::string unit;
  is >> unit;

  if (unit == "turn")
    units = PhaseScale::Turns;
  else if (unit == "deg")
    units = PhaseScale::Degrees;
  else if (unit == "rad")
    units = PhaseScale::Radians;
  else if (unit == "ms")
    units = PhaseScale::Milliseconds;
  else if (unit == "bin")
    units = PhaseScale::Bins;
  else
    is.setstate(std::istream::failbit);

  return is;
}

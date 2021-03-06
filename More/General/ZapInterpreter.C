/***************************************************************************
 *
 *   Copyright (C) 2007 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/ZapInterpreter.h"
#include "Pulsar/Integration.h"

#include "Pulsar/ChannelZapMedian.h"
#include "Pulsar/LawnMower.h"
#include "Pulsar/RobustMower.h"

#include "Pulsar/Profile.h"

#include "TextInterface.h"
#include "pairutil.h"

using namespace std;

string index_help (const string& cmd)
{
  return
    "usage: " + cmd + " iex1 [iex2 ...]\n"
    "  string iexN   unsigned index exression: [i|i1-i2|-iend|istart-]";
}

string pair_help (const string& cmd)
{
  return
    "usage: " + cmd + " i1,j1 [i2,j2 ...]\n"
    "  string i1,j1  unsigned index pair";
}

Pulsar::ZapInterpreter::ZapInterpreter ()
{
  add_command 
    ( &ZapInterpreter::median, 
      "median", "median smooth the passband and zap spikes",
      "usage: median <TI> \n"
      "  type 'zap median help' for text interface help" );

  add_command 
    ( &ZapInterpreter::mow, 
      "mow", "median smooth the profile and clean spikes",
      "usage: mow <TI> \n"
      "  type 'zap mow help' for text interface help" );

  add_command
    ( &ZapInterpreter::chan,
      "chan", "zap specified channels",
      index_help("chan") );

  add_command
    ( &ZapInterpreter::subint,
      "subint", "zap specified integrationss",
      index_help("subint") );

  add_command
    ( &ZapInterpreter::such,
      "such", "zap specified integration and channel",
      pair_help("such") );

  add_command
    ( &ZapInterpreter::edge,
      "edge", "zap fraction of band edges",
      "usage: edge <fraction> \n"
      "  float <fraction>  fraction of band zapped on each edge \n");

  add_command
    ( &ZapInterpreter::freq,
      "freq", "zap frequency range(s)",
      "usage: freq < MHz0:MHz1 | >MHz | <MHz > \n"
      "  float pair <MHz0:MHz1> range of frequencies to zap \n"
      "  float [>MHz] zap everything above frequency \n"
      "  float [<MHz] zap everything below frequency \n");

  add_command
    ( &ZapInterpreter::zerodm,
      "zerodm", "",
      "usage: zerodm \n"
      "  Use the 'ZeroDM' RFI removal scheme. \n");

}

Pulsar::ZapInterpreter::~ZapInterpreter ()
{
}

string Pulsar::ZapInterpreter::median (const string& args) try
{
  bool expand = false;
  vector<string> arguments = setup (args, expand);

  if (!zap_median)
    zap_median = new ChannelZapMedian;

  if (!arguments.size())
  {
    (*zap_median)( get() );
    return response (Good);
  }

  //! Zap median interface
  Reference::To<TextInterface::Parser> interface = zap_median->get_interface();

  string retval;
  for (unsigned icmd=0; icmd < arguments.size(); icmd++)
  {
    if (icmd)
      retval += " ";
    retval += interface->process (arguments[icmd]);
  }

  return retval;
}
catch (Error& error) {
  return response (Fail, error.get_message());
}

string Pulsar::ZapInterpreter::mow (const string& args) try
{ 
  if (args == "robust")
  {
    mower = new RobustMower;
    return response (Good);
  }

  vector<string> arguments = setup (args);

  if (!mower)
    mower = new LawnMower;

  if (!arguments.size())
  {
    Reference::To<Archive> data = get();
    for (unsigned isub = 0; isub < data->get_nsubint(); isub++)
    {
      cerr << "mowing subint " << isub << endl;
      mower->transform( data->get_Integration( isub ) );
    }
    return response (Good);
  }

  //! Zap median interface
  Reference::To<TextInterface::Parser> interface = mower->get_interface();

  string retval;
  for (unsigned icmd=0; icmd < arguments.size(); icmd++) {
    if (icmd)
      retval += " ";
    retval += interface->process (arguments[icmd]);
  }

  return retval;
}
catch (Error& error)
{
  return response (Fail, error.get_message());
}


string Pulsar::ZapInterpreter::empty ()
{
  return response (Fail, help());
}

void parse_indeces (vector<unsigned>& indeces,
		    const vector<string>& arguments,
		    unsigned limit)
{
  for (unsigned i=0; i<arguments.size(); i++)
    TextInterface::parse_indeces (indeces, "[" + arguments[i] + "]", limit);
}

string Pulsar::ZapInterpreter::chan (const string& args) try 
{
  vector<string> arguments = setup (args);

  vector<unsigned> channels;
  parse_indeces (channels, arguments, get()->get_nchan());

  // zap selected channels in all sub-integrations
  unsigned nsubint = get()->get_nsubint();
  for (unsigned isub=0; isub<nsubint; isub++) {
    Integration* subint = get()->get_Integration(isub);
    for (unsigned i=0; i<channels.size(); i++)
      subint->set_weight( channels[i], 0.0 );
  }
  
  return response (Good);
}
catch (Error& error) {
  return response (Fail, error.get_message());
}


string Pulsar::ZapInterpreter::subint (const string& args) try 
{
  vector<string> arguments = setup (args);

  vector<unsigned> subints;
  parse_indeces (subints, arguments, get()->get_nsubint());

  // zap all channels in selected sub-integrations
  unsigned nchan = get()->get_nchan();
  for (unsigned i=0; i<subints.size(); i++) {
    Integration* subint = get()->get_Integration( subints[i] );
    for (unsigned ichan=0; ichan<nchan; ichan++)
      subint->set_weight( ichan, 0.0 );
  }
  return response (Good);
}
catch (Error& error) {
  return response (Fail, error.get_message());
}


template<typename T, typename U>
void parse_pairs (vector< pair<T,U> >& pairs,
		  const vector<string>& arguments,
		  T limit_first, const string& name_first,
		  U limit_second, const string& name_second)
{
  // cerr << "parse_pairs: " << arguments.size() << " arguments" << endl;

  pairs.resize( arguments.size() );

  for (unsigned i=0; i<pairs.size(); i++)
  {
    // cerr << "parse_pairs: arg[" << i << "]=" << arguments[i] << endl;

    pairs[i] = fromstring< pair<T,U> > ( "(" + arguments[i] + ")" );
    
    if (pairs[i].first >= limit_first)
    {
      Error error (InvalidParam, "parse_pairs");
      error << "i" << name_first << "=" << pairs[i].first 
	    << " >= n" << name_first << "=" << limit_first;
      throw error;
    }

    if (pairs[i].second >= limit_second)
    {
      Error error (InvalidParam, "parse_pairs");
      error << "i" << name_second << "=" << pairs[i].second
	    << " >= n" << name_second << "=" << limit_second;
      throw error;
    }

    // cerr << pairs[i] << endl;
  }
}

string Pulsar::ZapInterpreter::such (const string& args) try 
{
  vector<string> arguments = setup (args);

  vector< pair<unsigned,unsigned> > pairs;
  parse_pairs (pairs, arguments,
	       get()->get_nsubint(), "subint",
	       get()->get_nchan(), "chan");
  
  for (unsigned i=0; i<pairs.size(); i++)
    get()->get_Integration(pairs[i].first)->set_weight(pairs[i].second,0.0);
  
  return response (Good);
}
catch (Error& error) {
  return response (Fail, error.get_message());
}


// //////////////////////////////////////////////////////////////////////
//
string Pulsar::ZapInterpreter::edge (const string& args)
try {

  float fraction = setup<float> (args);

  if (fraction <= 0.0 || fraction >= 0.5)
    return response (Fail, "invalid fraction " + tostring(fraction));

  Archive* archive = get();

  unsigned isub,  nsub = archive->get_nsubint();
  unsigned ichan, nchan = archive->get_nchan();

  unsigned nedge = unsigned( nchan * fraction );

  for (isub=0; isub < nsub; isub++) {
    Integration* subint = archive->get_Integration (isub);
    for (ichan=0; ichan < nedge; ichan++)
      subint->set_weight (ichan, 0.0);
    for (ichan=nchan-nedge; ichan < nchan; ichan++)
      subint->set_weight (ichan, 0.0);
  }

  return response (Good);

}
catch (Error& error)
{
  return response (Fail, error.get_message());
}

class range
{
  bool gt, lt;
  double x0,x1;
public:
  range () { gt = lt = false; x0 = x1 = 0; }
  friend ostream& operator<< (ostream&, const range&);
  friend istream& operator>> (istream&, range&);
  bool within (double x) const;
};

ostream& operator<< (ostream& os, const range& r)
{
  if (r.gt) os << ">" << r.x0;
  else if (r.lt) os << "<" << r.x0;
  else os << r.x0 << ":" << r.x1;
  return os;
}

istream& operator>> (istream& is, range& r)
{
  r.lt = is.peek() == '<';
  r.gt = is.peek() == '>';

  if (r.lt || r.gt)
  {
    is.get();
    is >> r.x0;
    return is;
  }

  char separator = 0;
  is >> r.x0 >> separator >> r.x1;

  if (r.x0 > r.x1)
    std::swap (r.x0, r.x1);

  if ( separator != ':' )
    is.setstate (std::istream::failbit);

  return is;
}

bool range::within (double x) const
{
  if (lt && x < x0)
    return true;

  if (gt && x > x0)
    return true;

  return (x>x0 && x<x1);
}

// //////////////////////////////////////////////////////////////////////
//
string Pulsar::ZapInterpreter::freq (const string& args) try
{
  vector<string> arguments = setup (args);

  for (unsigned iarg=0; iarg < arguments.size(); iarg++)
  {
    range r = fromstring<range> (arguments[iarg]);

    Archive* archive = get();

    unsigned isub,  nsub = archive->get_nsubint();
    unsigned ichan, nchan = archive->get_nchan();

    for (isub=0; isub < nsub; isub++)
    {
      Integration* subint = archive->get_Integration (isub);
      for (ichan=0; ichan < nchan; ichan++)
	if (r.within( subint->get_centre_frequency(ichan) ))
	  subint->set_weight (ichan, 0.0);
    }
  }

  return response (Good);
}
catch (Error& error)
{
  return response (Fail, error.get_message());
}


string Pulsar::ZapInterpreter::zerodm (const string& args)
try {

  Archive* archive = get();

  unsigned isub,  nsub = archive->get_nsubint();
  unsigned ichan, nchan = archive->get_nchan();
  for( isub=0; isub<nsub;isub++){
	  unsigned nbins=archive->get_Profile(isub,0,0)->get_nbin();
	  float* mean=(float*)calloc(nbins,sizeof(float));
	  for( ichan=0; ichan < nchan;ichan++){
		  float* profile = archive->get_Profile(isub,0,ichan)->get_amps();
		  for ( unsigned ibin=0; ibin<nbins;ibin++)
		  {
			  mean[ibin]+=profile[ibin];
		  }
	  }
	  for ( unsigned ibin=0; ibin<nbins;ibin++)
	  {
		  mean[ibin]/=(float)nchan;
	  }

	  for( ichan=0; ichan<nchan;ichan++){
		  float* profile = archive->get_Profile(isub,0,ichan)->get_amps();
		  for ( unsigned ibin=0; ibin<nbins;ibin++)
		  {
			  profile[ibin] -= mean[ibin];
		  }
	  }
	  free(mean);
  }

  return response (Good);

}


catch (Error& error) {
  return response (Fail, error.get_message());
}

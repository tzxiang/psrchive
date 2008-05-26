
/***************************************************************************
 *
 *   Copyright (C) 2008 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "tempo_impl.h"

using namespace std;

static void add_alias (const string& itoa_code, const string& alias);

static int default_aliases ()
{
  add_alias ("GB", "gbt");
  add_alias ("GB", "green bank");
  add_alias ("GB", "greenbank");

  add_alias ("NA", "atca");
  add_alias ("NA", "narrabri");

  add_alias ("AO", "arecibo");

  add_alias ("HO", "hobart");

  add_alias ("NS", "urumqi");

  add_alias ("TD", "tid");
  add_alias ("TD", "tidbinbilla");
  add_alias ("TD", "DSS43");
  add_alias ("TD", "DSS 43");

  add_alias ("PK", "pks");
  add_alias ("PK", "parkes");

  add_alias ("JB", "jodrell");
  add_alias ("JB", "jodrell bank");

  add_alias ("VL", "vla");

  add_alias ("BO", "northern cross");

  add_alias ("MO", "most");

  add_alias ("NC", "nancay");

  add_alias ("EF", "effelsberg");

  add_alias ("WT", "wsrt");
  add_alias ("WT", "westerbork");

  add_alias ("GM", "gmrt");

  return 1;
}

class aliases
{
public:
  aliases (const string& code, const string& alias)
  {
    itoa_code = code;
    aka.push_back (alias);
  }

  bool match (const string& name)
  {
    for (unsigned i=0; i<aka.size(); i++)
      if (strcasecmp (aka[i].c_str(), name.c_str()) == 0)
	return true;
    return false;
  }

  string itoa_code;
  vector<string> aka;
};

static int init = 0;

static vector<aliases> itoa_aliases;

string Tempo::itoa_code (const string& telescope_name)
{
  if (!init)
    default_aliases ();

  for (unsigned i=0; i<itoa_aliases.size(); i++)
    if (itoa_aliases[i].match( telescope_name ))
      return itoa_aliases[i].itoa_code;

  cerr << "itoa_code no alias found for " << telescope_name << endl;

  return string();
}

void add_alias (const string& itoa_code, const string& alias)
{
  for (unsigned i=0; i<itoa_aliases.size(); i++)
    if (itoa_aliases[i].itoa_code == itoa_code)
    {
      itoa_aliases[i].aka.push_back (alias);
      return;
    }

  itoa_aliases.push_back ( aliases( itoa_code, alias ) );
}


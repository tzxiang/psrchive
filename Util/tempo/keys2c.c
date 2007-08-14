/***************************************************************************
 *
 *   Copyright (C) 1998 by redwards Russell Edwards
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/
/* keys2fort.c -- redwards 1/12/97
 *
 * program to parse a keyword definition file and produce Fortran
 * arrays containing the information, for use by ephio.f. Produces
 * file keyinfo.com, based on file keys.dat. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* fatal errors ... */
#define FATAL_1(s1) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1); exit(1);}
#define FATAL_2(s1,s2) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1,s2); exit(1);}
#define FATAL_3(s1,s2,s3) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1,s2,s3); exit(1);}

int
main(int argc, char** argv)
{
  FILE *fin, *fouth, *foutc;
  char key[1024][64];
  char line[256];
  int type[1024], err[1024];
  int i, nelem, len, maxlen = 0;

  char* msg1 = 
    "/* This file is part of the 'psrchive/Util/tempo' module. */\n"
    "/* It was generated by keys2c. Do not modify directly */\n\n";
  
  
  if (argc < 2)
    FATAL_1("Must specify path to keys.dat");

  fin = fopen(argv[1], "r");
  if (!fin)
    FATAL_1("Couldn't open keys.dat");
  
  fouth = fopen("ephio.h", "w");
  if (!fouth)
    FATAL_1("Couldn't open ephio.h");
  
  foutc = fopen("ephio_def.c", "w");
  if (!foutc)
    FATAL_1("Couldn't open ephio_def.c");
  
  i = 0;
  while (fgets(line,256,fin)!=NULL)  {
     
    if ((sscanf(line,"%s", key[i])==1)	/* non empty line */
	&& (key[i][0]!='#'))	/* ignore comments */
      {
	sscanf(line,"%s %d %d", key[i], type+i, err+i);
	
	len = strlen(key[i]);
	if (len > maxlen)
	  maxlen = len;
	i++;
      }
  }
  fclose(fin);
  
  nelem = i;
  
  fprintf(fouth, msg1);
  fprintf(foutc, msg1);
  
  fprintf (fouth,
	   "#ifndef EPHIO_H\n#define EPHIO_H\n\n"
	   "#define EPH_NUM_KEYS %d\n", nelem);
  fprintf (fouth, 
	   "#define EPH_STR_LEN 32\n"
	   "#define EPH_MAX_KEY_LEN %d\n\n", maxlen);
  
  fprintf (fouth,
	   "#include \"ephio_func.h\"    /* I/O function declarations */\n\n");
  
  for (i=0; i < nelem; i++)
    fprintf(fouth, "#define EPH_%s %d\n", key[i], i);
  
  fprintf (fouth,
	  "\n\nextern char parmNames[EPH_NUM_KEYS][16];\n"
	   "extern int parmTypes[EPH_NUM_KEYS];\n"
	   "extern int parmError[EPH_NUM_KEYS];\n\n"
	   "\n\n#endif\n");

  fclose(fouth);

  /* the rest goes into the .c file and is thereby defined only once */

  fprintf(foutc, "#include \"ephio.h\"\n\n");

  fprintf(foutc, "char parmNames[EPH_NUM_KEYS][16] = {\n");
  for (i=0; i < nelem-1; i++)
    fprintf(foutc, "                      \"%s\",\n", key[i]);
  fprintf(foutc, "                      \"%s\"};\n\n", key[i]);
  
  fprintf(foutc, "int parmTypes[EPH_NUM_KEYS] = {\n");
  for (i=0; i < nelem-1; i++)
    fprintf(foutc, "                      %d,\n", type[i]);
  fprintf(foutc, "                      %d};\n\n", type[i]);

  fprintf(foutc, "int parmError[EPH_NUM_KEYS] = {\n");
  for (i=0; i < nelem-1; i++)
    fprintf(foutc, "                      %d,\n", err[i]);
  fprintf(foutc, "                      %d};\n", err[i]);

  fclose(foutc);
  
  return 0;
}


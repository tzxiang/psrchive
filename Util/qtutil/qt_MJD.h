#ifndef __QT_MJD_H
#define __QT_MJD_H

#include "qt_value.h"
#include "MJD.h"

class qt_MJD : public qt_value
{
  Q_OBJECT

 public:
  qt_MJD (bool with_error=false, QWidget *parent=NULL, const char *name=NULL);

  // error given in days
  int setMJD (const MJD& mjd, double error=0.0);
  int getMJD (MJD* mjd, double* error);

 protected:
  MJD  valset;

 protected slots:
  void value_Entered_CB ();
};

#endif

// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT


//author Witold Wolski


#ifndef IFEATUREALLACCESS_H
#define IFEATUREALLACCESS_H

#include "ifeatureaccess.h"
#include "ifeaturerangeaccess.h"

namespace ralab{
  struct IFeatureAllAccess : IFeatureAccess, IFeatureRangeAccess{
  };
}

#endif // IFEATUREALLACCESS_H

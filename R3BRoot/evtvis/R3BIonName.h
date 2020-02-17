/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

// -------------------------------------------------------------------------
// -----                   R3BIonName header file                      -----
// -----              Created 04/10/2011  by P.Cabanelas               -----
// -------------------------------------------------------------------------

#ifndef R3BIONNAME_H
#define R3BIONNAME_H

#include "TNamed.h"

class R3BIonName : public TNamed
{
  public:
    /** Default constructor **/
    R3BIonName();

    /** Destructor **/
    virtual ~R3BIonName();

    /** Accessors **/
    /**
     * Return the ion name if exists
     */
    Bool_t GetIonName(Int_t element, Char_t* name);

  private:
    /** Data members **/

    ClassDef(R3BIonName, 1);
};

#endif

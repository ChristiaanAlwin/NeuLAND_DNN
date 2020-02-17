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
// -----            R3BAmsStripCalData source file                -----
// -------------------------------------------------------------------------

#include "R3BAmsStripCalData.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BAmsStripCalData::R3BAmsStripCalData()
    : FairMultiLinkedData()
    , fDetId(-1)
    , fSideId(-1.)
    , fStripId(-1)
    , fEnergy(-1.)
{
}

// -----   Standard constructor   ------------------------------------------
R3BAmsStripCalData::R3BAmsStripCalData(Int_t detid, Int_t sideid, Int_t stripid, Double_t energy)
    : FairMultiLinkedData()
    , fDetId(detid)
    , fSideId(sideid)
    , fStripId(stripid)
    , fEnergy(energy)
{
}
// -------------------------------------------------------------------------

R3BAmsStripCalData::R3BAmsStripCalData(const R3BAmsStripCalData& right)
    : FairMultiLinkedData(right)
    , fDetId(right.fDetId)
    , fSideId(right.fSideId)
    , fStripId(right.fStripId)
    , fEnergy(right.fEnergy)
{
}

// -----   Destructor   ----------------------------------------------------
R3BAmsStripCalData::~R3BAmsStripCalData() {}

// -----   Public method Print   -------------------------------------------
void R3BAmsStripCalData::Print(const Option_t* opt) const
{
    cout << "-I- R3BAmsStripCalData: a stripCalData level hit in detector " << fDetId << " side " << fSideId
         << " strip identifier " << fStripId << endl;
    cout << " Energy = " << fEnergy << " (GeV in sim)" << endl;
}
// -------------------------------------------------------------------------

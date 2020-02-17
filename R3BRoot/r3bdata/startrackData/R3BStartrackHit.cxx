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
// -----                      R3BStartrackHit source file                  -----
// -------------------------------------------------------------------------

#include "R3BStartrackHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BStartrackHit::R3BStartrackHit()
    : FairHit()
{
    fEnergy = fTof = 0.;
    fDetectorID = -1;
}

// -----   Destructor   ----------------------------------------------------
R3BStartrackHit::~R3BStartrackHit() {}

// -----   Public method Print   -------------------------------------------
void R3BStartrackHit::Print(const Option_t* opt) const
{
    cout << "-I- R3BStartrackHit: Tracker Hit in detector " << fDetectorID << endl;
    cout << "    Tof: " << fTof << " cm,  Energy loss " << fEnergy * 1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------

ClassImp(R3BStartrackHit)

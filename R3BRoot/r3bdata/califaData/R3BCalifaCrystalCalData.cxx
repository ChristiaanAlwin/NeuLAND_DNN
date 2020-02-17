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

#include "R3BCalifaCrystalCalData.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

R3BCalifaCrystalCalData::R3BCalifaCrystalCalData()
    : FairMultiLinkedData()
    , fEnergy(-1.)
    , fNf(-1.)
    , fNs(-1.)
    , fToT_Energy(-1.)
    , fTime(0)
    , fCrystalId(-1)
{
}

R3BCalifaCrystalCalData::R3BCalifaCrystalCalData(Int_t ident,
                                                 Double_t energy,
                                                 Double_t Nf,
                                                 Double_t Ns,
                                                 ULong64_t time,
                                                 Double_t tot_energy)
    : FairMultiLinkedData()
    , fEnergy(energy)
    , fNf(Nf)
    , fNs(Ns)
    , fToT_Energy(tot_energy)
    , fTime(time)
    , fCrystalId(ident)
{
}

R3BCalifaCrystalCalData::R3BCalifaCrystalCalData(const R3BCalifaCrystalCalData& right)
    : FairMultiLinkedData(right)
    , fEnergy(right.fEnergy)
    , fNf(right.fNf)
    , fNs(right.fNs)
    , fToT_Energy(right.fToT_Energy)
    , fTime(right.fTime)
    , fCrystalId(right.fCrystalId)
{
}

void R3BCalifaCrystalCalData::Print(const Option_t* opt) const
{
    cout << "-I- R3BCalifaCrystalCalData: a crystalCalData level hit in crystal identifier " << fCrystalId << endl;
    cout << "    Energy = " << fEnergy << " (GeV in sim)" << endl;
    cout << "    ToT_Energy = " << fToT_Energy << " (GeV in sim)" << endl;
    cout << "    Nf = " << fNf << " (GeV in sim); fNs =" << fNs << " (GeV in sim)" << endl;
    cout << "    Time =" << fTime << " ns  " << endl;
}

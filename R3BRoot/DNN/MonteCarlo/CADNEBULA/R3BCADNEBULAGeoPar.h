#ifndef R3BCADNEBULAGEOPAR_H
#define R3BCADNEBULAGEOPAR_H

// C++ headers:
#include <algorithm>
#include <iostream>
#include <map>

// ROOT headers:
#include "TGeoBBox.h"
#include "TGeoMatrix.h"
#include "TVector3.h"
#include "TGeoNode.h"

// FAIR headers:
#include "FairParamList.h"
#include "FairParGenericSet.h"

class FairParamList;
class TVector3;

/**
 * NEBULA geometry parameter storage
 * @author Jan Mayer
 *
 * Stores the full NEBULA geo node used in the simulation for later reference, especially for coordinate
 * transformation from and to local and global coordinates.
 */

class R3BCADNEBULAGeoPar : public FairParGenericSet
{
  public:
    // needs to be public?
    TGeoNode* fNEBULAGeoNode;

    R3BCADNEBULAGeoPar(const char* name = "R3BCADNEBULAGeoPar",
                     const char* title = "NEBULA Geometry Parameters",
                     const char* context = "TestDefaultContext");
    ~R3BCADNEBULAGeoPar() override;

    void clear() override;
    void putParams(FairParamList*) override;
    Bool_t getParams(FairParamList*) override;
    void printParams() override;

    TGeoNode* GetNEBULAGeoNode() { return fNEBULAGeoNode; } // FIXME: const?
    void SetNEBULAGeoNode(const TGeoNode* const p);

    Double_t GetPaddleHalfLength() const;
    TVector3 ConvertToLocalCoordinates(const TVector3& position, const Int_t paddleID) const;
    TVector3 ConvertToGlobalCoordinates(const TVector3& position, const Int_t paddleID) const;
    TVector3 ConvertGlobalToPixel(const TVector3& position) const;

  private:
    std::map<Int_t, TGeoNode*> fPaddleGeoNodes;
    void BuildPaddleLookup();

    R3BCADNEBULAGeoPar(const R3BCADNEBULAGeoPar&);
    R3BCADNEBULAGeoPar& operator=(const R3BCADNEBULAGeoPar&);

    ClassDefOverride(R3BCADNEBULAGeoPar, 1)
};

#endif // R3BNEBULAGEOPAR_H

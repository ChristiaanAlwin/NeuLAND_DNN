#include "DrawGraphDoumaArray.h"
#include "DrawGraphDoumaArray3.h"

void MakeMultAllGraphs_600MeV()
{
    // NOTE: 1000 MeV:
    
    // Begin by creating the multiplicity graphs:
    DNNDataGraph* DNN_1n = new DNNDataGraph(); DNNDataGraph* DNN_Res_1n = new DNNDataGraph(); DNNDataGraph* TDR_1n = new DNNDataGraph(); DNNDataGraph* TDR_Res_1n = new DNNDataGraph(); DNNDataGraph* Perfect_1n = new DNNDataGraph();
    DNNDataGraph* DNN_2n = new DNNDataGraph(); DNNDataGraph* DNN_Res_2n = new DNNDataGraph(); DNNDataGraph* TDR_2n = new DNNDataGraph(); DNNDataGraph* TDR_Res_2n = new DNNDataGraph(); DNNDataGraph* Perfect_2n = new DNNDataGraph();
    DNNDataGraph* DNN_3n = new DNNDataGraph(); DNNDataGraph* DNN_Res_3n = new DNNDataGraph(); DNNDataGraph* TDR_3n = new DNNDataGraph(); DNNDataGraph* TDR_Res_3n = new DNNDataGraph(); DNNDataGraph* Perfect_3n = new DNNDataGraph();
    DNNDataGraph* DNN_4n = new DNNDataGraph(); DNNDataGraph* DNN_Res_4n = new DNNDataGraph(); DNNDataGraph* TDR_4n = new DNNDataGraph(); DNNDataGraph* TDR_Res_4n = new DNNDataGraph(); DNNDataGraph* Perfect_4n = new DNNDataGraph();
    DNNDataGraph* DNN_5n = new DNNDataGraph(); DNNDataGraph* DNN_Res_5n = new DNNDataGraph(); DNNDataGraph* TDR_5n = new DNNDataGraph(); DNNDataGraph* TDR_Res_5n = new DNNDataGraph(); DNNDataGraph* Perfect_5n = new DNNDataGraph();
    
    Double_t PerfectHeight = 142.0;
    Double_t TDRHeight = 115.0;
    Double_t DNNHeight = 130.0;
    Double_t SqrtN = 0.1;
    
    DNN_1n->SetErrorHeight(DNNHeight); DNN_Res_1n->SetErrorHeight(DNNHeight); TDR_1n->SetErrorHeight(TDRHeight); TDR_Res_1n->SetErrorHeight(TDRHeight); Perfect_1n->SetErrorHeight(PerfectHeight);
    DNN_2n->SetErrorHeight(DNNHeight); DNN_Res_2n->SetErrorHeight(DNNHeight); TDR_2n->SetErrorHeight(TDRHeight); TDR_Res_2n->SetErrorHeight(TDRHeight); Perfect_2n->SetErrorHeight(PerfectHeight);
    DNN_3n->SetErrorHeight(DNNHeight); DNN_Res_3n->SetErrorHeight(DNNHeight); TDR_3n->SetErrorHeight(TDRHeight); TDR_Res_3n->SetErrorHeight(TDRHeight); Perfect_3n->SetErrorHeight(PerfectHeight);
    DNN_4n->SetErrorHeight(DNNHeight); DNN_Res_4n->SetErrorHeight(DNNHeight); TDR_4n->SetErrorHeight(TDRHeight); TDR_Res_4n->SetErrorHeight(TDRHeight); Perfect_4n->SetErrorHeight(PerfectHeight);
    DNN_5n->SetErrorHeight(DNNHeight); DNN_Res_5n->SetErrorHeight(DNNHeight); TDR_5n->SetErrorHeight(TDRHeight); TDR_Res_5n->SetErrorHeight(TDRHeight); Perfect_5n->SetErrorHeight(PerfectHeight);
    
    // ------------------------------------------------------------
    
    // Then, add the 8dp scenario:
    DNN_1n->AddYpoint(0,56.9,54.7,55.4,56.4,54.9); DNN_Res_1n->AddYpoint(0,54.0,51.8,52.5,53.4,52.0); TDR_1n->AddYpoint(0,37.0,35.8,37.2,35.4,35.3); TDR_Res_1n->AddYpoint(0,35.6,34.3,35.7,34.0,33.9); Perfect_1n->AddYpoint(0,62.5,62.2,60.7,62.4,60.7);
    DNN_2n->AddYpoint(0,36.5,42.3,34.8,38.3,36.6); DNN_Res_2n->AddYpoint(0,23.6,26.6,21.9,24.6,23.0); TDR_2n->AddYpoint(0,36.2,37.0,35.2,36.9,35.9); TDR_Res_2n->AddYpoint(0,19.4,19.3,18.7,18.8,18.4); Perfect_2n->AddYpoint(0,39.5,39.2,37.7,39.2,37.7);
    DNN_3n->AddYpoint(0,25.3,20.2,24.2,22.9,22.2); DNN_Res_3n->AddYpoint(0,11.9,9.7,11.0,11.3,10.4);  TDR_3n->AddYpoint(0,23.4,24.1,22.2,24.3,23.2); TDR_Res_3n->AddYpoint(0,10.2,10.3,9.6,10.3,9.6);   Perfect_3n->AddYpoint(0,24.6,24.6,23.6,24.8,23.7);
    DNN_4n->AddYpoint(0,17.3,17.0,16.6,17.5,17.1); DNN_Res_4n->AddYpoint(0,6.2,5.6,5.7,6.4,5.8);      TDR_4n->AddYpoint(0,15.7,15.9,14.6,16.5,15.2); TDR_Res_4n->AddYpoint(0,5.4,5.5,5.1,5.6,5.1);      Perfect_4n->AddYpoint(0,15.6,15.7,15.1,15.8,15.0);
    DNN_5n->AddYpoint(0,17.6,23.4,17.2,16.4,17.1); DNN_Res_5n->AddYpoint(0,5.6,6.5,5.3,5.2,5.2);      TDR_5n->AddYpoint(0,16.9,17.0,15.3,17.8,16.5); TDR_Res_5n->AddYpoint(0,4.9,4.9,4.4,5.1,4.6);      Perfect_5n->AddYpoint(0,9.7,9.8,9.3,9.7,9.3);
    
    DNN_1n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_1n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_1n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_1n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_1n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_2n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_2n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_2n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_2n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_2n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_3n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_3n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_3n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_3n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_3n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_4n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_4n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_4n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_4n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_4n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_5n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_5n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_5n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_5n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_5n->AddYerror(0,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    
    DNN_1n->AddXpoint(0,8.0); DNN_Res_1n->AddXpoint(0,8.0); TDR_1n->AddXpoint(0,8.0); TDR_Res_1n->AddXpoint(0,8.0); Perfect_1n->AddXpoint(0,8.0);
    DNN_2n->AddXpoint(0,8.0); DNN_Res_2n->AddXpoint(0,8.0); TDR_2n->AddXpoint(0,8.0); TDR_Res_2n->AddXpoint(0,8.0); Perfect_2n->AddXpoint(0,8.0);
    DNN_3n->AddXpoint(0,8.0); DNN_Res_3n->AddXpoint(0,8.0); TDR_3n->AddXpoint(0,8.0); TDR_Res_3n->AddXpoint(0,8.0); Perfect_3n->AddXpoint(0,8.0);
    DNN_4n->AddXpoint(0,8.0); DNN_Res_4n->AddXpoint(0,8.0); TDR_4n->AddXpoint(0,8.0); TDR_Res_4n->AddXpoint(0,8.0); Perfect_4n->AddXpoint(0,8.0);
    DNN_5n->AddXpoint(0,8.0); DNN_Res_5n->AddXpoint(0,8.0); TDR_5n->AddXpoint(0,8.0); TDR_Res_5n->AddXpoint(0,8.0); Perfect_5n->AddXpoint(0,8.0);
    
    // ------------------------------------------------------------
    
    // Then, add the 12dp scenario:
    DNN_1n->AddYpoint(1,66.4,66.2,63.9,69.7,67.4); DNN_Res_1n->AddYpoint(1,64.6,64.4,62.0,67.8,65.6); TDR_1n->AddYpoint(1,51.2,51.4,45.8,55.5,50.3); TDR_Res_1n->AddYpoint(1,50.2,50.4,44.7,54.4,49.2); Perfect_1n->AddYpoint(1,76.8,76.8,76.4,76.8,76.4);
    DNN_2n->AddYpoint(1,50.7,48.0,48.9,48.2,47.0); DNN_Res_2n->AddYpoint(1,39.6,36.9,36.4,39.8,37.2); TDR_2n->AddYpoint(1,41.0,41.6,39.1,42.3,41.1); TDR_Res_2n->AddYpoint(1,30.4,31.0,26.5,33.3,29.9); Perfect_2n->AddYpoint(1,59.1,59.1,58.8,59.2,58.7);
    DNN_3n->AddYpoint(1,31.5,36.5,30.7,28.7,29.2); DNN_Res_3n->AddYpoint(1,21.0,23.3,18.7,20.9,19.3); TDR_3n->AddYpoint(1,30.9,30.8,30.8,29.1,30.4); TDR_Res_3n->AddYpoint(1,19.9,20.2,17.6,20.7,19.4); Perfect_3n->AddYpoint(1,45.4,45.5,45.1,45.5,45.2);
    DNN_4n->AddYpoint(1,31.2,25.5,32.0,26.6,30.2); DNN_Res_4n->AddYpoint(1,17.4,13.5,16.0,16.6,16.7); TDR_4n->AddYpoint(1,23.5,22.8,25.0,19.8,23.0); TDR_Res_4n->AddYpoint(1,13.2,13.0,12.0,12.6,12.8); Perfect_4n->AddYpoint(1,35.0,34.6,34.5,34.9,34.6);
    DNN_5n->AddYpoint(1,27.6,36.1,33.3,19.1,26.3); DNN_Res_5n->AddYpoint(1,15.2,18.2,17.1,11.5,14.3); TDR_5n->AddYpoint(1,28.4,27.0,37.3,19.9,27.3); TDR_Res_5n->AddYpoint(1,14.6,14.3,17.7,11.0,14.0); Perfect_5n->AddYpoint(1,26.5,26.7,26.6,26.5,26.3);
    
    DNN_1n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_1n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_1n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_1n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_1n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_2n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_2n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_2n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_2n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_2n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_3n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_3n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_3n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_3n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_3n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_4n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_4n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_4n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_4n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_4n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_5n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_5n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_5n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_5n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_5n->AddYerror(1,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    
    DNN_1n->AddXpoint(1,12.0); DNN_Res_1n->AddXpoint(1,12.0); TDR_1n->AddXpoint(1,12.0); TDR_Res_1n->AddXpoint(1,12.0); Perfect_1n->AddXpoint(1,12.0);
    DNN_2n->AddXpoint(1,12.0); DNN_Res_2n->AddXpoint(1,12.0); TDR_2n->AddXpoint(1,12.0); TDR_Res_2n->AddXpoint(1,12.0); Perfect_2n->AddXpoint(1,12.0);
    DNN_3n->AddXpoint(1,12.0); DNN_Res_3n->AddXpoint(1,12.0); TDR_3n->AddXpoint(1,12.0); TDR_Res_3n->AddXpoint(1,12.0); Perfect_3n->AddXpoint(1,12.0);
    DNN_4n->AddXpoint(1,12.0); DNN_Res_4n->AddXpoint(1,12.0); TDR_4n->AddXpoint(1,12.0); TDR_Res_4n->AddXpoint(1,12.0); Perfect_4n->AddXpoint(1,12.0);
    DNN_5n->AddXpoint(1,12.0); DNN_Res_5n->AddXpoint(1,12.0); TDR_5n->AddXpoint(1,12.0); TDR_Res_5n->AddXpoint(1,12.0); Perfect_5n->AddXpoint(1,12.0);
    
    // ------------------------------------------------------------
    
    // Then, add the 16dp scenario:
    DNN_1n->AddYpoint(2,75.1,75.3,74.4,75.9,75.3); DNN_Res_1n->AddYpoint(2,73.9,74.1,73.3,74.6,74.0); TDR_1n->AddYpoint(2,58.8,59.2,58.4,59.1,58.2); TDR_Res_1n->AddYpoint(2,58.1,58.5,57.7,58.4,57.4); Perfect_1n->AddYpoint(2,85.6,85.7,84.7,85.5,84.6);
    DNN_2n->AddYpoint(2,55.7,49.4,53.9,54.6,52.8); DNN_Res_2n->AddYpoint(2,49.4,43.3,47.4,48.7,46.7); TDR_2n->AddYpoint(2,49.3,49.2,48.2,49.3,48.2); TDR_Res_2n->AddYpoint(2,42.4,42.5,40.9,42.5,40.9); Perfect_2n->AddYpoint(2,73.1,73.4,71.9,73.2,71.8);
    DNN_3n->AddYpoint(2,39.7,44.8,37.9,40.4,38.5); DNN_Res_3n->AddYpoint(2,31.6,33.8,29.7,32.3,30.3); TDR_3n->AddYpoint(2,39.2,38.9,37.8,39.0,37.9); TDR_Res_3n->AddYpoint(2,31.3,31.2,29.4,31.0,29.5); Perfect_3n->AddYpoint(2,62.2,62.5,60.6,62.2,60.6);
    DNN_4n->AddYpoint(2,36.0,39.5,34.2,32.2,31.0); DNN_Res_4n->AddYpoint(2,25.1,27.3,23.3,22.7,21.0); TDR_4n->AddYpoint(2,30.8,30.7,29.7,31.0,29.9); TDR_Res_4n->AddYpoint(2,22.6,22.6,21.2,22.8,21.4); Perfect_4n->AddYpoint(2,52.5,52.9,51.0,52.9,50.9);
    DNN_5n->AddYpoint(2,43.3,41.8,41.2,44.2,44.2); DNN_Res_5n->AddYpoint(2,29.1,28.4,27.1,29.5,28.5); TDR_5n->AddYpoint(2,35.4,35.0,34.9,35.2,34.6); TDR_Res_5n->AddYpoint(2,24.2,23.9,23.2,24.0,23.0); Perfect_5n->AddYpoint(2,44.1,44.1,42.6,43.9,42.6);
    
    DNN_1n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_1n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_1n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_1n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_1n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_2n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_2n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_2n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_2n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_2n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_3n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_3n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_3n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_3n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_3n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_4n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_4n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_4n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_4n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_4n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_5n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_5n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_5n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_5n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_5n->AddYerror(2,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    
    DNN_1n->AddXpoint(2,16.0); DNN_Res_1n->AddXpoint(2,16.0); TDR_1n->AddXpoint(2,16.0); TDR_Res_1n->AddXpoint(2,16.0); Perfect_1n->AddXpoint(2,16.0);
    DNN_2n->AddXpoint(2,16.0); DNN_Res_2n->AddXpoint(2,16.0); TDR_2n->AddXpoint(2,16.0); TDR_Res_2n->AddXpoint(2,16.0); Perfect_2n->AddXpoint(2,16.0);
    DNN_3n->AddXpoint(2,16.0); DNN_Res_3n->AddXpoint(2,16.0); TDR_3n->AddXpoint(2,16.0); TDR_Res_3n->AddXpoint(2,16.0); Perfect_3n->AddXpoint(2,16.0);
    DNN_4n->AddXpoint(2,16.0); DNN_Res_4n->AddXpoint(2,16.0); TDR_4n->AddXpoint(2,16.0); TDR_Res_4n->AddXpoint(2,16.0); Perfect_4n->AddXpoint(2,16.0);
    DNN_5n->AddXpoint(2,16.0); DNN_Res_5n->AddXpoint(2,16.0); TDR_5n->AddXpoint(2,16.0); TDR_Res_5n->AddXpoint(2,16.0); Perfect_5n->AddXpoint(2,16.0);
    
    // ------------------------------------------------------------
    
    // Then, add the 23dp scenario:
    DNN_1n->AddYpoint(3,85.5,83.1,82.7,86.2,83.2); DNN_Res_1n->AddYpoint(3,84.9,82.5,82.0,85.6,82.6); TDR_1n->AddYpoint(3,68.7,69.8,57.3,79.2,71.3); TDR_Res_1n->AddYpoint(3,68.4,69.5,56.9,78.8,70.9); Perfect_1n->AddYpoint(3,93.3,93.2,93.0,93.3,93.1);
    DNN_2n->AddYpoint(3,64.4,68.0,59.9,66.2,61.9); DNN_Res_2n->AddYpoint(3,62.2,65.2,57.1,64.2,59.1); TDR_2n->AddYpoint(3,60.4,60.5,51.1,63.2,58.5); TDR_Res_2n->AddYpoint(3,57.3,57.5,46.4,61.6,55.8); Perfect_2n->AddYpoint(3,86.7,86.6,86.3,86.6,86.3);
    DNN_3n->AddYpoint(3,49.8,51.0,45.6,49.9,47.7); DNN_Res_3n->AddYpoint(3,45.7,47.0,40.2,46.6,42.7); TDR_3n->AddYpoint(3,51.3,50.7,45.8,47.7,48.9); TDR_Res_3n->AddYpoint(3,47.0,46.5,39.0,45.2,44.4); Perfect_3n->AddYpoint(3,80.1,79.9,79.5,79.9,79.3);
    DNN_4n->AddYpoint(3,43.9,42.6,41.2,41.7,40.9); DNN_Res_4n->AddYpoint(3,37.2,36.5,32.6,36.6,33.5); TDR_4n->AddYpoint(3,42.2,42.0,41.2,34.8,41.4); TDR_Res_4n->AddYpoint(3,37.0,36.7,32.8,31.7,35.6); Perfect_4n->AddYpoint(3,73.2,73.0,72.6,73.0,72.5);
    DNN_5n->AddYpoint(3,59.0,57.2,66.9,50.2,62.8); DNN_Res_5n->AddYpoint(3,47.9,46.5,51.9,41.5,49.6); TDR_5n->AddYpoint(3,46.7,47.1,63.9,31.0,48.7); TDR_Res_5n->AddYpoint(3,38.7,38.9,49.8,26.2,39.6); Perfect_5n->AddYpoint(3,66.2,66.1,65.3,65.9,65.5);
    
    DNN_1n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_1n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_1n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_1n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_1n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_2n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_2n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_2n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_2n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_2n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_3n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_3n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_3n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_3n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_3n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_4n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_4n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_4n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_4n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_4n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_5n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_5n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_5n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_5n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_5n->AddYerror(3,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    
    DNN_1n->AddXpoint(4,23.0); DNN_Res_1n->AddXpoint(4,23.0); TDR_1n->AddXpoint(4,23.0); TDR_Res_1n->AddXpoint(4,23.0); Perfect_1n->AddXpoint(4,23.0);
    DNN_2n->AddXpoint(4,23.0); DNN_Res_2n->AddXpoint(4,23.0); TDR_2n->AddXpoint(4,23.0); TDR_Res_2n->AddXpoint(4,23.0); Perfect_2n->AddXpoint(4,23.0);
    DNN_3n->AddXpoint(4,23.0); DNN_Res_3n->AddXpoint(4,23.0); TDR_3n->AddXpoint(4,23.0); TDR_Res_3n->AddXpoint(4,23.0); Perfect_3n->AddXpoint(4,23.0);
    DNN_4n->AddXpoint(4,23.0); DNN_Res_4n->AddXpoint(4,23.0); TDR_4n->AddXpoint(4,23.0); TDR_Res_4n->AddXpoint(4,23.0); Perfect_4n->AddXpoint(4,23.0);
    DNN_5n->AddXpoint(4,23.0); DNN_Res_5n->AddXpoint(4,23.0); TDR_5n->AddXpoint(4,23.0); TDR_Res_5n->AddXpoint(4,23.0); Perfect_5n->AddXpoint(4,23.0);
    
    // ------------------------------------------------------------
    
    // Then, add the 30dp scenario:
    DNN_1n->AddYpoint(4,88.5,87.3,85.4,90.6,87.9); DNN_Res_1n->AddYpoint(4,88.2,87.0,85.1,90.3,87.5); TDR_1n->AddYpoint(4,74.6,76.2,61.9,82.8,74.8); TDR_Res_1n->AddYpoint(4,74.4,76.0,61.7,82.6,74.6); Perfect_1n->AddYpoint(4,96.5,96.4,96.3,96.4,96.4);
    DNN_2n->AddYpoint(4,71.6,71.0,65.2,73.9,70.9); DNN_Res_2n->AddYpoint(4,70.4,69.8,63.6,73.0,69.6); TDR_2n->AddYpoint(4,67.9,67.5,54.3,72.9,67.2); TDR_Res_2n->AddYpoint(4,66.4,66.1,51.7,71.8,65.5); Perfect_2n->AddYpoint(4,92.7,92.7,92.3,92.6,92.4);
    DNN_3n->AddYpoint(4,58.7,62.3,52.5,58.3,57.9); DNN_Res_3n->AddYpoint(4,55.9,59.2,48.7,56.3,55.0); TDR_3n->AddYpoint(4,59.6,59.4,49.7,57.2,59.3); TDR_Res_3n->AddYpoint(4,56.7,56.6,45.2,55.4,56.2); Perfect_3n->AddYpoint(4,88.1,88.1,87.6,88.1,87.6);
    DNN_4n->AddYpoint(4,55.3,52.8,50.9,46.6,49.3); DNN_Res_4n->AddYpoint(4,50.2,48.1,44.3,43.6,44.4); TDR_4n->AddYpoint(4,50.6,51.1,45.9,40.6,50.9); TDR_Res_4n->AddYpoint(4,46.5,47.0,39.6,38.2,46.5); Perfect_4n->AddYpoint(4,82.9,83.0,82.3,83.2,82.2);
    DNN_5n->AddYpoint(4,62.6,64.2,71.8,48.8,64.1); DNN_Res_5n->AddYpoint(4,54.9,55.8,60.6,43.5,55.1); TDR_5n->AddYpoint(4,55.7,57.3,75.2,32.7,55.7); TDR_Res_5n->AddYpoint(4,48.9,50.0,63.0,29.1,48.3); Perfect_5n->AddYpoint(4,77.4,77.1,76.2,77.5,76.3);
    
    DNN_1n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_1n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_1n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_1n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_1n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_2n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_2n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_2n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_2n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_2n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_3n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_3n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_3n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_3n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_3n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_4n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_4n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_4n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_4n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_4n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    DNN_5n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); DNN_Res_5n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_5n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); TDR_Res_5n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN); Perfect_5n->AddYerror(4,SqrtN,SqrtN,SqrtN,SqrtN,SqrtN);
    
    DNN_1n->AddXpoint(4,30.0); DNN_Res_1n->AddXpoint(4,30.0); TDR_1n->AddXpoint(4,30.0); TDR_Res_1n->AddXpoint(4,30.0); Perfect_1n->AddXpoint(4,30.0);
    DNN_2n->AddXpoint(4,30.0); DNN_Res_2n->AddXpoint(4,30.0); TDR_2n->AddXpoint(4,30.0); TDR_Res_2n->AddXpoint(4,30.0); Perfect_2n->AddXpoint(4,30.0);
    DNN_3n->AddXpoint(4,30.0); DNN_Res_3n->AddXpoint(4,30.0); TDR_3n->AddXpoint(4,30.0); TDR_Res_3n->AddXpoint(4,30.0); Perfect_3n->AddXpoint(4,30.0);
    DNN_4n->AddXpoint(4,30.0); DNN_Res_4n->AddXpoint(4,30.0); TDR_4n->AddXpoint(4,30.0); TDR_Res_4n->AddXpoint(4,30.0); Perfect_4n->AddXpoint(4,30.0);
    DNN_5n->AddXpoint(4,30.0); DNN_Res_5n->AddXpoint(4,30.0); TDR_5n->AddXpoint(4,30.0); TDR_Res_5n->AddXpoint(4,30.0); Perfect_5n->AddXpoint(4,30.0);
  
    // ------------------------------------------------------------
    
    // Next, start building graphs:
    
    DNN_1n->ComputeGraph(); DNN_Res_1n->ComputeGraph(); TDR_1n->ComputeGraph(); TDR_Res_1n->ComputeGraph(); Perfect_1n->ComputeGraph();
    DNN_2n->ComputeGraph(); DNN_Res_2n->ComputeGraph(); TDR_2n->ComputeGraph(); TDR_Res_2n->ComputeGraph(); Perfect_2n->ComputeGraph();
    DNN_3n->ComputeGraph(); DNN_Res_3n->ComputeGraph(); TDR_3n->ComputeGraph(); TDR_Res_3n->ComputeGraph(); Perfect_3n->ComputeGraph();
    DNN_4n->ComputeGraph(); DNN_Res_4n->ComputeGraph(); TDR_4n->ComputeGraph(); TDR_Res_4n->ComputeGraph(); Perfect_4n->ComputeGraph();
    DNN_5n->ComputeGraph(); DNN_Res_5n->ComputeGraph(); TDR_5n->ComputeGraph(); TDR_Res_5n->ComputeGraph(); Perfect_5n->ComputeGraph();
    
    // ------------------------------------------------------------

    // Plot graphs:
    Double_t ScaleFactor = 4.0;
    gROOT->SetBatch(kTRUE);
    
    TGraphErrors** PerfectGraphs = new TGraphErrors*[5];
    PerfectGraphs[0] = Perfect_1n->GetDataGraph();
    PerfectGraphs[1] = Perfect_2n->GetDataGraph();
    PerfectGraphs[2] = Perfect_3n->GetDataGraph();
    PerfectGraphs[3] = Perfect_4n->GetDataGraph();
    PerfectGraphs[4] = Perfect_5n->GetDataGraph();
    
    TString* Titles = new TString[5];
    Titles[0] = "Particle Gun: n=1";
    Titles[1] = "Particle Gun: n=2";
    Titles[2] = "Particle Gun: n=3";
    Titles[3] = "Particle Gun: n=4";
    Titles[4] = "Particle Gun: n=5";
    
    TCanvas* c1 = DrawGraphDoumaArray(5,3,2,PerfectGraphs,Titles,"Nr. of NeuLAND dp","Efficiency [%]",ScaleFactor,0,"AP");
    // DNN_1n->GetDataGraph()->SetLineStyle(1);
    
    // Draw Perfect graphs:
    Perfect_1n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_1n->GetDataGraph()->SetLineColor(kBlack); c1->cd(1); Perfect_1n->GetDataGraph()->Draw("PLsame");
    Perfect_2n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_2n->GetDataGraph()->SetLineColor(kBlack); c1->cd(2); Perfect_2n->GetDataGraph()->Draw("PLsame");
    Perfect_3n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_3n->GetDataGraph()->SetLineColor(kBlack); c1->cd(3); Perfect_3n->GetDataGraph()->Draw("PLsame");
    Perfect_4n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_4n->GetDataGraph()->SetLineColor(kBlack); c1->cd(4); Perfect_4n->GetDataGraph()->Draw("PLsame");
    Perfect_5n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_5n->GetDataGraph()->SetLineColor(kBlack); c1->cd(5); Perfect_5n->GetDataGraph()->Draw("PLsame");
    
    // Draw DNN graphs:
    DNN_1n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_1n->GetDataGraph()->SetLineColor(kRed);   c1->cd(1); DNN_1n->GetDataGraph()->Draw("PLsame");
    DNN_2n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_2n->GetDataGraph()->SetLineColor(kRed);   c1->cd(2); DNN_2n->GetDataGraph()->Draw("PLsame");
    DNN_3n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_3n->GetDataGraph()->SetLineColor(kRed);   c1->cd(3); DNN_3n->GetDataGraph()->Draw("PLsame");
    DNN_4n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_4n->GetDataGraph()->SetLineColor(kRed);   c1->cd(4); DNN_4n->GetDataGraph()->Draw("PLsame");
    DNN_5n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_5n->GetDataGraph()->SetLineColor(kRed);   c1->cd(5); DNN_5n->GetDataGraph()->Draw("PLsame");

    // Draw TDR graphs:
    TDR_1n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_1n->GetDataGraph()->SetLineColor(kBlue);  c1->cd(1); TDR_1n->GetDataGraph()->Draw("PLsame");
    TDR_2n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_2n->GetDataGraph()->SetLineColor(kBlue);  c1->cd(2); TDR_2n->GetDataGraph()->Draw("PLsame");
    TDR_3n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_3n->GetDataGraph()->SetLineColor(kBlue);  c1->cd(3); TDR_3n->GetDataGraph()->Draw("PLsame");
    TDR_4n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_4n->GetDataGraph()->SetLineColor(kBlue);  c1->cd(4); TDR_4n->GetDataGraph()->Draw("PLsame");
    TDR_5n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_5n->GetDataGraph()->SetLineColor(kBlue);  c1->cd(5); TDR_5n->GetDataGraph()->Draw("PLsame");

    // Draw Perfect Physics Errors:
    Perfect_1n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_1n->GetPhysGraph()->SetLineColor(kBlack); Perfect_1n->GetPhysGraph()->SetFillStyle(3000); Perfect_1n->GetPhysGraph()->SetFillColor(kGray+2); c1->cd(1); Perfect_1n->GetPhysGraph()->Draw("3same");
    Perfect_2n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_2n->GetPhysGraph()->SetLineColor(kBlack); Perfect_2n->GetPhysGraph()->SetFillStyle(3000); Perfect_2n->GetPhysGraph()->SetFillColor(kGray+2); c1->cd(2); Perfect_2n->GetPhysGraph()->Draw("3same");
    Perfect_3n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_3n->GetPhysGraph()->SetLineColor(kBlack); Perfect_3n->GetPhysGraph()->SetFillStyle(3000); Perfect_3n->GetPhysGraph()->SetFillColor(kGray+2); c1->cd(3); Perfect_3n->GetPhysGraph()->Draw("3same");
    Perfect_4n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_4n->GetPhysGraph()->SetLineColor(kBlack); Perfect_4n->GetPhysGraph()->SetFillStyle(3000); Perfect_4n->GetPhysGraph()->SetFillColor(kGray+2); c1->cd(4); Perfect_4n->GetPhysGraph()->Draw("3same");
    Perfect_5n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); Perfect_5n->GetPhysGraph()->SetLineColor(kBlack); Perfect_5n->GetPhysGraph()->SetFillStyle(3000); Perfect_5n->GetPhysGraph()->SetFillColor(kGray+2); c1->cd(5); Perfect_5n->GetPhysGraph()->Draw("3same");
    
    // Draw DNN Physics Errors:
    DNN_1n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_1n->GetPhysGraph()->SetLineColor(kRed);   DNN_1n->GetPhysGraph()->SetFillStyle(3000); DNN_1n->GetPhysGraph()->SetFillColor(kRed-7);   c1->cd(1); DNN_1n->GetPhysGraph()->Draw("3same");
    DNN_2n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_2n->GetPhysGraph()->SetLineColor(kRed);   DNN_2n->GetPhysGraph()->SetFillStyle(3000); DNN_2n->GetPhysGraph()->SetFillColor(kRed-7);   c1->cd(2); DNN_2n->GetPhysGraph()->Draw("3same");
    DNN_3n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_3n->GetPhysGraph()->SetLineColor(kRed);   DNN_3n->GetPhysGraph()->SetFillStyle(3000); DNN_3n->GetPhysGraph()->SetFillColor(kRed-7);   c1->cd(3); DNN_3n->GetPhysGraph()->Draw("3same");
    DNN_4n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_4n->GetPhysGraph()->SetLineColor(kRed);   DNN_4n->GetPhysGraph()->SetFillStyle(3000); DNN_4n->GetPhysGraph()->SetFillColor(kRed-7);   c1->cd(4); DNN_4n->GetPhysGraph()->Draw("3same");
    DNN_5n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_5n->GetPhysGraph()->SetLineColor(kRed);   DNN_5n->GetPhysGraph()->SetFillStyle(3000); DNN_5n->GetPhysGraph()->SetFillColor(kRed-7);   c1->cd(5); DNN_5n->GetPhysGraph()->Draw("3same");

    // Draw TDR Physics Errors:
    TDR_1n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_1n->GetPhysGraph()->SetLineColor(kBlue);  TDR_1n->GetPhysGraph()->SetFillStyle(3000); TDR_1n->GetPhysGraph()->SetFillColor(kAzure-2); c1->cd(1); TDR_1n->GetPhysGraph()->Draw("3same");
    TDR_2n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_2n->GetPhysGraph()->SetLineColor(kBlue);  TDR_2n->GetPhysGraph()->SetFillStyle(3000); TDR_2n->GetPhysGraph()->SetFillColor(kAzure-2); c1->cd(2); TDR_2n->GetPhysGraph()->Draw("3same");
    TDR_3n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_3n->GetPhysGraph()->SetLineColor(kBlue);  TDR_3n->GetPhysGraph()->SetFillStyle(3000); TDR_3n->GetPhysGraph()->SetFillColor(kAzure-2); c1->cd(3); TDR_3n->GetPhysGraph()->Draw("3same");
    TDR_4n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_4n->GetPhysGraph()->SetLineColor(kBlue);  TDR_4n->GetPhysGraph()->SetFillStyle(3000); TDR_4n->GetPhysGraph()->SetFillColor(kAzure-2); c1->cd(4); TDR_4n->GetPhysGraph()->Draw("3same");
    TDR_5n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_5n->GetPhysGraph()->SetLineColor(kBlue);  TDR_5n->GetPhysGraph()->SetFillStyle(3000); TDR_5n->GetPhysGraph()->SetFillColor(kAzure-2); c1->cd(5); TDR_5n->GetPhysGraph()->Draw("3same");
    
    // Draw Restricted DNN graphs:
    //DNN_Res_1n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); DNN_Res_1n->GetDataGraph()->SetLineColor(kRed); DNN_Res_1n->GetDataGraph()->SetLineStyle(2); c1->cd(1); DNN_Res_1n->GetDataGraph()->Draw("PLsame");
    DNN_Res_2n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); DNN_Res_2n->GetDataGraph()->SetLineColor(kRed); DNN_Res_2n->GetDataGraph()->SetLineStyle(2); c1->cd(2); DNN_Res_2n->GetDataGraph()->Draw("PLsame");
    DNN_Res_3n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); DNN_Res_3n->GetDataGraph()->SetLineColor(kRed); DNN_Res_3n->GetDataGraph()->SetLineStyle(2); c1->cd(3); DNN_Res_3n->GetDataGraph()->Draw("PLsame");
    DNN_Res_4n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); DNN_Res_4n->GetDataGraph()->SetLineColor(kRed); DNN_Res_4n->GetDataGraph()->SetLineStyle(2); c1->cd(4); DNN_Res_4n->GetDataGraph()->Draw("PLsame");
    DNN_Res_5n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); DNN_Res_5n->GetDataGraph()->SetLineColor(kRed); DNN_Res_5n->GetDataGraph()->SetLineStyle(2); c1->cd(5); DNN_Res_5n->GetDataGraph()->Draw("PLsame");

    // Draw TDR graphs:
    //TDR_Res_1n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); TDR_Res_1n->GetDataGraph()->SetLineColor(kBlue); TDR_Res_1n->GetDataGraph()->SetLineStyle(2); c1->cd(1); TDR_Res_1n->GetDataGraph()->Draw("PLsame");
    TDR_Res_2n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); TDR_Res_2n->GetDataGraph()->SetLineColor(kBlue); TDR_Res_2n->GetDataGraph()->SetLineStyle(2); c1->cd(2); TDR_Res_2n->GetDataGraph()->Draw("PLsame");
    TDR_Res_3n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); TDR_Res_3n->GetDataGraph()->SetLineColor(kBlue); TDR_Res_3n->GetDataGraph()->SetLineStyle(2); c1->cd(3); TDR_Res_3n->GetDataGraph()->Draw("PLsame");
    TDR_Res_4n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); TDR_Res_4n->GetDataGraph()->SetLineColor(kBlue); TDR_Res_4n->GetDataGraph()->SetLineStyle(2); c1->cd(4); TDR_Res_4n->GetDataGraph()->Draw("PLsame");
    TDR_Res_5n->GetDataGraph()->SetLineWidth(ScaleFactor*0.8); TDR_Res_5n->GetDataGraph()->SetLineColor(kBlue); TDR_Res_5n->GetDataGraph()->SetLineStyle(2); c1->cd(5); TDR_Res_5n->GetDataGraph()->Draw("PLsame");

    // Save & Close:
    c1->SaveAs("./AllMult_600MeV.png");
    c1->Close();
    
    // ----------------------------------------------------------------------
    
    // Generate Relative graphs:
    Double_t DNN_RelPhsheight = 3.0;
    Double_t TDR_RelPhsheight = 2.5;
    
    DNN_1n->GetRelativeGraph(DNN_Res_1n,DNN_RelPhsheight);
    DNN_2n->GetRelativeGraph(DNN_Res_2n,DNN_RelPhsheight);
    DNN_3n->GetRelativeGraph(DNN_Res_3n,DNN_RelPhsheight);
    DNN_4n->GetRelativeGraph(DNN_Res_4n,DNN_RelPhsheight);
    DNN_5n->GetRelativeGraph(DNN_Res_5n,DNN_RelPhsheight);
    
    TDR_1n->GetRelativeGraph(TDR_Res_1n,TDR_RelPhsheight);
    TDR_2n->GetRelativeGraph(TDR_Res_2n,TDR_RelPhsheight);
    TDR_3n->GetRelativeGraph(TDR_Res_3n,TDR_RelPhsheight);
    TDR_4n->GetRelativeGraph(TDR_Res_4n,TDR_RelPhsheight);
    TDR_5n->GetRelativeGraph(TDR_Res_5n,TDR_RelPhsheight);
    
    // Make the next painting:
    TCanvas* c2 = DrawGraphDoumaArray3(5,3,2,PerfectGraphs,Titles,"Nr. of NeuLAND dp","False Pos./True Pos. [dim. less]",ScaleFactor,0,"AP");
    
    // Draw DNN Physics Errors:
    DNN_1n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_1n->GetPhysGraph()->SetLineColor(kRed);   DNN_1n->GetPhysGraph()->SetFillStyle(3000); DNN_1n->GetPhysGraph()->SetFillColor(kRed-7);   c2->cd(1); DNN_1n->GetPhysGraph()->Draw("3same");
    DNN_2n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_2n->GetPhysGraph()->SetLineColor(kRed);   DNN_2n->GetPhysGraph()->SetFillStyle(3000); DNN_2n->GetPhysGraph()->SetFillColor(kRed-7);   c2->cd(2); DNN_2n->GetPhysGraph()->Draw("3same");
    DNN_3n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_3n->GetPhysGraph()->SetLineColor(kRed);   DNN_3n->GetPhysGraph()->SetFillStyle(3000); DNN_3n->GetPhysGraph()->SetFillColor(kRed-7);   c2->cd(3); DNN_3n->GetPhysGraph()->Draw("3same");
    DNN_4n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_4n->GetPhysGraph()->SetLineColor(kRed);   DNN_4n->GetPhysGraph()->SetFillStyle(3000); DNN_4n->GetPhysGraph()->SetFillColor(kRed-7);   c2->cd(4); DNN_4n->GetPhysGraph()->Draw("3same");
    DNN_5n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); DNN_5n->GetPhysGraph()->SetLineColor(kRed);   DNN_5n->GetPhysGraph()->SetFillStyle(3000); DNN_5n->GetPhysGraph()->SetFillColor(kRed-7);   c2->cd(5); DNN_5n->GetPhysGraph()->Draw("3same");

    // Draw TDR Physics Errors:
    TDR_1n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_1n->GetPhysGraph()->SetLineColor(kBlue);  TDR_1n->GetPhysGraph()->SetFillStyle(3000); TDR_1n->GetPhysGraph()->SetFillColor(kAzure-2); c2->cd(1); TDR_1n->GetPhysGraph()->Draw("3same");
    TDR_2n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_2n->GetPhysGraph()->SetLineColor(kBlue);  TDR_2n->GetPhysGraph()->SetFillStyle(3000); TDR_2n->GetPhysGraph()->SetFillColor(kAzure-2); c2->cd(2); TDR_2n->GetPhysGraph()->Draw("3same");
    TDR_3n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_3n->GetPhysGraph()->SetLineColor(kBlue);  TDR_3n->GetPhysGraph()->SetFillStyle(3000); TDR_3n->GetPhysGraph()->SetFillColor(kAzure-2); c2->cd(3); TDR_3n->GetPhysGraph()->Draw("3same");
    TDR_4n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_4n->GetPhysGraph()->SetLineColor(kBlue);  TDR_4n->GetPhysGraph()->SetFillStyle(3000); TDR_4n->GetPhysGraph()->SetFillColor(kAzure-2); c2->cd(4); TDR_4n->GetPhysGraph()->Draw("3same");
    TDR_5n->GetPhysGraph()->SetLineWidth(ScaleFactor*1.5); TDR_5n->GetPhysGraph()->SetLineColor(kBlue);  TDR_5n->GetPhysGraph()->SetFillStyle(3000); TDR_5n->GetPhysGraph()->SetFillColor(kAzure-2); c2->cd(5); TDR_5n->GetPhysGraph()->Draw("3same");
    
    // Draw DNN graphs:
    DNN_1n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_1n->GetDataGraph()->SetLineColor(kRed);   c2->cd(1); DNN_1n->GetDataGraph()->Draw("PLsame");
    DNN_2n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_2n->GetDataGraph()->SetLineColor(kRed);   c2->cd(2); DNN_2n->GetDataGraph()->Draw("PLsame");
    DNN_3n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_3n->GetDataGraph()->SetLineColor(kRed);   c2->cd(3); DNN_3n->GetDataGraph()->Draw("PLsame");
    DNN_4n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_4n->GetDataGraph()->SetLineColor(kRed);   c2->cd(4); DNN_4n->GetDataGraph()->Draw("PLsame");
    DNN_5n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); DNN_5n->GetDataGraph()->SetLineColor(kRed);   c2->cd(5); DNN_5n->GetDataGraph()->Draw("PLsame");

    // Draw TDR graphs:
    TDR_1n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_1n->GetDataGraph()->SetLineColor(kBlue);  c2->cd(1); TDR_1n->GetDataGraph()->Draw("PLsame");
    TDR_2n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_2n->GetDataGraph()->SetLineColor(kBlue);  c2->cd(2); TDR_2n->GetDataGraph()->Draw("PLsame");
    TDR_3n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_3n->GetDataGraph()->SetLineColor(kBlue);  c2->cd(3); TDR_3n->GetDataGraph()->Draw("PLsame");
    TDR_4n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_4n->GetDataGraph()->SetLineColor(kBlue);  c2->cd(4); TDR_4n->GetDataGraph()->Draw("PLsame");
    TDR_5n->GetDataGraph()->SetLineWidth(ScaleFactor*1.5); TDR_5n->GetDataGraph()->SetLineColor(kBlue);  c2->cd(5); TDR_5n->GetDataGraph()->Draw("PLsame");
      
    c2->SaveAs("./Mult_SignalToNoise_600MeV.png");
    c2->Close();
}

void MakePlots()
{
    // Connect to InputsFile:
    R3BInputClass* Inputs = new R3BInputClass();
    Inputs->DisableErrorPrinting();
    Inputs->LinkFile("../INCLXX/INCLXX_OneBar/InputsFile.txt");
    Inputs->ReadFile();
    
    // Next, Generate the Plots-Class:
    R3B_DNN_Plots* ThePlots = new R3B_DNN_Plots();
    ThePlots->DisableErrorPrinting();
    ThePlots->LinkInputClass(Inputs);
    ThePlots->CheckFiles();
    ThePlots->SetScaleFactor(4.0);
    ThePlots->SetOutputPath("./");
    ThePlots->SetLogScaleMinimum(0.3);
    
    // Then, create the plots that we want:
    ThePlots->Compare3Graphs("../INCLXX/INCLXX_Samurai/NeuLAND_Histograms_NoElimination.root","../BERT/BERT_Samurai/NeuLAND_Histograms.root","../INCLXX/INCLXX_Samurai/NeuLAND_Histograms_3Elimination.root","JanDetectionGraph","Detection Threshold [MeV]","Detection Efficiency [dim. less]",kFALSE,0.0,0.4);
    ThePlots->Compare3Graphs("../INCLXX/INCLXX_Samurai/NeuLAND_Histograms_NoElimination.root","../BERT/BERT_Samurai/NeuLAND_Histograms.root","../INCLXX/INCLXX_Samurai/NeuLAND_Histograms_FullElimination.root","JanDetectionGraph","Detection Threshold [MeV]","Detection Efficiency [dim. less]",kFALSE,0.0,0.4);
    
    Bool_t UseLogScale_Edep = kTRUE;
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep0","Energy Deposition [keV]","Counts",2,UseLogScale_Edep);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep2","Energy Deposition [keV]","Counts",2,UseLogScale_Edep);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep9","Energy Deposition [keV]","Counts",2,UseLogScale_Edep);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep10","Energy Deposition [keV]","Counts",2,UseLogScale_Edep);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep11","Energy Deposition [keV]","Counts",2,UseLogScale_Edep);
    
    Bool_t UseLogScale_AlphaScatter = kFALSE;
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter0","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter1","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter2","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../BERT/BERT_OneBar/NeuLAND_Histograms.root","../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter4","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter9","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter10","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter11","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter12","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    
    Bool_t UseLogScale_Erel = kFALSE;
    ThePlots->CompareHist("../INCLXX/INCLXX_Large/NeuLAND_Histograms.root","../BERT/BERT_Large/NeuLAND_Histograms.root","ErelSpectrum_Perfect_MCData_4","Relative Energy [MeV]","Counts",2,UseLogScale_Erel);
    ThePlots->CompareHist("../INCLXX/INCLXX_Large/NeuLAND_Histograms.root","../BERT/BERT_Large/NeuLAND_Histograms.root","ErelSpectrum_Perfect_Signals_4","Relative Energy [MeV]","Counts",2,UseLogScale_Erel);
    ThePlots->CompareHist("../INCLXX/INCLXX_Large/NeuLAND_Histograms.root","../BERT/BERT_Large/NeuLAND_Histograms.root","ErelSpectrum_Perfect_Clusters_Time_4","Relative Energy [MeV]","Counts",2,UseLogScale_Erel);
    ThePlots->CompareHist("../INCLXX/INCLXX_Large/NeuLAND_Histograms.root","../BERT/BERT_Large/NeuLAND_Histograms.root","ErelSpectrum_Perfect_Clusters_Z_4","Relative Energy [MeV]","Counts",2,UseLogScale_Erel);
    ThePlots->CompareHist("../INCLXX/INCLXX_Large/NeuLAND_Histograms.root","../BERT/BERT_Large/NeuLAND_Histograms.root","ErelSpectrum_TradMed_TrueMult_4","Relative Energy [MeV]","Counts",2,UseLogScale_Erel);

    ThePlots->SetOutputPath("./INCLXX/");
    ThePlots->EdepHists("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./BERT/");
    ThePlots->EdepHists("../BERT/BERT_OneBar/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("../BERT/BERT_OneBar/NeuLAND_Histograms.root",kFALSE);
    
    // Print errors:
    Inputs->PrintAllErrors();
    ThePlots->PrintAllErrors();
}

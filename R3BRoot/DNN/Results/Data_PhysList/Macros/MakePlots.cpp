void MakePlots()
{
    // Connect to InputsFile:
    R3BInputClass* Inputs = new R3BInputClass();
    Inputs->DisableErrorPrinting();
    Inputs->LinkFile("../INCLXX_OneBar/InputsFile.txt");
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
    ThePlots->CompareHist("../INCLXX_OneBar/NeuLAND_Histograms.root","../BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep0","Energy Deposition [keV]","Counts",2,kTRUE);
    ThePlots->CompareHist("../INCLXX_OneBar/NeuLAND_Histograms.root","../BERT_OneBar/NeuLAND_Histograms.root","Hist_Edep2","Energy Deposition [keV]","Counts",2,kTRUE);
    
    ThePlots->SetOutputPath("./INCLXX/");
    ThePlots->EdepHists("../INCLXX_OneBar/NeuLAND_Histograms.root");
    
    ThePlots->SetOutputPath("./BERT/");
    ThePlots->EdepHists("../BERT_OneBar/NeuLAND_Histograms.root");
    
    // Print errors:
    Inputs->PrintAllErrors();
    ThePlots->PrintAllErrors();
}

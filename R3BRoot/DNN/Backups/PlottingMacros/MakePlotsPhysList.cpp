void MakePlotsPhysList()
{
    // Connect to InputsFile:
    R3BInputClass* Inputs = new R3BInputClass();
    Inputs->DisableErrorPrinting();
    Inputs->LinkFile("./QGSP_INCLXX_HP/InputsFile.txt");
    Inputs->ReadFile();
    
    // Next, Generate the Plots-Class:
    R3B_DNN_Plots* ThePlots = new R3B_DNN_Plots();
    ThePlots->DisableErrorPrinting();
    ThePlots->LinkInputClass(Inputs);
    ThePlots->CheckFiles();
    ThePlots->SetScaleFactor(4.0);
    ThePlots->SetLogScaleMinimum(0.3);
    ThePlots->SetOutputPath("./Plots/");
    
    Bool_t UseLogScale_AlphaScatter = kFALSE;
    //ThePlots->CompareHist("../INCLXX/INCLXX_OneBar/NeuLAND_Histograms.root","../BERT/BERT_OneBar/NeuLAND_Histograms.root","Hist_AlphaScatter0","Scattering Angle [deg]","Counts",4,UseLogScale_AlphaScatter);
    
    ThePlots->SetOutputPath("./Plots/QGSP_INCLXX_HP/");
    ThePlots->EdepHists("./QGSP_INCLXX_HP/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./QGSP_INCLXX_HP/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/QGSP_BERT_HP/");
    ThePlots->EdepHists("./QGSP_BERT_HP/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./QGSP_BERT_HP/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/QGSP_BIC_HP/");
    ThePlots->EdepHists("./QGSP_BIC_HP/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./QGSP_BIC_HP/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/QGSP_INCLXX/");
    ThePlots->EdepHists("./QGSP_INCLXX/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./QGSP_INCLXX/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/FTFP_INCLXX_HP/");
    ThePlots->EdepHists("./FTFP_INCLXX_HP/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./FTFP_INCLXX_HP/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/QBBC/");
    ThePlots->EdepHists("./QBBC/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./QBBC/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/ShieldingM/");
    ThePlots->EdepHists("./ShieldingM/NeuLAND_Histograms.root",kTRUE);
    ThePlots->ScatterHists("./ShieldingM/NeuLAND_Histograms.root",kFALSE);
    
    ThePlots->SetOutputPath("./Plots/");
    TCanvas* c1 = ThePlots->OneScatterHist("./QGSP_INCLXX_HP/NeuLAND_Histograms.root",kFALSE,"Protons",4);
    
    TFile* f1 = new TFile("./QGSP_INCLXX_HP/NeuLAND_Histograms.root","read");
    TH1D* h1 = (TH1D*) f1->Get("Hist_AlphaScatter2");
    h1->Rebin(4);
    h1->SetLineColor(kBlack);
    h1->SetLineWidth(4.0);
    c1->cd();
    h1->Draw("same");
    Double_t Ypos = 1.06;
    Double_t Xmin = h1->GetXaxis()->GetXmin();
    Double_t Xmax = h1->GetXaxis()->GetXmax();
    Double_t Ymin = h1->GetMinimum();
    Double_t Ymax = h1->GetMaximum();
    TPaveText* text1 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text1->SetFillColor(0); 
    text1->AddText("QGSP_INCLXX_HP"); 
    text1->SetTextColor(kBlack); 
    text1->SetTextSize(0.04); 
    text1->SetTextFont(1);
    text1->SetTextAngle(0.0); 
    text1->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f2 = new TFile("./QGSP_BERT_HP/NeuLAND_Histograms.root","read");
    TH1D* h2 = (TH1D*) f2->Get("Hist_AlphaScatter2");
    h2->Rebin(4);
    h2->SetLineColor(kRed);
    h2->SetLineWidth(4.0);
    c1->cd();
    h2->Draw("same");
    TPaveText* text2 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text2->SetFillColor(0); 
    text2->AddText("QGSP_BERT_HP"); 
    text2->SetTextColor(kRed); 
    text2->SetTextSize(0.04); 
    text2->SetTextFont(1);
    text2->SetTextAngle(0.0); 
    text2->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f3 = new TFile("./QGSP_BIC_HP/NeuLAND_Histograms.root","read");
    TH1D* h3 = (TH1D*) f3->Get("Hist_AlphaScatter2");
    h3->Rebin(4);
    h3->SetLineColor(kGreen+3);
    h3->SetLineWidth(4.0);
    c1->cd();
    h3->Draw("same");
    TPaveText* text3 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text3->SetFillColor(0); 
    text3->AddText("QGSP_BIC_HP"); 
    text3->SetTextColor(kGreen+3); 
    text3->SetTextSize(0.04); 
    text3->SetTextFont(1);
    text3->SetTextAngle(0.0); 
    text3->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f4 = new TFile("./QGSP_INCLXX/NeuLAND_Histograms.root","read");
    TH1D* h4 = (TH1D*) f4->Get("Hist_AlphaScatter2");
    h4->Rebin(4);
    h4->SetLineColor(kMagenta);
    h4->SetLineWidth(4.0);
    c1->cd();
    h4->Draw("same");
    TPaveText* text4 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text4->SetFillColor(0); 
    text4->AddText("QGSP_INCLXX"); 
    text4->SetTextColor(kMagenta); 
    text4->SetTextSize(0.04); 
    text4->SetTextFont(1);
    text4->SetTextAngle(0.0); 
    text4->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f5 = new TFile("./FTFP_INCLXX_HP/NeuLAND_Histograms.root","read");
    TH1D* h5 = (TH1D*) f5->Get("Hist_AlphaScatter2");
    h5->Rebin(4);
    h5->SetLineColor(kYellow+2);
    h5->SetLineWidth(4.0);
    c1->cd();
    h5->Draw("same");
    TPaveText* text5 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text5->SetFillColor(0); 
    text5->AddText("FTFP_INCLXX_HP"); 
    text5->SetTextColor(kYellow+2); 
    text5->SetTextSize(0.04); 
    text5->SetTextFont(1);
    text5->SetTextAngle(0.0); 
    text5->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f6 = new TFile("./QBBC/NeuLAND_Histograms.root","read");
    TH1D* h6 = (TH1D*) f6->Get("Hist_AlphaScatter2");
    h6->Rebin(4);
    h6->SetLineColor(kAzure+1);
    h6->SetLineWidth(4.0);
    c1->cd();
    h6->Draw("same");
    TPaveText* text6 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text6->SetFillColor(0); 
    text6->AddText("QBBC"); 
    text6->SetTextColor(kAzure+1); 
    text6->SetTextSize(0.04); 
    text6->SetTextFont(1);
    text6->SetTextAngle(0.0); 
    text6->Draw("same");
    Ypos = Ypos - 0.06;
    
    TFile* f7 = new TFile("./ShieldingM/NeuLAND_Histograms.root","read");
    TH1D* h7 = (TH1D*) f7->Get("Hist_AlphaScatter2");
    h7->Rebin(4);
    h7->SetLineColor(kOrange+2);
    h7->SetLineWidth(4.0);
    c1->cd();
    h7->Draw("same");
    TPaveText* text7 = new TPaveText(Xmin+(Xmax-Xmin)*0.75,Ymin+(Ymax-Ymin)*Ypos,Xmin+(Xmax-Xmin)*0.76,Ymin+(Ymax-Ymin)*Ypos*1.01,"NB");
    text7->SetFillColor(0); 
    text7->AddText("ShieldingM"); 
    text7->SetTextColor(kOrange+2); 
    text7->SetTextSize(0.04); 
    text7->SetTextFont(1);
    text7->SetTextAngle(0.0); 
    text7->Draw("same");
    Ypos = Ypos - 0.06;
    
    c1->SaveAs("./Test.png");
    
    // Print errors:
    Inputs->PrintAllErrors();
    ThePlots->PrintAllErrors();
}

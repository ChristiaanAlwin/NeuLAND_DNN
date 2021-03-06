inline void ConnectParFileToRuntimeDb(const TString parFile, FairRuntimeDb* rtdb)
{
    auto io = new FairParRootFileIo();
    io->open(parFile);
    rtdb->setFirstInput(io);
    rtdb->setOutput(io);
    rtdb->saveOutput();
}

void TradMed_Apply(Int_t const TotalNumberOfThreads = 1)
{
    // Applies the full traditional method:
    
    // Connect to InputsFile:
    R3BInputClass* Inputs = new R3BInputClass();
    Inputs->DisableErrorPrinting();
    Inputs->LinkFile("../InputFiles/InputsFile.txt");
    Inputs->ReadFile();
    
    // Retrieve General Inputs:
    TString TheOutputPath = Inputs->GetInputString("TheOutputPath");
    TString MCFile = TheOutputPath + Inputs->GetInputString("R3BRoot_MonteCarlo_OutputFile");
    TString ParFile = TheOutputPath + Inputs->GetInputString("R3BRoot_MonteCarlo_ParFile");
    TString VisFile = TheOutputPath + Inputs->GetInputString("R3BRoot_MonteCarlo_PlotFile");
    TString DigiFile = TheOutputPath + Inputs->GetInputString("NeuLAND_Digitizer_OutputFile");
    TString JanFile = TheOutputPath + Inputs->GetInputString("NeuLAND_Digitizer_JanMayer_Processing");
    TString SignalFile = TheOutputPath + Inputs->GetInputString("NeuLAND_SignalFile");
    TString ClusterFile = TheOutputPath + Inputs->GetInputString("NeuLAND_ClusterFile");
    TString FilteredFile = TheOutputPath + Inputs->GetInputString("NeuLAND_FilteredClusterFile");
    TString TradMedFile = TheOutputPath + Inputs->GetInputString("NeuLAND_TraditionalMethod_ReconstructionFile");
    TString ScoreFile = TheOutputPath + Inputs->GetInputString("NeuLAND_ScoringOutputFile");
    TString DNNScoreFile = TheOutputPath + Inputs->GetInputString("NeuLAND_DNNScoringOutputFile");
    TString DistFile = TheOutputPath + Inputs->GetInputString("NeuLAND_Distributions");
    TString DNNFile = TheOutputPath + Inputs->GetInputString("NeuLAND_DNN_OutputFile");
    TString FinalFile = TheOutputPath + Inputs->GetInputString("NeuLAND_Reconstruction_FinalFile");
    TString BetaFile = TheOutputPath + Inputs->GetInputString("BetaReconstruction_OutputFile");
    TString SingleFile = TheOutputPath + Inputs->GetInputString("SingleReconstruction_OutputFile");
    TString CombiFile = TheOutputPath + Inputs->GetInputString("NEBULA_Combination_OutputFile");
    Int_t nEvents = Inputs->GetInputInteger("R3BRoot_nEvents");
    Bool_t UseNEBULA = Inputs->GetInputBoolian("NEBULA_Include_in_SETUP");
    
    // Corrent the number of events for MT effects:
    if (TotalNumberOfThreads>1)
    {
        nEvents = nEvents/TotalNumberOfThreads;
        nEvents = nEvents*TotalNumberOfThreads;
    }
    
    // Then, execute the macro if there are no errors:
    if (Inputs->ContainsNoErrors()==kTRUE)
    {
        // Activate the timer:
        TStopwatch timer;
        timer.Start();

        // Create the Master FairTask:
        FairRunAna* run = new FairRunAna();
        run->SetInputFile(ClusterFile);
        run->AddFriend(DNNFile);
        run->SetOutputFile(TradMedFile);
        ConnectParFileToRuntimeDb(ParFile,run->GetRuntimeDb());
  
        // Create the R3B_TradMed_NeutronTracker that is to use cuts for the multiplicity:
        R3B_TradMed_NeutronTracker* MultCuts = new R3B_TradMed_NeutronTracker();
        MultCuts->SetDetector("NeuLAND");
        MultCuts->LinkInputClass(Inputs);
        MultCuts->SetNevents(nEvents);
        MultCuts->SetMultiplicityDetermination("Cuts");
        
        // Create the R3B_TradMed_NeutronTracker that is to use a perfect multiplicity:
        R3B_TradMed_NeutronTracker* MultPerfect = new R3B_TradMed_NeutronTracker();
        MultPerfect->SetDetector("NeuLAND");
        MultPerfect->LinkInputClass(Inputs);
        MultPerfect->SetNevents(nEvents);
        MultPerfect->SetMultiplicityDetermination("Perfect");
        
        // Create the R3B_TradMed_NeutronTracker that is to use a DNN multiplicity:
        R3B_TradMed_NeutronTracker* MultDNN = new R3B_TradMed_NeutronTracker();
        MultDNN->SetDetector("NeuLAND");
        MultDNN->LinkInputClass(Inputs);
        MultDNN->SetNevents(nEvents);
        MultDNN->SetMultiplicityDetermination("DNN");
        
        // Add all of them to the Mother of FairTasks:
        run->AddTask(MultCuts);
        run->AddTask(MultPerfect);
        run->AddTask(MultDNN);
        
        // Same for NEBULA:
        if (UseNEBULA==kTRUE)
        {
            R3B_TradMed_NeutronTracker* NEBMultCuts = new R3B_TradMed_NeutronTracker();
            NEBMultCuts->SetDetector("NEBULA");
            NEBMultCuts->LinkInputClass(Inputs);
            NEBMultCuts->SetNevents(nEvents);
            NEBMultCuts->SetMultiplicityDetermination("Cuts");
        
            R3B_TradMed_NeutronTracker* NEBMultPerfect = new R3B_TradMed_NeutronTracker();
            NEBMultPerfect->SetDetector("NEBULA");
            NEBMultPerfect->LinkInputClass(Inputs);
            NEBMultPerfect->SetNevents(nEvents);
            NEBMultPerfect->SetMultiplicityDetermination("Perfect");
        
            R3B_TradMed_NeutronTracker* NEBMultDNN = new R3B_TradMed_NeutronTracker();
            NEBMultDNN->SetDetector("NEBULA");
            NEBMultDNN->LinkInputClass(Inputs);
            NEBMultDNN->SetNevents(nEvents);
            NEBMultDNN->SetMultiplicityDetermination("DNN");
        
            run->AddTask(NEBMultCuts);
            run->AddTask(NEBMultPerfect);
            run->AddTask(NEBMultDNN);
        }
            

        // Run the FairTasks:
        run->Init();
        run->Run(0,nEvents);

        // Finish the timer:
        timer.Stop();

        // Give final output:
        cout << endl;
        cout << "Macro finished succesfully!" << endl;
        cout << "Output file written: " << TradMedFile << endl;
        cout << "Parameter file written: " << ParFile << endl;
        cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
        cout << endl;
    }
    else
    {
        // Then, print what has gone wrong:
        Inputs->PrintAllErrors();
    }

    // Done.
}

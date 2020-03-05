void R3BNeuLANDTranslator::TranslateSignals(TClonesArray* fDigiArray, TClonesArray* fSignalArray, TString const ArrayCase)
{
    // This function fills the TClonesArray fSignalArray
    // by using fDigiArray. It is a simple 1-1 corespondance.
    // Both the Simulation and the Experimental markings of the
    // signals will be put to kFALSE. These markings 
    // have to be dealt with by different functions.
    Int_t nDigis = fDigiArray->GetEntries();
    
    // Decide which digis get broken:
    Bool_t* BrokenChannels = new Bool_t [nDigis];
    Double_t RandomNumber;
    
    // Loop over all digis:
    for (Int_t k = 0; k<nDigis; ++k)
    {
        // Default: channels stay alive.
        BrokenChannels[k] = kFALSE;
        
        // Only break when we want to and when we are in validation mode:
        if ((BreakChannels==kTRUE)&&(ValidationMode==kTRUE))
        {
            // Decide upon breaking:
            RandomNumber = TheGenerator->Uniform(1.0);
            if (RandomNumber<BreakProbability) {BrokenChannels[k] = kTRUE;}
        }
    }
    
    // Start translating:
    if (nDigis==0) {} // If there are no signals, it is pointless to process them!
    else 
    {
        // Declare what we need to process the information 1-by-1:
        R3BNeulandDigi* Digi;
        R3BSignal* Signal;
        TVector3 Position;
        Double_t Energy;
        Double_t Time;
        Int_t Paddle;
        Int_t SignalIndex = 0;
        
        // Next, loop over the digis:
        for (Int_t k = 0; k<nDigis; ++k)
        {
            // Decide if the digi is broken or not:
            if (BrokenChannels[k]==kFALSE)
            {
                // Then, create the new signal:
                new ((*fSignalArray)[SignalIndex]) R3BSignal();
        
                // Retrieve individual digi and signal:
                Digi = (R3BNeulandDigi*) fDigiArray->At(k);
                Signal = (R3BSignal*) fSignalArray->At(SignalIndex);
        
                // And next, retrieve digi info:
                Position = Digi->GetPosition();
                Energy = Digi->GetE();
                Time = Digi->GetT();
                Paddle = Digi->GetPaddle(); // NOTE: ranges from 1-3000!
            
                // Check that it indeed starts at 1 and ends at 3000:
                if (Paddle<=0) {cout << "### NeuLANDTranslator-Error: Paddle index was interpreted wrong!\n";}
                if (Paddle>nDoublePlanes*nPaddlesPerPlane*2) {cout << "### NeuLANDTranslator-Error: Paddle index found to be too large!\n";}
        
                // And put this into the signal:
                Signal->SetPositionX(Position.X());
                Signal->SetPositionY(Position.Y());
                Signal->SetPositionZ(Position.Z());
                Signal->SetTime(Time);
                Signal->SetEnergy(Energy);
                Signal->SetCrystalIndex(Paddle);
        
                // Set default labels:
                Signal->SetNonPrimarySim();
                Signal->SetNonPrimaryExp();
                
                // Update SignalIndex:
                SignalIndex = SignalIndex + 1;
            }
            else
            {
                if (DisplayBreaks==kTRUE)
                {
                    Digi = (R3BNeulandDigi*) fDigiArray->At(k);
                    cout << "We broke Channel " << Digi->GetPaddle() << " in event " << EventCounter << " Prob = " << BreakProbability << "\n";
                }
            }
        }
    }
    
    // Now add additional signals to simulate the noise:
    // NOTE: We never add VETO or NEBULA-VETO noise as it is right now!
    if ((AddNoisyChannes==kTRUE)&&(ValidationMode==kTRUE)&&((ArrayCase=="NeuLAND")||(ArrayCase=="NEBULA")))
    {
        // Declare what we need:
        Int_t nSignals;
        Int_t nBars = nPaddlesPerPlane*nDoublePlanes*2;
        Int_t BarIndex;
        Int_t CurrentPlane;
        Int_t CurrentModule;
        Int_t LocalPaddle;
        Bool_t Bar_IsFilled;
        Double_t xpos;
        Double_t ypos;
        Double_t zpos;
        TVector3 Position;
        R3BSignal* Signal;
        
        // Loop over all existing paddles:
        for (Int_t ksig = 0; ksig<nBars; ++ksig)
        {
            // Decide if this one has to become noisy:
            RandomNumber = TheGenerator->Uniform(1.0);
            
            if (RandomNumber<NoiseProbability)
            {
                if (DisplayNoise==kTRUE) {cout << "Added Bar " << ksig+1 << " in event " << EventCounter << " as noise.";}
                
                // Then, it should become noisy. Check first that
                // it does not yet contain a signal:
                Bar_IsFilled = kFALSE;
                nSignals = fSignalArray->GetEntries();
                
                for (Int_t k = 0; k<nSignals; ++k)
                {
                    Signal = (R3BSignal*) fSignalArray->At(k);
                    BarIndex = Signal->GetCrystalIndex();
                    
                    // BarIndex starts at one:
                    if (BarIndex==(ksig+1))
                    {
                        // Then, we do nothing. Even though the bar was selected
                        // to produce noise, it already has a signal. So we keep that
                        // signal and do not generate noise.
                        Bar_IsFilled = kTRUE;
                    }
                    
                    // Close loop over existing signals:
                }
                
                // Now, if the bar is not yet filled, we will fill it with noise:
                if (Bar_IsFilled==kFALSE)
                {
                    // Begin by generating a new signal:
                    new ((*fSignalArray)[nSignals]) R3BSignal();
                    
                    // Set bar index properly:
                    Signal = (R3BSignal*) fSignalArray->At(nSignals);
                    Signal->SetCrystalIndex(ksig+1); // has to start at 1!
                    
                    if (ArrayCase=="NeuLAND")
                    {
                        // Extract current plane and local paddle index:
                        CurrentPlane = Signal->GetCurrentPlane("NeuLAND",nPaddlesPerPlane);
                        LocalPaddle = Signal->GetCrystalIndex() - (CurrentPlane-1)*nPaddlesPerPlane;
                    
                        // Define z-position:
                        zpos = -1.0*NeuLAND_TotalBarThicknessZ*((Int_t) nDoublePlanes) + (((Int_t) (CurrentPlane-1))+0.5)*NeuLAND_TotalBarThicknessZ;
                    
                        // Next, define x and y position:
                        if (Signal->IsHorPaddle("NeuLAND",nPaddlesPerPlane)==kTRUE)
                        {
                            // y-pos comes from geometry:
                            ypos = -0.5*NeuLAND_TotalBarThicknessXY*((Int_t) nPaddlesPerPlane) + (((Int_t) (LocalPaddle-1))+0.5)*NeuLAND_TotalBarThicknessXY;
                        
                            // x-pos comes from a random number:
                            xpos = -0.5*NeuLAND_Total_Bar_Length + TheGenerator->Uniform(NeuLAND_Total_Bar_Length);
                        }
                        else
                        {
                            // Then, x comes from geometry:
                            xpos = -0.5*NeuLAND_TotalBarThicknessXY*((Int_t) nPaddlesPerPlane) + (((Int_t) (LocalPaddle-1))+0.5)*NeuLAND_TotalBarThicknessXY;
                            
                            // And y is a random number:
                            ypos = -0.5*NeuLAND_Total_Bar_Length + TheGenerator->Uniform(NeuLAND_Total_Bar_Length);
                        }
                        
                        // Load the vectors into a TVector3:
                        Position.SetX(xpos);
                        Position.SetY(ypos);
                        Position.SetZ(zpos);
                        
                        // Perform the rotations:
                        Position.RotateX(NeuLAND_Rot_X*TMath::Pi()/180.0);
                        Position.RotateY(NeuLAND_Rot_Y*TMath::Pi()/180.0);
                        Position.RotateZ(NeuLAND_Rot_Z*TMath::Pi()/180.0);
                
                        // And then, shift:
                        xpos = Position.X() + NeuLAND_Center_X;
                        ypos = Position.Y() + NeuLAND_Center_Y;
                        zpos = Position.Z() + NeuLAND_Front_Z + NeuLAND_TotalBarThicknessZ*((Int_t) nDoublePlanes);
                        
                        // Next, we can fill our signal:
                        Signal->SetPositionX(xpos);
                        Signal->SetPositionY(ypos);
                        Signal->SetPositionZ(zpos);
                        
                        // Set default labels:
                        Signal->SetNonPrimarySim();
                        Signal->SetNonPrimaryExp();
                        
                        // Set energy and time:
                        Signal->SetEnergy(NeuLANDScorers->ComputeSignalRandomValue("Energy"));
                        Signal->SetTime(NeuLANDScorers->ComputeSignalRandomValue("TOF"));
                        
                        // Done.
                    }
                    else if (ArrayCase=="NEBULA")
                    {
                        // Extract current plane and local paddle index:
                        CurrentPlane = Signal->GetCurrentPlane("NeuLAND",NEBULAPaddlesPerPlane);
                        LocalPaddle = Signal->GetCrystalIndex() - (CurrentPlane-1)*NEBULAPaddlesPerPlane;
                    
                        // Define z-position:
                        zpos = -1.0*NEBULA_TotalBarThickness*((Int_t) NEBULADoublePlanes) + (((Int_t) (CurrentPlane-1))+0.5)*NEBULA_TotalBarThickness;
                        
                        // Add distance between modules:
                        CurrentModule = Signal->GetCurrentPlane("NeuLAND",2*NEBULAPaddlesPerPlane) - 1; // So it starts at zero:
                        zpos = zpos + ((Int_t) CurrentModule)*NEBULADoublePLaneDistance;
                        
                        // Define xpos:
                        xpos = -0.5*NEBULA_TotalBarThickness*((Int_t) NEBULAPaddlesPerPlane) + (((Int_t) (LocalPaddle-1))+0.5)*NEBULA_TotalBarThickness;
                        
                        // Define ypos:
                        ypos = -0.5*NEBULA_Total_Bar_Length + TheGenerator->Uniform(NEBULA_Total_Bar_Length);
                        
                        // Do transformations and rotations:
                        Position.SetX(xpos);
                        Position.SetY(ypos);
                        Position.SetZ(zpos);
                        
                        // Perform the rotations:
                        Position.RotateX(NEBULA_Rot_X*TMath::Pi()/180.0);
                        Position.RotateY(NEBULA_Rot_Y*TMath::Pi()/180.0);
                        Position.RotateZ(NEBULA_Rot_Z*TMath::Pi()/180.0);
                
                        // And then, shift:
                        xpos = Position.X() + NEBULA_Center_X;
                        ypos = Position.Y() + NEBULA_Center_Y;
                        zpos = Position.Z() + NEBULA_Front_Z + NEBULA_TotalBarThickness*((Int_t) NEBULADoublePlanes);
                        
                        // Next, we can fill our signal:
                        Signal->SetPositionX(xpos);
                        Signal->SetPositionY(ypos);
                        Signal->SetPositionZ(zpos);
                        
                        // Set default labels:
                        Signal->SetNonPrimarySim();
                        Signal->SetNonPrimaryExp();
                        
                        // Set energy and time:
                        Signal->SetEnergy(NEBULAScorers->ComputeSignalRandomValue("Energy"));
                        Signal->SetTime(NEBULAScorers->ComputeSignalRandomValue("TOF"));
                        
                        // Done.
                    }
                    else
                    {
                        // Just some default situation:
                        Signal->SetPositionX(0.0);
                        Signal->SetPositionY(0.0);
                        Signal->SetPositionZ(0.0);
                        
                        // Set default labels:
                        Signal->SetNonPrimarySim();
                        Signal->SetNonPrimaryExp();
                        
                        // Set energy and time:
                        Signal->SetEnergy(NeuLANDScorers->ComputeSignalRandomValue("Energy"));
                        Signal->SetTime(NeuLANDScorers->ComputeSignalRandomValue("TOF"));
                        
                        // Done.
                    }
                    
                    if (DisplayNoise==kTRUE)
                    {
                        cout << " E = " << Signal->GetEnergy();
                        cout << " t = " << Signal->GetTime();
                        cout << " x = " << Signal->GetPositionX();
                        cout << " y = " << Signal->GetPositionY();
                        cout << " z = " << Signal->GetPositionZ();
                        cout << " n = " << Signal->GetCrystalIndex();
                    }
            
                    // Close the fill-test
                }
                else
                {
                    if (DisplayNoise==kTRUE) {cout << " Bar already contained data.";}
                }
                
                if (DisplayNoise==kTRUE) {cout << "\n";}
                
                // close bar-loop:
            }
        }
        
        // Done adding noise.
    }
    
    // delete break-array:
    delete BrokenChannels;
}

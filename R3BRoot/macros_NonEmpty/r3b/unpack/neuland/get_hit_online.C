struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    EXT_STR_h101_LOS_TAMEX_t los_tamex;
    EXT_STR_h101_raw_nnp_tamex_t raw_nnp;
};

void get_hit_online()
{
    const Int_t nBarsPerPlane = 50; // number of scintillator bars per plane
    const Int_t nPlanes = 16;       // number of planes (for TCAL calibration)

    const Int_t nev = -1;     /* number of events to read, -1 - until CTRL+C */
    const Int_t trigger = -1; // 1 - onspill, 2 - offspill. -1 - all

    const Int_t runID = 999;
    const TString filename = "stream://lxir123"; // :7000 for standalone DAQ

    const TString ucesbPath =
        "/lynx/Lynx/land/usr/land/landexp/201902_s444/upexps/201902_s444/201902_s444 --allow-errors";

    // -------------------------------------------
    EXT_STR_h101 ucesbStruct;
    auto source = new R3BUcesbSource(filename, "RAW", ucesbPath, &ucesbStruct, sizeof(ucesbStruct));
    source->SetMaxEvents(nev);

    auto unpackReader = new R3BUnpackReader((EXT_STR_h101_unpack_t*)&ucesbStruct, offsetof(EXT_STR_h101, unpack));
    source->AddReader(unpackReader);

    auto losReader =
        new R3BLosReader((EXT_STR_h101_LOS_TAMEX_t*)&ucesbStruct.los_tamex, offsetof(EXT_STR_h101, los_tamex));
    source->AddReader(losReader);

    auto neulandReader =
        new R3BNeulandTamexReader((EXT_STR_h101_raw_nnp_tamex_t*)&ucesbStruct.raw_nnp, offsetof(EXT_STR_h101, raw_nnp));
    neulandReader->SetNofPlanes(nPlanes);
    source->AddReader(neulandReader);

    auto run = new FairRunOnline(source);
    run->SetRunId(runID);
    run->ActivateHttpServer(1, 60000);
    run->SetSink(new FairRootFileSink("out.root"));

    // ----- Runtime DataBase info -----------------------------------------------
    auto parList = new TList();
    parList->Add(new TObjString("params_los.root"));
    parList->Add(new TObjString("params_neuland_tcal_cosmics_190219.root"));
    parList->Add(new TObjString("params_neuland_sync_online_190219.root"));

    auto parIO = new FairParRootFileIo(false);
    parIO->open(parList);

    auto rtdb = run->GetRuntimeDb();
    rtdb->setFirstInput(parIO);
    rtdb->addRun(runID);

    rtdb->getContainer("LosTCalPar");
    rtdb->setInputVersion(runID, (char*)"LosTCalPar", 1, 1);
    rtdb->getContainer("LandTCalPar");
    rtdb->setInputVersion(runID, (char*)"LandTCalPar", 1, 1);
    rtdb->getContainer("NeulandHitPar");
    rtdb->setInputVersion(runID, (char*)"NeulandHitPar", 2, 1);

    // -------------------------------------------
    auto losMapped2Cal = new R3BLosMapped2Cal();
    losMapped2Cal->SetNofModules(1, 8);
    losMapped2Cal->SetTrigger(trigger);
    run->AddTask(losMapped2Cal);

    auto tcal = new R3BNeulandMapped2Cal();
    tcal->SetTrigger(trigger);
    tcal->SetNofModules(nPlanes, nBarsPerPlane);
    tcal->SetNhitmin(1);
    run->AddTask(tcal);

    auto nlhit = new R3BNeulandCal2Hit();
    nlhit->SetDistanceToTarget(1520.);
    nlhit->SetGlobalTimeOffset(1800.);
    run->AddTask(nlhit);

    auto r3bNeulandOnlineSpectra = new R3BNeulandOnlineSpectra();
    run->AddTask(r3bNeulandOnlineSpectra);

    // -------------------------------------------
    run->Init();
    FairLogger::GetLogger()->SetLogScreenLevel("ERROR");
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);
}

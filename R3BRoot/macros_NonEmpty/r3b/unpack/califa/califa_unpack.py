#!/usr/bin/python
from __future__ import print_function
import sys, os
if __name__=="__main__":
    if len(sys.argv)>1 and sys.argv[1]=="--valgrind":
        args=["/usr/bin/valgrind",
              "--gen-suppressions=all",
              "--log-file=valgrind.log",
               "--suppressions=%s/valgrind-root-python.supp"%os.environ["PWD"],
               "/usr/bin/python", sys.argv[0]]+sys.argv[2:]
        print("running %s"%args)
        os.execvp(args[0],args)
    if len(sys.argv)>1 and sys.argv[1]=="--gdb":
        args=["/usr/bin/gdb",
              "--args",
              "/usr/bin/python", sys.argv[0]]+sys.argv[2:]
        print("running %s"%args)
        os.execvp(args[0],args)




import argparse, re, subprocess
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True # this should be the default
# run this once
ROOT.gROOT.ProcessLine(
    "struct EXT_STR_h101_t {"
    "EXT_STR_h101_unpack_t unpack;"
    "EXT_STR_h101_CALIFA_t califa;};"
    "const size_t offset_unpack=offsetof(EXT_STR_h101_t, unpack);"
    "const size_t offset_califa=offsetof(EXT_STR_h101_t, califa);")
# pk: yes, that is ugly, and yes, in principle, we could do better
# by allocating a buffer in python.
# In reality, instantiating a EXT_STR_h101_CALIFA[_t] does not even work
#  after gSystem->Load("libR3Bsource"). Whatever.

def unpack(infiles, output, nev, runNo, exp, ntuple, calibration):
    ROOT.gSystem.ResetSignals()

    ucesb_struct=ROOT.EXT_STR_h101()
    source=ROOT.R3BUcesbSource(infiles, ntuple, exp,
                               ucesb_struct, sys.getsizeof(ucesb_struct))
    source.SetMaxEvents(nev)
    source.AddReader(ROOT.R3BUnpackReader(
        ucesb_struct.unpack,ROOT.offset_unpack))
    source.AddReader(ROOT.R3BCalifaFebexReader(
        ucesb_struct.califa,ROOT.offset_califa))

    run=ROOT.FairRunOnline(source)
    run.SetRunId(1)
    if calibration:
        if re.match(".*[.]root$", calibration):
            calFile=ROOT.FairParRootFileIo()
        else:
            calFile=ROOT.FairParAsciiFileIo()
        calFile.open(calibration, "in")
        rtdb=run.GetRuntimeDb()
        rtdb.setFirstInput(calFile)
        rtdb.print()
        map2cal=ROOT.R3BCalifaMapped2CrystalCal()
        run.AddTask(map2cal)
        cluster=ROOT.R3BCalifaCrystalCal2Hit()
        cluster.SetConeAlg()
        cluster.SetVerbose(999);
        run.AddTask(cluster)
    
    run.SetSink(ROOT.FairRootFileSink(output))
    run.Init()
    run.Run(-(nev<0), nev)
#        calAna.WriteHistos()
    print("all done")



if __name__=="__main__":
    parser=argparse.ArgumentParser(sys.argv[0],\
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,\
        epilog="Any options after -- are passed to ucesb.")

    parser.add_argument("-n", "--max-events",
                        type=int, default=-1, dest='nev',
                        help="number of events to process, negative for all")
    parser.add_argument("-o", "--output", metavar="foobar.root",
                        type=str, default='/dev/null',
                        help="output root file")
    parser.add_argument("-c", "--calibration", metavar="cal.root/cal.par",
                        type=str, default='',
                        help="calibration file to use. (No file->Raw only)")
    parser.add_argument(metavar="foobar.lmd",
                        nargs='+', type=str, dest="infiles",
                        help="input files or network source")
    parser.add_argument("-r", "--run-number", metavar="N",
                        type=int, dest="runNo",
                        help="run number, used to pick exp unpacker")
    parser.add_argument("-x", "--exp", dest="exp",
                        type=str, metavar="sXYZ",
                        help="Experiment unpacker to use")
    parser.add_argument("-t", "--time-stitch", dest="time-stitch",
                        type=int, metavar="N", default=argparse.SUPPRESS,
                        help="Time stitch events within N ns using WRTS.")
    parser.add_argument("-N", "--ntuple", dest="ntuple",
                        type=str, default='RAW',
                        help="replace default ntuple LVL arg from ucesb")

    #parser.print_help()

    raw_args=sys.argv[1:]
    ucesb_options=[]
    if "--" in raw_args:
        sep=raw_args.index("--")+1
        ucesb_options=raw_args[sep+1:]
        raw_args=raw_args[0:sep-1]
    ucesb_options.append("--input-buffer=100Mi")
    args=vars(parser.parse_args())

    # deducing runnumber ######################################
    if not args["runNo"]:
        m=re.match(".*[^0-9]([0-9]+)_[^/]*[.]lmd", args["infiles"][0])
        if m:
            r=int(m.group(1))
            args["runNo"]=r
            print("Deducing run number from first file name to %d." % r)
        else:
            print("Warning, could not deduce runnumber from filename %s"\
                % args["infiles"][0])

    for f in args["infiles"]:
        m=re.match("(.*)://(.*)", f)
        if m:
            if m.group(1)!="file":
                continue  # can't stat
            else:
                f=m.group(2) # file:// style url
        if not os.access(f, os.R_OK):
            print("Error: count not access input file %s" % f)
            exit(-1)

    # finding the unpacker ####################################
    if not args["exp"]:
        r=args["runNo"]
        if r==None:
            print("Error: no exp/ucesb unpacker specified, "\
                " and no runnumber found either. Specify --exp")
            exit(-1)
        elif r<=179:
            args["exp"]="s444"
        elif r<=239:
            args["exp"]="s473"
        elif r<=410:
            args["exp"]="s454"
        else:
            print("Error: No idea which experiment run %d was in." % r)
            exit(-1)
        print("Run number %d seems to be part of experiment %s"\
            %(r, args["exp"]))
    full_unpacker_names={"s444":"201902_s444",
                         "s473":"201902_s473",
                         "s454":"201904_s454"}
    if args["exp"] in full_unpacker_names:
        args["exp"]=full_unpacker_names[args["exp"]]
    ucesb_dir=os.getenv("UCESB_DIR")
    if not os.access(args["exp"], os.X_OK):
        exp=args["exp"]
        print("Trying to find ucesb unpacker %s" % exp)
        if not ucesb_dir:
            print("warning: ucesb_dir unset. Searching. ")
            for l in os.getenv("LD_LIBRARY_PATH").split(":"):
                f=l+"/libR3Bsource.so"
                if os.access(f, os.F_OK):
                    ldd_o=subprocess.check_output(["ldd", f])
                    m=re.search("=> ([^ ]*)/hbook/libext_data_clnt.so", ldd_o)
                    if m:
                        print("found ucesb_dir=%s"%m.group(1))
                        ucesb_dir=m.group(1)
        if not ucesb_dir:
            print("Did not find ucesb dir. Set env var UCESB_DIR or specify full path with --exp")
            exit(-1)
        args["exp"]="%s/../upexps/%s/%s"%(ucesb_dir, args["exp"], args["exp"])
        if not os.access(args["exp"], os.X_OK):
            print("Error: no unpacker at %s" % args["exp"])
            exit(-1)
    if "time-stitch" in args.keys():    ######################## HERE BE DRAGONS
        # timestitching in the unpacker does not work
        # so we call the empty unpacker to
        args["infiles"]=["--time-stitch=wr,%d"%args["time-stitch"], "--input-buffer=100Mi",\
                         "--max-events=%d"%args["nev"],"--output=-"]\
                        + args["infiles"] + [ "|", args["exp"], "--file=-"]
        args["nev"]=-1
        for p in [ucesb_dir, os.path.dirname(args["exp"])+"../../ucesb/"]:
            empty=p+"/empty/empty"
            if os.access(empty, os.X_OK):
                break
            empty=None
        if not empty:
            print("Error: could not find 'empty' unpacker, needed for time-stitching.")
            exit(-1)
        args["exp"]=empty
        del args["time-stitch"]

    # flatten infiles, add ucesb_options ######################
    args["infiles"]=reduce(lambda x, y: "%s %s"%(x,y),
                           args["infiles"]+ucesb_options)

    unpack(**args)
#    print args

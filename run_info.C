// plot.C
// Plots histograms for all detectors
// To run: root -l "plot.C(XXX)" where XXX is run number
// or: root -l "plot.C(XXX, YYY)" sums all runs from XXX to YYY

using namespace std;
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "TRint.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TH1.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TFile.h"

#include "include/MDPP16_SCP.h"


void run_info(int run_num){

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);

    double elapsed_time = 0;
    MDPP16_SCP rabbit(run_num);
    
    //out file
    FILE *file_ptr;
    file_ptr = fopen("datafiles/run_info.dat","a");

    cout << rabbit.file->Get("start_time_SCP")->GetTitle() << endl;
    cout << rabbit.file->Get("stop_time_SCP")->GetTitle() << endl;

    //Variables
    Long64_t nentries = rabbit.fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    
    nb = rabbit.GetEntry(nentries-1);
    elapsed_time = rabbit.seconds/3600.;
    cout << "Elapsed time " << elapsed_time << endl;

    //output to file
    fprintf(file_ptr, "%i\t", run_num);
    fprintf(file_ptr, "%s\t", rabbit.file->Get("start_time_SCP")->GetTitle());
    fprintf(file_ptr, "%s\t", rabbit.file->Get("stop_time_SCP")->GetTitle());
    fprintf(file_ptr, "%f\t\n", elapsed_time);

    cout << "Run " << run_num << " Output complete!" << endl;

}

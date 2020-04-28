
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
#include "TRandom3.h"
#include "TFile.h"
#include "TCut.h"


void plot_cycle(int run_num){

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);

    //Variables
    const int num_det = 2;
    
    //Histograms
    TH1F *hCycle = new TH1F("hCycle", "hCycle", 10*450, -10, 440);

    //get histos
    int k = run_num;
        cout << "Run number:  " << k << endl;
        TFile *fHist = new TFile(Form("data_processed/RABITTS_%i.root",k));

        //get histos
        hCycle->Add((TH1F*) fHist->Get("histos/hCycleTime"));
        TH1F *hIrr = (TH1F*) fHist->Get("histos/hIrrTime");
        TH1F *hCount = (TH1F*) fHist->Get("histos/hCountTime");
        cout << hIrr->Integral(1, 1000000) << endl;
        cout << hCount->Integral(1, 1000000) << endl;

        fHist->Close();
        delete fHist;

        new TCanvas();
        hCycle->GetXaxis()->SetRangeUser(-5,  20);
        hCycle->GetXaxis()->SetRangeUser(-5, 155);
        hCycle->GetXaxis()->SetRangeUser(-5, 425);
        hCycle->GetXaxis()->SetRangeUser(-5, 110);
        hCycle->Draw();

}


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

void plot_FC(int run_num){


    //Variables
    const int num_det = 4;
    int dets[num_det] = {4, 5, 6, 7};
    //int thresh[num_det] = {8000, 8000, 8000, 8000};
    int thresh[num_det] = {9000, 9000, 9000, 9000};
    int rebin = 16;

    TLine *lThresh[num_det]; 

    //Histograms
    TH1F *hFC[num_det];

    //get histos
    TFile *fHist;
    if (run_num<10){
        fHist = new TFile(Form("data_root/HIgS_00%i.root", run_num));
    }
    else if (run_num<100){
        fHist = new TFile(Form("data_root/HIgS_0%i.root", run_num));
    }
    else{
        fHist = new TFile(Form("data_root/HIgS_%i.root", run_num));
    }

    //TCanvas *cFC = new TCanvas("cFC","Summed segments",1200, 600);
    //cFC->Divide(num_det);
    TCanvas *cFC = new TCanvas("cFC","Summed segments", 800, 1200);
    cFC->Divide(1, num_det);

    Double_t        seconds;
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    fHist->GetObject("MDPP16_SCP",fChain);
    TBranch        *b_seconds;   //!
    fChain->SetBranchAddress("seconds", &seconds, &b_seconds);
    Long64_t nentries = fChain->GetEntriesFast();
    fChain->GetEntry(nentries-1);
    Double_t elapsed_time = seconds;
    cout << run_num << "\t" << elapsed_time << "\t" ;


    for (int j=0; j<num_det; j++){
        hFC[j] = (TH1F*) (fHist->Get(Form("histos_SCP/hADC%i", dets[j])))->Clone();
        hFC[j]->SetName(Form("run%i_Det%i", run_num, j+1));
        hFC[j]->Rebin(rebin);
        hFC[j]->GetXaxis()->SetRangeUser(2000, 65500);
        cout << hFC[j]->Integral(thresh[j]/rebin, 65500/rebin) << "\t";
        cFC->cd(j+1);
        hFC[j]->Draw();

        lThresh[j] = new TLine(thresh[j], 0, thresh[j], hFC[j]->GetMaximum());
        lThresh[j]->SetLineColor(2);
        lThresh[j]->SetLineWidth(2);
        lThresh[j]->Draw("same");
    }
    cout << endl;


}

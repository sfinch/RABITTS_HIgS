
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

#include "include/RabVar.hh"

void plot_FC(int run_num){


    //Variables
    TLine *lThresh[RabVar::num_FC]; 

    //Histograms
    TH1F *hFC[RabVar::num_FC];

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

    TCanvas *cFC = new TCanvas("cFC","Summed segments", 800, 1200);
    cFC->Divide(1, RabVar::num_FC);

    Double_t        seconds;
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    fHist->GetObject("MDPP16_SCP",fChain);
    TBranch        *b_seconds;   //!
    fChain->SetBranchAddress("seconds", &seconds, &b_seconds);
    Long64_t nentries = fChain->GetEntriesFast();
    fChain->GetEntry(nentries-1);
    Double_t elapsed_time = seconds;
    cout << run_num << "\t" << elapsed_time << "\t" ;


    for (int j=0; j<RabVar::num_FC; j++){
        hFC[j] = (TH1F*) (fHist->Get(Form("histos_SCP/hADC%i", RabVar::FC_chn[j])))->Clone();
        hFC[j]->SetName(Form("run%i_Det%i", run_num, j+1));
        cout << hFC[j]->Integral(RabVar::FC_threshold[j], 65500) << "\t";
        hFC[j]->Rebin(RabVar::FC_rebin);
        hFC[j]->GetXaxis()->SetRangeUser(2000, 65500);
        cFC->cd(j+1);
        hFC[j]->Draw();

        lThresh[j] = new TLine(RabVar::FC_threshold[j], 0, RabVar::FC_threshold[j], hFC[j]->GetMaximum());
        lThresh[j]->SetLineColor(2);
        lThresh[j]->SetLineWidth(2);
        lThresh[j]->Draw("same");
    }
    cout << endl;


}

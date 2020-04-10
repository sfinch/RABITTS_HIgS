
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

#include "src/hist2TKA.C"

//void plot_3s9s(int run_num, int run_num2 = 0){
void plot_3s9s(){

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);

    int run_num = 0;
    int run_num2 = 0;

    if (run_num2 < run_num){
        run_num2 = run_num;
    }

    //Variables
    const int num_det = 2;
    const int rebin = 1;
    //const int num_win = 3;
    const int num_win = 9;
    
    //Histograms
    TH1F *hCycle = new TH1F("hCycle", "hCycle", 3000, -10, 20);
    TH1F *hIrr   = new TH1F("hEn_Irr_BothDets", "hIrr", 30000, 0, 3000);
    TH1F *hCount = new TH1F("hEn_AllCount_BothDets", "hCount", 30000, 0, 3000);
    TH1F *hEnCount[num_det+1];
    TH1F *hEnWin[num_win][3]; //[window 1-3][clover 1, 2, summed]

    for (int i=0; i<num_win; i++){ //window
        for (int j=0; j<2; j++){ //clover
            hEnWin[i][j] = new TH1F(Form("hEn_Time%i_Det%i", i, j), Form("hEnWin%iDet%i", i, j), 50000, 0, 5000);
        }
        hEnWin[i][2] = new TH1F(Form("hEn_Time%i_BothDets", i), Form("hEnWin%i", i), 50000, 0, 5000);
    }
    for (int i=0; i<num_det; i++){
        hEnCount[i] = new TH1F(Form("hEn_AllCount_Det%i", i), Form("hEnCount%i", i), 50000, 0, 5000);
    }
    hEnCount[num_det] = new TH1F(Form("hEn_AllCount_BothDet"), Form("hEnCountBoth"), 50000, 0, 5000);

    //get histos
    for (int k=run_num; k<=run_num2; k++){
        cout << "Run number:  " << k << endl;
        //TFile *fHist = new TFile(Form("data_hist/RABBIT_%i.root",k));
        //TFile *fHist = new TFile(Form("data_hist/RABBIT_6s9s.root"));
        //TFile *fHist = new TFile(Form("data_hist/RABBIT_60s90s.root"));
        //TFile *fHist = new TFile(Form("data_hist/RABBIT_180s240s.root"));
        TFile *fHist = new TFile(Form("data_hist/RABBIT_131_10s.root"));

        //hCycle->Add((TH1F*) fHist->Get("hCycle"));
        //hIrr->Add((TH1F*) fHist->Get("hIrr"));
        //hCount->Add((TH1F*) fHist->Get("hCount"));
        for (int i=0; i<num_win; i++){
            for (int j=0; j<2; j++){
                hEnWin[i][j]->Add((TH1F*) fHist->Get(Form("hEn_Time%i_Det%i", i, j)));
            }
            hEnWin[i][num_det]->Add((TH1F*) fHist->Get(Form("hEn_Time%i_BothDet", i)));
        }
        for (int i=0; i<num_det; i++){
            //hEnCount[i]->Add((TH1F*) fHist->Get(Form("hEn_AllCount_Det%i", i)));
        }
        //hEnCount[num_det]->Add((TH1F*) fHist->Get(Form("hEn_AllCount_BothDet")));

    //get histos

        fHist->Close();
        delete fHist;
    }

    hist2TKA(hCount);
    hist2TKA(hIrr);
    for (int i=0; i<num_win; i++){
        for (int j=0; j<3; j++){
            hist2TKA(hEnWin[i][j]);
        }
    }

}

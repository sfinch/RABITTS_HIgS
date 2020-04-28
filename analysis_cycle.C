
#include <iostream>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "include/processed.hh"

using std::cout;
using std::cerr;
using std::endl;

void analysis_cycle(int run_num){

    //cuts
    double time_win[RabVar::num_win][2];
    for (int i=0; i<RabVar::num_win; i++){
        time_win[i][0] = (RabVar::time_bin*i)+RabVar::time_count[0];
        time_win[i][1] = (RabVar::time_bin*(i+1))+RabVar::time_count[0];
    }

    //in file
    processed rabbit(run_num);
    //out file
    TFile *fHist = new TFile(Form("data_hist/RABBIT_%i_0.5s.root", run_num), "RECREATE");
    
    //Hitsos
    TH1F *hCycle = new TH1F("hCycle", "hCycle", 4500, -10, 440);
    TH1F *hEnIrr[RabVar::num_det];
    TH1F *hEnCount[RabVar::num_det];
    TH1F *hEnWin[RabVar::num_win][RabVar::num_det]; //[window 1-3][det 1, 2]

    for (int i=0; i<RabVar::num_win; i++){
        for (int j=0; j<2; j++){
            hEnWin[i][j] = new TH1F(Form("hEn_Time%i_Det%i", i, j), 
                                    Form("hEn_Time%i_Det%i", i, j), 
                                    50000, 0, 5000);
        }
        for (int j=2; j<4; j++){
            hEnWin[i][j] = new TH1F(Form("hEn_Time%i_Det%i", i, j), 
                                    Form("hEn_Time%i_Det%i", i, j), 
                                    100000, 0, 10000);
        }
    }
    for (int i=0; i<2; i++){
        hEnCount[i] = new TH1F(Form("hEn_AllCount_Det%i", i), 
                               Form("hEn_AllCount_Det%i", i), 
                               50000, 0, 5000);
        hEnIrr[i] = new TH1F(Form("hEn_Irr_Det%i", i), 
                             Form("hEn_Irr_Det%i", i), 
                             50000, 0, 5000);
    }
    for (int i=2; i<4; i++){
        hEnCount[i] = new TH1F(Form("hEn_AllCount_Det%i", i), 
                               Form("hEn_AllCount_Det%i", i), 
                               100000, 0, 10000);
        hEnIrr[i] = new TH1F(Form("hEn_Irr_Det%i", i), 
                             Form("hEn_Irr_Det%i", i), 
                             100000, 0, 10000);
    }

    //loop over data
    Long64_t nentries = rabbit.fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    
    for (Long64_t jentry=0; jentry<nentries; jentry++) {
        nb = rabbit.GetEntry(jentry);   nbytes += nb;
        if (jentry%100000==0){
            cout << '\r' << "Processing event " << jentry;
        }
        if ((rabbit.cycle_time > 0.00001)||(rabbit.cycle_time<-0.00001)){
            for (int det=0; det<RabVar::num_det; det++){
                if (rabbit.En[det]>10){
                    hCycle->Fill(rabbit.cycle_time);
                }
            }
        }

        if ((rabbit.cycle_time>RabVar::time_irr[0]) 
         && (rabbit.cycle_time<RabVar::time_irr[1])){
            for (int det=0; det<RabVar::num_det; det++){
                if (rabbit.En[det]>10){
                    hEnIrr[det]->Fill(rabbit.En[det]);
                }
            }
        }
        else if ((rabbit.cycle_time>RabVar::time_count[0]) 
              && (rabbit.cycle_time<RabVar::time_count[1])){
            for (int det=0; det<RabVar::num_det; det++){
                if (rabbit.En[det]>10){
                    hEnCount[det]->Fill(rabbit.En[det]);
                }
                for (int window=0; window<RabVar::num_win; window++){
                    if ((rabbit.cycle_time>time_win[window][0]) 
                     && (rabbit.cycle_time<time_win[window][1])){
                        if (rabbit.En[det]>10){
                            hEnWin[window][det]->Fill(rabbit.En[det]);
                        }
                    }
                }//end time windows
            }
        }//end counting time

    }//end loop over events

    //write histos to file
    fHist->cd();

    hCycle->Write();
    for (int det=0; det<RabVar::num_det; det++){
        hEnIrr[det]->Write();
        hEnCount[det]->Write();
    }
    for (int i=0; i<RabVar::num_win; i++){
        for (int j=0; j<RabVar::num_det; j++){
            hEnWin[i][j]->Write();
        }
    }

    fHist->Write();
    fHist->Close();
    
}

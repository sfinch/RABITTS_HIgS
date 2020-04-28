
#include <iostream>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "processed.hh"
//#include "processed_old.hh"

using std::cout;
using std::cerr;
using std::endl;

void analysis_overnight(int run_num){

    //Variables
    const int num_det = 4;
    //const int num_det = 2;

    const int num_win = 12;
    const double win_time =3600; //in sec
    //const double win_time =7200; //in sec

    TH1F *hEnAll[num_det];
    TH1F *hEnWin[num_win][num_det]; //[window 1-3][det 1, 2, summed]

    for (int i=0; i<num_win; i++){
        for (int j=0; j<num_det; j++){
            hEnWin[i][j] = new TH1F(Form("run%i_Hour%i_Det%i", run_num, (i+1), j), Form("hEnWin%i%i", i, j), 40000, 0, 4000);
        }
    }

    for (int i=0; i<num_det; i++){
        hEnAll[i] = new TH1F(Form("hEnAll%i", i), Form("hEnAll%i", i), 40000, 0, 4000);
    }

    //cuts
    double time_win[num_win][2];
    for (int i=0; i<num_win; i++){
        time_win[i][0] = i*win_time;
        time_win[i][1] = (i+1)*win_time;
    }

    //in file
    processed rabbit(run_num);

    //out file
    TFile *fHist = new TFile(Form("data_hist/RABBIT_%i.root", run_num), "RECREATE");

    //loop over data
    Long64_t nentries = rabbit.fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    
    for (Long64_t jentry=0; jentry<nentries; jentry++) {
        nb = rabbit.GetEntry(jentry);   nbytes += nb;
        if (jentry%100000==0){
            cout << '\r' << "Processing event " << jentry;
        }

        for (int det=0; det<num_det; det++){
            if (rabbit.En[det]>10){
                hEnAll[det]->Fill(rabbit.En[det]);

                for (int window=0; window<num_win; window++){
                    if ((rabbit.seconds>time_win[window][0]) && (rabbit.seconds<time_win[window][1])){
                            hEnWin[window][det]->Fill(rabbit.En[det]);
                    }
                }//end time windows
            }
        }


    }//end loop over events
    cout << endl;
    int cycles_complete = rabbit.seconds/win_time;
    cout << rabbit.seconds << " s elapsed" <<  endl;
    cout << rabbit.seconds/3600. << " h elapsed" <<  endl;
    cout << rabbit.seconds/win_time << " total time cycles" <<  endl;
    cout << cycles_complete << " complete cycles" <<  endl;
    if (cycles_complete>num_win){
        cycles_complete=num_win;
    }

    TCanvas *cDet1 = new TCanvas("cDet1","Det 1",1000, 400);
    hEnWin[0][0]->Draw();
    for (int i=1; i<cycles_complete; i++){
        hEnWin[i][0]->SetLineColor(i+2);
        hEnWin[i][0]->Draw("same");
     }

    TCanvas *cDet2 = new TCanvas("cDet2","Det 2",1000, 400);
    hEnWin[0][1]->Draw();
    for (int i=1; i<cycles_complete; i++){
        hEnWin[i][1]->SetLineColor(i+2);
        hEnWin[i][1]->Draw("same");
    }

    //write histos to file
    fHist->cd();

    for (int det=0; det<num_det; det++){
        hEnAll[det]->Write();
    }
    for (int i=0; i<cycles_complete; i++){
        for (int j=0; j<num_det; j++){
            hEnWin[i][j]->Write();
        }
    }
    for (int j=0; j<num_det; j++){
        hEnWin[cycles_complete][j]->SetName(Form("run%i_Part%i_Det%i", run_num, (cycles_complete+1), j+1));
        hEnWin[cycles_complete][j]->Write();
    }

    fHist->Write();
    fHist->Close();
    
}

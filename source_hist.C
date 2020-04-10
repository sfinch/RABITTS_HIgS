
#include <iostream>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "include/processed.h"
//#include "include/processed_old.h"

using std::cout;
using std::cerr;
using std::endl;

void source_hist(int run_num){

    //Variables
    //const int num_det = 2;
    const int num_det = 4;

    TH1F *hEnCount[num_det];

    for (int i=0; i<num_det; i++){
        hEnCount[i] = new TH1F(Form("hEn_Det%i", i), Form("hEn_Det%i", i), 40000, 0, 4000);
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
                hEnCount[det]->Fill(rabbit.En[det]);
            }
        }

    }//end loop over events

    //write histos to file
    fHist->cd();

        //hEnCount[0]->Write();
        //hEnCount[2]->Write();
    for (int det=0; det<2; det++){
    //for (int det=0; det<num_det; det++){
        hEnCount[det]->Write();
    }

    fHist->Write();
    fHist->Close();
    
}

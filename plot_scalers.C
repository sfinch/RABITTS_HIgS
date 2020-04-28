
#include <iostream>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "include/processed.hh"

using std::cout;
using std::cerr;
using std::endl;

void plot_scalers(int run_num){

    TApplication* theApp = new TApplication("App", 0, 0);
     
    //Variables
    TH1F *hHPGe[2];

    double bin_size = 1.; //time in s

    //cuts
    int min_HPGe_E[2] = {100, 100};

    //in file
    processed rabbit(run_num);

    //loop over data
    Long64_t nentries = rabbit.fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    cout << nentries << endl;


    rabbit.GetEntry(nentries-1);
    //Double_t total_time = rabbit.seconds;
    //int num_bins = total_time/bin_size;
    double total_time = 200;
    int num_bins = 200;
    cout << total_time  << " " << num_bins << endl;

    for (int i=0; i<2; i++){
        hHPGe[i] = new TH1F(Form("hHPGe%i", i), Form("hHPGe%i", i), num_bins, 0, total_time);
    }
    
    for (Long64_t jentry=0; jentry<nentries; jentry++) {
        nb = rabbit.GetEntry(jentry);   nbytes += nb;
        if (jentry%100000==0){
            cout << '\r' << "Processing event " << jentry;
        }
        if (rabbit.ADC[0]>min_HPGe_E[0]){
            hHPGe[0]->Fill(rabbit.seconds);
        }
        if (rabbit.ADC[1]>min_HPGe_E[1]){
            hHPGe[1]->Fill(rabbit.seconds);
        }

    }//end loop over events
    cout << endl;

    //plot
    TCanvas *cScalers = new TCanvas("cScalers", "cScalers", 500, 1000);
    cScalers->Divide(1,2);
    cScalers->cd(2);
    hHPGe[0]->Draw();
    cScalers->cd(3);
    hHPGe[1]->Draw();
    cScalers->Modified();
    cScalers->Update();
    theApp->Run();
    gSystem->ProcessEvents();


    //cCycle->SaveAs(Form("time_spec/%i.png", run_num));
    //cCycle->SaveAs(Form("time_spec/%i.C", run_num));

    
}

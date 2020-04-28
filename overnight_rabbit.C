
#include <iostream>
#include <fstream>

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "include/MDPP16_SCP.hh"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

struct calibration{     //struct storing calibration data for both dets
    Float_t m[2];
    Float_t b[2];
};
    
calibration read_in_cal(int run_num); // function to read in calibration from file

int main(int argc, char *argv[]){

    if (argc<2)
    {
        cerr << "Invalid number of arguments" << endl;
        cerr << "Usage: " << argv[0] << " <run number>" << endl;
        return 1;
    }
    int run_num = atoi(argv[1]); 
    //Variables
    const int num_det = 2;
    int det_chn[num_det] = {0, 2};
    const int num_win = 14;
    double time_start[num_win];
    double time_stop[num_win];
    for (int i=0; i<num_win; i++){
        time_start[i] = i*1800;
        time_stop[i] = (i+1)*1800;
    }

    //in file
    MDPP16_SCP rabbit(run_num);
    //check for any additional calibration corrections
    calibration cal = read_in_cal(run_num);

    //output tree variables
    Float_t cycle_time = 0;
    Float_t En[num_det];
    TRandom3 r;

    //out file
    TFile *fOut = new TFile(Form("data_processed/RABITTS_%i.root", run_num), "RECREATE");
    TTree *tProcessed = new TTree("processed", "Processed rabbit data");

    tProcessed->Branch(Form("En[%i]", num_det), &En, Form("En[%i]/F", num_det));
    tProcessed->Branch("cycle_time", &cycle_time, "cycle_time/F");

    //Histrograms
    TH1F *hEn[num_det];
    TH1F *hEnWin[num_det][num_win];

    for (int det=0; det<num_det; det++){
        hEn[det] = new TH1F(Form("hEn_Det%i", det), Form("hEn%i", det), 1*3000, 0, 3000);
        for (int win=0; win<num_win; win++){
            hEnWin[det][win] = new TH1F(Form("hEn_Det%i_Time%i", det, win), Form("hEn%i%i", det, win), 5*3000, 0, 3000);
        }
    }

    //loop over data
    Long64_t nentries = rabbit.fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;
    
    //loop over all events
    for (Long64_t jentry=0; jentry<nentries; jentry++) {
        nb = rabbit.GetEntry(jentry);   nbytes += nb;
        if (jentry%100000==0){
            cout << '\r' << "Processing event " << jentry;
        }

        //energy calibrate detectors
        for (int det=0; det<num_det; det++){ //loop over detectors
            En[det] = 0;
            if ((rabbit.TDC[det_chn[det]]>10)&&(!(rabbit.overflow[det_chn[det]]))){
                //Calibrate
                En[det] = (rabbit.ADC[det_chn[det]]+r.Rndm()-0.5)*( (*rabbit.m) )[det_chn[det]] + ( (*rabbit.b) )[det_chn[det]];
                En[det_chn[det]] = En[det_chn[det]]*cal.m[det_chn[det]] + cal.b[det_chn[det]];
                
                //fill histos
                hEn[det]->Fill(En[det]);
                for (int i=0; i<num_win; i++){
                    if ((rabbit.seconds > time_start[i]) && (rabbit.seconds < time_stop[i])){
                        hEnWin[det][i]->Fill(En[det]);
                    }
                }
            }
        }

    }
    cout << endl;
    cout << nentries << " events processed" << endl;
    
    //write data to file
    fOut->cd();
    //tProcessed->Write();

    for (int det=1; det<num_det; det++){
        //hEn[det]->Write();
        for (int win=0; win<num_win; win++){
            hEnWin[det][win]->Write();
        }
    }

    fOut->Close();
    
}

// Reads in the calibration file and finds the calibration for the given run
calibration read_in_cal(int run_num){

    //variables to read in from file
    Float_t m_file[2], b_file[2];
    int run, run2;
    
    //calibration data
    ifstream infile;
    infile.open("datafiles/det_cal.dat");

    //return values
    calibration cal;
    for (int i = 0; i<2; i++){
        cal.m[i] = 1.;
        cal.b[i] = 0;
    }

    //read in data from file
    do{
        infile>>run;
        if (run<0 || run>10000){
            break;
        }
        for (int j=0; j<2; j++){
            infile>>m_file[j];
        }
        infile>>run2;
        if ((run != run2) && (run2 != 0)){
            cout << "ERROR IN DATA FILE! Entry:  " << run << endl;
        }
        for (int j=0; j<2; j++){
            infile>>b_file[j];
        }
        if (run == run_num){
            break;
        }
    }while(!infile.eof());
    infile.close();
    
    //There exists calibration points for the given run
    if (run == run_num){
        //print calibration values
        cout << "Found run " << run << ", using calibration:" << endl;
        cout << "m:  ";
        for (int i = 0; i<2; i++){
            cout << m_file[i] << "  ";
        }
        cout << endl << "b:  ";
        for (int i = 0; i<2; i++){
            cout << b_file[i] << "  ";
        }
        cout << endl;
        //check with user
        //cout << "Is this OK? (y or n)  ";
        char ans = 'y';
        //cin >> ans;
        if  (ans == 'y'){
            for (int i = 0; i<2; i++){
                cal.m[i] = m_file[i];
                cal.b[i] = b_file[i];
            }
        }
    }
    else{
        cout << "No additional calibration found in datafiles/det_cal.dat. Using only MVME calibration." 
             << endl;
    }
    return cal; //return calibration
}


//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb  6 13:16:29 2019 by ROOT version 5.34/38
// from TTree processed/Processed rabbit data
// found on file: data_processed/RABBIT_DEC18_12.root
//////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "RabVar.hh"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class processed {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TFile          *file;
   TFile          *rawfile;

   // Declaration of leaf types
   Float_t         En[RabVar::num_det];
   Float_t         cycle_time;

   Int_t           ADC[16];
   Int_t           TDC[16];
   Bool_t          overflow[16];
   Bool_t          pileup[16];
   Double_t        seconds;
   Int_t           time_stamp;

   // List of branches
   TBranch        *b_En;   //!
   TBranch        *b_cycle_time;   //!

   TBranch        *b_ADC;   //!
   TBranch        *b_TDC;   //!
   TBranch        *b_overflow;   //!
   TBranch        *b_pileup;   //!
   TBranch        *b_seconds;   //!
   TBranch        *b_time_stamp;   //!

   processed(int run_num);
   virtual ~processed();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init();
   virtual void     Show(Long64_t entry = -1);
};

processed::processed(int run_num)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   file = new TFile(Form("data_processed/RABITTS_%i.root", run_num));
   file->GetObject("processed", fChain);
    if (run_num<10){
        fChain->AddFriend("MDPP16_SCP", Form("data_root/HIgS_00%i.root", run_num));
        rawfile = new TFile(Form("data_root/HIgS_00%i.root", run_num));
    }
    else if (run_num<100){
        fChain->AddFriend("MDPP16_SCP", Form("data_root/HIgS_0%i.root", run_num));
        rawfile = new TFile(Form("data_root/HIgS_0%i.root", run_num));
    }
    else{
        fChain->AddFriend("MDPP16_SCP", Form("data_root/HIgS_%i.root", run_num));
        rawfile = new TFile(Form("data_root/HIgS_%i.root", run_num));
    }

   Init();
}

processed::~processed()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t processed::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

void processed::Init()
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.

   // Set branch addresses and branch pointers
   if (!fChain) return;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress(Form("En[%i]", RabVar::num_det), En, &b_En);
   fChain->SetBranchAddress("cycle_time", &cycle_time, &b_cycle_time);

   fChain->SetBranchAddress("ADC[16]", ADC, &b_ADC);
   fChain->SetBranchAddress("TDC[16]", TDC, &b_TDC);
   fChain->SetBranchAddress("overflow[16]", overflow, &b_overflow);
   fChain->SetBranchAddress("pileup[16]", pileup, &b_pileup);
   fChain->SetBranchAddress("seconds", &seconds, &b_seconds);
   fChain->SetBranchAddress("time_stamp", &time_stamp, &b_time_stamp);
}

void processed::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

/*
 * This file constructs a friend TTree containing the oscillation
 * probability for each event in the FastMC file provided to it.
 */

#include "/dune/app/users/lblpwg_tools/INSTALLATION/include/Prob3++/BargerPropagator.h"
#include <fstream>
#include <TTree.h>
#include <TFile.h>
#include "Configuration.C"

int ConstructProbabilityFriend()
{
    // Oscprobs from Capozi et al. (used in CDR)
    // I am assuming normal ordering, delta-cp = 0
    const double x13 = 0.0234; // sin^2(x)
    const double x12 = 0.308; // sin^2(x)
    const double x23 = 0.437; // sin^2(x)
    const double dm21 = 0.0000754; // eV^2
    const double dm31 = 0.00243; // eV^2
    const double dcp = 0.0;
    const bool THETA_STYLE = true;
    const double BASELINE = 1300; // km
    const double DENSITY = 2.7; // g/cm^3
    std::vector<std::string> filenames;
    filenames.push_back("nuflux_numuflux_numu");
    filenames.push_back("nuflux_nueflux_nue");
    filenames.push_back("nuflux_numubarflux_numubar");
    filenames.push_back("nuflux_nuebarflux_nuebar");
    filenames.push_back("nuflux_numuflux_nue");
    filenames.push_back("nuflux_numubarflux_nuebar");
    filenames.push_back("nuflux_numuflux_nutau");
    filenames.push_back("nuflux_numubarflux_nutaubar");
    filenames.push_back("anuflux_numuflux_numu");
    filenames.push_back("anuflux_nueflux_nue");
    filenames.push_back("anuflux_numubarflux_numubar");
    filenames.push_back("anuflux_nuebarflux_nuebar");
    filenames.push_back("anuflux_numuflux_nue");
    filenames.push_back("anuflux_numubarflux_nuebar");
    filenames.push_back("anuflux_numuflux_nutau");
    filenames.push_back("anuflux_numubarflux_nutaubar");
    std::vector<int> neutrinostartspecies;
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(1);
    neutrinostartspecies.push_back(-2);
    neutrinostartspecies.push_back(-1);
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(-2);
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(-2);
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(1);
    neutrinostartspecies.push_back(-2);
    neutrinostartspecies.push_back(-1);
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(-2);
    neutrinostartspecies.push_back(2);
    neutrinostartspecies.push_back(-2);
    std::vector<int> neutrinoendspecies;
    neutrinoendspecies.push_back(2);
    neutrinoendspecies.push_back(1);
    neutrinoendspecies.push_back(-2);
    neutrinoendspecies.push_back(-1);
    neutrinoendspecies.push_back(1);
    neutrinoendspecies.push_back(-1);
    neutrinoendspecies.push_back(3);
    neutrinoendspecies.push_back(-3);
    neutrinoendspecies.push_back(2);
    neutrinoendspecies.push_back(1);
    neutrinoendspecies.push_back(-2);
    neutrinoendspecies.push_back(-1);
    neutrinoendspecies.push_back(1);
    neutrinoendspecies.push_back(-1);
    neutrinoendspecies.push_back(3);
    neutrinoendspecies.push_back(-3);
    std::string prefix = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal";
    prefix.append("/fastmcNtp_20160404_lbne_g4lbnev3r2p4b_");
    std::string suffix = "_LAr_1_g280_Ar40_5000_GENIE_2100.root";
    std::vector<std::string>::iterator it = filenames.begin();
    std::vector<int>::iterator startit = neutrinostartspecies.begin();
    std::vector<int>::iterator endit = neutrinoendspecies.begin();
    for(it; it != filenames.end(); ++it, ++startit, ++endit)
    {
        // Create the correct file name
        std::string temp = *it;
        it->assign(prefix);
        it->append(temp);
        it->append(suffix);
        std::cout << *it << std::endl;
        std::string& filename = *it;

        // Get the correct neutrino types
        int neutrinostart = *startit;
        int neutrinoend = *endit;

        // Set up the output tree
        char outputdir[100];
        TFile* trialfile = TFile::Open((std::string(CFG_OutputDirectory(outputdir)) +
                    "oscprob2/" + temp + "__OSCPROB.root").c_str(), "READ");
        if(trialfile != 0)
        {
            continue;
        }
        TFile* fout = TFile::Open((std::string(CFG_OutputDirectory(outputdir)) +
                "oscprob2/" + temp + "__OSCPROB.root").c_str(), "RECREATE");
        TTree* oscprob = new TTree("OSCPROB",
                "Oscillation Probabilities for FMC events");
        double probability = 0;
        double energy = 0;
        oscprob->Branch("probability", &probability);
        oscprob->Branch("Ev", &energy);

        // Set up the input tree
        TFile* fin = new TFile(filename.c_str(), "READ");
        TTree* fmcData = (TTree*) fin->Get("gst");

        fmcData->SetBranchStatus("*", 0);
        fmcData->SetBranchStatus("Ev", 1);
        fmcData->SetBranchAddress("Ev", &energy);

        oscprob->SetDirectory(fout);

        Long64_t nentries = fmcData->GetEntries();
        for(Long64_t entry = 0; entry < nentries; ++entry)
        {
            fmcData->GetEntry(entry);
            BargerPropagator* BP = new BargerPropagator();
            // Note: only the sign of the last parameter is relevant, so
            // neutrinostart and neutrinoend would both work.
            BP->SetMNS(x12, x13, x23, dm21, dm31, dcp, energy,
                    THETA_STYLE, neutrinostart);
            BP->propagateLinear(neutrinostart, BASELINE, DENSITY);
            probability = BP->GetProb(neutrinostart, neutrinoend);
            oscprob->Fill();
            delete BP;
        }
        oscprob->SetDirectory(fout);
        fout->Write();
        fout->Close();
        fin->Close();
    }
}

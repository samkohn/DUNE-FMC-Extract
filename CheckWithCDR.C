/*
 * This ROOT macro file contains scripts that will compute the signal
 * and background values in the same way they are calculated for the
 * CDR.
 *
 * Structure of the plotting commands:
 *
 * OSCPROB.probability is like the FMC OSCWeight variable, denoting the
 * oscillation probability for that particular energy of neutrino.
 *
 * POTWeight*POTperYear*3.125*40 gives the weight of the event given the
 * POT weight, event cross section, mass of the detector (40 kt), and
 * run time (3.125 years, which, combined with 40 kt and 1.2 MW gives
 * 150 kt-MW-yr).
 *
 * (Ev_reco < 8 && Ev_reco > 0.5) ensures that the events are within the histogram
 * bounds.
 *
 * cc/nc selects events which interacted via the charge current or neutral
 * current, respectively
 *
 * (EvClass_reco == 1) selects events reconstructed to be nueCC-like,
 * i.e. events that could be confused with a true nueCC interaction.
 */
const std::string FMCFOLDER = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal/";
const std::string OSCPROBFOLDER = "/afs/fnal.gov/files/home/room3/skohn/outputs/oscprob2/";
const char* EVENTCUT_NUE = "(EvClass_reco == 1 && Tau_Prob_nue > 0.6 && NC_Prob_nue > 0.75)";
const char* NORMALIZATION = "(POTWeight * POTperYear * 3.125 * 40)";
const char* ENERGY_CUT = "(Ev_reco < 8 && Ev_reco > 0.5)";
const char* OSC_PROBABILITY = "OSCPROB.probability";
double numode_nue_signal(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_nue_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_nue__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nue signal: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_nuebar_signal(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_nuebar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_nuebar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nuebar signal: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_total_signal(bool verbose=true)
{
    double nue_signal = numode_nue_signal(verbose);
    double nuebar_signal = numode_nuebar_signal(verbose);
    double total_signal = nue_signal + nuebar_signal;
    if(verbose)
    {
        std::cout << "total signal: " << total_signal << "\n";
    }
    return total_signal;
}

double numode_beamnue_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_nueflux_nue_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_nueflux_nue__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "beam nue background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_beamnuebar_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_nuebarflux_nuebar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_nuebarflux_nuebar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "beam nuebar background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_total_beam_bg(bool verbose=true)
{
    double beamnue_background = numode_beamnue_bg(verbose);
    double beamnuebar_background = numode_beamnuebar_bg(verbose);
    double total_bg = beamnue_background + beamnuebar_background;
    if(verbose)
    {
        std::cout << "total beam background: " << total_bg << "\n";
    }
    return total_bg;
}

double numode_nutauCC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_nutau_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_nutau__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nutau CC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_nutaubarCC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_nutaubar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_nutaubar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nutaubar CC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_total_nutau_bg(bool verbose=true)
{
    double nutau_background = numode_nutauCC_bg(verbose);
    double nutaubar_background = numode_nutaubarCC_bg(verbose);
    double total_bg = nutau_background + nutaubar_background;
    if(verbose)
    {
        std::cout << "total nutau background: " << total_bg << "\n";
    }
    return total_bg;
}

double numode_numuCC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_numu_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_numu__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "numu CC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_numubarCC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_numubar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_numubar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * cc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "numubar CC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_total_numu_bg(bool verbose=true)
{
    double numu_background = numode_numuCC_bg(verbose);
    double numubar_background = numode_numubarCC_bg(verbose);
    double total_bg = numu_background + numubar_background;
    if(verbose)
    {
        std::cout << "total numu background: " << total_bg << "\n";
    }
    return total_bg;
}

double numode_nueNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_nue_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_nue__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nue NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_beamnueNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_nueflux_nue_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_nueflux_nue__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "beam nue NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_nuebarNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_nuebar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_nuebar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nuebar NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_beamnuebarNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_nuebarflux_nuebar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_nuebarflux_nuebar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "beam nuebar NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_numuNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_numu_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_numu__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "numu NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_numubarNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_numubar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_numubar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "numubar NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_nutauNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numuflux_nutau_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numuflux_nutau__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nutau NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_nutaubarNC_bg(bool verbose=true)
{
    TFile* fin = TFile::Open((FMCFOLDER + "fastmcNtp_20160404_lbne_g4lbnev3r2p4b_nuflux_numubarflux_nutaubar_LAr_1_g280_Ar40_5000_GENIE_2100.root").c_str(), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", (OSCPROBFOLDER + "nuflux_numubarflux_nutaubar__OSCPROB.root").c_str());
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * nc * %s/%d", OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(verbose)
    {
        std::cout << "nutaubar NC background: " << num_signal_events << "\n";
    }
    fin->Close();
    return num_signal_events;
}

double numode_total_NC_bg(bool verbose=true)
{
    double nue_bg = numode_nueNC_bg(verbose);
    double beamnue_bg = numode_beamnueNC_bg(verbose);
    double numu_bg = numode_numuNC_bg(verbose);
    double nutau_bg = numode_nutauNC_bg(verbose);
    double nuebar_bg = numode_nuebarNC_bg(verbose);
    double beamnuebar_bg = numode_beamnuebarNC_bg(verbose);
    double numubar_bg = numode_numubarNC_bg(verbose);
    double nutaubar_bg = numode_nutaubarNC_bg(verbose);
    double total_bg = nue_bg + beamnue_bg + numu_bg + nutau_bg +
        nuebar_bg + beamnuebar_bg + numubar_bg + nutaubar_bg;
    if(verbose)
    {
        std::cout << "total NC background: " << total_bg << "\n";
    }
    return total_bg;
}

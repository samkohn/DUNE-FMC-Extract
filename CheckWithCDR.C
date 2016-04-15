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
const char* FMCFOLDER = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal/";
const char* OSCPROBFOLDER = "/afs/fnal.gov/files/home/room3/skohn/outputs/oscprob2/";
const char* EVENTCUT_NUE = "(EvClass_reco == 1 && Tau_Prob_nue > 0.6 && NC_Prob_nue > 0.75)";
const char* NORMALIZATION = "(POTWeight * POTperYear * 3.125 * 40)";
const char* ENERGY_CUT = "(Ev_reco < 8 && Ev_reco > 0.5)";
const char* OSC_PROBABILITY = "OSCPROB.probability";

double get_uncertainty(TTree* fmcdata, std::string event_cut)
{
    TH1D* hist = new TH1D("stat", "stat", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>stat", event_cut.c_str());
    double num_unweighted_events = hist->Integral();
    double error_unweighted = TMath::Sqrt(num_unweighted_events);
    double fractional_error = error_unweighted/num_unweighted_events;
    return fractional_error;
}

double get_events(std::string fluxtype, std::string cc_or_nc, std::string message, double* error, bool verbose)
{
    TFile* fin = TFile::Open(Form("%sfastmcNtp_20160404_lbne_g4lbnev3r2p4b_%s_LAr_1_g280_Ar40_5000_GENIE_2100.root", FMCFOLDER, fluxtype.c_str()), "READ");
    TTree* fmcdata = (TTree*) fin->Get("gst");
    fmcdata->AddFriend("OSCPROB", Form("%s%s__OSCPROB.root", OSCPROBFOLDER, fluxtype.c_str()));
    long num_events = fmcdata->GetEntries();
    TH1D* hist = new TH1D("h", "h", 100, 0.5, 8);
    fmcdata->Draw("Ev_reco>>h", Form("%s*%s*%s * %s * %s/%d", cc_or_nc.c_str(), OSC_PROBABILITY, ENERGY_CUT, NORMALIZATION, EVENTCUT_NUE, num_events));
    double num_signal_events = hist->Integral();
    if(error != 0)
    {
        (*error) = get_uncertainty(fmcdata, std::string(Form("%s*%s*%s",ENERGY_CUT, EVENTCUT_NUE, cc_or_nc.c_str())));
        (*error) *= num_signal_events;
    }
    if(verbose)
    {
        std::cout << message << ": " << num_signal_events << "\n";
        if(error != 0)
        {
            std::cout << "    error: +/- " << (*error) << "\n";
        }
    }
    fin->Close();
    return num_signal_events;
}

double numode_nue_signal(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_nue",
            "cc",
            "nue signal",
            error,
            verbose
    );
}

double numode_nuebar_signal(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_nuebar",
            "cc",
            "nuebar signal",
            error,
            verbose
    );
}

double numode_total_signal(double* error=0, bool verbose=true)
{
    double nue_error = 0;
    double nuebar_error = 0;
    double nue_signal = numode_nue_signal(&nue_error, verbose);
    double nuebar_signal = numode_nuebar_signal(&nuebar_error, verbose);
    double total_signal = nue_signal + nuebar_signal;
    if(error != 0)
    {
        (*error) = TMath::Hypot(nue_error, nuebar_error);
    }
    if(verbose)
    {
        std::cout << "total signal: " << total_signal << "\n";
        if(error != 0)
        {
            std::cout << "total error: +/- " << (*error) << "\n";
        }
    }
    return total_signal;
}

double numode_beamnue_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_nueflux_nue",
            "cc",
            "beam nue bg",
            error,
            verbose
    );
}

double numode_beamnuebar_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_nuebarflux_nuebar",
            "cc",
            "beam nuebar bg",
            error,
            verbose
    );
}

double numode_total_beam_bg(double* error=0, bool verbose=true)
{
    double nue_error = 0;
    double nuebar_error = 0;
    double beamnue_background = numode_beamnue_bg(&nue_error, verbose);
    double beamnuebar_background = numode_beamnuebar_bg(&nuebar_error, verbose);
    double total_bg = beamnue_background + beamnuebar_background;
    if(error != 0)
    {
        (*error) = TMath::Hypot(nue_error, nuebar_error);
    }
    if(verbose)
    {
        std::cout << "total beam background: " << total_bg << "\n";
        if(error != 0)
        {
            std::cout << "total error: +/- " << (*error) << "\n";
        }
    }
    return total_bg;
}

double numode_nutauCC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_nutau",
            "cc",
            "nutau CC bg",
            error,
            verbose
    );
}

double numode_nutaubarCC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_nutaubar",
            "cc",
            "nutaubar CC bg",
            error,
            verbose
    );
}

double numode_total_nutau_bg(double* error=0, bool verbose=true)
{
    double tau_error = 0;
    double taubar_error = 0;
    double nutau_background = numode_nutauCC_bg(&tau_error, verbose);
    double nutaubar_background = numode_nutaubarCC_bg(&taubar_error, verbose);
    double total_bg = nutau_background + nutaubar_background;
    if(error != 0)
    {
        (*error) = TMath::Hypot(tau_error, taubar_error);
    }
    if(verbose)
    {
        std::cout << "total nutau background: " << total_bg << "\n";
        if(error != 0)
        {
            std::cout << "total error: +/- " << (*error) << "\n";
        }
    }
    return total_bg;
}

double numode_numuCC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_numu",
            "cc",
            "numu CC bg",
            error,
            verbose
    );
}

double numode_numubarCC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_numubar",
            "cc",
            "numubar CC bg",
            error,
            verbose
    );
}

double numode_total_numu_bg(double* error=0, bool verbose=true)
{
    double mu_error = 0;
    double mubar_error = 0;
    double numu_background = numode_numuCC_bg(&mu_error, verbose);
    double numubar_background = numode_numubarCC_bg(&mubar_error, verbose);
    double total_bg = numu_background + numubar_background;
    if(error != 0)
    {
        (*error) = TMath::Hypot(mu_error, mubar_error);
    }
    if(verbose)
    {
        std::cout << "total numu background: " << total_bg << "\n";
        if(error != 0)
        {
            std::cout << "total error: +/0 " << (*error) << "\n";
        }
    }
    return total_bg;
}

double numode_nueNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_nue",
            "nc",
            "nue NC bg",
            error,
            verbose
    );
}

double numode_beamnueNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_nueflux_nue",
            "nc",
            "beam nue NC bg",
            error,
            verbose
    );
}

double numode_nuebarNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_nuebar",
            "nc",
            "nuebar NC bg",
            error,
            verbose
    );
}

double numode_beamnuebarNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_nuebarflux_nuebar",
            "nc",
            "beam nuebar NC bg",
            error,
            verbose
    );
}

double numode_numuNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_numu",
            "nc",
            "numu NC bg",
            error,
            verbose
    );
}

double numode_numubarNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_numubar",
            "nc",
            "numubar NC bg",
            error,
            verbose
    );
}

double numode_nutauNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numuflux_nutau",
            "nc",
            "nutau NC bg",
            error,
            verbose
    );
}

double numode_nutaubarNC_bg(double* error=0, bool verbose=true)
{
    return get_events(
            "nuflux_numubarflux_nutaubar",
            "nc",
            "nutaubar NC bg",
            error,
            verbose
    );
}

double numode_total_NC_bg(double* error=0, bool verbose=true)
{
    double e_error = 0;
    double beame_error = 0;
    double mu_error = 0;
    double tau_error = 0;
    double ebar_error = 0;
    double beamebar_error = 0;
    double mubar_error = 0;
    double taubar_error = 0;
    double nue_bg = numode_nueNC_bg(&e_error, verbose);
    double beamnue_bg = numode_beamnueNC_bg(&beame_error, verbose);
    double numu_bg = numode_numuNC_bg(&mu_error, verbose);
    double nutau_bg = numode_nutauNC_bg(&tau_error, verbose);
    double nuebar_bg = numode_nuebarNC_bg(&ebar_error, verbose);
    double beamnuebar_bg = numode_beamnuebarNC_bg(&beamebar_error, verbose);
    double numubar_bg = numode_numubarNC_bg(&mubar_error, verbose);
    double nutaubar_bg = numode_nutaubarNC_bg(&taubar_error, verbose);
    double total_bg = nue_bg + beamnue_bg + numu_bg + nutau_bg +
        nuebar_bg + beamnuebar_bg + numubar_bg + nutaubar_bg;
    if(error != 0)
    {
        (*error) = TMath::Sqrt(
                e_error*e_error +
                beame_error*beame_error +
                mu_error*mu_error +
                tau_error*tau_error +
                ebar_error*ebar_error +
                beamebar_error*beamebar_error +
                mubar_error*mubar_error +
                taubar_error*taubar_error
        );
    }
    if(verbose)
    {
        std::cout << "total NC background: " << total_bg << "\n";
        if(error != 0)
        {
            std::cout << "total error: +/0 " << (*error) << "\n";
        }
    }
    return total_bg;
}

#include "Configuration.C"
/*
 * This macro extracts the beam flux from the input to the Fast Monte
 * Carlo. The flux is provided from 0 GeV to approximately 100 GeV in
 * increments of 125 MeV. When the energy bins requested do not line up
 * with 125 MeV increments, a linear interpolation is performed.
 */
int ExtractBeamFluxes(const int NBINS, const double EMIN,
        const double EMAX, bool isNuMode=true)
{

    char outputdir[100];
    std::string filename = std::string(CFG_OutputDirectory(outputdir)) +
        "fmcCD1CDRflux/v3r2p4b/NuMI_Improved_120GeV_StandardDP/";
    if(isNuMode)
    {
        filename += "g4lbne_v3r2p4b_FHC_FD_RIK.root";
    }
    else
    {
        filename += "g4lbne_v3r2p4b_RHC_FD_RIK.root";
    }
    TFile* fin = TFile::Open(filename.c_str(), "READ");
    if(!fin)
    {
        std::cout << "ERROR: ROOT file does not exist.\n";
        return 1;
    }
    std::vector<std::string> histogramnames;
    histogramnames.push_back("numu_flux");
    histogramnames.push_back("nue_flux");
    histogramnames.push_back("numubar_flux");
    histogramnames.push_back("nuebar_flux");
    histogramnames.push_back("nutau_flux");
    histogramnames.push_back("nutaubar_flux");
    std::vector<std::string>::iterator it = histogramnames.begin();
    for(it; it != histogramnames.end(); ++it)
    {
        // Create the correct file name
        std::string& histname = *it;
        std::cout << histname << std::endl;

        TCanvas* c1 = new TCanvas();
        TH1D* spectrum = (TH1D*) (fin->Get(histname.c_str()));
        // The flux is given in units of neutrinos/GeV/m^2/POT
        // Expect histogram to have <500 bins
        const size_t INPUT_HIST_BINS = 500;
        double spectrumx[INPUT_HIST_BINS];
        double spectrumy[INPUT_HIST_BINS];
        for(int i = 1; i <= spectrum->GetNbinsX(); ++i)
        {
            spectrumx[i-1] = spectrum->GetBinLowEdge(i);
            spectrumy[i-1] = spectrum->GetBinContent(i);
        }
        TGraph* spectrum_gr = new TGraph(spectrum->GetNbinsX(), spectrumx, spectrumy);
        // Dump the results to a CSV file
        std::ofstream outputfile;
        outputfile.open((std::string("/nashome/s/") +
                    "skohn/outputs/120/flux/" + histname +
                    Form("%d_%snumode.csv", NBINS, isNuMode?"":"a")).c_str());
        if(!outputfile.is_open())
        {
            std::cout << "ERROR: Could not open file\n";
            return 2;
        }
        else
        {
            std::cout << "INFO: Opened output file\n";
        }
        const double ESTEP = (EMAX - EMIN)/NBINS;
        for(size_t ebin = 0; ebin < NBINS; ++ebin)
        {
            double energy = EMIN + ESTEP * ebin;
            double value = spectrum_gr->Eval(energy);
            outputfile << value << "\n";
        }
        outputfile.close();
        delete spectrum_gr;
    }
    fin->Close();
    return 0;
}

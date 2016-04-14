#include <fstream>
#include "Configuration.C"
#include <TCut.h>
int ExtractEfficiency(const int NBINSSQUARE)
{
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
    std::vector<std::string> eventcuts;
    eventcuts.push_back("EvClass_reco == 0");
    eventcuts.push_back("EvClass_reco == 1");
    eventcuts.push_back("EvClass_reco == 2");
    std::vector<std::string> eventcutnames;
    eventcutnames.push_back("_numuCC-like");
    eventcutnames.push_back("_nueCC-like");
    eventcutnames.push_back("_NC-like");
    std::string prefix = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal";
    prefix.append("/fastmcNtp_20160404_lbne_g4lbnev3r2p4b_");
    std::string suffix = "_LAr_1_g280_Ar40_5000_GENIE_2100.root";
    for(size_t i = 0; i < eventcuts.size(); ++i)
    {
        std::string eventcutname = eventcutnames.at(i);
        std::string eventcut = eventcuts.at(i);
        std::vector<std::string>::iterator it;
        for(it = filenames.begin(); it != filenames.end(); ++it)
        {
            // Create the correct file name
            std::string fluxtype = *it;
            std::string filename = prefix;
            filename.append(fluxtype);
            filename.append(suffix);
            std::cout << filename << std::endl;

            // Open the file
            TFile* fin = TFile::Open(filename.c_str(), "READ");
            std::cout << "fin = " << fin << std::endl;
            TTree* fluxData = (TTree*) fin->Get("gst");
            std::cout << "fluxData TTree = " << fluxData << std::endl;
            TCanvas* c1 = new TCanvas();
            //const int NBINSSQUARE = 20;
            const double MINSQUARE = 0;
            const double MAXSQUARE = 10;
            const int XBINS = NBINSSQUARE;
            const int YBINS = NBINSSQUARE;
            const int XMIN = MINSQUARE;
            const int XMAX = MAXSQUARE;
            const int YMIN = MINSQUARE;
            const int YMAX = MAXSQUARE;
            TH1D* enuresponse = new TH1D(fluxtype.c_str(), fluxtype.c_str(), XBINS, XMIN, XMAX);
            enuresponse->GetXaxis()->SetLabelSize(0.05);
            enuresponse->GetYaxis()->SetLabelSize(0.05);
            enuresponse->GetXaxis()->SetTitleSize(0.05);
            enuresponse->GetYaxis()->SetTitleSize(0.05);
            enuresponse->GetXaxis()->SetTitleOffset(0.9);
            enuresponse->GetYaxis()->SetTitleOffset(1.1);
            fluxData->Draw((std::string("Ev_reco>>") + fluxtype).c_str(), (eventcut + " && cc").c_str());
            enuresponse->GetXaxis()->SetTitle("E_{#nu, reco} [GeV]");
            enuresponse->GetYaxis()->SetTitle("Number of events");
            double nentries = enuresponse->Integral();
            TH1D* normalizationHist = new TH1D("norm", "norm", XBINS, XMIN, XMAX);
            fluxData->Draw("Ev_reco>>norm", "cc");
            enuresponse->Divide(normalizationHist); // Normalize
            //c1->Print((std::string("~/outputs/detector-response/") + fluxtype + eventcutname + "_trueNC.pdf").c_str());
            // Print out the matrix information
            std::ofstream outputfile;
            char outputdir[100];
            outputfile.open((std::string(CFG_OutputDirectory(outputdir)) +
                        "efficiencies2/" + fluxtype + eventcutname +
                        Form("_trueCC%d.csv", NBINSSQUARE)).c_str());
            if(!outputfile.is_open())
            {
                std::cout << "ERROR\n";
                return 0;
            }
            for(int column = 1; column <= XBINS; ++column)
            {
                outputfile << (enuresponse->GetBinContent(column));
                if(column != XBINS)
                {
                    outputfile << ", ";
                }
                else
                {
                    outputfile << "\n";
                }
            }
            outputfile.close();
            fin->Close();

        }
    }

    return 0;
}
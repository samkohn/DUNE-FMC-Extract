#include <fstream>
#include "Configuration.C"
#include <TCut.h>
int ExtractDetectorResponseMatrix(const int NBINSSQUARE, std::string channel, bool factored=false)
{
    std::string channel_caps;
    if(channel.compare("cc") == 0)
    {
        channel_caps = "CC";
    }
    else if(channel.compare("nc") == 0)
    {
        channel_caps = "NC";
    }
    else
    {
        std::cout << "ERROR: invalid channel (" << channel << ")\n";
        return 1;
    }
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
    std::vector<std::string> eventcutnames;
    if(factored)
    {
        eventcuts.push_back("1");
        eventcutnames.push_back("");
    }
    else
    {
        eventcuts.push_back("(EvClass_reco == 0 && Tau_Prob_numu > 0.2 && NC_Prob_numu > 0.2)");
        eventcuts.push_back("(EvClass_reco == 1 && Tau_Prob_nue > 0.6 && NC_Prob_nue > 0.75)");
        eventcuts.push_back("(EvClass_reco == 2)");
        eventcutnames.push_back("_numuCC-like");
        eventcutnames.push_back("_nueCC-like");
        eventcutnames.push_back("_NC-like");
    }
    std::string prefix = CFG_InputDir + CFG_IDRMDir;
    //std::string prefix = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal";
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
            std::cout << "INFO: Processing flux type: " << fluxtype << std::endl;

            // Open the file
            TFile* fin = TFile::Open(filename.c_str(), "READ");
            if(fin != 0)
            {
                std::cout << "INFO: Opened file at " << filename << std::endl;
            }
            else
            {
                std::cout << "ERROR: Could not open file at " << filename << std::endl;
                return 1;
            }
            TTree* fluxData = (TTree*) fin->Get("gst");
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
            TH2D* enuresponse = new TH2D(fluxtype.c_str(), fluxtype.c_str(), XBINS, XMIN, XMAX,
                    YBINS, YMIN, YMAX);
            enuresponse->GetXaxis()->SetLabelSize(0.05);
            enuresponse->GetYaxis()->SetLabelSize(0.05);
            enuresponse->GetXaxis()->SetTitleSize(0.05);
            enuresponse->GetYaxis()->SetTitleSize(0.05);
            enuresponse->GetXaxis()->SetTitleOffset(0.9);
            enuresponse->GetYaxis()->SetTitleOffset(1.1);
            fluxData->Draw((std::string("Ev_reco:Ev>>") + fluxtype).c_str(), (eventcut + " && " + channel).c_str(), "colz");
            enuresponse->GetXaxis()->SetTitle("E_{#nu} [GeV]");
            enuresponse->GetYaxis()->SetTitle("E_{#nu, reco} [GeV]");
            // Print out the matrix information
            std::ofstream outputfile;
            std::string outfilename = CFG_OutputDir + CFG_DRMDir +
                        fluxtype + eventcutname + Form("_true%s%d.csv",
                                channel_caps.c_str(), NBINSSQUARE);
            outputfile.open(outfilename.c_str());
            if(outputfile.is_open())
            {
                std::cout << "INFO: writing output to " << outfilename << "\n";
            }
            else
            {
                std::cout << "ERROR: could not open output file at " << outfilename << "\n";
                return 1;
            }
            std::string outputheader = "# Source: " + filename;
            outputheader += "\n# Event cuts: \"" + eventcut + " && " + channel + "\"";
            outputheader += "\n# True event type: " + channel + "\n";
            outputfile << outputheader;
            for(int row = 1; row <= YBINS; ++row)
            {
                for(int column = 1; column <= XBINS; ++column)
                {
                    outputfile << (enuresponse->GetBinContent(column, row));
                    if(column != XBINS)
                    {
                        outputfile << ", ";
                    }
                    else
                    {
                        outputfile << "\n";
                    }
                }
            }
            outputfile.close();
            fin->Close();

        }
    }

    return 0;
}

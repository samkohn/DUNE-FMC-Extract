/*
 * This macro will attempt to extract all of the inputs required for the
 * matrix-based DUNE FD spectrum calculator.
 *
 * To configure the system, edit the Configuration.C file to provide the
 * appropriate output directories. For the input directories, the
 * suggestion is to symlink/alias the directories containing input files
 * into a single "inputs" directory. For example:
 * ./inputs:
 *     input_flux_dir (symlink)
 *     input_xsec_dir (symlink)
 *     etc...
 *
 * Of course, you should also make sure that the directories specified
 * in the Configuration.C file exist.
 *
 * Lastly, check in each of the Extract<...>.C macros to make sure the
 * input files are the ones you want. For example, in the FMC outputs
 * directory used for the DRM and Efficiency extractors, there are many
 * different runs of the FMC, distinguished only by file names. These
 * file names are not set by the Configuration.C file. They are set in
 * the code itself.
 */

int Extract(const int NBINS, const double EMIN, const double EMAX)
{
    int success = 0;
    std::cout << "INFO: loading ROOT macros...\n";
    success += gROOT->ProcessLine(".L ExtractBeamFluxes.C");
    gROOT->SetMacroPath("/dune/app/users/lblpwg_tools/INSTALLATION/lib/");
    success += gROOT->LoadMacro("libThreeProb_2.10.so");
    gROOT->SetMacroPath((char*) 0);
    success += gROOT->ProcessLine(".L CreateOscillationVectors.C+");
    success += gROOT->ProcessLine(".L ExtractCrossSectionVector.C");
    success += gROOT->ProcessLine(".L ExtractDetectorResponseMatrix.C");
    success += gROOT->ProcessLine(".L ExtractEfficiency.C");
    std::cout << "INFO: Check to make sure all files loaded succesfully.\n";
    std::cout << "      Enter 0 to continue, 1 to abort.\n";
    int go_on = 0;
    std::cin >> go_on;
    if(go_on == 1)
    {
        std::cout << "INFO: Aborting...\n";
        return 1;
    }
    else
    {
        std::cout << "INFO: continuing...\n";
    }

    // Beam flux
    std::cout << "INFO: Processing beam flux\n";
    success += ExtractBeamFluxes(NBINS, EMIN, EMAX, true);
    success += ExtractBeamFluxes(NBINS, EMIN, EMAX, false);
    if(success == 0)
    {
        std::cout << "INFO: successfully processed beam flux\n";
    }
    else
    {
        std::cout << "ERROR: could not process beam flux\n";
        return success;
    }

    // Oscillation Probability
    std::cout << "INFO: Processing oscillation probability\n";
    CreateOscillationVectors(NBINS, EMIN, EMAX);
    CreateManyOscillationVectors(NBINS, EMIN, EMAX);
    if(success == 0)
    {
        std::cout << "INFO: successfully processed oscillation probability\n";
    }
    else
    {
        std::cout << "ERROR: could not process oscillation probability\n";
        return success;
    }

    // Cross Section
    std::cout << "INFO: Processing cross sections\n";
    ExtractAllCrossSections(NBINS, EMIN, EMAX);
    if(success == 0)
    {
        std::cout << "INFO: successfully processed cross section\n";
    }
    else
    {
        std::cout << "ERROR: could not process cross section\n";
        return success;
    }

    // Detector Response
    std::cout << "INFO: Processing detector response\n";
    ExtractDetectorResponseMatrix(NBINS, EMIN, EMAX, "cc", true);
    ExtractDetectorResponseMatrix(NBINS, EMIN, EMAX, "cc", false);
    ExtractDetectorResponseMatrix(NBINS, EMIN, EMAX, "nc", true);
    ExtractDetectorResponseMatrix(NBINS, EMIN, EMAX, "nc", false);
    if(success == 0)
    {
        std::cout << "INFO: successfully processed detector response\n";
    }
    else
    {
        std::cout << "ERROR: could not process detector response\n";
        return success;
    }

    // Efficiency
    std::cout << "INFO: processing efficiency\n";
    ExtractEfficiency(NBINS, EMIN, EMAX, "cc");
    ExtractEfficiency(NBINS, EMIN, EMAX, "nc");
    if(success == 0)
    {
        std::cout << "INFO: successfully processed efficiency\n";
    }
    else
    {
        std::cout << "ERROR: could not process efficiency\n";
        return success;
    }
    return 0;
}

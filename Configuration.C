#ifndef CONFIGURATION_C
#define CONFIGURATION_C
/*
 * This "macro" file contains the system-specific configuration. To use
 * the other macros/scripts on a different system, modify this file
 * accordingly.
 */

std::string CFG_InputDir("");
std::string CFG_IFluxDir("/dune/data/users/lblpwg_tools/FastMC_Data/flux_files/v3r2p4b/nominal/");
std::string CFG_IXSecDir("/dune/app/users/lblpwg_tools/SOFTWARE/genie-2.10.0-20151103/splines/");
std::string CFG_IDRMDir("/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal/");
std::string CFG_IEffDir("/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal/");

std::string CFG_OutputDir("/nashome/s/skohn/outputs/test/");
std::string CFG_FluxDir("/flux/");
std::string CFG_OscDir("/oscvectors/");
std::string CFG_OscSetsDir("/oscvectorsets/");
std::string CFG_XSecDir("/cross-sections/");
std::string CFG_DRMDir("/detector-response/");
std::string CFG_EffDir("/efficiencies/");
#endif

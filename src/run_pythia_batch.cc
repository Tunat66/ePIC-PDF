// Modified by Tuna Tasali 2026
// THIS VERSION: Creates "batches" of hepmc files under a /hepmc
// directory, with each batch containing 100 events. 2000 events
// are simulated in 4 hours for a single core on our cluster. 
// I estimate 20mins for each batch of 100 events, including reconstruction.
// Copyright (C) 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Mikhail Kirsanov <Mikhail.Kirsanov@cern.ch>

// Keywords: basic usage; command file; command line option; hepmc; LHE file

// This program illustrates how HepMC files can be written by Pythia8.
// Input and output files are specified on the command line, e.g. like
//     ./main132 -c main132.cmnd -o main132.hepmc > main132.log
// Either internal Pythia processes or Les Houches Event Files can be used.
// The main program contains no analysis; this is intended to happen later.
// It therefore "never" has to be recompiled to handle different tasks.

// WARNING: typically one needs 25 MB/100 events at the LHC.
// Therefore large event samples may be impractical.

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/InputParser.h"
#include "Pythia8Plugins/HepMC3.h"
//#ifndef HEPMC2
//#include "Pythia8Plugins/HepMC3.h"
//#else
//#include "Pythia8Plugins/HepMC2.h"
//#endif

using namespace Pythia8;

//==========================================================================

//a method which takes a reference to the pythia generator and a batch number, and writes the batch of events to a file
void write_batch(Pythia& pythia, string batch_dir, int batch_num, int batch_size, int nAbort = 10) 
{
  // Interface for conversion from Pythia8::Event to HepMC event.
  // Specify file where HepMC events will be stored.
  
  string batch_out = batch_dir + "/batch_" + to_string(batch_num) + ".hepmc";
  //update the toHepMC object with the new output file
  Pythia8ToHepMC* toHepMC = new Pythia8ToHepMC(batch_out);
  
  
  //cout << "\n >>> Writing batch " << batch_num << " to file '" << batch_out << "' <<< \n";  

  // Begin event loop.
  int iAbort = 0;
  for (int iEvent = 0; iEvent < batch_size; ++iEvent) {

    // Generate event.
    if (!pythia.next()) {

      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) {
        cout << " Aborted since reached end of Les Houches Event File\n";
        break;
      }

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }

    // Construct new empty HepMC event, fill it and write it out.
    toHepMC->writeNextEvent( pythia );

  // End of event loop. Statistics.
  }
  delete toHepMC;
}

void write_batch_weighted(Pythia& pythia, string batch_dir, int batch_num, int batch_size, int nAbort = 10) 
{
}

int main(int argc, char* argv[]) {
  //lhapdf path
  //setenv("LHAPDF_DATA_PATH", "/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/", 1);


  // Set up command line options.
  InputParser ip("This program illustrates how HepMC files can be written by"
    " Pythia8.", {"./run_pythia -c main132.cmnd -o main132"});
  ip.require("c", "Use this user-written command file.", {"-cmnd"});
  ip.require("o", "Specify output directory to which hepmc files will be written.", {"-out"});

  
  // Initialize the parser and exit if necessary.
  InputParser::Status status = ip.init(argc, argv);
  if (status != InputParser::Valid) return status;
  
  

  // Confirm that external files will be used for input and output.
  string cmnd(ip.get<string>("c")), out(ip.get<string>("o"));
  cout << "\n >>> PYTHIA settings will be read from file '" << cmnd
       << "' <<< \n >>> HepMC events will be written to files with prefix '"
       << out << "' <<< \n";

  out = out + "/hepmc";
  
  // Generator.
  Pythia pythia;
  
  // Read in commands from external file.
  pythia.readFile(cmnd);
  
  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  
  // If Pythia fails to initialize, exit with error.
  cout << "\n >>> Welcome to the Pythia8 HepMC event writer example <<< \n";  
  if (!pythia.init()) return 1;
  
  //calculate the number of batches
  int batch_size = 100;
  if (nEvent % batch_size != 0) {
    cout << "Warning: number of events is not a multiple of batch size (100).";
    throw std::runtime_error("Number of events must be a multiple of batch size (100).");
  }
  int n_batches = nEvent / batch_size;
  cout << "\n >>> Total number of events: " << nEvent
       << "\n >>> Batch size: " << batch_size
       << "\n >>> Number of batches: " << n_batches
       << "\n";
  
  //loop over the batches and write
  for(int i = 0; i < n_batches; ++i) {
    write_batch(pythia, out, i, batch_size, nAbort);
  }  

  pythia.stat();

  // Done.
  return 0;
}



# ePIC-PDF
PDF and DIS cross section Studies using the EIC Collaboration Software

Some stuff to run on the eic-shell available on the physics server.

If you are going to do stuff with xFitter, source xFitter_setup.sh

Notice there is a run_pythia and run_pythia_batch. 'batch' creates
compact 100 event hepmc files which can be simulated in parallel.

For now, I skipped setting up BeAGLE. I will reweight pythia8 events
I will then try to somehow get different shadowing scenarios with BeAGLE.

This means I am generating ep events at eA energies. This causes abconv to
fail. For now I will not abconv the data.

Step 1: You need a .cmnd steering file for pythia, one is usually provided
Step 2: You create the hepmc file from the .cmnd file with a small executable I wrote:

'../eic-shell' (pythia is available here)
'./run_pythia -c {cmnd_file.cmnd} -o {output_file.hepmc}' 

Step 3: You need to apply 'afterburner' to the hepmc file for beam crossing angle corrections
(note that we are still in eic shell)
'abconv $File -o $OutputFilename'

Step 4: Introduce the detector and run a simulation, and reconstruction. For instance
(we are still in eic-shell)
source /opt/detector/epic-main/bin/thisepic.sh
npsim --compactFile $DETECTOR_PATH/epic_craterlake_5x41.xml --numberOfEvents 10 --inputFiles eic_DEMPgen_5on41_ip6_pi+_1B_1.hepmc --outputFile DEMPgen_5on41_pi+_10_TestOutput.edm4hep.root

What you really one do is to use a premade script like: simu_reco.sh

Step 5: Feed the eicrecon output .root file to the root macro eicrecon_to_xfitter.C to extract the cross sections
and related statistical errors. I am still working on this






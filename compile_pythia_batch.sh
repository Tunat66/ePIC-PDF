g++ src/run_pythia_batch.cc -o run_pythia_batch \
  $(/home/pemb7000/eic/pythia8317/bin/pythia8-config --cxxflags --libs --hepmc3)
 #$(/opt/local/bin/pythia8-config --cxxflags --libs --hepmc3) \
  #-I/opt/local/include -L/opt/local/lib -lHepMC3\
  #$(/home/pemb7000/eic/pythia8317/bin/pythia8-config --cxxflags --libs --hepmc3) #\
 
  #-I/home/pemb7000/eic/pythia8317/include/Pythia8 \
  #-I/home/pemb7000/eic/pythia8317/include/Pythia8Plugins 
  #`./pythia8-config --hepmc3`
  
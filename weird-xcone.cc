#include "fastjet/ClusterSequence.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/XConePlugin.hh"

#include <iostream>
#include <vector>
using namespace fastjet;
using namespace std;
using namespace contrib;


// Compile with Makefile:
//
// make
//
// Run with:
//
// ./weird-xcone < weirdEvent.dat


void
initPlugin(std::unique_ptr<NjettinessPlugin> & ptr, int N, double R0, double beta, bool usePseudoXCone)
{
  if (usePseudoXCone) {
    ptr.reset(new PseudoXConePlugin(N, R0, beta));
  } else {
    ptr.reset(new XConePlugin(N, R0, beta));
  }
}


int main () {
  // fastjet::GhostedAreaSpec gas;
  // std::vector<int> seeds(2);
  // unsigned int runNum_uint = static_cast <unsigned int> (1);
  // unsigned int evNum_uint = static_cast <unsigned int> (794651);
  // uint minSeed_ = 14327;
  // seeds[0] = std::max(runNum_uint, minSeed_ + 3) + 3 * evNum_uint;
  // seeds[1] = std::max(runNum_uint, minSeed_ + 5) + 5 * evNum_uint;
  // gas.set_random_status(seeds);

  vector<PseudoJet> _psj;
  double px, py , pz, E;
  while (cin >> px >> py >> pz >> E) {
    // create a fastjet::PseudoJet with these components and put it onto
    // back of the input_particles vector
    _psj.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }
  
  bool usePseudoXCone_ = true;
  uint NJets_ = 2;
  double RJets_ = 1.2;
  double BetaJets_ = 2.0;
  uint NSubJets_ = 3;
  double RSubJets_ = 0.4;
  double BetaSubJets_ = 2.0;

  vector<PseudoJet> fatjets;
  XConePlugin plugin_xcone(NJets_, RJets_, BetaJets_);
  cout << "Cluster fatjets" << endl;
  JetDefinition jet_def_xcone(&plugin_xcone);
  ClusterSequence clust_seq_xcone(_psj, jet_def_xcone);
  fatjets = clust_seq_xcone.inclusive_jets(0);

  if (fatjets.size() != NJets_) {
    cout << "XConePlugin has only found " << fatjets.size() << " jets but requested " << NJets_;
  }

  vector<int> list_fat = clust_seq_xcone.particle_jet_indices(fatjets);
  
  // loop over all fatjets and cluster subjets
  for(unsigned int i=0; i<fatjets.size(); i++){
    vector<PseudoJet> particles_in_fatjet;
    for (unsigned int ipart=0; ipart < _psj.size(); ++ipart) {
      if (list_fat[ipart] < 0) continue;
      else if (abs(list_fat[ipart])==i) {
        particles_in_fatjet.push_back(_psj.at(ipart));
      }
    }

    // check if there are more particles then required subjets
    bool enoughParticles = (particles_in_fatjet.size() > NSubJets_);

    // Run second clustering step (subjets) for each fat jet
    vector<PseudoJet> subjets;
    cout << "*** Cluster subjet for fatjet " << i << endl;
    ClusterSequence * clust_seq_sub;
    if (enoughParticles) {
      XConePlugin plugin_xcone_sub(NSubJets_, RSubJets_, BetaSubJets_);
      JetDefinition jet_def_sub(&plugin_xcone_sub);
      clust_seq_sub = new ClusterSequence(particles_in_fatjet, jet_def_sub);
      subjets = clust_seq_sub->inclusive_jets(0);
    }

    if (subjets.size() != NSubJets_) {
      cout << "Only found " << subjets.size() << " subjets but requested " << NSubJets_ << ". Fatjet had " << particles_in_fatjet.size() << " constituents" << endl;
      std::cout << "fatjet: " << fatjets[i].pt() << " : " << fatjets[i].eta() << " : " << fatjets[i].phi() << endl;
      
      vector<int> list_sub = clust_seq_sub->particle_jet_indices(subjets);
      for (uint jj=0; jj<subjets.size(); jj++) {
        std::cout << "proper subjet: " << subjets[jj].pt() << " : " << subjets[jj].eta() << " : " << subjets[jj].phi() << endl;
        vector<PseudoJet> particle_in_subjet;
        for (unsigned int jpart=0; jpart < _psj.size(); ++jpart) {
          if (list_sub[jpart] < 0) continue;
          else if (abs(list_sub[jpart])==jj) {
            particle_in_subjet.push_back(_psj.at(jpart));
          }
        }
        for (const auto & citr : sorted_by_rapidity(particle_in_subjet)) {
          std::cout << "proper subjet constitutent: " << citr.pt() << " : " << citr.eta() << " : " << citr.phi() << endl;
        }

      }
      
      // for (const auto & citr : sorted_by_rapidity(particle_in_subjet)) {
      //     std::cout << "subjet constitutent: " << citr.pt() << " : " << citr.eta() << " : " << citr.phi() << endl;
      // }

      for ( const auto & itr: subjets) {
        std::cout << "subjet: " << itr.pt() << " : " << itr.eta() << " : " << itr.phi() << endl;
        for (const auto & citr : sorted_by_rapidity(itr.constituents())) {
          std::cout << "subjet constitutent: " << citr.pt() << " : " << citr.eta() << " : " << citr.phi() << endl;
        }
      }
    }

  }

}
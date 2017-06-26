#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
#include <iostream>


#include <boost/functional/hash.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>

#include "variants.hpp"

using namespace sdsl;
using namespace std;


#ifndef GRAMTOOLS_BWT_SEARCH_H
#define GRAMTOOLS_BWT_SEARCH_H


using CSA = csa_wt<wt_int<bit_vector, rank_support_v5<>>, 2, 16777216>;

template<typename SEQUENCE>
struct seq_hash {
    std::size_t operator()(const SEQUENCE &seq) const {
        std::size_t hash = 0;
        boost::hash_range(hash, seq.begin(), seq.end());
        return hash;
    }
};

template<typename SEQUENCE, typename T>
using sequence_map = std::unordered_map<SEQUENCE, T, seq_hash<SEQUENCE>>;

template<typename SEQUENCE>
using sequence_set = std::unordered_set<SEQUENCE, seq_hash<SEQUENCE>>;

CSA csa_constr(std::string fname, std::string int_al_fname,
               std::string memory_log_fname, std::string csa_file,
               bool fwd, bool verbose);

void calc_kmer_matches(CSA &csa, int k,
                       sequence_map<std::vector<uint8_t>, std::list<std::pair<uint64_t, uint64_t>>> &kmer_idx,
                       sequence_map<std::vector<uint8_t>, std::list<std::pair<uint64_t, uint64_t>>> &kmer_idx_rev,
                       sequence_map<std::vector<uint8_t>, std::list<std::vector<std::pair<uint32_t, std::vector<int>>>>> &kmer_sites,
                       std::vector<int> &mask_a, uint64_t maxx, sequence_set<std::vector<uint8_t>> &kmers_in_ref,
                       std::vector<std::vector<uint8_t>> &kmerfile, const VariantMarkers &variants,
                       unordered_map<uint8_t, vector<uint64_t>> &rank_all);

void precalc_ranks(CSA &csa, std::unordered_map<uint8_t,vector<uint64_t>>& rank_all);

uint64_t bidir_search(const FM_Index &fm_index,
                      uint64_t& left, uint64_t& right,
                      uint64_t& left_rev, uint64_t& right_rev,
                      uint8_t c, std::unordered_map<uint8_t,std::vector<uint64_t>> &rank_all);

std::pair<uint32_t, std::vector<int>> get_location(const FM_Index &fm_index,
                                                   const uint64_t marker_idx, const uint64_t marker,
                                                   const bool last, std::vector<int> &allele,
                                                   const std::vector<int> &mask_a);

bool skip(const FM_Index &fm_index,
          uint64_t& left, uint64_t& right,
          uint64_t& left_rev, uint64_t& right_rev,
          uint64_t num, uint64_t maxx);

void bidir_search_bwd(const FM_Index &fm_index,
                      uint64_t left, uint64_t right,
                      uint64_t left_rev, uint64_t right_rev, // not used in bwd
                      const std::vector<uint8_t>::iterator fasta_pattern_begin,
                      const std::vector<uint8_t>::iterator fasta_pattern_end,
                      std::list<std::pair<uint64_t, uint64_t>> &sa_intervals,
                      std::list<std::pair<uint64_t, uint64_t>> &sa_intervals_rev, // not used in bwd
                      std::list<std::vector<std::pair<uint32_t, std::vector<int>>>> &sites,
                      std::vector<int> &mask_a, const uint64_t maxx, bool &first_del,
                      const bool kmer_precalc_done, const VariantMarkers &variants,
                      std::unordered_map<uint8_t, vector<uint64_t>> &rank_all);


std::vector<uint8_t>::iterator bidir_search_fwd(CSA &csa,
                                                uint64_t left, uint64_t right,
                                                uint64_t left_rev, uint64_t right_rev,
                                                std::vector<uint8_t>::iterator pat_begin,
                                                std::vector<uint8_t>::iterator pat_end,
                                                std::list<std::pair<uint64_t, uint64_t>> &sa_intervals,
                                                std::list<std::pair<uint64_t, uint64_t>> &sa_intervals_rev,
                                                std::list<std::vector<std::pair<uint32_t, std::vector<int>>>> &sites,
                                                std::vector<int> &mask_a, uint64_t maxx, bool &first_del,
                                                bool kmer_precalc_done, unordered_map<uint8_t,vector<uint64_t>>& rank_all);

#endif //GRAMTOOLS_BWT_SEARCH_H

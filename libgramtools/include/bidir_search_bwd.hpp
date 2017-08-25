#include "ranks.hpp"
#include "kmers.hpp"


#ifndef GRAMTOOLS_BIDIR_SEARCH_BWD_HPP_HPP
#define GRAMTOOLS_BIDIR_SEARCH_BWD_HPP_HPP


void bidir_search_bwd(SA_Intervals &sa_intervals, Sites &sites, bool &delete_first_interval,
                      const std::vector<uint8_t>::const_iterator read_begin,
                      const std::vector<uint8_t>::const_iterator read_end, const std::vector<int> &allele_mask,
                      const uint64_t maxx, const bool kmer_precalc_done, const DNA_Rank &rank_all,
                      const FM_Index &fm_index);

bool reduce_sa_intervals(Sites &sites, SA_Intervals &sa_intervals, const bool delete_first_interval,
                         const uint8_t read_char,
                         const bool read_char_is_last, const std::vector<int> &allele_mask, const uint64_t maxx,
                         const bool kmer_precalc_done, const DNA_Rank &rank_all, const FM_Index &fm_index);

void process_reads_overlapping_variants(SA_Intervals &sa_intervals, SA_Interval &sa_interval,
                                        Sites &sites,
                                        Site &site, const bool delete_first, const uint64_t maxx,
                                        const std::vector<int> &allele_mask, const FM_Index &fm_index);

bool match_next_charecter(const uint8_t next_char, bool delete_first_interval, SA_Intervals &sa_intervals,
                          SA_Intervals::iterator &sa_intervals_it, Sites &sites, Sites::iterator &sites_it,
                          const DNA_Rank &rank_all, const FM_Index &fm_index);

bool match_next_charecter(const uint8_t next_char, bool delete_first_interval, SA_Intervals &sa_intervals,
                          SA_Intervals::iterator &sa_intervals_it, Sites &sites, Sites::iterator &sites_it,
                          const DNA_Rank &rank_all, const FM_Index &fm_index);

void add_sa_interval_for_skip(uint64_t &left_new, uint64_t &right_new, uint64_t &last_begin, bool &second_to_last,
                              bool &ignore, const uint64_t marker, const uint64_t marker_idx,
                              const uint64_t previous_marker);

bool update_sites_crossed_by_reads(SA_Intervals &sa_intervals, Sites &sites, const uint64_t left_new,
                                   const uint64_t right_new, const bool at_first_sa_interval, const bool ignore,
                                   const bool last, const uint64_t last_begin, const bool second_to_last,
                                   const std::vector<int> &allele_mask, const bool delete_first, const uint64_t marker,
                                   const uint64_t marker_idx, const FM_Index &fm_index);

std::pair<uint64_t, std::vector<int>> get_variant_site_edge(std::vector<int> &allele,
                                                            const uint64_t marker,
                                                            const uint64_t marker_idx,
                                                            const std::vector<int> &allele_mask,
                                                            const bool last,
                                                            const FM_Index &fm_index);


#endif //GRAMTOOLS_BIDIR_SEARCH_BWD_HPP_HPP

/*
 * Copyright [2017] Jacek Weremko
 */

#ifndef SRC_FUZZON_H_
#define SRC_FUZZON_H_

#include "./fuzzon_executor.h"

#include <boost/filesystem.hpp>
#include <utils/time.hpp>
#include <string>
#include <vector>

#include "./fuzzon_corpus.h"

namespace fuzzon {

class Fuzzon {
 public:
  Fuzzon(std::string output_dir,
         std::string sut_path,
         const std::vector<std::string>& env_flags,
         int sut_runtime_timeout,
         Executor::Mode mode,
         Coverage::TrackMode track_mode,
         int test_timeout,
         int max_testcases);

  void ScanCorpus(std::string corpus_base);
  void TestInput(std::string test_me);
  void Generation(std::string input_format, int test_cases_to_generate);
  void MutationDeterministic(int level, bool white_chars_preservation);
  void MutationJSONBased(int level, bool white_chars_preservation);
  void MutationNonDeterministic(int test_cases_to_mutate,
                                bool white_chars_preservation);

  void PrintStats();
  void Dump();

 private:
  bool shall_finish() {
    return test_timeout_() || (corpus_.total_testcases() > max_testcases_);
  }

  const std::string output_dir_;
  const Timeout test_timeout_;
  const int max_testcases_;

  Corpus corpus_;
  Executor execution_monitor_;
};

}  // namespace fuzzon

#endif  // SRC_FUZZON_H_

/*
 * Copyright [2017] Jacek Weremko
 */

#ifndef SRC_FUZZON_CORPUS_H_
#define SRC_FUZZON_CORPUS_H_

#include "./fuzzon_executiondata.h"

#include <boost/filesystem.hpp>
#include <list>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include "./fuzzon_testcase.h"

namespace fuzzon {

class Corpus {
 public:
  explicit Corpus(std::string output_path);

  bool AddIfInteresting(ExecutionDataSP add_me_to_corpus);

  ExecutionData* const SelectFavorite();
  TestCase* const SelectRandom();
  TestCase* const SelectNotYetExhaustMutated();

  std::stringstream GetShortStats();
  std::stringstream GetFullStats();

  std::vector<boost::filesystem::path> GatherCoprusFiles() const;

  void Dump();

  int total_testcases() { return summary_.test_cases; }

 private:
  const std::string DIR_NAME_CORPUS = "corpus";
  const std::string DIR_NAME_RESULTS = "results";
  const std::string DIR_NAME_CRASH = "crash";
  const std::string DIR_NAME_TIMEOUE = "timeout";

  const std::chrono::system_clock::time_point start_;

  boost::filesystem::path output_path_;
  std::vector<ExecutionDataSP> data_;
  CampaignSummary summary_;

  bool IsInteresting(const ExecutionData& am_i);
  void AddExecutionData(ExecutionDataSP add_me_to_corpus);
};

} /* namespace fuzzon */

#endif  // SRC_FUZZON_CORPUS_H_

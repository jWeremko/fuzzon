/*
 * Copyright [2017] Jacek Weremko
 */

#include "./fuzzon.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <vector>
#include <string>
#include <utility>

#include "./fuzzon_random.h"
#include "./utils/logger.h"

#ifndef UNIT_TEST

namespace stdch = std::chrono;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char** argv) {
  auto time_now = boost::posix_time::second_clock::local_time();
  auto time_now_str = boost::posix_time::to_simple_string(time_now);

  std::string verbosity_values;

  po::options_description desc("Options");
  desc.add_options()("help", "produce help message");
  desc.add_options()("verbose,v", po::value<>(&verbosity_values)->implicit_value(""), "Logging level");
  desc.add_options()("sut,s", po::value<fs::path>(), "Software under test path");
  desc.add_options()("env_flag,e", po::value<std::vector<std::string>>(), "Environmental flag set for SUT.");
  desc.add_options()("input_format,i", po::value<fs::path>(), "JSON Input format file path");
  desc.add_options()("out,o", po::value<fs::path>(), "Output directory path");
  desc.add_options()("corpus_seeds,c", po::value<fs::path>(),
                     "Path to directory with sample input files aka corpus seed.");
  desc.add_options()("single_test_timeout,e", po::value<int>(), "Max allowed time to execute SUT");
  desc.add_options()("total_timeout,t", po::value<int>(), "Total tests timeout.");
  desc.add_options()("generate,g", po::value<int>(), "Generation phase: number of test cases to generate.");
  desc.add_options()("mutate_d,md", po::value<int>(),
                     "Deterministic mutation phase: level of mutations: 0-None, 1-Max");
  desc.add_options()("mutate_nd,mnd", po::value<int>(),
                     "Non-deterministic mutation phase: number of test cases to mutate.");
  desc.add_options()("white_chars_preservation,w", po::value<int>(), "Preserve white characters during mutations.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.empty() || vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  if (!vm.count("sut") || !vm.count("input_format")) {
    std::cout << "REQUIRED : sut and input_format" << std::endl;
    return 2;
  }

  auto sut = vm["sut"].as<fs::path>();
  auto input_format = vm["input_format"].as<fs::path>();

  auto output = vm.count("out") ? vm["out"].as<fs::path>() : sut.parent_path();
  output /= (fs::path("fuzzon_").concat(sut.filename().c_str())) / time_now_str;

  auto env_flags = vm.count("env_flag") ? vm["env_flag"].as<std::vector<std::string>>() : std::vector<std::string>();
  auto corpus_base = vm.count("corpus_seeds") ? vm["corpus_seeds"].as<fs::path>() : "";
  auto sut_timeout = vm.count("single_test_timeout") ? vm["single_test_timeout"].as<int>() : 1000;
  auto test_timeout = vm.count("total_timeout") ? vm["total_timeout"].as<int>() : 1000 * 60 * 5;
  auto generate = vm.count("generate") ? vm["generate"].as<int>() : 500;
  auto mutate_d = vm.count("mutate_d") ? vm["mutate_d"].as<int>() : 1;
  auto mutate_nd = vm.count("mutate_nd") ? vm["mutate_nd"].as<int>() : 5000;
  auto white_chars_preservation =
      vm.count("white_chars_preservation") ? vm["white_chars_preservation"].as<bool>() : true;

  auto verbose_level = verbosity_values.length();
  Logger::Get(output.string(), verbose_level);

  std::string input_alphabet("abcd");
  fuzzon::Random::Get()->SetAlphabet(input_alphabet);
  fuzzon::Fuzzon crazy_fuzzer(output.string(), sut.string(), std::move(env_flags), sut_timeout, test_timeout);

  //  std::vector<std::string> samples = {"a",     "ab",     "abc",    "abcd",
  //                                      "abcda", "abcdab", "abcdabc"};
  //  for (auto& sample : samples) {
  //    crazy_fuzzer.TestInput(sample);
  //  }
  //  std::vector<std::string> samples = {"0",
  //                                      "1 0",
  //                                      "1 1 1",
  //                                      "1 5 1 1 1 1 1",
  //                                      "1 21 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1",
  //                                      "2 3 1 2 3 21 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1",
  //                                      "2 21 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 21 1 1 1 1 1 1 1 1 1 1 1 "
  //                                      "1 1 1 1 1 1 1 1 1 1"};
  //  for (auto& sample : samples) {
  //    crazy_fuzzer.TestInput(sample);
  //  }

  crazy_fuzzer.ScanCorpus(corpus_base.string());
  crazy_fuzzer.Generation(input_ / format.string(), generate);
  crazy_fuzzer.MutationDeterministic(mutate_d, white_chars_preservation);
  crazy_fuzzer.MutationNonDeterministic(mutate_nd, white_chars_preservation);
  crazy_fuzzer.PrintStats();
  crazy_fuzzer.Dump();
  return 0;
}

#endif

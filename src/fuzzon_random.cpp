/*
 * Copyright [2017] Jacek Weremko
 */

#include "./fuzzon_random.h"

#include <boost/algorithm/string/find.hpp>
#include <ctime>
#include <cstdint>
#include <string>

#include "./utils/logger.h"

namespace fuzzon {

Random::Random() {
  generator_ = boost::random::mt19937(static_cast<std::uint32_t>(std::time(0)));
  alphabets_ = {
      {AlphabetType::SmallLetters, "abcdefghijklmnopqrstuvwxyz"},
      {AlphabetType::CapitalLetters, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
      {AlphabetType::Nums, "1234567890"},
      {AlphabetType::NumsSpecials, "!@#$%^&*()"},
      {AlphabetType::OtherSpecials, "`~-_=+[{]}\\|;:'\",<.>/? "},
  };
  alphabet_ = std::string(alphabets_[AlphabetType::SmallLetters] +
                          alphabets_[AlphabetType::CapitalLetters] +
                          alphabets_[AlphabetType::Nums]);
}

void Random::AddAlphabet(AlphabetType type) {
  alphabet_ += alphabets_[type];
}

void Random::SetAlphabet(std::string alphabet) {
  alphabet_ = alphabet;
}

int Random::GenerateInt(int min, int max) {
  static const int default_min = 1;
  static const int default_max = 3;

  BOOST_ASSERT(min <= max);
  min = min == -1 ? default_min : min;
  max = max == -1 ? default_max : max;

  boost::random::uniform_int_distribution<> min_to_max(min, max);
  return min_to_max(generator_);
}

std::string Random::GenerateString(int length) {
  static const int min_length = 1;
  static const int max_length = 10;

  length = length == -1 ? max_length : length;

  const auto string_length = GenerateInt(min_length, max_length);
  std::stringstream random;
  for (size_t i = 0; i < string_length; i++) {
    random << GenerateChar();
  }
  return random.str();
}

char Random::GenerateChar() {
  return GenerateChar(alphabet_);
}

char Random::GenerateChar(const std::string& alphabet) {
  return alphabet[GenerateInt(0, alphabet.size() - 1)];
}

char Random::GenerateChar(char same_type_as_me) {
  auto alphabet_type = FindCharType(same_type_as_me);
  if (alphabet_type != AlphabetType::Unknown) {
    const auto& alphabet = alphabets_[alphabet_type];
    return GenerateChar(alphabet);
  }
  Logger::Get()->critical("Alphabet not found! Generating random");
  return GenerateChar();
}

Random::AlphabetType Random::FindCharType(char what_is_my_type) {
  for (const auto& alphabet : alphabets_) {
    if (alphabet.second.find(what_is_my_type) != std::string::npos) {
      return alphabet.first;
    }
  }
  return AlphabetType::Unknown;
}

} /* namespace fuzzon */

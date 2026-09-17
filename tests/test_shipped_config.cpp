#include <catch2/catch_test_macros.hpp>
#include "core/Config.h"

#include <filesystem>

using namespace sidecar;

// assets/sidecar.toml rides in the release archive so the first launch is the
// Recommended preset. It is a file in the tree that nothing compiles, which is
// exactly the kind of file that goes stale: a renamed key, or a default that
// moved, and the shipped config would quietly start a fresh install somewhere
// other than where the manager says "Recommended" is.
//
// So the file is read here the way the manager reads it, and compared with the
// defaults the Recommended preset is defined as resetting to.

namespace {
std::filesystem::path ShippedConfig() {
  return std::filesystem::path(SIDECAR_SOURCE_DIR) / "assets" / "sidecar.toml";
}
}  // namespace

TEST_CASE("the shipped sidecar.toml parses without a single warning", "[unit]") {
  std::vector<std::string> warnings;
  const auto loaded = LoadConfig(ShippedConfig(), warnings);
  REQUIRE(loaded.has_value());
  CAPTURE(warnings);
  REQUIRE(warnings.empty());
}

TEST_CASE("the shipped sidecar.toml is the Recommended preset", "[unit]") {
  std::vector<std::string> warnings;
  const auto loaded = LoadConfig(ShippedConfig(), warnings);
  REQUIRE(loaded.has_value());

  // The preset is defined in the manager as "every field back to its default",
  // and MatchingPreset compares on these, so these are what matter.
  const Config defaults;
  CHECK(loaded->neuralPass == defaults.neuralPass);
  CHECK(loaded->dlssPreset == defaults.dlssPreset);
  CHECK(loaded->flowGridSize == defaults.flowGridSize);
  CHECK(loaded->syntheticDepth == defaults.syntheticDepth);
  CHECK(loaded->neuralPasses == defaults.neuralPasses);
  CHECK(loaded->neural.intensity == defaults.neural.intensity);
  CHECK(loaded->neural.colorStrength == defaults.neural.colorStrength);
  CHECK(loaded->neural.transferStrength == defaults.neural.transferStrength);
  CHECK(loaded->neural.paperWhiteScale == defaults.neural.paperWhiteScale);
  CHECK(loaded->neural.preset == defaults.neural.preset);
  CHECK(loaded->neural.style == defaults.neural.style);
  CHECK(loaded->neural.upscaling == defaults.neural.upscaling);
  CHECK(loaded->neural.localStructure == defaults.neural.localStructure);
  CHECK(loaded->neural.localTone == defaults.neural.localTone);
  CHECK(loaded->neural.skinStructure == defaults.neural.skinStructure);

  // And the two things a shipped file must not decide for the person: which
  // language they read, and where their game is.
  CHECK(loaded->language == "en");
  CHECK(loaded->wowDir.empty());
}

#include "../../share/json_helper.hpp"
#include "complex_modifications_assets_file.hpp"
#include <boost/ut.hpp>
#include <iostream>

void run_complex_modifications_assets_file_test(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "lint"_test = [] {
    auto assets_json = krbn::unit_testing::json_helper::load_jsonc("json/lint/assets.jsonc");
    for (const auto& assets_json_entry : assets_json) {
      std::vector<std::string> error_messages;
      try {
        auto file_path = "json/lint/" + assets_json_entry.at("input").get<std::string>();
        error_messages = krbn::complex_modifications_assets_file(file_path,
                                                                 krbn::core_configuration::error_handling::loose)
                             .lint();
      } catch (std::exception& e) {
        error_messages.push_back(e.what());
      }

      expect(error_messages == assets_json_entry.at("errors").get<std::vector<std::string>>());
    }
  };
}

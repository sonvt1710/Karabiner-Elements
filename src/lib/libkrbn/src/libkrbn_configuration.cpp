#include "complex_modifications_utility.hpp"
#include "libkrbn/impl/libkrbn_components_manager.hpp"
#include "libkrbn/impl/libkrbn_configuration_monitor.hpp"
#include "libkrbn/impl/libkrbn_cpp.hpp"

namespace {
auto empty_core_configuration = gsl::make_not_null(std::make_shared<krbn::core_configuration::core_configuration>());
auto empty_device = gsl::make_not_null(std::make_shared<krbn::core_configuration::details::device>());

gsl::not_null<std::shared_ptr<krbn::core_configuration::core_configuration>> get_current_core_configuration(void) {
  if (auto manager = libkrbn_cpp::get_components_manager()) {
    if (auto c = manager->get_current_core_configuration()) {
      return c;
    }
  }

  return empty_core_configuration;
}

krbn::core_configuration::details::simple_modifications* find_simple_modifications(const libkrbn_device_identifiers* device_identifiers) {
  auto di = libkrbn_cpp::make_device_identifiers(device_identifiers);
  auto c = get_current_core_configuration();
  if (di.empty()) {
    return &(c->get_selected_profile().get_simple_modifications());
  } else {
    return c->get_selected_profile().find_simple_modifications(di);
  }
}

krbn::core_configuration::details::simple_modifications* find_fn_function_keys(const libkrbn_device_identifiers* device_identifiers) {
  auto di = libkrbn_cpp::make_device_identifiers(device_identifiers);
  auto c = get_current_core_configuration();
  if (di.empty()) {
    return &(c->get_selected_profile().get_fn_function_keys());
  } else {
    return c->get_selected_profile().find_fn_function_keys(di);
  }
}
} // namespace

bool libkrbn_core_configuration_save(char* error_message_buffer,
                                     size_t error_message_buffer_length) {
  if (error_message_buffer && error_message_buffer_length > 0) {
    error_message_buffer[0] = '\0';
  }

  if (auto manager = libkrbn_cpp::get_components_manager()) {
    if (auto m = manager->get_libkrbn_configuration_monitor()) {
      if (auto c = m->get_weak_core_configuration().lock()) {
        try {
          c->sync_save_to_file();
          return true;
        } catch (const std::exception& e) {
          strlcpy(error_message_buffer, e.what(), error_message_buffer_length);
          return false;
        }
      }
    }
  }

  strlcpy(error_message_buffer, "core_configuration is not ready", error_message_buffer_length);
  return false;
}

bool libkrbn_core_configuration_get_global_configuration_check_for_updates_on_startup(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_check_for_updates_on_startup();
}

void libkrbn_core_configuration_set_global_configuration_check_for_updates_on_startup(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_check_for_updates_on_startup(value);
}

bool libkrbn_core_configuration_get_global_configuration_show_in_menu_bar(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_show_in_menu_bar();
}

void libkrbn_core_configuration_set_global_configuration_show_in_menu_bar(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_show_in_menu_bar(value);
}

bool libkrbn_core_configuration_get_global_configuration_show_profile_name_in_menu_bar(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_show_profile_name_in_menu_bar();
}

void libkrbn_core_configuration_set_global_configuration_show_profile_name_in_menu_bar(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_show_profile_name_in_menu_bar(value);
}

bool libkrbn_core_configuration_get_global_configuration_enable_notification_window(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_enable_notification_window();
}

void libkrbn_core_configuration_set_global_configuration_enable_notification_window(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_enable_notification_window(value);
}

bool libkrbn_core_configuration_get_global_configuration_ask_for_confirmation_before_quitting(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_ask_for_confirmation_before_quitting();
}

void libkrbn_core_configuration_set_global_configuration_ask_for_confirmation_before_quitting(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_ask_for_confirmation_before_quitting(value);
}

bool libkrbn_core_configuration_get_global_configuration_unsafe_ui(void) {
  auto c = get_current_core_configuration();
  return c->get_global_configuration().get_unsafe_ui();
}

void libkrbn_core_configuration_set_global_configuration_unsafe_ui(bool value) {
  auto c = get_current_core_configuration();
  c->get_global_configuration().set_unsafe_ui(value);
}

bool libkrbn_core_configuration_get_machine_specific_enable_multitouch_extension(void) {
  auto c = get_current_core_configuration();
  return c->get_machine_specific().get_entry().get_enable_multitouch_extension();
}

void libkrbn_core_configuration_set_machine_specific_enable_multitouch_extension(bool value) {
  auto c = get_current_core_configuration();
  c->get_machine_specific().get_entry().set_enable_multitouch_extension(value);
}

size_t libkrbn_core_configuration_get_profiles_size(void) {
  auto c = get_current_core_configuration();
  return c->get_profiles().size();
}

bool libkrbn_core_configuration_get_profile_name(size_t index,
                                                 char* buffer,
                                                 size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  const auto& profiles = c->get_profiles();
  if (index < profiles.size()) {
    strlcpy(buffer, profiles[index]->get_name().c_str(), length);
    return true;
  }

  return false;
}

void libkrbn_core_configuration_set_profile_name(size_t index, const char* value) {
  if (value) {
    auto c = get_current_core_configuration();
    c->set_profile_name(index, value);
  }
}

bool libkrbn_core_configuration_get_profile_selected(size_t index) {
  auto c = get_current_core_configuration();
  const auto& profiles = c->get_profiles();
  if (index < profiles.size()) {
    return profiles[index]->get_selected();
  }
  return false;
}

void libkrbn_core_configuration_select_profile(size_t index) {
  auto c = get_current_core_configuration();
  c->select_profile(index);
}

bool libkrbn_core_configuration_get_selected_profile_name(char* buffer,
                                                          size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  strlcpy(buffer, c->get_selected_profile().get_name().c_str(), length);
  return true;
}

void libkrbn_core_configuration_push_back_profile(void) {
  auto c = get_current_core_configuration();
  c->push_back_profile();
}

void libkrbn_core_configuration_duplicate_profile(size_t source_index) {
  auto c = get_current_core_configuration();
  const auto& profiles = c->get_profiles();
  if (source_index < profiles.size()) {
    c->duplicate_profile(*(profiles[source_index]));
  }
}

void libkrbn_core_configuration_move_profile(size_t source_index, size_t destination_index) {
  auto c = get_current_core_configuration();
  c->move_profile(source_index, destination_index);
}

void libkrbn_core_configuration_erase_profile(size_t index) {
  auto c = get_current_core_configuration();
  c->erase_profile(index);
}

int libkrbn_core_configuration_get_selected_profile_parameters_delay_milliseconds_before_open_device(void) {
  auto c = get_current_core_configuration();
  auto count = c->get_selected_profile().get_parameters().get_delay_milliseconds_before_open_device().count();
  return static_cast<int>(count);
}

void libkrbn_core_configuration_set_selected_profile_parameters_delay_milliseconds_before_open_device(int value) {
  auto c = get_current_core_configuration();
  c->get_selected_profile().get_parameters().set_delay_milliseconds_before_open_device(
      std::chrono::milliseconds(value));
}

size_t libkrbn_core_configuration_get_selected_profile_simple_modifications_size(const libkrbn_device_identifiers* device_identifiers) {
  if (auto m = find_simple_modifications(device_identifiers)) {
    return m->get_pairs().size();
  }
  return 0;
}

bool libkrbn_core_configuration_get_selected_profile_simple_modification_from_json_string(size_t index,
                                                                                          const libkrbn_device_identifiers* device_identifiers,
                                                                                          char* buffer,
                                                                                          size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  if (auto m = find_simple_modifications(device_identifiers)) {
    const auto& pairs = m->get_pairs();
    if (index < pairs.size()) {
      strlcpy(buffer, pairs[index].first.c_str(), length);
      return true;
    }
  }

  return false;
}

bool libkrbn_core_configuration_get_selected_profile_simple_modification_to_json_string(size_t index,
                                                                                        const libkrbn_device_identifiers* device_identifiers,
                                                                                        char* buffer,
                                                                                        size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  if (auto m = find_simple_modifications(device_identifiers)) {
    const auto& pairs = m->get_pairs();
    if (index < pairs.size()) {
      strlcpy(buffer, pairs[index].second.c_str(), length);
      return true;
    }
  }
  return false;
}

void libkrbn_core_configuration_replace_selected_profile_simple_modification(size_t index,
                                                                             const char* from_json_string,
                                                                             const char* to_json_string,
                                                                             const libkrbn_device_identifiers* device_identifiers) {
  if (auto m = find_simple_modifications(device_identifiers)) {
    if (from_json_string &&
        to_json_string) {
      m->replace_pair(index, from_json_string, to_json_string);
    }
  }
}

void libkrbn_core_configuration_push_back_selected_profile_simple_modification(const libkrbn_device_identifiers* device_identifiers) {
  if (auto m = find_simple_modifications(device_identifiers)) {
    m->push_back_pair();
  }
}

void libkrbn_core_configuration_erase_selected_profile_simple_modification(size_t index,
                                                                           const libkrbn_device_identifiers* device_identifiers) {
  if (auto m = find_simple_modifications(device_identifiers)) {
    m->erase_pair(index);
  }
}

size_t libkrbn_core_configuration_get_selected_profile_fn_function_keys_size(const libkrbn_device_identifiers* device_identifiers) {
  if (auto k = find_fn_function_keys(device_identifiers)) {
    return k->get_pairs().size();
  }
  return 0;
}

bool libkrbn_core_configuration_get_selected_profile_fn_function_key_from_json_string(size_t index,
                                                                                      const libkrbn_device_identifiers* device_identifiers,
                                                                                      char* buffer,
                                                                                      size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  if (auto k = find_fn_function_keys(device_identifiers)) {
    const auto& pairs = k->get_pairs();
    if (index < pairs.size()) {
      strlcpy(buffer, pairs[index].first.c_str(), length);
      return true;
    }
  }

  return false;
}

bool libkrbn_core_configuration_get_selected_profile_fn_function_key_to_json_string(size_t index,
                                                                                    const libkrbn_device_identifiers* device_identifiers,
                                                                                    char* buffer,
                                                                                    size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  if (auto k = find_fn_function_keys(device_identifiers)) {
    const auto& pairs = k->get_pairs();
    if (index < pairs.size()) {
      strlcpy(buffer, pairs[index].second.c_str(), length);
      return true;
    }
  }

  return false;
}

void libkrbn_core_configuration_replace_selected_profile_fn_function_key(const char* from_json_string,
                                                                         const char* to_json_string,
                                                                         const libkrbn_device_identifiers* device_identifiers) {
  if (auto k = find_fn_function_keys(device_identifiers)) {
    if (from_json_string &&
        to_json_string) {
      k->replace_second(from_json_string, to_json_string);
    }
  }
}

size_t libkrbn_core_configuration_get_selected_profile_complex_modifications_rules_size(void) {
  auto c = get_current_core_configuration();
  return c->get_selected_profile().get_complex_modifications().get_rules().size();
}

void libkrbn_core_configuration_push_front_complex_modifications_rule_to_selected_profile(const krbn::core_configuration::details::complex_modifications_rule& rule) {
  auto c = get_current_core_configuration();
  c->get_selected_profile().push_front_complex_modifications_rule(rule);
}

void libkrbn_core_configuration_erase_selected_profile_complex_modifications_rule(size_t index) {
  auto c = get_current_core_configuration();
  c->get_selected_profile().erase_complex_modifications_rule(index);
}

void libkrbn_core_configuration_move_selected_profile_complex_modifications_rule(size_t source_index, size_t destination_index) {
  auto c = get_current_core_configuration();
  c->get_selected_profile().move_complex_modifications_rule(source_index, destination_index);
}

bool libkrbn_core_configuration_get_selected_profile_complex_modifications_rule_description(size_t index,
                                                                                            char* buffer,
                                                                                            size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  const auto& rules = c->get_selected_profile().get_complex_modifications().get_rules();
  if (index < rules.size()) {
    strlcpy(buffer, rules[index].get_description().c_str(), length);
    return true;
  }

  return false;
}

bool libkrbn_core_configuration_get_selected_profile_complex_modifications_rule_json_string(size_t index,
                                                                                            char* buffer,
                                                                                            size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  const auto& rules = c->get_selected_profile().get_complex_modifications().get_rules();
  if (index < rules.size()) {
    auto json_string = krbn::json_utility::dump(rules[index].get_json());
    // Return false if no enough space.
    if (json_string.length() < length) {
      strlcpy(buffer, json_string.c_str(), length);
      return true;
    }
  }

  return false;
}

void libkrbn_core_configuration_replace_selected_profile_complex_modifications_rule(size_t index,
                                                                                    const char* json_string,
                                                                                    char* error_message_buffer,
                                                                                    size_t error_message_buffer_length) {
  if (error_message_buffer && error_message_buffer_length > 0) {
    error_message_buffer[0] = '\0';
  }

  try {
    auto c = get_current_core_configuration();
    auto&& complex_modifications = c->get_selected_profile().get_complex_modifications();
    krbn::core_configuration::details::complex_modifications_rule rule(
        krbn::json_utility::parse_jsonc(std::string(json_string)),
        complex_modifications.get_parameters());

    auto error_messages = krbn::complex_modifications_utility::lint_rule(rule);
    if (error_messages.size() > 0) {
      std::ostringstream os;
      std::copy(std::begin(error_messages),
                std::end(error_messages),
                std::ostream_iterator<std::string>(os, "\n"));
      strlcpy(error_message_buffer,
              pqrs::string::trim_copy(os.str()).c_str(),
              error_message_buffer_length);
      return;
    }

    complex_modifications.replace_rule(index, rule);

  } catch (const std::exception& e) {
    auto message = fmt::format("error: {0}", e.what());
    strlcpy(error_message_buffer, message.c_str(), error_message_buffer_length);
  }
}

void libkrbn_core_configuration_push_front_selected_profile_complex_modifications_rule(const char* json_string,
                                                                                       char* error_message_buffer,
                                                                                       size_t error_message_buffer_length) {
  if (error_message_buffer && error_message_buffer_length > 0) {
    error_message_buffer[0] = '\0';
  }

  try {
    auto c = get_current_core_configuration();
    auto&& complex_modifications = c->get_selected_profile().get_complex_modifications();
    krbn::core_configuration::details::complex_modifications_rule rule(
        krbn::json_utility::parse_jsonc(std::string(json_string)),
        complex_modifications.get_parameters());

    auto error_messages = krbn::complex_modifications_utility::lint_rule(rule);
    if (error_messages.size() > 0) {
      std::ostringstream os;
      std::copy(std::begin(error_messages),
                std::end(error_messages),
                std::ostream_iterator<std::string>(os, "\n"));
      strlcpy(error_message_buffer,
              pqrs::string::trim_copy(os.str()).c_str(),
              error_message_buffer_length);
      return;
    }

    complex_modifications.push_front_rule(rule);

  } catch (const std::exception& e) {
    auto message = fmt::format("error: {0}", e.what());
    strlcpy(error_message_buffer, message.c_str(), error_message_buffer_length);
  }
}

int libkrbn_core_configuration_get_selected_profile_complex_modifications_parameter(const char* name) {
  if (name) {
    auto c = get_current_core_configuration();
    if (auto value = c->get_selected_profile().get_complex_modifications().get_parameters().get_value(name)) {
      return *value;
    }
  }
  return 0;
}

void libkrbn_core_configuration_set_selected_profile_complex_modifications_parameter(const char* name,
                                                                                     int value) {
  if (name) {
    auto c = get_current_core_configuration();
    c->get_selected_profile().set_complex_modifications_parameter(name, value);
  }
}

void libkrbn_core_configuration_get_new_complex_modifications_rule_json_string(char* buffer,
                                                                               size_t length) {
  auto json_string = krbn::complex_modifications_utility::get_new_rule_json_string();
  strlcpy(buffer, json_string.c_str(), length);
}

uint8_t libkrbn_core_configuration_get_selected_profile_virtual_hid_keyboard_country_code() {
  auto c = get_current_core_configuration();
  return static_cast<uint8_t>(type_safe::get(c->get_selected_profile()
                                                 .get_virtual_hid_keyboard()
                                                 .get_country_code()));
}

void libkrbn_core_configuration_set_selected_profile_virtual_hid_keyboard_country_code(uint8_t value) {
  auto c = get_current_core_configuration();
  c->get_selected_profile()
      .get_virtual_hid_keyboard()
      .set_country_code(pqrs::hid::country_code::value_t(value));
}

int libkrbn_core_configuration_get_selected_profile_virtual_hid_keyboard_mouse_key_xy_scale(void) {
  auto c = get_current_core_configuration();
  return c->get_selected_profile()
      .get_virtual_hid_keyboard()
      .get_mouse_key_xy_scale();
}

void libkrbn_core_configuration_set_selected_profile_virtual_hid_keyboard_mouse_key_xy_scale(int value) {
  auto c = get_current_core_configuration();
  c->get_selected_profile()
      .get_virtual_hid_keyboard()
      .set_mouse_key_xy_scale(value);
}

bool libkrbn_core_configuration_get_selected_profile_virtual_hid_keyboard_indicate_sticky_modifier_keys_state(void) {
  auto c = get_current_core_configuration();
  return c->get_selected_profile()
      .get_virtual_hid_keyboard()
      .get_indicate_sticky_modifier_keys_state();
}

void libkrbn_core_configuration_set_selected_profile_virtual_hid_keyboard_indicate_sticky_modifier_keys_state(bool value) {
  auto c = get_current_core_configuration();
  c->get_selected_profile()
      .get_virtual_hid_keyboard()
      .set_indicate_sticky_modifier_keys_state(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_ignore(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_ignore();
}

void libkrbn_core_configuration_set_selected_profile_device_ignore(const libkrbn_device_identifiers* device_identifiers,
                                                                   bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_ignore(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_manipulate_caps_lock_led(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_manipulate_caps_lock_led();
}

void libkrbn_core_configuration_set_selected_profile_device_manipulate_caps_lock_led(const libkrbn_device_identifiers* device_identifiers,
                                                                                     bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_manipulate_caps_lock_led(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_treat_as_built_in_keyboard(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_treat_as_built_in_keyboard();
}

void libkrbn_core_configuration_set_selected_profile_device_treat_as_built_in_keyboard(const libkrbn_device_identifiers* device_identifiers,
                                                                                       bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_treat_as_built_in_keyboard(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_disable_built_in_keyboard_if_exists(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_disable_built_in_keyboard_if_exists();
}

void libkrbn_core_configuration_set_selected_profile_device_disable_built_in_keyboard_if_exists(const libkrbn_device_identifiers* device_identifiers,
                                                                                                bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_disable_built_in_keyboard_if_exists(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_flip_x(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_flip_x();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_flip_x(const libkrbn_device_identifiers* device_identifiers,
                                                                         bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_flip_x(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_flip_y(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_flip_y();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_flip_y(const libkrbn_device_identifiers* device_identifiers,
                                                                         bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_flip_y(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_flip_vertical_wheel(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_flip_vertical_wheel();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_flip_vertical_wheel(const libkrbn_device_identifiers* device_identifiers,
                                                                                      bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_flip_vertical_wheel(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_flip_horizontal_wheel(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_flip_horizontal_wheel();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_flip_horizontal_wheel(const libkrbn_device_identifiers* device_identifiers,
                                                                                        bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_flip_horizontal_wheel(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_swap_xy(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_swap_xy();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_swap_xy(const libkrbn_device_identifiers* device_identifiers,
                                                                          bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_swap_xy(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_mouse_swap_wheels(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_mouse_swap_wheels();
}

void libkrbn_core_configuration_set_selected_profile_device_mouse_swap_wheels(const libkrbn_device_identifiers* device_identifiers,
                                                                              bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_mouse_swap_wheels(value);
}

bool libkrbn_core_configuration_get_selected_profile_device_game_pad_swap_sticks(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_swap_sticks();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_swap_sticks(const libkrbn_device_identifiers* device_identifiers,
                                                                                 bool value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_swap_sticks(value);
}

// game_pad_xy_stick_continued_movement_absolute_magnitude_threshold

double libkrbn_core_configuration_get_selected_profile_device_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                              double value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold(value);
}

double libkrbn_core_configuration_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_xy_stick_continued_movement_absolute_magnitude_threshold());
}

// game_pad_xy_stick_continued_movement_interval_milliseconds

int libkrbn_core_configuration_get_selected_profile_device_game_pad_xy_stick_continued_movement_interval_milliseconds(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_xy_stick_continued_movement_interval_milliseconds();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_xy_stick_continued_movement_interval_milliseconds(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                       int value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->set_game_pad_xy_stick_continued_movement_interval_milliseconds(value);
}

int libkrbn_core_configuration_game_pad_xy_stick_continued_movement_interval_milliseconds_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_xy_stick_continued_movement_interval_milliseconds());
}

// game_pad_xy_stick_flicking_input_window_milliseconds

int libkrbn_core_configuration_get_selected_profile_device_game_pad_xy_stick_flicking_input_window_milliseconds(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_xy_stick_flicking_input_window_milliseconds();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_xy_stick_flicking_input_window_milliseconds(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                 int value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_xy_stick_flicking_input_window_milliseconds(value);
}

int libkrbn_core_configuration_game_pad_xy_stick_flicking_input_window_milliseconds_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_xy_stick_flicking_input_window_milliseconds());
}

// game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold

double libkrbn_core_configuration_get_selected_profile_device_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                                  double value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold(value);
}

double libkrbn_core_configuration_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_wheels_stick_continued_movement_absolute_magnitude_threshold());
}

// game_pad_wheels_stick_continued_movement_interval_milliseconds

int libkrbn_core_configuration_get_selected_profile_device_game_pad_wheels_stick_continued_movement_interval_milliseconds(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_wheels_stick_continued_movement_interval_milliseconds();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_wheels_stick_continued_movement_interval_milliseconds(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                           int value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_wheels_stick_continued_movement_interval_milliseconds(value);
}

int libkrbn_core_configuration_game_pad_wheels_stick_continued_movement_interval_milliseconds_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_wheels_stick_continued_movement_interval_milliseconds());
}

// game_pad_wheels_stick_flicking_input_window_milliseconds

int libkrbn_core_configuration_get_selected_profile_device_game_pad_wheels_stick_flicking_input_window_milliseconds(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  return d->get_game_pad_wheels_stick_flicking_input_window_milliseconds();
}

void libkrbn_core_configuration_set_selected_profile_device_game_pad_wheels_stick_flicking_input_window_milliseconds(const libkrbn_device_identifiers* device_identifiers,
                                                                                                                     int value) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_wheels_stick_flicking_input_window_milliseconds(value);
}

int libkrbn_core_configuration_game_pad_wheels_stick_flicking_input_window_milliseconds_default_value(void) {
  return empty_device->find_default_value(
      empty_device->get_game_pad_wheels_stick_flicking_input_window_milliseconds());
}

// game_pad_stick_x_formula

bool libkrbn_core_configuration_get_selected_profile_device_game_pad_stick_x_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                     char* buffer,
                                                                                     size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  auto formula = d->get_game_pad_stick_x_formula();
  // Return false if no enough space.
  if (formula.length() < length) {
    strlcpy(buffer, formula.c_str(), length);
    return true;
  }

  return false;
}

bool libkrbn_core_configuration_set_selected_profile_device_game_pad_stick_x_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                     const char* value) {
  if (!krbn::core_configuration::details::device::validate_stick_formula(value)) {
    return false;
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_x_formula(value);

  return true;
}

void libkrbn_core_configuration_reset_selected_profile_device_game_pad_stick_x_formula(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_x_formula(
      d->find_default_value(
          d->get_game_pad_stick_x_formula()));
}

// game_pad_stick_y_formula

bool libkrbn_core_configuration_get_selected_profile_device_game_pad_stick_y_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                     char* buffer,
                                                                                     size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  auto formula = d->get_game_pad_stick_y_formula();
  // Return false if no enough space.
  if (formula.length() < length) {
    strlcpy(buffer, formula.c_str(), length);
    return true;
  }

  return false;
}

bool libkrbn_core_configuration_set_selected_profile_device_game_pad_stick_y_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                     const char* value) {
  if (!krbn::core_configuration::details::device::validate_stick_formula(value)) {
    return false;
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_y_formula(value);

  return true;
}

void libkrbn_core_configuration_unset_selected_profile_device_game_pad_stick_y_formula(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_y_formula(
      d->find_default_value(
          d->get_game_pad_stick_y_formula()));
}

// game_pad_stick_vertical_wheel_formula

bool libkrbn_core_configuration_get_selected_profile_device_game_pad_stick_vertical_wheel_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                                  char* buffer,
                                                                                                  size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  auto formula = d->get_game_pad_stick_vertical_wheel_formula();
  // Return false if no enough space.
  if (formula.length() < length) {
    strlcpy(buffer, formula.c_str(), length);
    return true;
  }

  return false;
}

bool libkrbn_core_configuration_set_selected_profile_device_game_pad_stick_vertical_wheel_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                                  const char* value) {
  if (!krbn::core_configuration::details::device::validate_stick_formula(value)) {
    return false;
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_vertical_wheel_formula(value);

  return true;
}

void libkrbn_core_configuration_unset_selected_profile_device_game_pad_stick_vertical_wheel_formula(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_vertical_wheel_formula(
      d->find_default_value(
          d->get_game_pad_stick_vertical_wheel_formula()));
}

// game_pad_stick_horizontal_wheel_formula

bool libkrbn_core_configuration_get_selected_profile_device_game_pad_stick_horizontal_wheel_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                                    char* buffer,
                                                                                                    size_t length) {
  if (buffer && length > 0) {
    buffer[0] = '\0';
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  auto formula = d->get_game_pad_stick_horizontal_wheel_formula();
  // Return false if no enough space.
  if (formula.length() < length) {
    strlcpy(buffer, formula.c_str(), length);
    return true;
  }

  return false;
}

bool libkrbn_core_configuration_set_selected_profile_device_game_pad_stick_horizontal_wheel_formula(const libkrbn_device_identifiers* device_identifiers,
                                                                                                    const char* value) {
  if (!krbn::core_configuration::details::device::validate_stick_formula(value)) {
    return false;
  }

  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_horizontal_wheel_formula(value);

  return true;
}

void libkrbn_core_configuration_unset_selected_profile_device_game_pad_stick_horizontal_wheel_formula(const libkrbn_device_identifiers* device_identifiers) {
  auto c = get_current_core_configuration();
  auto d = c->get_selected_profile().get_device(libkrbn_cpp::make_device_identifiers(device_identifiers));
  d->set_game_pad_stick_horizontal_wheel_formula(
      d->find_default_value(
          d->get_game_pad_stick_horizontal_wheel_formula()));
}

// game_pad_*

bool libkrbn_core_configuration_game_pad_validate_stick_formula(const char* formula) {
  return krbn::core_configuration::details::device::validate_stick_formula(formula);
}

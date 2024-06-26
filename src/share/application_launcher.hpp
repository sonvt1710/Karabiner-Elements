#pragma once

#include <cstdlib>
#include <spdlog/fmt/fmt.h>
#include <sstream>

namespace krbn {
class application_launcher final {
public:
  static void launch_app_icon_switcher(int number) {
    auto command = fmt::format("'/Library/Application Support/org.pqrs/Karabiner-Elements/Karabiner-AppIconSwitcher.app/Contents/MacOS/Karabiner-AppIconSwitcher' {:03} &", number);
    system(command.c_str());
  }

  static void launch_event_viewer(void) {
    system("open '/Applications/Karabiner-EventViewer.app'");
  }

  static void launch_settings(void) {
    system("open '/Applications/Karabiner-Elements.app'");
  }

  static void killall_settings(void) {
    system("killall Karabiner-Elements");
  }

  static void launch_multitouch_extension(void) {
    system("open '/Library/Application Support/org.pqrs/Karabiner-Elements/Karabiner-MultitouchExtension.app' --args --show-ui");
  }

  static void launch_uninstaller(void) {
    // Use nohup because uninstaller kill the Settings Window.
    system("/usr/bin/nohup osascript '/Library/Application Support/org.pqrs/Karabiner-Elements/scripts/uninstaller.applescript' >/dev/null 2>&1 &");
  }
};
} // namespace krbn

import SwiftUI

struct AdvancedView: View {
    @ObservedObject var settings = Settings.shared

    var body: some View {
        VStack(alignment: .leading, spacing: 12.0) {
            GroupBox(label: Text("Export & Import")) {
                VStack(alignment: .leading, spacing: 12.0) {
                    HStack {
                        Button(action: {
                            let url = URL(fileURLWithPath: String(cString: libkrbn_get_user_configuration_directory()), isDirectory: true)
                            NSWorkspace.shared.open(url)
                        }) {
                            Label("Open config folder (~/.config/karabiner)", systemImage: "arrow.up.forward.app")
                        }

                        Spacer()
                    }
                }
                .padding(6.0)
            }

            GroupBox(label: Text("System default configuration")) {
                VStack(alignment: .leading, spacing: 12.0) {
                    VStack(alignment: .leading, spacing: 0) {
                        Text("You can use Karabiner-Elements even before login by setting the system default configuration.")
                        Text("(These operations require the administrator privilege.)")
                    }

                    Button(action: {
                        settings.installSystemDefaultProfile()
                    }) {
                        Label("Copy the current configuration to the system default configuration", systemImage: "square.and.arrow.down")
                    }

                    if settings.systemDefaultProfileExists {
                        Button(action: {
                            settings.removeSystemDefaultProfile()
                        }) {
                            Label("Remove the system default configuration", systemImage: "trash")
                        }
                    } else {
                        Text("System default configuration is not set.").foregroundColor(Color.primary.opacity(0.5))
                    }
                }
                .padding(6.0)
            }

            Spacer()
        }
        .padding()
    }
}

struct AdvancedView_Previews: PreviewProvider {
    static var previews: some View {
        AdvancedView()
    }
}
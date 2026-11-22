#include "menu_model.hpp"
#include "pad_input.hpp"
#include "ps5_pad_backend.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#if !defined(LICHII_PS5)
#include <chrono>
#include <thread>
#endif

namespace {

void Configure(MenuModel& model) {
    model.AddSetting("Renderer", "Vulkan empfohlen, OpenGL als Fallback.",
                     {"Vulkan", "OpenGL"});
    model.AddSetting("Aufloesung", "Interne Renderaufloesung.",
                     {"0.5x", "1x", "2x", "3x", "4x"}, 1);
    model.AddSetting("Bildrate", "Begrenzt die Spiel-Bildrate.",
                     {"Aus", "30 FPS", "40 FPS", "60 FPS"}, 1);
    model.AddSetting("Frame Generation", "Experimentell, standardmaessig aus.",
                     {"Aus", "MihawkGen", "Optischer Fluss"});
}

void LoadGames(MenuModel& model) {
    std::filesystem::path root = "/data/lichii/roms";
#if !defined(LICHII_PS5)
    if (const char* override_path = std::getenv("LICHII_ROMS")) {
        root = override_path;
    }
#endif
    std::error_code error;
    if (!std::filesystem::is_directory(root, error)) {
        return;
    }
    for (std::filesystem::recursive_directory_iterator it(
             root, std::filesystem::directory_options::skip_permission_denied, error),
         end;
         it != end && !error; it.increment(error)) {
        if (!it->is_regular_file(error)) {
            continue;
        }
        const auto extension = it->path().extension().string();
        if (extension != ".nup" && extension != ".nsp" && extension != ".xci") {
            continue;
        }
        const auto size = it->file_size(error);
        const std::string meta = error
                                     ? extension
                                     : extension + "  " +
                                           std::to_string(size / (1024 * 1024)) + " MB";
        model.AddGame(it->path().string(), it->path().stem().string(), meta);
    }
}

void Draw(const ViewModel& view) {
#if !defined(LICHII_PS5)
    std::cout << "\033[2J\033[H";
    std::cout << "Lichii Modern PS5 - " <<
                 (view.tab == MenuTab::Games ? "Spiele" : "Einstellungen") << '\n';
    if (view.tab == MenuTab::Games) {
        if (view.games.empty()) {
            std::cout << "Keine Spiele in /data/lichii/roms gefunden.\n";
        }
        for (std::size_t i = 0; i < view.games.size(); ++i) {
            std::cout << (i == view.game_index ? " > " : "   ") <<
                         view.games[i].title << "  [" << view.games[i].meta << "]\n";
        }
    } else {
        for (std::size_t i = 0; i < view.settings.size(); ++i) {
            const auto& setting = view.settings[i];
            std::cout << (i == view.setting_index ? " > " : "   ") <<
                         setting.name << ": " << setting.choices[setting.value] << '\n';
        }
    }
    if (!view.message.empty()) {
        std::cout << view.message << '\n';
    }
    std::cout << "Kreis = Start/Zurueck  R1/L1 = Tab  Optionen = Beenden\n";
    std::cout.flush();
#endif
}

}  // namespace

int main() {
    MenuModel model;
    Configure(model);
    LoadGames(model);

#if defined(LICHII_PS5)
    Ps5PadBackend pad;
#else
    NullPadBackend pad;
#endif
    PadInput input(pad);
    if (!input.Initialize()) {
        return 1;
    }

    Draw(model.View());
    while (!model.QuitRequested()) {
        input.Poll();
        UiAction action = input.Consume();
        if (action != UiAction::None) {
            model.Apply(action);
            input.Feedback(action);
            Draw(model.View());
        }
#if !defined(LICHII_PS5)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
#endif
    }

    input.Shutdown();
    return 0;
}

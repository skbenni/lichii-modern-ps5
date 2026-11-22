#pragma once

#include "ui_actions.hpp"

#include <cstddef>
#include <string>
#include <vector>

struct GameEntry {
    std::string path;
    std::string title;
    std::string meta;
};

struct SettingEntry {
    std::string name;
    std::string description;
    std::vector<std::string> choices;
    std::size_t value = 0;
};

enum class MenuTab : std::uint8_t {
    Games,
    Settings,
};

struct ViewModel {
    MenuTab tab = MenuTab::Games;
    std::size_t game_index = 0;
    std::size_t setting_index = 0;
    std::vector<GameEntry> games;
    std::vector<SettingEntry> settings;
    std::string message;
};

class MenuModel {
public:
    void AddGame(std::string path, std::string title, std::string meta);
    void AddSetting(std::string name, std::string description,
                    std::vector<std::string> choices, std::size_t value = 0);

    void Apply(UiAction action);
    const ViewModel& View() const {
        return view_;
    }
    bool QuitRequested() const {
        return quit_;
    }
    std::string SelectedGamePath() const;

private:
    void MoveGame(int delta);
    void MoveSetting(int delta);
    void ChangeSetting(int delta);

    ViewModel view_;
    bool quit_ = false;
};

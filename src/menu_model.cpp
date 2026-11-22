#include "menu_model.hpp"

#include <algorithm>

void MenuModel::AddGame(std::string path, std::string title, std::string meta) {
    if (title.empty()) {
        title = path;
    }
    view_.games.push_back(GameEntry{std::move(path), std::move(title), std::move(meta)});
}

void MenuModel::AddSetting(std::string name, std::string description,
                           std::vector<std::string> choices, std::size_t value) {
    if (choices.empty()) {
        return;
    }
    value = std::min(value, choices.size() - 1);
    view_.settings.push_back(SettingEntry{
        std::move(name), std::move(description), std::move(choices), value});
}

void MenuModel::MoveGame(int delta) {
    if (view_.games.empty()) {
        return;
    }
    const auto count = static_cast<int>(view_.games.size());
    const int current = static_cast<int>(view_.game_index);
    view_.game_index = static_cast<std::size_t>((current + delta % count + count) % count);
}

void MenuModel::MoveSetting(int delta) {
    if (view_.settings.empty()) {
        return;
    }
    const auto count = static_cast<int>(view_.settings.size());
    const int current = static_cast<int>(view_.setting_index);
    view_.setting_index = static_cast<std::size_t>((current + delta % count + count) % count);
}

void MenuModel::ChangeSetting(int delta) {
    if (view_.settings.empty()) {
        return;
    }
    auto& setting = view_.settings[view_.setting_index];
    const auto count = static_cast<int>(setting.choices.size());
    const int current = static_cast<int>(setting.value);
    const int next = current + delta;
    setting.value = static_cast<std::size_t>((next % count + count) % count);
}

void MenuModel::Apply(UiAction action) {
    switch (action) {
        case UiAction::Up:
            if (view_.tab == MenuTab::Games) {
                MoveGame(-1);
            } else {
                MoveSetting(-1);
            }
            break;
        case UiAction::Down:
            if (view_.tab == MenuTab::Games) {
                MoveGame(1);
            } else {
                MoveSetting(1);
            }
            break;
        case UiAction::Left:
            if (view_.tab == MenuTab::Games) {
                MoveGame(-1);
            } else {
                ChangeSetting(-1);
            }
            break;
        case UiAction::Right:
            if (view_.tab == MenuTab::Games) {
                MoveGame(1);
            } else {
                ChangeSetting(1);
            }
            break;
        case UiAction::Confirm:
            if (view_.tab == MenuTab::Games && !view_.games.empty()) {
                view_.message = "Start: " + view_.games[view_.game_index].title;
            } else if (!view_.settings.empty()) {
                view_.message = "Gespeichert: " + view_.settings[view_.setting_index].name;
            }
            break;
        case UiAction::Back:
            if (view_.tab == MenuTab::Settings) {
                view_.tab = MenuTab::Games;
                view_.message.clear();
            } else {
                view_.message = "Zurueck mit Kreis";
            }
            break;
        case UiAction::NextTab:
            view_.tab = view_.tab == MenuTab::Games ? MenuTab::Settings : MenuTab::Games;
            view_.message.clear();
            break;
        case UiAction::PrevTab:
            view_.tab = view_.tab == MenuTab::Games ? MenuTab::Settings : MenuTab::Games;
            view_.message.clear();
            break;
        case UiAction::Quit:
            quit_ = true;
            break;
        case UiAction::None:
            break;
    }
}

std::string MenuModel::SelectedGamePath() const {
    if (view_.games.empty()) {
        return {};
    }
    return view_.games[view_.game_index].path;
}

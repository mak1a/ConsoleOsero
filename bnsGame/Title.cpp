#include "Title.hpp"
#include <iostream>

bnsGame::Title::Title(const InitData& init_)
: IScene(init_) {}

void bnsGame::Title::Update() {
    std::string s;
    std::cin >> s;

    if (s == "y") {
        ChangeScene(bnsGame::Scene::Game);
    }
}

void bnsGame::Title::Draw() const {}

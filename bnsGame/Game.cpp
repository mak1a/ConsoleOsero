#include "Game.hpp"
#include <iostream>

bnsGame::Game::Game(const InitData& init_)
: IScene(init_) {
    std::cout << "�Q�[���V�[��" << std::endl;
}

void bnsGame::Game::Update() {
    std::string s;
    std::cin >> s;

    if (s == "y") {
        ChangeScene(bnsGame::Scene::Title);
    }
}

void bnsGame::Game::Draw() const {}

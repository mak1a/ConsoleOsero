#include "Game.hpp"
#include <iostream>

bnsGame::Game::Game(const InitData& init_)
    : IScene(init_)
    , m_displayBuffer(){
    system("cls");
    for (auto buf : m_displayBuffer) {
        buf = reinterpret_cast<char*>(getData().m_alloc->Alloc(12));
    }
}

void bnsGame::Game::Update() {
    std::string s;
    std::cin >> s;

    if (s == "y") {
        ExitGame();
    }
}

void bnsGame::Game::UpdateChangeOut() {
    for (auto buf : m_displayBuffer) {
        getData().m_alloc->Free(buf);
    }
}

void bnsGame::Game::Draw() const {}

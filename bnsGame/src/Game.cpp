#include "Game.hpp"
#include <iostream>
#include <cstdio>
#include <conio.h>

bnsGame::Game::Game(const InitData& init_)
    : IScene(init_)
    , m_displayBuffer(k_fieldHeight, 0)
    , m_field(k_fieldHeight, 0)
    , m_playerPos(5, 0)
    , m_minoType(MinoType::I) 
    , m_minoAngle(MinoAngle::deg0) {
    ClearDisplay();
    for (uint32_t y{}; y < k_fieldHeight; ++y) {
        m_field[y] = reinterpret_cast<uint32_t*>(getData().m_alloc->Alloc(sizeof(uint32_t) * k_fieldWidth));
        m_displayBuffer[y] = reinterpret_cast<uint32_t*>(getData().m_alloc->Alloc(sizeof(uint32_t) * k_fieldWidth));
    }

    for (uint32_t y{}; y < k_fieldHeight; ++y) {
        for (uint32_t x{}; x < k_fieldWidth; ++x) {
            m_field[y][x] = 0;
            m_displayBuffer[y][x] = 0;
        }
    }

    for (uint32_t y{}; y < k_fieldHeight; ++y) {
        m_field[y][0] = 1;
        m_field[y][k_fieldWidth - 1] = 1;
    }

    for (uint32_t x{}; x < k_fieldWidth; ++x) {
        m_field[k_fieldHeight - 1][x] = 1;
    }

    m_displayBuffer = m_field;

    for (uint32_t y{}; y < 4; ++y) {
        for (uint32_t x{}; x < 4; ++x) {
            m_displayBuffer[m_playerPos.y + y][m_playerPos.x + x] |= k_minoShapes[static_cast<uint32_t>(m_minoType)][m_minoAngle][y][x];
        }
    }
}

void bnsGame::Game::Update() {
    char s = _getch();

    if (s == 'e') {
        ExitGame();
    }

    if (s == 'y') {
        ChangeScene(bnsGame::Scene::Title);
    }
}

void bnsGame::Game::UpdateChangeOut() {
    for (uint32_t y{}; y < k_fieldHeight; ++y) {
        getData().m_alloc->Free(m_displayBuffer[y]);
        getData().m_alloc->Free(m_field[y]);
    }
}

void bnsGame::Game::Draw() const {
    ClearDisplay();
    for (uint32_t y{}; y < k_fieldHeight; ++y) {
        for (uint32_t x{}; x < k_fieldWidth; ++x) {
            std::cout << ((m_displayBuffer[y][x]) ? "¡" : "@");
        }
        std::cout << std::endl;
    }
}

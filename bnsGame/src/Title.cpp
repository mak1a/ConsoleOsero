#include "Title.hpp"
#include <conio.h>

bnsGame::Title::Title(const InitData& init_)
: IScene(init_)
, m_displayBuffer(k_boardHeight){
    //ClearDisplay();
    for (uint32_t y{}; y < k_boardHeight; ++y) {
        m_displayBuffer[y] = reinterpret_cast<int32_t*>(getData().m_alloc->Alloc(sizeof(int32_t) * k_boardWidth));
    }

    for (uint32_t y{}; y < k_boardHeight; ++y) {
        for (uint32_t x{}; x < k_boardWidth; ++x) {
            m_displayBuffer[y][x] = 0;
        }
    }
}

void bnsGame::Title::Update() {
    char s = _getch();

    if (s == 'e') {
        ExitGame();
    }

    if (s == 'y') {
        ChangeScene(bnsGame::Scene::Game);
    }
    if (s == 's') {
        ChangeScene(bnsGame::Scene::Study);
    }
}

void bnsGame::Title::UpdateChangeOut() {
    for (uint32_t y{}; y < k_boardHeight; ++y) {
        getData().m_alloc->Free(m_displayBuffer[y]);
    }
}

void bnsGame::Title::Draw() const {}

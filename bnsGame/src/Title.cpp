#include "Title.hpp"
#include <iostream>
#include <conio.h>

bnsGame::Title::Title(const InitData& init_)
: IScene(init_) {
    ClearDisplay();
}

void bnsGame::Title::Update() {
    char s = _getch();

    if (s == 'e') {
        ExitGame();
    }

    if (s == 'y') {
        ChangeScene(bnsGame::Scene::Game);
    }
}

void bnsGame::Title::Draw() const {}

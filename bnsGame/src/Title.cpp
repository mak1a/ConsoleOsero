#include "Title.hpp"
#include <conio.h>
#include <iostream>

bnsGame::Title::Title(const InitData& init_) : IScene(init_)
, m_displayBuffer(k_boardHeight){
    ClearDisplay();
    for (uint32_t y{}; y < k_boardHeight; ++y) {
        m_displayBuffer[y] = reinterpret_cast<int32_t*>(getData().m_alloc->Alloc(sizeof(int32_t) * k_boardWidth));
    }

    for (uint32_t y{}; y < k_boardHeight; ++y) {
        for (uint32_t x{}; x < k_boardWidth; ++x) {
            m_displayBuffer[y][x] = 0;
        }
    }

    m_displayBuffer[2][4] = 1;
    m_displayBuffer[2][9] = 2;
    m_displayBuffer[2][14] = 1;
    m_displayBuffer[2][15] = 1;
    m_displayBuffer[2][16] = 1;
    m_displayBuffer[2][17] = 1;

    m_displayBuffer[3][2] = 1;
    m_displayBuffer[3][3] = 1;
    m_displayBuffer[3][4] = 1;
    m_displayBuffer[3][5] = 1;
    m_displayBuffer[3][8] = 2;
    m_displayBuffer[3][9] = 2;
    m_displayBuffer[3][10] = 2;
    m_displayBuffer[3][11] = 2;
    m_displayBuffer[3][14] = 1;
    m_displayBuffer[3][17] = 1;

    m_displayBuffer[4][3] = 1;
    m_displayBuffer[4][4] = 1;
    m_displayBuffer[4][9] = 2;
    m_displayBuffer[4][11] = 2;
    m_displayBuffer[4][14] = 1;
    m_displayBuffer[4][17] = 1;

    m_displayBuffer[5][2] = 1;
    m_displayBuffer[5][4] = 1;
    m_displayBuffer[5][9] = 2;
    m_displayBuffer[5][14] = 1;
    m_displayBuffer[5][17] = 1;

    m_displayBuffer[6][1] = 1;
    m_displayBuffer[6][4] = 1;
    m_displayBuffer[6][9] = 2;
    m_displayBuffer[6][10] = 2;
    m_displayBuffer[6][11] = 2;
    m_displayBuffer[6][14] = 1;
    m_displayBuffer[6][15] = 1;
    m_displayBuffer[6][16] = 1;
    m_displayBuffer[6][17] = 1;

    Draw();
}

void bnsGame::Title::Update() {
    char s = _getch();

    if (s == 'e') {
        ExitGame();
        return;
    }

    if (s == 'y') {
        ClearDisplay();
        std::cout << "��Փx��ݒ肵�Ă��������B" << std::endl;
        std::cout << "1�L�[ : ���x��1" << std::endl;
        std::cout << "2�L�[ : ���x��2" << std::endl;
        std::cout << "3�L�[ : ���x��3" << std::endl;

        for (; true;) {
            s = _getch();
            if (s == '1') {
                getData().depth = 1;
                break;
            }
            if (s == '2') {
                getData().depth = 3;
                break;
            }
            if (s == '3') {
                getData().depth = 5;
                break;
            }
        }

        std::cout << "��Ԃ����߂Ă��������B" << std::endl;
        std::cout << "1�L�[ : ��U" << std::endl;
        std::cout << "2�L�[ : ��U" << std::endl;

        for (; true;) {
            s = _getch();
            if (s == '1') {
                getData().turn = Turn::Player;
                break;
            }
            if (s == '2') {
                getData().turn = Turn::Enemy;
                break;
            }
        }

        ChangeScene(bnsGame::Scene::Game);
        return;
    }
    if (s == 's') {
        ChangeScene(bnsGame::Scene::Study);
        return;
    }
}

void bnsGame::Title::UpdateChangeOut() {
    for (uint32_t y{}; y < k_boardHeight; ++y) {
        getData().m_alloc->Free(m_displayBuffer[y]);
    }
}

void bnsGame::Title::DrawColorRect(const Color color_, std::string_view str_) const {
    switch (color_) {
    case Color::Green:
        std::cout << "\x1b[42m" << str_ << "\x1b[49m";
        break;
    case Color::White:
        std::cout << "\x1b[47m" << str_ << "\x1b[49m";
        break;
    case Color::Black:
        std::cout << "\x1b[40m" << str_ << "\x1b[49m";
        break;
    default:
        break;
    }
}

void bnsGame::Title::Draw() const {
    ClearDisplay();

    for (uint32_t y{}; y < k_boardHeight; ++y) {
        for (uint32_t x{}; x < k_boardWidth; ++x) {
            if (m_displayBuffer[y][x] == 1) {
                DrawColorRect(Color::White, "�@");
                continue;
            }
            if (m_displayBuffer[y][x] == 2) {
                DrawColorRect(Color::Black, "�@");
                continue;
            }

            DrawColorRect(Color::Green, "�@");
        }
        std::cout << std::endl;
    }

    std::cout << "\x1b[42m" << "y�L�[ : �Q�[�����[�h�@�@�@�@�@�@�@�@  " << std::endl;
    std::cout << "\x1b[42m" << "s�L�[ : AI�w�K���[�h�@�@�@�@�@�@�@�@  " << std::endl;
    //std::cout << "\x1b[42m" << "c�L�[ : �`���[�g���A�����[�h�@�@�@�@ " << std::endl;
    std::cout << "\x1b[42m" << "e�L�[ : �I������@�@�@�@�@�@�@�@�@�@  " << std::endl;
    std::cout << "\x1b[42m" << "�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  " << std::endl;
    
    std::cout << "\x1b[49m" << std::endl;
}

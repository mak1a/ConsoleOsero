#include "Utility/Allocator/iMemoryAllocator.hpp"
#include "Utility/Allocator/MyAllocator.hpp"
#include "Common.hpp"
#include "Title.hpp"
#include "Game.hpp"
#include "Study.hpp"
#include <Windows.h>

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    PCONSOLE_FONT_INFOEX font = new CONSOLE_FONT_INFOEX();
    font->cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(hConsole, false, font);
    font->dwFontSize.X = 35;
    font->dwFontSize.Y = 35;
    SetCurrentConsoleFontEx(hConsole, false, font);

    constexpr SMALL_RECT rect{0, 0, 36, 15};
    SetConsoleWindowInfo(hConsole, TRUE, &rect);

    constexpr size_t size = 1 * 1024 * 1024;
    void* ptr = new char[size];

    bnsGame::MyScene manager(std::make_shared<bnsGame::GameData>(ptr, size));
    manager.Add<bnsGame::Title>(bnsGame::Scene::Title).Add<bnsGame::Game>(bnsGame::Scene::Game).Add<bnsGame::Study>(bnsGame::Scene::Study);

    while (manager.Update()) {
    }
}

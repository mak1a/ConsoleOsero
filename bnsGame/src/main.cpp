#include "Utility/Allocator/iMemoryAllocator.hpp"
#include "Utility/Allocator/MyAllocator.hpp"
#include "Common.hpp"
#include "Title.hpp"
#include "Game.hpp"

int main() {
    constexpr size_t size = 1 * 1024 * 1024;
    void* ptr = new char[size];

    bnsGame::MyScene manager(std::make_shared<bnsGame::GameData>(ptr, size));
    manager.Add<bnsGame::Title>(bnsGame::Scene::Title).Add<bnsGame::Game>(bnsGame::Scene::Game);

    while (manager.Update()) {
    }
}

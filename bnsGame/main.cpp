#include "iMemoryAllocator.hpp"
#include "MyAllocator.hpp"
#include "Common.hpp"
#include "Title.hpp"
#include "Game.hpp"

int main() {
    /*size_t size = 1 * 1024 * 1024;
    void* ptr = new char[size];
    std::unique_ptr<allocator::iMemoryAllocator> allocate = std::make_unique<allocator::MyAllocator>(ptr, size);*/

    bnsGame::MyScene manager;
    manager.Add<bnsGame::Title>(bnsGame::Scene::Title).Add<bnsGame::Game>(bnsGame::Scene::Game);

    while (manager.Update()) {
    }
}

#pragma once

#include <cstdint>
#include <memory>
#include "Utility/Allocator/iMemoryAllocator.hpp"

namespace allocator {
    class MyAllocator : public iMemoryAllocator {
        using voidPtr_t = void*;

    private:
        size_t m_usableSize;

        struct AllocatedMemory {
            bool m_isUsedMemory;
            size_t m_usedSize;
            AllocatedMemory* m_nextPtr;
            AllocatedMemory* m_prevPtr;

            AllocatedMemory() = default;
            
            void Initialize(const size_t size_, const bool isUsed_ = false, AllocatedMemory* prev_ = nullptr, AllocatedMemory* next_ = nullptr) {
                m_usedSize = size_;
                m_isUsedMemory = isUsed_;
                m_prevPtr = prev_;
                m_nextPtr = next_;
            }
        };

        std::shared_ptr<void> m_buffer;
        AllocatedMemory* m_rootAlloc;

    public:
        MyAllocator(void* ptr_, const size_t size_);
        virtual ~MyAllocator();

        virtual void* Alloc(const size_t size_) override;
        virtual void Free(voidPtr_t ptr_) override;
        // virtual void Deallocate(voidPtr_t ptr_, const size_t size_) override;
    };
}  // namespace allocator

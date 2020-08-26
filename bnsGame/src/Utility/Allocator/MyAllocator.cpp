#include "Utility/Allocator/MyAllocator.hpp"
#include <iostream>

allocator::MyAllocator::MyAllocator(void* ptr_, const size_t size_)
	: m_buffer(ptr_)
	, m_usableSize(size_) {
	m_rootAlloc = reinterpret_cast<AllocatedMemory*>(m_buffer.get());
	m_rootAlloc->Initialize(m_usableSize - sizeof(AllocatedMemory));
}

allocator::MyAllocator::~MyAllocator() {
}

void* allocator::MyAllocator::Alloc(const size_t size_) {
	if (size_ == 0) {
		return nullptr;
	}

	voidPtr_t address = nullptr;
	for (auto node = m_rootAlloc; node != nullptr; node = node->m_nextPtr) {
		if (node->m_isUsedMemory || node->m_usedSize < size_) {
			continue;
		}

		node->m_isUsedMemory = true;
		auto areaAddress = reinterpret_cast<uintptr_t>(node) + sizeof(AllocatedMemory);
		address = reinterpret_cast<voidPtr_t>(areaAddress);

		if (auto restSize = node->m_usedSize - size_; restSize > sizeof(AllocatedMemory)) {
			auto nextAddress = reinterpret_cast<uintptr_t>(node) + sizeof(AllocatedMemory) + size_;
			auto nextPtr = static_cast<AllocatedMemory*>(reinterpret_cast<voidPtr_t>(nextAddress));

			nextPtr->Initialize(restSize - sizeof(AllocatedMemory), false, node, node->m_nextPtr);
			if (node->m_nextPtr != nullptr) {
				node->m_nextPtr->m_prevPtr = nextPtr;
			}

			node->m_usedSize = size_;
			node->m_nextPtr = nextPtr;
		}

		break;
	}

	return address;
}

void allocator::MyAllocator::Free(voidPtr_t ptr_) {
	if (ptr_ == nullptr) {
		return;
	}

	for (auto node = m_rootAlloc; node != nullptr; node = node->m_nextPtr) {
		if (auto areaAddress = reinterpret_cast<uintptr_t>(node) + sizeof(AllocatedMemory);
			(!node->m_isUsedMemory) || (reinterpret_cast<voidPtr_t>(areaAddress) != ptr_)) {
			continue;
		}

		node->m_isUsedMemory = false;

		if (node->m_nextPtr != nullptr && !node->m_nextPtr->m_isUsedMemory) {
			node->m_usedSize += (node->m_nextPtr->m_usedSize + sizeof(AllocatedMemory));
			node->m_nextPtr = node->m_nextPtr->m_nextPtr;

			if (node->m_nextPtr != nullptr) {
				node->m_nextPtr->m_prevPtr = node;
			}
		}

		if (node->m_prevPtr != nullptr && !node->m_prevPtr->m_isUsedMemory) {
			node->m_prevPtr->m_usedSize += (node->m_usedSize + sizeof(AllocatedMemory));
			node->m_prevPtr->m_nextPtr = node->m_nextPtr;
			if (node->m_nextPtr != nullptr) {
				node->m_nextPtr->m_prevPtr = node->m_prevPtr;
			}
		}
	}
}

//void allocator::MyAllocator::Deallocate(voidPtr_t ptr_, const size_t size_) {
//	::operator delete(ptr_, size_);
//	std::cout << "ŠJ•úŠ®—¹" << std::endl;
//}

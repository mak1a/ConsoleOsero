#pragma once

namespace allocator {
	//! メモリアロケータ　インターフェース
	class iMemoryAllocator
	{
	public:
		virtual ~iMemoryAllocator() {}

		virtual void* Alloc(const size_t size) = 0;
		virtual void Free(void* ptr) = 0;
		//virtual void Deallocate(void* ptr, const size_t size) = 0;
	};
}
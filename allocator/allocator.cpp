
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstring>

struct Block
{
	uint8_t* data;
	size_t   size;
};

class Allocator
{
private:

	Block pool;
	std::vector<Block>     freeBlocks;
	std::vector<Block> occupiedBlocks;

public:

	Allocator();
	Allocator(const size_t size);
	~Allocator();

	Allocator(const Allocator& allocator) = delete;
	Allocator(Allocator&& allocator);

	Allocator& operator =(const Allocator& allocator) = delete;
	Allocator& operator =(Allocator&& allocator);

	Block Alloc(const size_t size);
	void Free(const Block block);
};

Allocator::Allocator()
:
	pool { nullptr, 0 },
	freeBlocks { },
	occupiedBlocks { }
{
}

Allocator::Allocator(const size_t size)
:
	pool { static_cast<uint8_t*>(std::malloc(size)), size },
	freeBlocks { pool },
	occupiedBlocks { }
{
	assert(pool.data != nullptr);
}

Allocator::~Allocator()
{
	std::free(pool.data);
}

Allocator::Allocator(Allocator&& allocator)
:
	pool { allocator.pool },
	freeBlocks { std::move(allocator.freeBlocks) },
	occupiedBlocks { std::move(allocator.occupiedBlocks) }
{
	allocator.pool = Block { nullptr, 0 };
	allocator.freeBlocks.clear();
	allocator.occupiedBlocks.clear();
}

Allocator& Allocator::operator =(Allocator&& allocator)
{
	this->pool = allocator.pool;
	this->freeBlocks = std::move(allocator.freeBlocks);
	this->occupiedBlocks = std::move(allocator.occupiedBlocks);

	allocator.pool = Block { nullptr, 0 };
	allocator.freeBlocks.clear();
	allocator.occupiedBlocks.clear();

	return *this;
}

Block Allocator::Alloc(const size_t size)
{
	const auto blockComparerBySize = [](const Block& left, const Block& right)
	{
		return left.size < right.size;
	};

	// find smallest free block
	const auto smallestFreeBlock = std::min_element
	(
		freeBlocks.begin(), freeBlocks.end(), blockComparerBySize
	);

	// remove free block from pool of free blocks
	freeBlocks.erase(smallestFreeBlock);

	// occupy found block
	occupiedBlocks.emplace_back(Block { smallestFreeBlock->data, size });

	if (occupiedBlocks.back().size < smallestFreeBlock->size)
	{
		// return remaining memory to pool of free blocks
		const auto remainingMemory = Block
		{
			smallestFreeBlock->data + size,
			smallestFreeBlock->size - occupiedBlocks.back().size
		};
		freeBlocks.push_back(remainingMemory);
	}

	return occupiedBlocks.back();
}

void Allocator::Free(const Block block)
{
	// invariant: there cannot be two adjacent free blocks

	//  ---+------------+-------------------+--
	// ... | free block | to be freed block | ...
	//  ---+------------+-------------------+--
	const auto isFreeBlockLeftAdjacent = [block](const Block& freeBlock)
	{
		return (freeBlock.data + freeBlock.size) == block.data;
	};

	//  ---+-------------------+------------+--
	// ... | to be freed block | free block | ...
	//  ---+-------------------+------------+--
	const auto isFreeBlockRightAdjacent = [block](const Block& freeBlock)
	{
		return (block.data + block.size) == freeBlock.data;
	};

	const auto blockLeftAdjacent = std::find_if
	(
		freeBlocks.begin(), freeBlocks.end(), isFreeBlockLeftAdjacent
	);
	const auto blockRightAdjacent = std::find_if
	(
		freeBlocks.begin(), freeBlocks.end(), isFreeBlockRightAdjacent
	);

	if (blockLeftAdjacent  != freeBlocks.end() &&
		blockRightAdjacent != freeBlocks.end())
	{
		//   --+------------+-------------------+------------+--
		// ... | free block | to be freed block | free block | ...
		//   --+------------+-------------------+------------+--
		blockLeftAdjacent->size += (blockRightAdjacent->size + block.size);
		freeBlocks.erase(blockRightAdjacent);
	}
	else if (blockLeftAdjacent != freeBlocks.end())
	{
		blockLeftAdjacent->size += block.size;
	}
	else if (blockRightAdjacent != freeBlocks.end())
	{
		blockRightAdjacent->data = block.data;
		blockRightAdjacent->size += block.size;
	}
	else
	{
		freeBlocks.push_back(block);
	}
}

int main()
{
	auto allocator = Allocator { 128 };

	const auto block = allocator.Alloc(32);
	{
		std::memset(block.data, '\0', block.size);
	}
	allocator.Free(block);

	return 0;
}

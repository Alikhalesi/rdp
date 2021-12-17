#pragma once
#include <memory>
#include <vector>
#include <list>
#include <algorithm>

template <typename T>
struct block
{
	block(T* start,unsigned int count)
		:start_{start}, count_{ count },free_{true}
	{
	}
	 T* start_;
	unsigned int count_;
	bool free_;
};
template <typename T>
class ArrayPool;

template <typename T>
class ArrayPoolDeleter
{
public:
	ArrayPoolDeleter()
		:blockContainer_{ ArrayPool<T>::GetInstance().blocks_ }
	{

	}

	void operator()(T* block)
	{
		auto resultBlock = std::find_if(blockContainer_.begin(), blockContainer_.end(),
			[&] (const auto& blockItem)
			{
				return blockItem.start_ == block;
			});

		assert(resultBlock != blockContainer_.end());

		//coalesce block
		CoalesceBlock(resultBlock);



	}
private:
	template <typename Iter>
	void CoalesceBlock(Iter block)
	{
		auto mainBlock = block;
		mainBlock->free_ = true;

		
		block = std::next(block);
		while (block != blockContainer_.end())
		{
			if (block->free_)
			{
				mainBlock->count_ += block->count_;
				auto blockcpy = block;
				block = std::next(block);
				blockContainer_.erase(blockcpy);
			}
			else
			{
				break;
			}
		}
		//
		if (mainBlock != blockContainer_.begin())
		{
			do
			{
				block = std::prev(mainBlock);
				if (block->free_)
				{
					mainBlock->count_ += block->count_;
					mainBlock->start_ = block->start_;
					auto blockcpy = block;
					if (block != blockContainer_.begin())
					{
						block = std::prev(block);
					}
					blockContainer_.erase(blockcpy);
				}
				else
				{
					break;
				}
				
			} while (block != blockContainer_.begin());
			
			
			
			

		}
	}
	std::list<block<T>>& blockContainer_;

};


template <typename T>
class ArrayPool
{
public:
	friend class ArrayPoolDeleter<T>;
	static ArrayPool& GetInstance()
	{
		static ArrayPool instance_{};
		return instance_;
	}
	//========================================================================================================
	ArrayPool()
	{
		data_ = new T[len_];
		blocks_.push_back(block<T>{data_, len_});
	}
	//========================================================================================================
	std::unique_ptr<T, ArrayPoolDeleter<T>> GetArray(unsigned int count)
	{
		auto firstAvailableBlock = std::find_if(blocks_.begin(), blocks_.end(), [&](const block<T>& block)
			{
				return block.free_ && block.count_ >= count;
			});
		if(firstAvailableBlock!=blocks_.end())
		{
				if(firstAvailableBlock->count_> count)
				{
					block<T> firstPart{ firstAvailableBlock->start_,count };
					firstPart.free_ = false;

					block<T> secondPart{ firstAvailableBlock->start_ + count,firstAvailableBlock->count_-count };
				
					blocks_.erase(firstAvailableBlock);
					blocks_.push_back(firstPart);
					blocks_.push_back(secondPart);
					return std::unique_ptr<T, ArrayPoolDeleter<T>>(firstPart.start_);
				}
				else
				{
					return std::unique_ptr<T, ArrayPoolDeleter<T>>{firstAvailableBlock->start_};
				}
		}
		//no block available
		return std::unique_ptr<T, ArrayPoolDeleter<T>>{nullptr};
	}
	//========================================================================================================
	~ArrayPool(){
		delete[] data_;
	}
private:
	T* data_;
	unsigned int len_=20*1024*1024;
	std::list<block<T>> blocks_;
	
	
};


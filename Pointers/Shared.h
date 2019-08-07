#pragma once

#include "../delete/DebugDelete/DebugDelete.h"
#include <iostream>
#include <functional>

template<typename T>
class Shared_ptr;

template<typename T>
auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept
{
	using std::swap;
	swap(lhs.ptr, rhs.ptr);
	swap(lhs.ref_count, rhs.ref_count);
	swap(lhs.deleter, rhs.deleter);
}



template<typename T>
class Shared_ptr
{
public:
	Shared_ptr()
	:ptr(nullptr), ref_count(new size_t(1)), deleter(DebugDelete{}){}
	explicit Shared_ptr(T* raw_ptr)
	:ptr(raw_ptr), ref_count(new size_t(1)), deleter(DebugDelete{}){}
	Shared_ptr(T* raw_ptr, const std::function<void(T*)>& d)
		: ptr(raw_ptr), ref_count(new size_t(1)), deleter(d){}
	Shared_ptr(Shared_ptr const& other) 
		: ptr{ other.ptr }, ref_count{ other.ref_count }, deleter{ other.deleter }
		{
			++* ref_count;
		}
	Shared_ptr(Shared_ptr&& other) noexcept
		: ptr{ other.ptr }, ref_count{ other.ref_count }, deleter{ std::move(other.deleter) }
	{
		other.ptr = nullptr;
		other.ref_count = nullptr;
	}
	Shared_ptr& operator=(const Shared_ptr& rhs){
		++* rhs.ref_count;
		decr_n_dest();
		ptr = rhs.ptr;
		ref_count = rhs.ref_count;
		deleter = rhs.deleter;
		return *this;
	}

	Shared_ptr& operator=(Shared_ptr&& rhs) noexcept
	{
		swap(*this, rhs);
		rhs.decr_n_dest();
		return *this;
	}
	~Shared_ptr() { decr_n_dest(); };

	explicit operator bool() const { return ptr? true : false; }
	T& operator* () const { return *ptr; }
	T* operator->() const { return &*ptr; }

	void reset() { decr_n_dest(); };
	void reset(T* pointer)
	{
		if (ptr != pointer)
		{
			decr_n_dest();
			ptr = pointer;
			ref_count = new std::size_t(1);
		}
	}
	void reset(T* pointer, const std::function<void(T*)>& d)
	{
		reset(pointer);
		deleter = d;
	}
	void swap(Shared_ptr& rhs) noexcept
	{
		swap(*this, rhs);
	}
	
	T* get() const { return ptr; }
	long use_count() const noexcept { return *ref_count; }
	bool unique() const { return 1 == *ref_count; }
private:
	T *ptr;
	std::size_t* ref_count;
	std::function<void(T*)> deleter;

	void decr_n_dest()
	{
		if (ptr && 0 == -- * ref_count)
		{
			delete ref_count, deleter(ptr);
		}
		else if (!ptr)
			delete ref_count;
		ptr = nullptr;
		ref_count = nullptr;
	}
};
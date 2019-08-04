#pragma once

#include "../delete/DebugDelete/DebugDelete.h"
#include <iostream>
#include <functional>

template<typename T>
class Shared_ptr;

template<typename T>
auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept;

template<typename T>
class Shared_ptr
{
	friend  auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept;
public:
	constexpr Shared_ptr() 
	:ptr(nullptr), ref_count(new size_t(1)), deleter(DebugDelete{}){}
	constexpr Shared_ptr(std::nullptr_t) 
	: ptr(nullptr), ref_count(new std::size_t(1)), deleter(DebugDelete{}){}
	explicit Shared_ptr(T* raw_ptr) 
	:ptr(raw_ptr), ref_count(new size_t(1)), deleter(DebugDelete{}){}
	template<class Deleter >
	Shared_ptr(T* raw_ptr, Deleter d)
		: ptr(raw_ptr), ref_count(new size_t(1)), deleter(d){}
	Shared_ptr(Shared_ptr const& other){
		ptr = other.ptr;
		ref_count = other.ref_count;
		deleter = other.deleter;
		++* ref_count;
	}
	Shared_ptr(Shared_ptr && other) noexcept{
		ptr = other.ptr;
		ref_count = other.ref_count;
		deleter = std::move(other.deleter);
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

	Shared_ptr& operator=(Shared_ptr&& rhs) noexcept{
		swap(*this, rhs);
		rhs.decr_n_dest();
		return *this;
	}
	~Shared_ptr() { decr_n_dest(); };

	explicit operator bool() const{ return ptr? true : false; }
	T& operator* () const { return *ptr; }
	T* operator->() const { return &*ptr; }

	void reset() noexcept { decr_n_dest(); };
	void reset(T* raw_ptr)
	{
		if (ptr != raw_ptr)
		{
			decr_n_dest();
			ptr = raw_ptr;
			ref_count = new std::size_t(1);
		}
	}
	template<class Deleter >
	void reset(T* raw_ptr, Deleter d)
	{
		reset(raw_ptr);
		deleter = d;
	}
	//void swap(Shared_ptr& r) noexcept;
	
	T* get() const noexcept { return ptr; }
	long use_count() const noexcept{ return *ref_count; }
	bool unique() const noexcept { return 1 == *ref_count; }
private:
	T *ptr;
	std::size_t* ref_count;
	std::function<void(T*)> deleter;

	void decr_n_dest()
	{
		if (ptr && -- * ref_count)
		
			delete ref_count,
			deleter(ptr);
		
		else if (!ptr)
			delete ref_count;
		ptr = nullptr;
		ref_count = nullptr;
	}
};

template<typename T>
auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept
{
	using std::swap;
	swap(lhs.ptr, rhs.ptr);
	swap(lhs.ref_count, rhs.ref_count);
	swap(lhs.deleter, rhs.deleter);
}
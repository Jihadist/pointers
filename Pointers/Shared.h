#pragma once

#include "../delete/DebugDelete/DebugDelete.h"
#include <iostream>
#include <functional>

template<class T>
class Shared_ptr
{
	friend  auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept;
public:
	constexpr Shared_ptr();
	constexpr Shared_ptr(std::nullptr_t);
	explicit Shared_ptr(T* raw_ptr);
	template<class Deleter >
	Shared_ptr(T* raw_ptr, Deleter d);
	Shared_ptr(Shared_ptr const&);
	Shared_ptr(Shared_ptr &&) noexcept;

	Shared_ptr& operator=(const Shared_ptr&);

	Shared_ptr& operator=(Shared_ptr&&) noexcept;
	~Shared_ptr();

	explicit operator bool() const{ return ptr? true : false; }
	T& operator* () const { return *ptr; }
	T* operator->() const { return &*ptr; }

	void reset() noexcept { decr_n_dest(); };
	void reset(T* raw_ptr);
	template<class Deleter >
	void reset(T* ptr, Deleter d);
	//void swap(Shared_ptr& r) noexcept;
	
	T* get() const noexcept { return ptr; }
	long use_count() const noexcept;
	bool unique() const noexcept { return 1 == *ref_count; }
private:
	T *ptr;
	std::size_t* ref_count;
	std::function<void(T*)> deleter;

	void decr_n_dest();
};



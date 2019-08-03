#pragma once

#include <iostream>

template<typename T>
class Shared_ptr
{
public:
	constexpr Shared_ptr();
	constexpr Shared_ptr(std::nullptr_t);
	explicit Shared_ptr(T* ptr);
	template<class Deleter >
	Shared_ptr(T* ptr, Deleter d);
	~Shared_ptr();

	//Shared_ptr(T *p):ptr(new T(p)){}
	void reset();
	void reset(T* ptr);
	template<class Deleter >
	void reset(T* ptr, Deleter d);
	void swap(Shared_ptr& r) noexcept;
	
	T* get() const { return ptr; }
private:
	T *ptr;
};



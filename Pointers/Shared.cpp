#include "Shared.h"


template<typename T>
constexpr Shared_ptr<T>::Shared_ptr()
:ptr(nullptr),ref_count(new size_t(1)),deleter(DebugDelete{})
{
}

template<typename T>
constexpr Shared_ptr<T>::Shared_ptr(std::nullptr_t): ptr(nullptr), ref_count(new std::size_t(1)), deleter(DebugDelete{})
{
}

template<typename T>
Shared_ptr<T>::Shared_ptr(T* raw_ptr)
:ptr(raw_ptr), ref_count(new size_t(1)), deleter(DebugDelete{})
{
}

template<class T>
Shared_ptr<T>::Shared_ptr(Shared_ptr const& other)
{
	ptr = other.ptr;
	ref_count = other.ref_count;
	deleter = other.deleter;
	++* ref_count;
}

template<class T>
Shared_ptr<T>::Shared_ptr(Shared_ptr&& other)noexcept
{
	ptr = other.ptr;
	ref_count = other.ref_count;
	deleter = std::move(other.deleter);
	other.ptr = nullptr;
	other.ref_count = nullptr;
}

template<class T>
Shared_ptr<T>& Shared_ptr<T>::operator=(const Shared_ptr& rhs)
{
	++* rhs.ref_count;
	decr_n_dest();
	ptr = rhs.ptr;
	ref_count = rhs.ref_count;
	deleter = rhs.deleter;
	return *this;
	// TODO: вставьте здесь оператор return
}



template<class T>
Shared_ptr<T>& Shared_ptr<T>::operator=(Shared_ptr&& rhs) noexcept
{
	swap(*this, rhs);
	rhs.decr_n_dest();
	return *this;
	// TODO: вставьте здесь оператор return
}

template<typename T>
Shared_ptr<T>::~Shared_ptr()
{
	decr_n_dest();
}


template<typename T>
void Shared_ptr<T>::reset(T* raw_ptr)
{
	if(ptr!=raw_ptr)
	{
		decr_n_dest();
		ptr = raw_ptr;
		ref_count = new std::size_t(1);
	}
}

template<class T>
long Shared_ptr<T>::use_count() const noexcept
{
	return *ref_count;
}

template<class T>
void Shared_ptr<T>::decr_n_dest()
{
	if (ptr && -- * ref_count)
	{
		delete ref_count;
		deleter(ptr);
	}
	else if (!ptr)
		delete ref_count;
	ptr = nullptr;
	ref_count = nullptr;

}

template<typename T>
auto swap(Shared_ptr<T>& lhs, Shared_ptr<T>& rhs) noexcept
{
	using std::swap;
	swap(lhs.ptr, rhs.ptr);
	swap(lhs.ref_count, rhs.ref_count);
	swap(lhs.deleter, rhs.deleter);
}


template<typename T>
template<class Deleter>
Shared_ptr<T>::Shared_ptr(T* raw_ptr, Deleter d)
:ptr(raw_ptr), ref_count(new size_t(1)), deleter(d)
{
}

template <typename T>
template <class Deleter>
void Shared_ptr<T>::reset(T* raw_ptr, Deleter d)
{
	reset(raw_ptr);
	deleter = d;
}
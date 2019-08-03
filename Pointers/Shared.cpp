#include "Shared.h"


template<typename T>
constexpr Shared_ptr<T>::Shared_ptr()
{
}

template<typename T>
constexpr Shared_ptr<T>::Shared_ptr(std::nullptr_t)
{
}

template<typename T>
Shared_ptr<T>::Shared_ptr(T* ptr)
{
}

template<typename T>
Shared_ptr<T>::~Shared_ptr()
{
}

template<typename T>
void Shared_ptr<T>::reset()
{
}

template<typename T>
void Shared_ptr<T>::reset(T* ptr)
{
}

template <typename T>
void Shared_ptr<T>::swap(Shared_ptr& r)
{
	std::swap(*this, r);

}

template<typename T>
template<class Deleter>
Shared_ptr<T>::Shared_ptr(T* ptr, Deleter d)
{
}

template <typename T>
template <class Deleter>
void Shared_ptr<T>::reset(T* ptr, Deleter d)
{
}
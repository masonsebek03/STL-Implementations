#pragma once
#include <memory>

#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H

template<typename T, typename Deleter = std::default_delete<T>>
class Unique_ptr {
private:
	T* _data;
	Deleter deleter;

public:
	constexpr Unique_ptr() noexcept;
	explicit Unique_ptr(T* p) noexcept;
	
	// Rule of 5
	~Unique_ptr();
	Unique_ptr(Unique_ptr&& p);
	Unique_ptr(const Unique_ptr& ptr) = delete;
	Unique_ptr<T>& operator=(const Unique_ptr& ptr) = delete;
	Unique_ptr<T>& operator=(Unique_ptr<T>&& ptr);

	T* get() const noexcept;
	Deleter& get_deleter() noexcept;
};

template<typename T, typename Deleter>
Deleter& Unique_ptr<T, Deleter>::get_deleter() noexcept
{
	return _deleter;
}

template<typename T, typename Deleter>
T* Unique_ptr<T, Deleter>::get() const noexcept
{
	return _data;
}

template<typename T, typename Deleter>
constexpr Unique_ptr<T, Deleter>::Unique_ptr() noexcept : _data(nullptr), _deleter(Deleter())
{
}

template<typename T, typename Deleter>
Unique_ptr<T, Deleter>::Unique_ptr(T* p) : _data(p), _deleter(Deleter())
{
}

template<typename T, typename Deleter>
Unique_ptr<T, Deleter>::~Unique_ptr()
{
	if (get() == nullptr) {
		return;
	}

	_deleter(_data);
}

template<typename T, typename Deleter>
Unique_ptr<T, Deleter>::Unique_ptr(Unique_ptr&& p) : _data(p._data), _deleter(std::move(p._deleter))
{
	p._data = nullptr;
}

template<typename T, typename Deleter>
Unique_ptr<T>& Unique_ptr<T, Deleter>::operator=(Unique_ptr<T>&& ptr)
{
	if (this != &ptr) {
		_deleter(_data);
		_data = ptr._data;
		_deleter = std::move(ptr.get_deleter);
		ptr._data = nullptr;
	}
	
	return *this;
}





#endif
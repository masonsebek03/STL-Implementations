#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <new>
#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
private:
	T* _data;
	size_t _size;
	size_t _capacity;

public:
	// default constructor
	Vector();
	Vector(std::initializer_list<T> list);
	Vector(const Vector<T>& other);
	Vector(Vector<T>&& other);
	~Vector();

	T* begin() { return _data; }
	T* end() { return _data + _size; }

	T* erase(T* pos);

	template<typename... Args>
	void emplace_back(Args&&... args);

	void push_back(const T& item);
	void checkCapacity();
	void reserve(size_t count);
	void clear();

	void print();
	void reset();
	void pop_back();
	void print_data();
	

	T& operator[](size_t index);
	Vector<T>& operator=(Vector<T>& other);
	Vector<T>& operator=(Vector<T>&& other);

	size_t size() { return _size; }
	size_t capacity() { return _capacity; }


};

template<typename T, typename Allocator>
template<typename... Args>
void Vector<T, Allocator>::emplace_back(Args&&... args)
{
	checkCapacity();

	new (_data + _size++) T(std::forward<Args>(args)...);
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::print_data()
{
	std::cout << "size = " << size() << ", capacity = " << capacity() << std::endl;
}

template<typename T, typename Allocator>
T* Vector<T, Allocator>::erase(T* pos)
{
	if (_data == nullptr) { return begin(); }

	try {
		pos->~T();

		T* it = pos;
		while (it + 1 != end())
		{
			*it = std::move(*(it + 1));
			it = it + 1;
		}

		(end() - 1)->~T();
		_size = _size - 1;
		return pos;
	}
	catch (const std::out_of_range& ex)
	{
		throw std::out_of_range("Invalid position");
	}
	
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::pop_back()
{
	if (_size == 0) {
		return;
	}

	_data[_size - 1].~T();
	_size = _size - 1;
}


template<typename T, typename Allocator>
void Vector<T, Allocator>::reset()
{
	clear();
	Allocator allocator;
	allocator.deallocate(_data, _capacity);
	_data = nullptr;
	_size = 0;
	_capacity = 0;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::clear()
{
	if (_data != nullptr)
	{
		for (size_t i = 0; i < _size; i++)
		{
			_data[i].~T();
		}
	}

	_size = 0;
}


template<typename T, typename Allocator>
void Vector<T, Allocator>::print()
{
	if (_data != nullptr) {
		for (size_t i = 0; i < _size; i++) {
			std::cout << _data[i] << " ";
		}
		std::cout << "\n";
	}
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector() : _data(nullptr), _size(0), _capacity(0)
{
}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> list)
{
	reserve(list.size());

	for (T val : list) {
		new (_data + _size++) T{ val };
	}


}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T>& other)
{
	reserve(other._size);
	for (size_t i = 0; i < other._size; i++) {
		new (_data + i) T{ other._data[i] };
	}

	_size = other._size;

}

template<typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector<T>&& other) : _data(other._data), _size(other._size), _capacity(other._capacity)
{
	other._data = nullptr;
	other._capacity = 0;
	other._size = 0;
}

template<typename T, typename Allocator>
Vector<T, Allocator>::~Vector()
{

	if (_data == nullptr) { return; }

	reset();

}

template<typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& item)
{
	checkCapacity();

	new (_data + _size++) T{ item };

}

template<typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_t count)
{
	if (count < _capacity) { return; }

	Allocator allocator;
	T* allocatedMemory = allocator.allocate(count);

	if (_data != nullptr)
	{
		for (size_t i = 0; i < _size; i++) {
			new (allocatedMemory + i) T{ _data[i] };
		}

		for (size_t i = 0; i < _size; i++) {
			_data[i].~T();
		}

		allocator.deallocate(_data, _capacity);
	}

	_data = allocatedMemory;
	_capacity = count;
}

template<typename T, typename Allocator>
void Vector<T, Allocator>::checkCapacity()
{
	if (_size == _capacity)
	{
		reserve(_capacity == 0 ? 1 : (_capacity + _capacity / 2));
	}


}

template<typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](size_t index)
{
	if (index >= _size) {
		throw new std::out_of_range("Index is out of range");
	}

	return _data[index];
}

template<typename T, typename Allocator>
Vector<T>& Vector<T, Allocator>::operator=(Vector<T>& other)
{
	if (this == &other) {
		return *this;
	}
	
	if (other._data == nullptr) {
		_data = nullptr;
		_size = 0;
		_capacity = 0;
		return *this;
	}
	
	reset();
	reserve(other._size);

	for (size_t i = 0; i < other._size; i++) {
		new (_data + i) T{ other._data[i] };
	}

	_size = other._size;
	_capacity = other._size;

	return *this;

}
// Vector<T>& operator=(Vector<T>&& other);
template<typename T, typename Allocator>
Vector<T>& Vector<T, Allocator>::operator=(Vector<T>&& other)
{
	// need to deallocate current memory

	if (this == &other) { return *this; }

	if (_data != nullptr) {
		reset();
	}

	_data = other._data;
	_size = other._size;
	_capacity = other._capacity;

	other._data = nullptr;
	other._size = 0;
	other._capacity = 0;

	return *this;

}



#endif

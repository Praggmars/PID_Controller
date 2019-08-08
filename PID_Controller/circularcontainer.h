#pragma once

template <typename TYPE, size_t SIZE>
class CircularContainer
{
	TYPE m_buffer[SIZE];
	size_t m_firstElementIndex;
	size_t m_lastElementIndex;
	size_t m_elementCount;

public:
	CircularContainer() :
		m_firstElementIndex(0),
		m_lastElementIndex(0),
		m_elementCount(0) {}
	size_t Capacity() { return SIZE; }
	size_t Size() { return m_elementCount; }
	TYPE operator[](size_t index) const
	{
		return m_buffer[(m_firstElementIndex + index) % SIZE];
	}
	TYPE& operator[](size_t index)
	{
		return m_buffer[(m_firstElementIndex + index) % SIZE];
	}
	void PushBack(TYPE t)
	{
		m_buffer[m_lastElementIndex] = t;
		m_lastElementIndex = (m_lastElementIndex + 1) % SIZE;
		if (m_elementCount == SIZE)
			m_firstElementIndex = (m_firstElementIndex + 1) % SIZE;
		else
			m_elementCount++;
	}
	void Clear()
	{
		m_firstElementIndex = 0;
		m_lastElementIndex = 0;
		m_elementCount = 0;
	}
};
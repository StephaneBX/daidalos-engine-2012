/*
==============================================================================================================================================================

Author: Stephane Baudoux
Attribution-Noncommercial-Share Alike 3.0 Unported

You are free:
to Share - to copy, distribute and transmit the work
to Remix - to adapt the work

Under the following conditions:
Attribution - You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work).
Noncommercial - You may not use this work for commercial purposes.
Share Alike - If you alter, transform, or build upon this work, you may distribute the resulting work only under the same or similar license to this one.

With the understanding that:
Waiver - Any of the above conditions can be waived if you get permission from the copyright holder.
Public Domain - Where the work or any of its elements is in the public domain under applicable law, that status is in no way affected by the license.
Other Rights - In no way are any of the following rights affected by the license:
Your fair dealing or fair use rights, or other applicable copyright exceptions and limitations;
The author's moral rights;
Rights other persons may have either in the work itself or in how the work is used, such as publicity or privacy rights.
Notice - For any reuse or distribution, you must make clear to others the license terms of this work. The best way to do this is with a link to this web page.

==============================================================================================================================================================
*/

#ifndef __CMEMORYALLOCATOR_H
#define	__CMEMORYALLOCATOR_H

#include <algorithm>
#include "../../Core/Core.h"

namespace DE {
	/**
	* CMemoryAllocator template class.
	* Manages memory allocation.
	* The goal of this class is to give an interface to allocate memory tables.
	* In order to avoid dynamic memory allocation/desallocation, this interface allocate all the necessary memory of a defined type once,
	* wich will be availibe dynamically.
	* This is important to minimize memory errors and optimize the game during the real time rendering.
	*/
	template <typename T> class CMemoryAllocator {
	private:
		int m_usedElements;
		int m_arraySize;
		bool * m_alive;
		int * m_available;
		T * m_array;
		T m_empty;

	public:
		/**
		* Constructor
		*/
		CMemoryAllocator() {
			this->m_alive = nullptr;
			this->m_available = nullptr;
			this->m_array = nullptr;
		}

		/**
		* Constructor
		* @param size The size
		*/
		CMemoryAllocator(const int size) {
			this->m_alive = nullptr;
			this->m_available = nullptr;
			this->m_array = nullptr;

			Allocate(size);
		}

		/**
		* Destructor
		*/
		~CMemoryAllocator() {
			SAFE_DELETE_ARRAY(this->m_alive);
			SAFE_DELETE_ARRAY(this->m_available);
			SAFE_DELETE_ARRAY(this->m_array);
		}

		/**
		* Allocate the memory space
		* @param size The size of the memory
		*/
		void Allocate(const int size) {
			this->m_usedElements = 0;
			this->m_arraySize = size;
			this->m_alive = new bool[this->m_arraySize];
			this->m_available = new int[this->m_arraySize];
			this->m_array = new T[this->m_arraySize];

			for(int i = 0; i < this->m_arraySize; i++) {
				this->m_alive[i] = false;
			}

			for(int i = 0; i < this->m_arraySize; i++) {
				this->m_available[i] = i;
			}
		}

		/**
		* Get a free space in the memory
		* @throw Memory allocation fail if there is not free space available
		* @return A pointer the the memory space
		*/
		T * New() {
			if(GetNumFreeElements() < 1) {
				return nullptr;
			}

			int usedElement = this->m_available[0];
			this->m_alive[this->m_available[0]] = true;
			this->m_array[this->m_available[0]] = m_empty;
			this->m_available[0] = this->m_available[this->m_arraySize - (this->m_usedElements + 1)];
			this->m_usedElements++;

			return &this->m_array[usedElement];
		}

		/**
		* Get The pointer to the element
		* @param index The index
		* @return The pointer the element
		*/
		T * GetAt(const int index) {
			if(index >= 0 && index < this->m_arraySize) {
				return &this->m_array[index];
			} else {
				return nullptr;
			}
		}

		/**
		* Get a copy of the element
		* @param index The index
		* @return A copy of the element
		*/
		T GetElementAt(const int index) {
			if(index >= 0 && index < this->m_arraySize) {
				return this->m_array[index];
			} else {
				return this->m_empty;
			}
		}

		/**
		* Get A reference to the element
		* @param index The index
		* @return A reference to the element
		*/
		T & GetReferenceAt(const int index) {
			if(index >= 0 && index < this->m_arraySize) {
				return this->m_array[index];
			} else {
				return this->m_empty;
			}
		}

		/**
		* Return the array
		* @return The array
		*/
		T * GetArray() {
			return this->m_array;
		}

		/**
		* Return the array size
		* @return The array size
		*/
		const int GetArraySize() const {
			return this->m_arraySize;
		}

		/**
		* Return the number of free elements available
		* @return The number of free elements available
		*/
		const int GetNumFreeElements() const {
			return this->m_arraySize - this->m_usedElements;
		}

		/**
		* Return the number of used elements
		* @return The number of used elements
		*/
		const int GetNumUsedElements() const {
			return this->m_usedElements;
		}

		/**
		* Check if an element is alive
		* @param index The index to test
		* @return The result
		*/
		const bool IsAlive(const int index) const {
			return this->m_alive[index];
		}

		/**
		* Replace an element
		* @param element A new element
		* @param index The index to replace
		* @param alive The alive state
		*/
		void SetAt(const T element, const int index, const bool alive) {
			this->m_array[index] = element;
			this->m_alive[index] = alive;
		}

		/**
		* Swap two elements
		* @param a Index of the first element
		* @param b Index of the second element
		*/
		void Swap(const int a, const int b) {
			std::swap(this->m_array[a], this->m_array[b]);
			std::swap(this->m_alive[a], this->m_alive[b]);
		}

		/**
		* Delete an element
		* @param index The index to delete
		*/
		void Delete(const int index) {
			if(index >= 0 && index < this->m_arraySize && IsAlive(index)) {
				this->m_alive[index] = false;
				this->m_usedElements--;
				this->m_available[this->m_arraySize - (this->m_usedElements + 1)] = index;
			}
		}

		/**
		* Delete an element
		* @param element A point to the element to delete
		*/
		void Delete(T * const element) {
			bool found = false;
			for(int i = 0; i < this->m_arraySize && !found; i++) {
				if((int)(this->m_array + i) == (int)element) {
					Delete(i);
					found = true;
				}
			}
		}

		/**
		* Delete all the elements
		*/
		void DeleteAll() {
			for(int i = 0; i < this->m_arraySize; i++) {
				Delete(i);
			}
		}
	};
}

#endif

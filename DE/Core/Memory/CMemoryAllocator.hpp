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

#ifndef __CMEMORYALLOCATOR_HPP
#define	__CMEMORYALLOCATOR_HPP

#include <algorithm>
#include "../../Core/Core.hpp"

namespace DE {
	/**
	* CMemoryAllocator template class.
	* Manages memory allocation.
	* The goal of this class is to give an interface to allocate memory tables.
	* In order to avoid dynamic memory allocation/desallocation, this interface allocate all the necessary memory of a defined type once,
	* wich will be available dynamically.
	* This is important to minimize memory errors and optimize the game during real time rendering.
	*/
	template <typename T> class CMemoryAllocator {
	private:
		int m_count;
		int m_size;
		int * m_available;
		T * m_data;
		T m_empty;

	public:
		/**
		* Constructor
		*/
		CMemoryAllocator() {
			this->m_count = 0;
			this->m_size = 0;
			this->m_available = nullptr;
			this->m_data = nullptr;
		}

		/**
		* Constructor
		* @param size The size
		*/
		CMemoryAllocator(const int size) {
			this->m_available = nullptr;
			this->m_data = nullptr;

			Allocate(size);
		}

		/**
		* Destructor
		*/
		~CMemoryAllocator() {
			SAFE_DELETE_ARRAY(this->m_available);
			SAFE_DELETE_ARRAY(this->m_data);
		}

		/**
		* Allocate the memory space
		* @param size The size of the memory
		*/
		void Allocate(const int size) {
			this->m_count = 0;
			this->m_size = size;
			this->m_available = new int[size];
			this->m_data = new T[size];

			for(int i = 0; i < this->m_size; i++) {
				this->m_available[i] = i;
			}
		}

		/**
		* Get a free space in the memory
		* @throw Memory allocation fail if there is not free space available
		* @return the index of the allocated element
		*/
		int New() {
			if(GetCount() >= GetSize()) {
				return -1;
			}

			this->m_data[this->m_available[this->m_count]] = this->m_empty;
			this->m_count++;

			return this->m_available[this->m_count - 1];
		}

		/**
		* Get The pointer to the element
		* @param index The index
		* @return The pointer the element
		*/
		T * GetAt(const int index) {
			if(index >= 0 && index < this->m_size) {
				return &this->m_data[index];
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
			if(index >= 0 && index < this->m_size) {
				return this->m_data[index];
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
			if(index >= 0 && index < this->m_size) {
				return this->m_data[index];
			} else {
				return this->m_empty;
			}
		}

		/**
		* Return the array size
		* @return The array size
		*/
		const int GetSize() const {
			return this->m_size;
		}

		/**
		* Return the number of used elements
		* @return The number of used elements
		*/
		const int GetCount() const {
			return this->m_count;
		}

		/**
		* Swap two elements
		* @param a Index of the first element
		* @param b Index of the second element
		*/
		void Swap(const int a, const int b) {
			std::swap(this->m_available[a], this->m_available[b]);
		}

		/**
		* Delete an element
		* @param index The index to delete
		*/
		void Delete(const int index) {

			if(index >= 0 && index < this->m_count) {

				bool found = false;
				for(int i = 0; i < this->m_count && !found; i++) {
					if(this->m_available[i] == index) {
						Swap(i, this->m_count - 1);
						this->m_count--;
						found = true;
					}
				}
			}
		}

		/**
		* Delete all the elements
		*/
		void DeleteAll() {
			this->m_count = 0;
		}
	};
}

#endif

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

#ifndef __CARRAYSORTER_H
#define	__CARRAYSORTER_H

#include <algorithm>

namespace DE {

	enum EArraySorterOrder {
		ARRAY_SORTER_ORDER_ASC,
		ARRAY_SORTER_ORDER_DESC
	};

	/**
	* CArraySorter template class.
	* Manages array sorting algorithms.
	*/
	template <typename T> class CArraySorter {
	private:
		int m_temp;
		int * m_order;
		T m_temp2;
		T * m_arrayList;

	public:

		/**
		* Constructor
		*/
		CArraySorter() {
			this->m_order = nullptr;
			this->m_arrayList = nullptr;
		}

		/**
		* Destructor
		*/
		~CArraySorter() {}

		/**
		* BubbleSort algorithm
		* @param arrayList The data array
		* @param order The order array
		* @param arraySize The data array size
		*/
		void BubbleSort(T * const arrayList, int * const order, const int arraySize, EArraySorterOrder sort) {
			this->m_arrayList = arrayList;
			this->m_order = order;
			BubbleSortPerform(arraySize);
		}

		/**
		* Quick Sort algorithm
		* @param arrayList The data array
		* @param order The order array
		* @param first The starting index
		* @param last The last index
		*/
		void QuickSort(T * const arrayList, int * const order, const int first, const int last, EArraySorterOrder sort) {
			this->m_arrayList = arrayList;
			this->m_order = order;
			QuickSortPerform(first, last, sort);
		}

	private:

		/**
		* Perform the bubble sort
		* @param arraySize The data size
		*/
		void BubbleSortPerform(const int arraySize, EArraySorterOrder sort) {
			bool toDo = true;

			for(int i = 0; i < arraySize && toDo; i++) {
				toDo = false;
				for(int j = 0; j < arraySize - 1 - i; j++) {
					if((sort == ARRAY_SORTER_ORDER_ASC && this->m_arrayList[j] > this->m_arrayList[j + 1]) || (sort == ARRAY_SORTER_ORDER_DESC && this->m_arrayList[j] < this->m_arrayList[j + 1])) {
						Swap(j, j + 1);
						toDo = true;
					}
				}
			}
		}

		/**
		* Perform a Quick Sort between two indexes
		* @param first The first index
		* @param last The last index
		*/
		void QuickSortPerform(const int first, const int last, EArraySorterOrder sort) {
			int pivot;
			if(first < last) {
				pivot = QuickSortPivotSelection(first, last);
				pivot = QuickSortPartition(first, last, pivot, sort);
				QuickSortPerform(first, pivot - 1, sort);
				QuickSortPerform(pivot + 1, last, sort);
			}
		}

		/**
		* Swap two values
		* @param a the first value
		* @param b The second value
		*/
		void Swap(const int a, const int b) {
			std::swap(this->m_arrayList[a], this->m_arrayList[b]);

			if(this->m_order != nullptr) {
				std::swap(this->m_order[a], this->m_order[b]);
			}
		}

		/**
		* Sort the data by using the pivot
		* @param first The first index
		* @param last The last index
		* @param pivot The pivot
		*/
		const int QuickSortPartition(const int first, const int last, const int pivot, EArraySorterOrder sort) {
			int j;
			Swap(pivot, last);
			j = first;
			for(int i = first; i < last; i++) {
				if((sort == ARRAY_SORTER_ORDER_ASC && this->m_arrayList[i] < this->m_arrayList[last]) || (sort == ARRAY_SORTER_ORDER_DESC && this->m_arrayList[i] > this->m_arrayList[last])) {
					Swap(i, j);
					j++;
				}
			}
			Swap(last, j);
			return j;
		}

		/**
		* Select the pivot for the Quick Sort algorithm
		* @param first The first value
		* @param last The last value
		*/
		const int QuickSortPivotSelection(const int first, const int last) {
			return first;
		}
	};
}

#endif

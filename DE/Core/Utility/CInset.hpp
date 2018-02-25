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

#ifndef __CINSET_HPP
#define __CINSET_HPP

namespace daidalosengine {
	/**
	* Inset template class
	* Handle an inset
	*/
	template <class T> class CInset {
	private:
		T m_left;
		T m_right;
		T m_top;
		T m_bottom;

	public:
		CInset(const T left = 0, const T top = 0, const T right = 0, const T bottom = 0);
		const T GetTop() const;
		const T GetLeft() const;
		const T GetBottom() const;
		const T GetRight() const;
		void SetLeft(const T left);
		void SetTop(const T top);
		void SetRight(const T right);
		void SetBottom(const T bottom);
		const CInset & operator=(const CInset & copy);
	};

	/**
	* Constructor
	* @param left The left inset
	* @param top The top inset
	* @param right The right inset
	* @param bottom The bottom inset
	*/
	template <class T> inline CInset<T>::CInset(const T left, const T top, const T right, const T bottom) {
		this->m_left = left;
		this->m_top = top;
		this->m_right = right;
		this->m_bottom = bottom;
	}

	/**
	* Get the top inset
	* @return The top inset
	*/
	template <class T> inline const T CInset<T>::GetTop() const {
		return this->m_top;
	}

	/**
	* Get the left inset
	* @return The left inset
	*/
	template <class T> inline const T CInset<T>::GetLeft() const {
		return this->m_left;
	}

	/**
	* Get the bottom inset
	* @return The bottom inset
	*/
	template <class T> inline const T CInset<T>::GetBottom() const {
		return this->m_bottom;
	}

	/**
	* Get the right inset
	* @return The right inset
	*/
	template <class T> inline const T CInset<T>::GetRight() const {
		return this->m_right;
	}

	/**
	* Set the left inset
	* @param left The left inset
	*/
	template <class T> inline void CInset<T>::SetLeft(const T left) {
		this->m_left = left;
	}

	/**
	* Set the top inset
	* @param top The top inset
	*/
	template <class T> inline void CInset<T>::SetTop(const T top) {
		this->m_top = top;
	}

	/**
	* Set the right inset
	* @param right The right inset
	*/
	template <class T> inline void CInset<T>::SetRight(const T right) {
		this->m_right = right;
	}

	/**
	* Set the bottom inset
	* @param bottom The bottom inset
	*/
	template <class T> inline void CInset<T>::SetBottom(const T bottom) {
		this->m_bottom = bottom;
	}

	/**
	* = operator override
	* @param copy A CGUIText element copy
	* @return The CGUIText
	*/
	template <class T> const CInset<T> & CInset<T>::operator=(const CInset<T> & copy) {
		this->m_left = copy.GetLeft();
		this->m_top = copy.GetTop();
		this->m_right = copy.GetRight();
		this->m_bottom = copy.GetBottom();
		return *this;
	}

	typedef CInset<int> CInsetI;
	typedef CInset<float> CInsetF;
}

#endif

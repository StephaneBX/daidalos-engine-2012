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

#ifndef __CTIMER_HPP
#define __CTIMER_HPP

#include <Windows.h>

namespace daidalosengine {
	/**
	* CTimer class.
	* Manages time.
	*/
	class CTimer {
	private:
		double m_start;
		double m_stop;
		double m_corr;
		double m_freq;
		double m_last;

	public:

		/**
		* Constructor
		*/
		CTimer() {
			LARGE_INTEGER timer;
			this->m_corr = 0.0;
			QueryPerformanceFrequency(&timer);
			this->m_freq = double(timer.QuadPart);
			this->m_start = Get();
			this->m_stop = Get();
			this->m_corr = this->m_stop - this->m_start;
			this->m_last = Get();
		};

		/**
		* Get the current time
		* @param pt A point to save the time into
		* @return The current time
		*/
		double Get(double * pt = nullptr) {
			LARGE_INTEGER timer;
			QueryPerformanceCounter(&timer);
			double t = (double)timer.QuadPart / this->m_freq;
			if(pt != nullptr) *pt = t;
			return t;
		};

		/**
		* Get the elapsed time
		* @return The elapsed time
		*/
		double Elapsed() {
			double tcur = Get();
			return tcur - this->m_last - this->m_corr;
		};

		/**
		* Get the delta
		* @return The delta
		*/
		double Delta() {
			double tsave = this->m_last;
			this->m_last = Get();
			return this->m_last - tsave - this->m_corr;
		};
	};
}

#endif

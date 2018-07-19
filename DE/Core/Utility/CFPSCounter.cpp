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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/Utility/CFPSCounter.hpp"

namespace daidalosengine {
	/**
	* Constructor
	*/
	CFPSCounter::CFPSCounter() {
		LOG_TRACE();
		Initialize();
	}

	/**
	* Destructor
	*/
	CFPSCounter::~CFPSCounter() {
		LOG_TRACE();
	}

	/**
	* Initialize the manager
	*/
	void CFPSCounter::Initialize() {
		LOG_TRACE();
		Reset();
	}

	/**
	* Update the manager
	* @param time Elapsed time
	*/
	void CFPSCounter::Update(const float time) {
		LOG_TRACE();

		this->m_frameCount++;
		this->m_frameCountRate++;
		this->m_executionTime += time;
		this->m_fpsUpdateRateTimer += time;

		if(this->m_fpsUpdateRateTimer >= FPS_UPDATE_RATE) {
			this->m_fps = (int)(this->m_frameCountRate / (this->m_fpsUpdateRateTimer / 1000.0F));
			this->m_fpsUpdateRateTimer = 0;
			this->m_frameCountRate = 0;
			this->m_fpsStream.str("");
			this->m_fpsStream << this->m_fps;
		}
	}

	/**
	* Reset the fps counter
	*/
	void CFPSCounter::Reset() {
		LOG_TRACE();

		this->m_frameCount = 0;
		this->m_frameCountRate = 0;
		this->m_executionTime = 0;
		this->m_fpsUpdateRateTimer = 0;
		this->m_fpsStream << "0";
	}

	/**
	* Get the current fps
	* @return The current fps
	*/
	int CFPSCounter::GetFPS() const {
		LOG_TRACE();
		return this->m_fps;
	}

	/**
	* Get the current fps as a string
	* @return The current fps string
	*/
	std::string CFPSCounter::GetText() const {
		LOG_TRACE();
		return this->m_fpsStream.str();
	}
}

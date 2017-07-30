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

#ifndef __ISOUNDBASE_HPP
#define __ISOUNDBASE_HPP

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "../Core/Enums.hpp"
#include "../Core/Utility/CVector3.hpp"
#include "../Core/IResource.hpp"

namespace DE {
	/**
	* Basic class for sounds
	*/
	class ISoundBase : public IResource {
	protected:
		ALuint m_source;
		ESoundFormat m_format;
		int m_samplesCount;
		int m_sampleRate;

	public:
		/**
		* Destructor
		*/
		virtual ~ISoundBase();

		/**
		* Set the sound status
		* @param status The status
		*/
		virtual void SetStatus(ESoundStatus status);

		/**
		* Set the gain (volume)
		* @param gain The gain between 0.0 and 1.0
		*/
		void SetGain(const float gain);

		/**
		* Set the position
		* @param position The position
		*/
		void SetPosition(const CVector3F & position);

		/**
		* Set the orientation
		* @param orientation A 6 float array for the at and the up vectors
		*/
		void SetOrientation(float * orientation);

		/**
		* Set the velocity
		* @param velocity The velocity
		*/
		void SetVelocity(const CVector3F & velocity);

		/**
		* Set the cone inner angle
		* @param angle The cone angle
		*/
		void SetConeInnerAngle(const float angle);

		/**
		* Set the cone outer angle
		* @param angle The cone angle
		*/
		void SetConeOuterAngle(const float angle);

		/**
		* Set the distance from wich the gain will be maximal
		* @param distance The distance
		*/
		void SetReferenceDistance(const float distance);

		/**
		* Set the distance from wich the gain will be minimal
		* @param distance The distance
		*/
		void SetMaxDistance(const float distance);

		/**
		* Set the position relative to the listener
		* @param relative The relative value
		*/
		void SetRelativeToListener(const bool relative);

		/**
		* Set the sound as looped
		* @param loop The loop value
		*/
		virtual void SetLooped(const bool loop);

		/**
		* Update a sound
		*/
		virtual void Update();

		/**
		* Get the format
		* @return The format
		*/
		ESoundFormat GetFormat() const;

		/**
		* Get the status
		* @return The status
		*/
		ESoundStatus GetStatus() const;

		/**
		* Check if the position is relative to the listener
		* @return The relative position state
		*/
		const bool IsRelativeToListener() const;

		/**
		* Get the looped status
		* @return The looped status
		*/
		virtual const bool IsLooped() const;

		/**
		* Get the number of channels
		* @return The number of channels
		*/
		const int GetChannelsCount() const;

		/**
		* Get the gain
		* @return The gain
		*/
		const float GetGain() const;

		/**
		* Get the cone inner angle
		* @return The cone inner angle
		*/
		const float GetConeInnerAngle() const;

		/**
		* Get the cone outer angle
		* @return The cone outer angle
		*/
		const float GetConeOuterAngle() const;

		/**
		* Get the distance from wich the gain will be maximal
		* @return The distance from wich the gain will be maximal
		*/
		const float GetReferenceDistance() const;

		/**
		* Get the distance from wich the gain will be minimal
		* @return The distance from wich the gain will be minimal
		*/
		const float GetMaxDistance() const;

	protected:
		/**
		* Constructor
		*/
		ISoundBase(const std::string & name);

		/**
		* Get the OpenAL sound format
		* @return The format
		*/
		ALenum GetALFormat() const;

	private:
		ISoundBase(const ISoundBase & copy);
		const ISoundBase & operator=(const ISoundBase & copy);
	};
}

#endif

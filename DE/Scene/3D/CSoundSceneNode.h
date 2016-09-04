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

#ifndef __CSOUNDSCENENODE_H
#define __CSOUNDSCENENODE_H

#include "../../Core/Enums.h"
#include "../../Medias/CSound.h"
#include "../../Scene/3D/ISceneNode.h"

namespace DE {
	/**
	* Handle a sound node
	*/
	class CSoundSceneNode : public ISceneNode {
	protected:
		bool m_autoStart;
		CSound m_sound;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param filename The sound filename
		* @param type The sound type
		* @param parent The node parent
		* @param name The node name
		*/
		CSoundSceneNode(I3DScene * scene, const std::string & filename, ESoundType type, ISceneNode * parent = nullptr, const std::string & name = "");

		/**
		* Destructor
		*/
		~CSoundSceneNode();

		/**
		* Update the node
		* @param time The elapsed time
		*/
		void Update(const float time = 0);

		/**
		* Set the sound status
		* @param status The status
		*/
		void SetSoundStatus(ESoundStatus status);

		/**
		* Set the gain (volume)
		* @param gain The gain between 0.0 and 1.0
		*/
		void SetSoundGain(const float gain);

		/**
		* Set the velocity
		* @param velocity The velocity
		*/
		void SetSoundVelocity(const CVector3F & velocity);

		/**
		* Set the cone inner angle
		* @param angle The cone angle
		*/
		void SetSoundConeInnerAngle(const float angle);

		/**
		* Set the cone outer angle
		* @param angle The cone angle
		*/
		void SetSoundConeOuterAngle(const float angle);

		/**
		* Set the distance from wich the gain will be maximal
		* @param distance The distance
		*/
		void SetSoundReferenceDistance(const float distance);

		/**
		* Set the distance from wich the gain will be minimal
		* @param distance The distance
		*/
		void SetSoundMaxDistance(const float distance);

		/**
		* Set the position relative to the listener
		* @param relative The relative value
		*/
		void SetSoundRelativeToListener(const bool relative);

		/**
		* Set the sound as looped
		* @param loop The loop value
		*/
		void SetSoundLooped(const bool loop);

		/**
		* Set if the sound play on startup
		* @param The auto start value
		*/
		void SetSoundAutoStart(const bool autoStart);

		/**
		* Get the format
		* @return The format
		*/
		ESoundFormat GetSoundFormat() const;

		/**
		* Get the status
		* @return The status
		*/
		ESoundStatus GetSoundStatus() const;

		/**
		* Get the sound type
		* @return The sound type
		*/
		ESoundType GetSoundType() const;

		/**
		* Get the looped status
		* @return The looped status
		*/
		const bool IsSoundLooped() const;

		/**
		* Get if the position relative to the listener
		* @return If the position relative to the listener
		*/
		const bool IsSoundRelativeToListener() const;

		/**
		* Check if the sound should play on startup
		* @return If the sound should play on startup
		*/
		const bool IsSoundAutoStart() const;

		/**
		* Get the number of channels
		* @return The number of channels
		*/
		const int GetSoundChannelsCount() const;

		/**
		* Get the soudn gain
		* @return The sound gain
		*/
		const float GetSoundGain() const;

		/**
		* Get the cone inner angle
		* @return The cone inner angle
		*/
		const float GetSoundConeInnerAngle() const;

		/**
		* Get the cone outer angle
		* @return The cone inner angle
		*/
		const float GetSoundConeOuterAngle() const;

		/**
		* Get the distance from wich the gain will be maximal
		* @return The distance from wich the gain will be maximal
		*/
		const float GetSoundReferenceDistance() const;

		/**
		* Get the distance from wich the gain will be minimal
		* @return The distance from wich the gain will be minimal
		*/
		const float GetSoundMaxDistance() const;

		/**
		* Get the sound name
		* @return The sound name
		*/
		const std::string GetSoundName() const;
	};
}

#endif

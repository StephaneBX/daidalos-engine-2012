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

#ifndef __CPARTICLEENTITY_H
#define __CPARTICLEENTITY_H

#include "../../Core/Utility/CVector3.h"
#include "../../Core/Utility/CVector2.h"
#include "../../Core/Utility/CColor.h"

namespace DE {
	/**
	* The particle data structure for the shaders
	*/
	struct SParticleData {
		CVector4F positionRotation;
		CVector4F m_color;
		CVector2F sizeTextureId;
	};

	/**
	* A particle entity
	*/
	class CParticleEntity {
	protected:
		bool m_visible;
		float m_life;
		float m_lifeMax;
		float m_sizeMin;
		float m_sizeMax;
		float m_rotationSpeed;
		float m_textureId;
		CColor m_color;
		CColor m_colorMin;
		CColor m_colorMax;
		CVector3F m_position;
		CVector3F m_velocity;
		SParticleData m_data;

	public:
		/**
		* The constructor
		*/
		CParticleEntity();

		/**
		* Set the visibility
		* @param visible If the particle is visible
		*/
		void SetVisible(const bool visible);

		/**
		* Set the life
		* @param life The life
		*/
		void SetLife(const float life);

		/**
		* Set the max life
		* @param lifeMax The max life
		*/
		void SetLifeMax(const float lifeMax);

		/**
		* Set the size
		* @param size The size
		*/
		void SetSize(const float size);

		/**
		* Set the min size
		* @param sizeMin The min size
		*/
		void SetSizeMin(const float sizeMin);

		/**
		* Set the max size
		* @param sizeMax The max size
		*/
		void SetSizeMax(const float sizeMax);

		/**
		* Set the rotation
		* @param rotation The rotation
		*/
		void SetRotation(const float rotation);

		/**
		* Set the rotation speed
		* @param rotationSpeed The rotation
		*/
		void SetRotationSpeed(const float rotationSpeed);

		/**
		* Set the texure id
		* @param textureId The texture id
		*/
		void SetTextureId(const float textureId);

		/**
		* Set the position
		* @param position The position
		*/
		void SetPosition(const CVector3F & position);

		/**
		* Set the color
		* @param color The color
		*/
		void SetColor(const CColor & color);

		/**
		* Set the min color
		* @param colorMin The min color
		*/
		void SetColorMin(const CColor & colorMin);

		/**
		* Set the max color
		* @param colorMax The max color
		*/
		void SetColorMax(const CColor & colorMax);

		/**
		* Set the velocity
		* @param velocity The velocity
		*/
		void SetVelocity(const CVector3F velocity);

		/**
		* Get the visibility state
		* @return The visibility state
		*/
		const bool GetVisible() const;

		/**
		* Get the life
		* @return The life
		*/
		const float GetLife() const;

		/**
		* Get the max life
		* @return The max life
		*/
		const float GetLifeMax() const;

		/**
		* Get the life as a normalized value
		* @return The normalized life value
		*/
		const float GetLifeNormalized() const;

		/**
		* Get the size
		*/
		const float GetSize() const;

		/**
		* Get the min size
		*/
		const float GetSizeMin() const;

		/**
		* Get the max size
		*/
		const float GetSizeMax() const;

		/**
		* Get the rotation
		*/
		const float GetRotation() const;

		/**
		* Get the rotation speed
		*/
		const float GetRotationSpeed() const;

		/**
		* Get the position
		*/
		const CVector3F & GetPosition() const;

		/**
		* Get the color
		*/
		const CColor & GetColor() const;

		/**
		* Get the min color
		*/
		const CColor & GetColorMin() const;

		/**
		* Get the max color
		*/
		const CColor & GetColorMax() const;

		/**
		* Get the velocity
		*/
		const CVector3F & GetVelocity() const;

		/**
		* Get the data used for rendering
		*/
		SParticleData * GetData();
	};
}

#endif

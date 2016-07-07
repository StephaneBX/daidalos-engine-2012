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
		CColor m_color;
		CVector2F sizeTextureId;
	};

	/**
	* A particle entity
	*/
	class CParticleEntity {
	protected:
		bool m_visible;
		float m_life;
		float m_size;
		float m_rotation;
		float m_textureId;
		CVector3F m_position;
		CColor m_color;
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
		* Set the size
		* @param size The size
		*/
		void SetSize(const float size);

		/**
		* Set the rotation
		* @param rotation The rotation
		*/
		void SetRotation(const float rotation);

		/**
		* Set the texure id
		* @param The texture id
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
		* Get the size
		*/
		const float GetSize() const;

		/**
		* Get the rotation
		*/
		const float GetRotation() const;

		/**
		* Get the position
		*/
		const CVector3F & GetPosition() const;

		/**
		* Get the color
		*/
		const CColor & GetColor() const;

		/**
		* Get the data used for rendering
		*/
		const SParticleData & GetData();
	};
}

#endif

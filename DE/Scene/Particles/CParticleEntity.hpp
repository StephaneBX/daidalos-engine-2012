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

#ifndef __CPARTICLEENTITY_HPP
#define __CPARTICLEENTITY_HPP

#include "../../Core/Utility/CVector3.hpp"
#include "../../Core/Utility/CVector2.hpp"
#include "../../Core/Utility/CColor.hpp"

namespace daidalosengine {
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
	private:
		bool m_visible;
		float m_life;
		float m_lifeMax;
		float m_rotationSpeed;
		float m_textureId;
		float m_mass;
		CColor m_color;
		CVector3F m_position;
		CVector3F m_emitterPosition;
		CVector3F m_linearVelocity;
		CVector3F m_sphereVelocity;
		CVector3F m_gravityVelocity;
		float m_circularVelocity;
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
		* Set the mass
		* @param mass The mass
		*/
		void SetMass(const float mass);

		/**
		* Set the circular velocity
		* @param velocity The circular velocity
		*/
		void SetCircularVelocity(const float velocity);

		/**
		* Set the position
		* @param position The position
		*/
		void SetPosition(const CVector3F & position);

		/**
		* Set the emitter position
		* @param position The emitter position
		*/
		void SetEmitterPosition(const CVector3F & position);

		/**
		* Set the color
		* @param color The color
		*/
		void SetColor(const CColor & color);

		/**
		* Set the linear velocity
		* @param velocity The linear velocity
		*/
		void SetLinearVelocity(const CVector3F & velocity);

		/**
		* Set the sphere velocity
		* @param velocity The sphere velocity
		*/
		void SetSphereVelocity(const CVector3F & velocity);

		/**
		* Set the gravity velocity
		* @param velocity The gravity velocity
		*/
		void SetGravityVelocity(const CVector3F & velocity);

		/**
		* Get the visibility state
		* @return The visibility state
		*/
		bool GetVisible() const;

		/**
		* Get the life
		* @return The life
		*/
		float GetLife() const;

		/**
		* Get the max life
		* @return The max life
		*/
		float GetLifeMax() const;

		/**
		* Get the life as a normalized value
		* @return The normalized life value
		*/
		float GetLifeNormalized() const;

		/**
		* Get the size
		*/
		float GetSize() const;

		/**
		* Get the rotation
		*/
		float GetRotation() const;

		/**
		* Get the rotation speed
		*/
		float GetRotationSpeed() const;

		/**
		* Get the mass
		* @return The mass
		*/
		float GetMass() const;

		/**
		* Get the circular velocity
		*/
		float GetCircularVelocity() const;

		/**
		* Get the position
		*/
		const CVector3F & GetPosition() const;

		/**
		* Get the emitter position
		*/
		const CVector3F & GetEmitterPosition() const;

		/**
		* Get the color
		*/
		const CColor & GetColor() const;

		/**
		* Get the linear velocity
		*/
		const CVector3F & GetLinearVelocity() const;

		/**
		* Get the sphere velocity
		*/
		const CVector3F & GetSphereVelocity() const;

		/**
		* Get the sphere velocity
		*/
		const CVector3F & GetGravityVelocity() const;

		/**
		* Get the data used for rendering
		*/
		SParticleData * GetData();
	};
}

#endif

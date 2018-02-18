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

#include "../../Scene/Particles/CParticleEntity.hpp"

namespace DE {

	/**
	* The constructor
	*/
	CParticleEntity::CParticleEntity() {
		this->m_life = 0;
		this->m_lifeMax = 0;
		this->m_color = CColor(0, 0, 0, 0);
		this->m_rotationSpeed = 0;
		this->m_textureId = 0;
		this->m_mass = 1;
		this->m_linearVelocity = CVector3F();
		this->m_sphereVelocity = CVector3F();
		this->m_gravityVelocity = CVector3F();
		this->m_circularVelocity = 0;
	}

	/**
	* Set the visibility
	* @param visible If the particle is visible
	*/
	void CParticleEntity::SetVisible(const bool visible) {
		this->m_visible = visible;
	}

	/**
	* Set the life
	* @param life The life
	*/
	void CParticleEntity::SetLife(const float life) {
		this->m_life = life;
	}

	/**
	* Set the max life
	* @param lifeMax The max life
	*/
	void CParticleEntity::SetLifeMax(const float lifeMax) {
		this->m_lifeMax = lifeMax;
	}

	/**
	* Set the size
	* @param size The size
	*/
	void CParticleEntity::SetSize(const float size) {
		this->m_data.sizeTextureId.x = size;
	}

	/**
	* Set the rotation
	* @param rotation The rotation
	*/
	void CParticleEntity::SetRotation(const float rotation) {
		this->m_data.positionRotation.w = rotation;
	}

	/**
	* Set the rotation speed
	* @param rotationSpeed The rotation
	*/
	void CParticleEntity::SetRotationSpeed(const float rotationSpeed) {
		this->m_rotationSpeed = rotationSpeed;
	}

	/**
	* Set the texture id
	* @param textureId The texture id
	*/
	void CParticleEntity::SetTextureId(const float textureId) {
		this->m_textureId = textureId;
		this->m_data.sizeTextureId.y = textureId;
	}

	/**
	* Set the mass
	* @param mass The mass
	*/
	void CParticleEntity::SetMass(const float mass) {
		this->m_mass = mass;
	}

	/**
	* Set the circular velocity
	* @param velocity The circular velocity
	*/
	void CParticleEntity::SetCircularVelocity(const float velocity) {
		this->m_circularVelocity = velocity;
	}

	/**
	* Set the position
	* @param position The position
	*/
	void CParticleEntity::SetPosition(const CVector3F & position) {
		this->m_position = position;
		this->m_data.positionRotation = CVector4F(position, this->m_data.positionRotation.w);
	}

	/**
	* Set the emitter position
	* @param position The emitter position
	*/
	void CParticleEntity::SetEmitterPosition(const CVector3F & position) {
		this->m_emitterPosition = position;
	}

	/**
	* Set the color
	* @param color The color
	*/
	void CParticleEntity::SetColor(const CColor & color) {
		this->m_color = color;
		this->m_data.m_color = color.ToRGBAVector();
	}

	/**
	* Set the linear velocity
	* @param velocity The linear velocity
	*/
	void CParticleEntity::SetLinearVelocity(const CVector3F & velocity) {
		this->m_linearVelocity = velocity;
	}

	/**
	* Set the sphere velocity
	* @param velocity The sphere velocity
	*/
	void CParticleEntity::SetSphereVelocity(const CVector3F & velocity) {
		this->m_sphereVelocity = velocity;
	}

	/**
	* Set the gravity velocity
	* @param velocity The gravity velocity
	*/
	void CParticleEntity::SetGravityVelocity(const CVector3F & velocity) {
		this->m_gravityVelocity = velocity;
	}

	/**
	* Get the visibility state
	* @return The visibility state
	*/
	const bool CParticleEntity::GetVisible() const {
		return this->m_visible;
	}

	/**
	* Get the life
	* @return The life
	*/
	const float CParticleEntity::GetLife() const {
		return this->m_life;
	}

	/**
	* Get the max life
	*/
	const float CParticleEntity::GetLifeMax() const {
		return this->m_lifeMax;
	}

	/**
	* Get the life as a normalized value
	* @return The normalized life value
	*/
	const float CParticleEntity::GetLifeNormalized() const {
		float value = GetLife() / GetLifeMax();
		if(value > 1.0F) {
			value = 1.0F;
		}
		return value;
	}

	/**
	* Get the size
	*/
	const float CParticleEntity::GetSize() const {
		return this->m_data.sizeTextureId.x;
	}

	/**
	* Get the rotation
	*/
	const float CParticleEntity::GetRotation() const {
		return this->m_data.positionRotation.w;
	}

	/**
	* Get the rotation speed
	*/
	const float CParticleEntity::GetRotationSpeed() const {
		return this->m_rotationSpeed;
	}

	/**
	* Get the mass
	* @return The mass
	*/
	const float CParticleEntity::GetMass() const {
		return this->m_mass;
	}

	/**
	* Get the circular velocity
	*/
	const float CParticleEntity::GetCircularVelocity() const {
		return this->m_circularVelocity;
	}

	/**
	* Get the position
	*/
	const CVector3F & CParticleEntity::GetPosition() const {
		return this->m_position;
	}

	/**
	* Get the emitter position
	*/
	const CVector3F & CParticleEntity::GetEmitterPosition() const {
		return this->m_emitterPosition;
	}

	/**
	* Get the color
	*/
	const CColor & CParticleEntity::GetColor() const {
		return this->m_color;
	}

	/**
	* Get the linear velocity
	*/
	const CVector3F & CParticleEntity::GetLinearVelocity() const {
		return this->m_linearVelocity;
	}

	/**
	* Get the sphere velocity
	*/
	const CVector3F & CParticleEntity::GetSphereVelocity() const {
		return this->m_sphereVelocity;
	}

	/**
	* Get the sphere velocity
	*/
	const CVector3F & CParticleEntity::GetGravityVelocity() const {
		return this->m_gravityVelocity;
	}

	/**
	* Get the data used for rendering
	*/
	SParticleData * CParticleEntity::GetData() {
		return &this->m_data;
	}
}
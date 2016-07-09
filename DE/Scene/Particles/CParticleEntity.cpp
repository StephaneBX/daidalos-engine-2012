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

#include "../../Scene/Particles/CParticleEntity.h"

namespace DE {

	/**
	* The constructor
	*/
	CParticleEntity::CParticleEntity() {

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
	* @param life The max life
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
	* Set the min size
	* @param sizeMin The min size
	*/
	void CParticleEntity::SetSizeMin(const float sizeMin) {
		this->m_sizeMin = sizeMin;
	}

	/**
	* Set the max size
	* @param sizeMax The max size
	*/
	void CParticleEntity::SetSizeMax(const float sizeMax) {
		this->m_sizeMax = sizeMax;
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
	* Set the texure id
	* @param The texture id
	*/
	void CParticleEntity::SetTextureId(const float textureId) {
		this->m_textureId = textureId;
	}

	/**
	* Set the position
	* @param position The position
	*/
	void CParticleEntity::SetPosition(const CVector3F & position) {
		this->m_position = position;
		this->m_data.positionRotation = CVector4F(position, this->m_data.positionRotation.x);
	}

	/**
	* Set the color
	* @param color The color
	*/
	void CParticleEntity::SetColor(const CColor & color) {
		this->m_data.m_color = color;
	}

	/**
	* Set the min color
	* @param colorMin The min color
	*/
	void CParticleEntity::SetColorMin(const CColor & colorMin) {
		this->m_colorMin = colorMin;
	}

	/**
	* Set the max color
	* @param colorMax The max color
	*/
	void CParticleEntity::SetColorMax(const CColor & colorMax) {
		this->m_colorMax = colorMax;
	}

	/**
	* Set the velocity
	* @param velocity The velocity
	*/
	void CParticleEntity::SetVelocity(const CVector3F velocity) {
		this->m_velocity = velocity;
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
	* Get the size
	*/
	const float CParticleEntity::GetSize() const {
		return this->m_data.sizeTextureId.x;
	}

	/**
	* Get the min size
	*/
	const float CParticleEntity::GetSizeMin() const {
		return this->m_sizeMin;
	}

	/**
	* Get the max size
	*/
	const float CParticleEntity::GetSizeMax() const {
		return this->m_sizeMax;
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
	* Get the position
	*/
	const CVector3F & CParticleEntity::GetPosition() const {
		return this->m_position;
	}

	/**
	* Get the color
	*/
	const CColor & CParticleEntity::GetColor() const {
		return this->m_data.m_color;
	}

	/**
	* Get the min color
	*/
	const CColor & CParticleEntity::GetColorMin() const {
		return this->m_colorMin;
	}

	/**
	* Get the max color
	*/
	const CColor & CParticleEntity::GetColorMax() const {
		return this->m_colorMax;
	}

	/**
	* Get the velocity
	*/
	const CVector3F & CParticleEntity::GetVelocity() const {
		return this->m_velocity;
	}

	/**
	* Get the data used for rendering
	*/
	const SParticleData & CParticleEntity::GetData() {
		return this->m_data;
	}
}
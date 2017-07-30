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

#ifndef __DAIDALOS_IWEAPON_HPP
#define __DAIDALOS_IWEAPON_HPP

#include <Medias/CSound.hpp>
#include <Scene/3D/C3DScene.hpp>
#include <Scene/3D/CSpotLightSceneNode.hpp>
#include <Scene/3D/CParticleEmitterSceneNode.hpp>
#include <Level/ILevel.hpp>

namespace DAIDALOS {

	enum EWeaponState {
		WEAPON_STATE_NONE,
		WEAPON_STATE_FIRE,
		WEAPON_STATE_RELOAD,
		WEAPON_STATE_SWITCH
	};

	struct SProjectile {
		int damage;
		float distance;
		DE::CVector3F position;
		DE::CVector3F direction;
	};

	/**
	* Abstract weapon
	*/
	class IWeapon {
	protected:
		EWeaponState m_state;
		unsigned int m_damage;
		unsigned int m_magazineSize;
		unsigned int m_maxMagazineSize;
		unsigned int m_remainingAmmo;
		unsigned int m_maxAmmo;
		unsigned int m_precision;
		unsigned int m_hitCount;
		float m_fireAnimationRate;
		float m_reloadAnimationRate;
		float m_fireDelay;
		float m_reloadDelay;
		float m_range;
		float m_remainingDelay;
		float m_wobblePositionTime;
		float m_smokeRemainingDelay;
		std::pair<float, float> m_fireAnimation;
		std::pair<float, float> m_reloadAnimation;
		DE::CSound m_fireSound;
		DE::CSound m_reloadSound;
		DE::CAnimatedMeshSceneNode * m_meshNode;
		GE::ILevel * m_parent;
		std::vector<DE::CParticleEmitter*> m_bulletImpactParticle;

		std::vector<int> m_activeProjectiles;
		DE::CMemoryAllocator<SProjectile> m_projectiles;

		DE::CSpotLightSceneNode * m_fireLight;
		DE::CParticleEmitterSceneNode * m_smokeEmitter;
		DE::CParticleEmitterSceneNode * m_flashEmitter;

	public:
		/**
		* Constructor
		* @param parent The parent
		*/
		IWeapon(GE::ILevel * parent);

		/**
		* Fire the weapon
		*/
		virtual void Fire();

		/**
		* Reload the weapon
		*/
		virtual void Reload();

		/**
		* Add ammo
		* @param ammo The ammo count
		*/
		void AddAmmo(const unsigned int ammo);

		/**
		* Update the weapon
		* @param time elapsed time
		*/
		virtual void Update(const float time);

		/**
		* Get the magazine size
		* @return The magazine size
		*/
		const unsigned int GetMagazineSize() const;

		/**
		* Get the remaining ammo
		* @return The remaining ammo
		*/
		const unsigned int GetRemainingAmmo() const;

	protected:
		/**
		* Initialize the weapon node
		* @param playerNode The player node to attach the weapon node to
		*/
		virtual void InitializeWeaponNode(DE::ISceneNode * playerNode) = 0;

		/**
		* Initialize the particle emitters
		*/
		virtual void InitializeParticleEmitters() = 0;

		/**
		* Initialize the smoke emitter
		*/
		virtual void InitializeFireSmokeEmitter() = 0;

		/**
		* Initialize the flash emitter
		*/
		virtual void InitializeFireFlashEmitter() = 0;
	};
}

#endif

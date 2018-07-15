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

namespace daidalos {

	enum EWeaponState {
		WEAPON_STATE_NONE,
		WEAPON_STATE_FIRE,
		WEAPON_STATE_RELOAD,
		WEAPON_STATE_SWITCH
	};

	struct SProjectile {
		int damage;
		float distance;
		daidalosengine::CVector3F position;
		daidalosengine::CVector3F direction;
	};

	/**
	* Abstract weapon
	*/
	class IWeapon {
	private:
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
		daidalosengine::CSound m_fireSound;
		daidalosengine::CSound m_reloadSound;
		daidalosengine::CAnimatedMeshSceneNode * m_meshNode;
		daidalosgameengine::ILevel * m_parent;
		std::vector<daidalosengine::CParticleEmitter*> m_bulletImpactParticle;

		std::vector<int> m_activeProjectiles;
		daidalosengine::CMemoryAllocator<SProjectile> m_projectiles;

		daidalosengine::CSpotLightSceneNode * m_fireLight;
		daidalosengine::CParticleEmitterSceneNode * m_smokeEmitter;
		daidalosengine::CParticleEmitterSceneNode * m_flashEmitter;

	public:
		/**
		* Constructor
		* @param parent The parent
		*/
		explicit IWeapon(daidalosgameengine::ILevel * parent);

		/**
		* Destructor
		*/
		virtual ~IWeapon();

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
		unsigned int GetMagazineSize() const;

		/**
		* Get the remaining ammo
		* @return The remaining ammo
		*/
		unsigned int GetRemainingAmmo() const;

	protected:
		/**
		* Initialize the weapon node
		* @param playerNode The player node to attach the weapon node to
		*/
		virtual void InitializeWeaponNode(daidalosengine::ISceneNode * playerNode) = 0;

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

		/**
		* Initialize the projectile buffer
		* @param bufferSize The buffer size
		*/
		void InitializeProjectileBuffer(const int bufferSize);

		/**
		* Set the magazine size related stats
		* @param initialMagazineSize The initial magazine size
		* @param maxMagazineSize The max magazine size
		*/
		void SetMagazineStats(const unsigned int initialMagazineSize, const unsigned int maxMagazineSize);

		/**
		* Set the remaining ammo related stats
		* @param remainingAmmo The remaining ammo
		* @param maxAmmo The max ammo
		*/
		void SetAmmoStats(const unsigned int remainingAmmo, const unsigned int maxAmmo);

		/**
		* Set the offensive stats
		* @param damage The damage
		* @param hitCount The number of impacts
		* @param precision The precision in percentage
		* @param range The range
		*/
		void SetOffensiveStats(const unsigned int damage, const unsigned int hitCount, const unsigned int precision, const unsigned int range);

		/**
		* Set the fire stats
		* @param animationRate The animation rate
		* @param animationDelay The delay between two animations
		*/
		void SetFireStats(const float animationRate, const float animationDelay);

		/**
		* Set the reload stats
		* @param animationRate The animation rate
		* @param animationDelay The delay between two animations
		*/
		void SetReloadStats(const float animationRate, const float animationDelay);

		/**
		* Set the fire sound
		* @param soundFile The sound file
		*/
		void SetFireSound(const std::string soundFile);

		/**
		* Set the fire sound
		* @param soundFile The sound file
		*/
		void SetReloadSound(const std::string soundFile);

		/**
		* Set the fire animation
		* @param firstFrame The first frame
		* @param lastFrame The last frame
		*/
		void SetFireAnimation(const float firstFrame, const float lastFrame);

		/**
		* Set the reload animation
		* @param firstFrame The first frame
		* @param lastFrame The last frame
		*/
		void SetReloadAnimation(const float firstFrame, const float lastFrame);

		/**
		* Set the mesh node
		* @param meshNode The mesh node
		*/
		void SetMeshNode(daidalosengine::CAnimatedMeshSceneNode * meshNode);

		/**
		* Set the fire light
		* @param fireLight The fire light
		*/
		void SetFireLight(daidalosengine::CSpotLightSceneNode * fireLight);

		/**
		* Set the smoke emitter
		* @param emitter The emitter
		*/
		void SetSmokeEmitter(daidalosengine::CParticleEmitterSceneNode * emitter);

		/**
		* Set the fire emitter
		* @param emitter The emitter
		*/
		void SetFlashEmitter(daidalosengine::CParticleEmitterSceneNode * emitter);

		/**
		* Add a particle emitter
		* @param emitter The particle emitter
		*/
		void AddParticleEmitter(daidalosengine::CParticleEmitter * emitter);

		/**
		* Get the parent
		* @return The parent
		*/
		daidalosgameengine::ILevel * GetParent() const;

		/**
		* Get the mesh node
		* @return The mesh node
		*/
		daidalosengine::CAnimatedMeshSceneNode * GetMeshNode() const;
	};
}

#endif

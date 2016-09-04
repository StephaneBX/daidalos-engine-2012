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

#ifndef __CANIMATEDMESHSCENENODE_H
#define __CANIMATEDMESHSCENENODE_H

#include "../../Medias/CAnimatedMesh.h"
#include "../../Scene/3D/IMeshSceneNode.h"

namespace DE {
	/**
	* Mesh scene node for animated meshes
	*/
	class CAnimatedMeshSceneNode : public IMeshSceneNode {
	protected:
		bool m_looped;
		float m_animationStart;
		float m_animationEnd;
		float m_animationSpeed;
		float m_animationCurrent;
		EMeshAnimationState m_animationState;
		CAnimatedMesh m_mesh;

	public:
		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param filename The mesh filename
		*/
		CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename);

		/**
		* Constructor
		* @param scene The associated scene
		* @param parent The parent
		* @param name The node name
		* @param filename The mesh filename
		* @param position The local position
		* @param rotation The rotation
		* @param scale The scale
		*/
		CAnimatedMeshSceneNode(I3DScene * scene, ISceneNode * parent, const std::string & name, const std::string & filename, const CVector3F & position, const CVector3F & rotation, const CVector3F & scale);

		/**
		* Set the mesh
		* @param filename The mesh filename
		*/
		void SetMesh(const std::string filename);

		/**
		* Set the looped state
		* @param looped If the animation is looped
		*/
		void SetLooped(const bool looped);

		/**
		* Set the start frame
		* @param startFrame The start frame
		*/
		void SetStartFrame(const float startFrame);

		/**
		* Set the end frame
		* @param endFrame The end frame
		*/
		void SetEndFrame(const float endFrame);

		/**
		* Set the current frame
		* @param currentFrame The current frame
		*/
		void SetCurrentFrame(const float currentFrame);

		/**
		* Set the animation speed
		* @param speed The animation
		*/
		void SetAnimationSpeed(const float speed);

		/**
		* Start an animation
		* @param startFrame The starting frame
		* @param endFrame The ending frame
		* @param speed The animation speed
		*/
		void StartAnimation(const float startFrame, const float endFrame, const float speed);

		/**
		* Pause the animation
		*/
		void PauseAnimation();

		/**
		* Stop the animation
		*/
		void StopAnimation();

		/**
		* Resume the animation
		*/
		void ResumeAnimation();

		/**
		* Update the absolute position
		* @param time The elapsed time
		*/
		void Update(const float time = 0);

		/**
		* Get the number of frames
		* @return The number of frames
		*/
		const int GetNumberOfFrames() const;

		/**
		* Get the animation state
		*/
		EMeshAnimationState GetAnimationState() const;

		/**
		* Get the mesh name
		*/
		const std::string GetMeshName() const;

	protected:
		/**
		* Finalize the geometry before rendering the mesh
		*/
		void FinalizeGeometry();
	};
}

#endif

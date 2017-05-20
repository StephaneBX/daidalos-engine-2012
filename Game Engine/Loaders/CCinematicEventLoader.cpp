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

#include <Core/Logger/ILogger.h>
#include <Core/Memory/CExceptionLoad.h>
#include "CCinematicEventLoader.hpp"

namespace GE {

	/**
	* Constructor
	*/
	CCinematicEventLoader::CCinematicEventLoader() {

	}

	/**
	* Destructor
	*/
	CCinematicEventLoader::~CCinematicEventLoader() {

	}

	/**
	* Load a material from a file
	* @param filename The material file path
	* @return The material
	*/
	ICinematicEvent * CCinematicEventLoader::LoadFromFile(const std::string & filename) {
		this->m_filename = filename;
		tinyxml2::XMLElement * element = nullptr;
		SCinematicEventInternalData * data = new SCinematicEventInternalData();

		tinyxml2::XMLDocument doc;
		int loadOk = doc.LoadFile(filename.c_str());
		if (loadOk == tinyxml2::XML_NO_ERROR) {
			tinyxml2::XMLHandle hDoc(&doc);

			// Read the camera steps
			element = hDoc.FirstChildElement("cinematic").FirstChildElement("camera").FirstChildElement("step").ToElement();
			for (element; element; element = element->NextSiblingElement()) {
				LoadCameraStep(data, tinyxml2::XMLHandle(element));
			}
			
		} else {
			DE::ILogger::Kill();
			throw DE::CExceptionLoad(this->m_filename);
		}

		return new ICinematicEvent(filename, data);
	}

	void CCinematicEventLoader::LoadCameraStep(SCinematicEventInternalData * data, tinyxml2::XMLHandle & handle) {
		DE::CVector3F position, rotation;
		std::string transition, fade;
		float duration, fadeDuration;

		GetNodeData(handle, "position", position, DE::CVector3F());
		GetNodeData(handle, "rotation", rotation, DE::CVector3F());
		GetNodeData(handle, "duration", duration, 1.0f);
		GetNodeData(handle, "transition", transition, "none");
		GetNodeData(handle, "fade", fade, "none");
		GetNodeData(handle, "fadeDuration", fadeDuration, 500.0f);

		SCinematicCameraStep step;
		step.position = position;
		step.rotation = rotation;
		step.speed = 1000.0f / duration;
		step.transition = TRANSITION_NONE;
		step.fade = FADE_NONE;
		step.fadeDuration = fadeDuration;

		// Transition
		if (transition == "interpolate") {
			step.transition = TRANSITION_INTERPOLATE;
		}

		// Fade
		if (fade == "in") {
			step.fade = FADE_IN;

		} else if (fade == "out") {
			step.fade = FADE_OUT;

		} else if ("both") {
			step.fade = FADE_BOTH;
		}

		data->cameraSteps.push_back(step);
	}
}

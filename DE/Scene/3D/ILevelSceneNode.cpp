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

#include "../../Core/Logger/ILogger.hpp"
#include "../../Core/IRenderer.hpp"
#include "../../Scene/3D/ILevelSceneNode.hpp"

namespace daidalosengine {

	ILevelSceneNode::ILevelSceneNode(I3DScene * scene) : ISceneNode(scene) {
		LOG_TRACE();

		this->m_levelBuffer.SetScene(scene);
		this->m_useGlobalAmbientColor = false;
	}

	ILevelSceneNode::~ILevelSceneNode() = default;

	void ILevelSceneNode::SetUseGlobalAmbientColor(const bool useGlobalAmbientColor) {
		this->m_useGlobalAmbientColor = useGlobalAmbientColor;
	}

	void ILevelSceneNode::SetMeshFormat(const int format) {
		LOG_TRACE();

		this->m_levelBuffer.SetMeshFormat(format);
		ISceneNode::SetMeshFormat(format);
	}

	/**
	* Set the numbe of visible triangles
	* @param visibleTriangles The number of visible triangles
	*/
	void ILevelSceneNode::SetNumberOfVisibleTriangles(const int visibleTriangles) {
		LOG_TRACE();

		this->m_numberOfVisibleTriangles = visibleTriangles;
	}

	void ILevelSceneNode::SetMeshShadingType(EMeshShadingType shadingType) {
		LOG_TRACE();

		this->m_shadingType = shadingType;

		//ISceneNode::SetMeshShadingType(shadingType);
	}

	void ILevelSceneNode::SetAmbientColor(const CVector4F & ambientColor) {
		LOG_TRACE();

		this->m_ambientColor = ambientColor;
	}

	void ILevelSceneNode::SetSkyName(const std::string & name) {
		LOG_TRACE();

		this->m_skyName = name;
	}

	void ILevelSceneNode::SetSkyAmbientColor(const CVector4F & ambientColor) {
		LOG_TRACE();

		this->m_skyAmbientColor = ambientColor;
	}

	/**
	* Set the level bounding box
	* @param levelBoundingBox the level bonding box
	*/
	void ILevelSceneNode::SetLevelBoundingBox(const CAABoundingBoxF & levelBoundingBox) {
		LOG_TRACE();

		this->m_levelBoundingBox = levelBoundingBox;
	}

	/**
	* Set the active camera
	* @param camera The active camera
	*/
	void ILevelSceneNode::SetActiveCamera(ICameraSceneNode * camera) {
		LOG_TRACE();

		this->m_activeCamera = camera;
	}

	/**
	* Add a diffuse texture
	* @param texture The diffuse texture
	*/
	void ILevelSceneNode::AddDiffuseTexture(ITexture2DBase * texture) {
		LOG_TRACE();

		this->m_diffuseTextures.insert(texture);
	}

	/**
	* Add a normal texture
	* @param texture The normal texture
	*/
	void ILevelSceneNode::AddNormalTexture(ITexture2DBase * texture) {
		LOG_TRACE();

		this->m_normalTextures.insert(texture);
	}

	bool ILevelSceneNode::GetUseGlobalAmbientColor() const {
		LOG_TRACE();

		return this->m_useGlobalAmbientColor;
	}

	/**
	* Check if thetexturing is enabled
	* @return True if the texturing is enabled
	*/
	bool ILevelSceneNode::IsTexturingEnabled() const {
		LOG_TRACE();

		return (this->m_levelBuffer.GetMeshFormat() & MESHBUFFER_ELEMENT_TEXCOORD) != 0;
	}

	int ILevelSceneNode::GetNumberOfVisibleTriangles() const {
		LOG_TRACE();

		return this->m_numberOfVisibleTriangles;
	}

	/**
	* Get the mesh shading type
	* @return The mesh shading type
	*/
	EMeshShadingType ILevelSceneNode::GetMeshShadingType() const {
		LOG_TRACE();

		return this->m_shadingType;
	}

	const CVector4F & ILevelSceneNode::GetAmbientColor() const {
		LOG_TRACE();

		return this->m_ambientColor;
	}

	const std::string & ILevelSceneNode::GetSkyName() const {
		LOG_TRACE();

		return this->m_skyName;
	}

	const CVector4F & ILevelSceneNode::GetSkyAmbientColor() const {
		LOG_TRACE();

		return this->m_skyAmbientColor;
	}

	const CAABoundingBoxF & ILevelSceneNode::GetLevelBoundingBox() {
		LOG_TRACE();

		return this->m_levelBoundingBox;
	}

	/**
	* Get the active camera
	* @return The active camera
	*/
	ICameraSceneNode * ILevelSceneNode::GetActiveCamera() const {
		LOG_TRACE();

		return this->m_activeCamera;
	}

	/**
	* Set the level buffer
	* @param triangles The triangles
	*/
	void ILevelSceneNode::SetLevelBuffer(const std::vector<CTriangle3D *>& triangles) {
		LOG_TRACE();

		this->m_levelBuffer.Transform(triangles, this->m_shadingType);
	}

	/**
	* Render the primtives from the buffer
	*/
	void ILevelSceneNode::RenderPrimitives() {
		LOG_TRACE();

		auto* renderer = IRenderer::GetRenderer();
		renderer->SetVertexBuffer(0, this->m_levelBuffer.GetVertexBuffer().GetBuffer(), this->m_levelBuffer.GetStride());
		renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_levelBuffer.GetNumberOfElements() / 3);
	}
}

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

#include "../Core/IRenderer.h"
#include "../Core/Memory/CExceptionAssert.h"
#include "../GUI/IGUIButton.h"

namespace DE
{
	IGUIButton::IGUIButton(const std::string & texture)
	{
		this->m_enabled = true;
		this->m_clicked = false;
		this->m_resizableRectangle = CRectangleF(0,0,1,1);
		this->m_changed = true;
	}

	void IGUIButton::SetEnabled(const bool enabled)
	{
		this->m_enabled = enabled;
	}

	void IGUIButton::SetResizableRectangle(const CRectangleF & rectangle)
	{
		if(rectangle.GetLeft() >= 0.0f && rectangle.GetTop() >= 0.0f && rectangle.GetRight() <= 1.0f && rectangle.GetBottom() <= 1.0f) 
		{
			this->m_resizableRectangle = rectangle;
			this->m_changed = true;
		}
	}

	void IGUIButton::SetDefaultTexture(const std::string & texture)
	{
		if(texture != "")
		{
			this->m_defaultTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultTextureFilter());
		}
		else
		{
			this->m_defaultTexture = CTexture2D();
		}
	}

	void IGUIButton::SetClickedTexture(const std::string & texture)
	{
		if(texture != "")
		{
			this->m_clickedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultTextureFilter());
		}
		else
		{
			this->m_clickedTexture = CTexture2D();
		}
	}

	void IGUIButton::SetFocusedTexture(const std::string & texture)
	{
		if(texture != "")
		{
			this->m_focusedTexture.CreateFromFile(texture, PIXEL_FORMAT_A8R8G8B8, IRenderer::GetRenderer()->GetDefaultTextureFilter());
		}
		else
		{
			this->m_focusedTexture = CTexture2D();
		}
	}

	void IGUIButton::Update()
	{
		if(this->m_changed)
		{
			this->m_boundingRectangle = CRectangleI(this->m_position.x, this->m_position.y, this->m_preferredsize.x, this->m_preferredsize.y);

			IRenderer * renderer = IRenderer::GetRenderer();

			GenerateGeometry();

			if(this->m_data.size() > 0)
			{
				// Create a new buffer if the current one is too small
				if((int)this->m_buffer.GetSize() < this->m_data.size() * sizeof(SGUIData))
					this->m_buffer = renderer->CreateVertexBuffer(sizeof(SGUIData), this->m_data.size(), BUFFER_DYNAMIC | BUFFER_WRITEONLY, nullptr);

				// Lock and copy the data
				unsigned char * lock = (unsigned char*)this->m_buffer.Lock(sizeof(SGUIData), 0, 0, LOCK_WRITEONLY);
				memcpy(lock, &this->m_data[0], this->m_data.size() * sizeof(SGUIData));
				this->m_buffer.Unlock();
			}
		}

		// Update the subviews
		IGUIView::Update();
	}

	void IGUIButton::Render()
	{
		if(this->m_data.size() > 0)
		{
			// Handle the transparency
			CColor color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
			if(!this->m_enabled)
			{
				color = CColor(1.0f, 1.0f, 1.0f, 0.5f);
			}

			IRenderer * renderer = IRenderer::GetRenderer();
			IShaderProgramBase * shaderProgram = renderer->GetCurrentShaderProgram();
			Assert(shaderProgram != nullptr);

			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_VERTEX, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TRANSLATION), this->m_parent != nullptr ? this->m_parent->GetAbsolutePosition() : CVector2I());
			shaderProgram->SetParameter(SHADERPROGRAM_FLAG_PIXEL, renderer->GetShaderProgramParameterString(SHADERPROGRAM_PARAMETER_GUI_TEXT_COLOR), color);

			renderer->SetTexture(0, GetTextureToRender());
			renderer->SetTexture(1, nullptr);
			renderer->SetVertexBuffer(0, this->m_buffer, sizeof(SGUIData));
			renderer->DrawPrimitives(PRIMITIVE_TYPE_TRIANGLE_LIST, 0, this->m_data.size() / 3);
		}

		// Render the subviews
		IGUIView::Render();
	}

	void IGUIButton::LeftClick()
	{
		if(this->m_hitTest)
		{
			this->m_clicked = true;
		}
	}

	void IGUIButton::RightClick()
	{

	}

	void IGUIButton::LeftClickRelease()
	{
		this->m_clicked = false;
		// callback
	}

	void IGUIButton::RightClickRelease()
	{

	}

	const bool IGUIButton::IsEnabled() const
	{
		return this->m_enabled;
	}

	void IGUIButton::GenerateGeometry()
	{
		this->m_data.clear();

		// Create the simple button if the resizable rectange fill the entire button or if no texture is given
		if((this->m_resizableRectangle.GetHeight() == 1.0f && this->m_resizableRectangle.GetWidth() == 1.0f) || this->m_defaultTexture.GetTexture() == nullptr)
		{
			// Simple button with only on quad
			this->m_data = GenerateQuad(GetBoundingRectangle(), this->m_resizableRectangle);
		}
		else
		{
			// Generate a 9 quad button

			// Create the top left part
			CRectangleI topLeftPosition		= CRectangleI(this->m_position.x, this->m_position.y, (int)(this->m_resizableRectangle.GetLeft() * this->m_defaultTexture.GetSize().x), (int)(this->m_resizableRectangle.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topLeftCoordinates	= CRectangleF(0, 0, this->m_resizableRectangle.GetLeft(), this->m_resizableRectangle.GetTop());
			std::vector<SGUIData> topLeft	= GenerateQuad(topLeftPosition, topLeftCoordinates);							
			this->m_data.insert(this->m_data.end(), topLeft.begin(), topLeft.end());

			// Create the bottom left part
			CRectangleI bottomLeftPosition		= CRectangleI(this->m_position.x, this->m_position.y + (this->m_preferredsize.y - (int)((1.0f - this->m_resizableRectangle.GetBottom()) * this->m_defaultTexture.GetSize().y)), topLeftPosition.GetWidth(), (int)((1.0f - this->m_resizableRectangle.GetBottom()) * this->m_defaultTexture.GetSize().y));
			CRectangleF bottomLeftCoordinates	= CRectangleF(0, this->m_resizableRectangle.GetBottom(), this->m_resizableRectangle.GetLeft(), (1.0f - this->m_resizableRectangle.GetBottom()));
			std::vector<SGUIData> bottomLeft	= GenerateQuad(bottomLeftPosition, bottomLeftCoordinates);
			this->m_data.insert(this->m_data.end(), bottomLeft.begin(), bottomLeft.end());

			// Create the middle left part
			CRectangleI middleLeftPosition		= CRectangleI(topLeftPosition.GetLeft(), topLeftPosition.GetBottom(), topLeftPosition.GetWidth(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			CRectangleF middleLeftCoordinates	= CRectangleF(0, this->m_resizableRectangle.GetTop(), this->m_resizableRectangle.GetLeft(), this->m_resizableRectangle.GetHeight());
			std::vector<SGUIData> middleLeft	= GenerateQuad(middleLeftPosition, middleLeftCoordinates);
			this->m_data.insert(this->m_data.end(), middleLeft.begin(), middleLeft.end());

			// Create the top right part
			CRectangleI topRightPosition	= CRectangleI(this->m_position.x + (this->m_preferredsize.x - (int)((1.0f - this->m_resizableRectangle.GetRight()) * this->m_defaultTexture.GetSize().x)), this->m_position.y, (int)((1.0f - this->m_resizableRectangle.GetRight()) * this->m_defaultTexture.GetSize().x), (int)(this->m_resizableRectangle.GetTop() * this->m_defaultTexture.GetSize().y));
			CRectangleF topRightCoordinates = CRectangleF(this->m_resizableRectangle.GetRight(), 0, (1.0f - this->m_resizableRectangle.GetRight()), this->m_resizableRectangle.GetTop());
			std::vector<SGUIData> topRight	= GenerateQuad(topRightPosition, topRightCoordinates);
			this->m_data.insert(this->m_data.end(), topRight.begin(), topRight.end());

			// Create the top part
			CRectangleI topPosition		= CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetTop(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), topLeftPosition.GetHeight());
			CRectangleF topCoordinates	= CRectangleF(this->m_resizableRectangle.GetLeft(), 0, this->m_resizableRectangle.GetWidth(), this->m_resizableRectangle.GetTop());
			std::vector<SGUIData> top	= GenerateQuad(topPosition, topCoordinates);
			this->m_data.insert(this->m_data.end(), top.begin(), top.end());

			// Create the bottom right part
			CRectangleI bottomRightPosition		= CRectangleI(topRightPosition.GetLeft(), bottomLeftPosition.GetTop(), topRightPosition.GetWidth(), bottomLeftPosition.GetHeight());
			CRectangleF bottomRightCoordinates	= CRectangleF(this->m_resizableRectangle.GetRight(), this->m_resizableRectangle.GetBottom(), (1.0f - this->m_resizableRectangle.GetRight()), (1.0f - this->m_resizableRectangle.GetBottom()));
			std::vector<SGUIData> bottomRight	= GenerateQuad(bottomRightPosition, bottomRightCoordinates);
			this->m_data.insert(this->m_data.end(), bottomRight.begin(), bottomRight.end());

			// Create the middle right part
			CRectangleI middleRightPosition		= CRectangleI(topRightPosition.GetLeft(), topRightPosition.GetBottom(), topRightPosition.GetWidth(), bottomRightPosition.GetTop() - topRightPosition.GetBottom());
			CRectangleF middleRightCoordinates	= CRectangleF(this->m_resizableRectangle.GetRight(), this->m_resizableRectangle.GetTop(), (1.0f - this->m_resizableRectangle.GetRight()), this->m_resizableRectangle.GetHeight());
			std::vector<SGUIData> middleRight	= GenerateQuad(middleRightPosition, middleRightCoordinates);
			this->m_data.insert(this->m_data.end(), middleRight.begin(), middleRight.end());

			// Create the bottom part
			CRectangleI bottomPosition = CRectangleI(bottomLeftPosition.GetRight(), bottomLeftPosition.GetTop(), bottomRightPosition.GetLeft() - bottomLeftPosition.GetRight(), bottomLeftPosition.GetHeight());
			CRectangleF bottomCoordinates = CRectangleF(this->m_resizableRectangle.GetLeft(), this->m_resizableRectangle.GetBottom(), this->m_resizableRectangle.GetWidth(), (1.0f - this->m_resizableRectangle.GetBottom()));
			std::vector<SGUIData> bottom = GenerateQuad(bottomPosition, bottomCoordinates);
			this->m_data.insert(this->m_data.end(), bottom.begin(), bottom.end());

			// Create the center part
			CRectangleI centerPosition = CRectangleI(topLeftPosition.GetRight(), topLeftPosition.GetBottom(), topRightPosition.GetLeft() - topLeftPosition.GetRight(), bottomLeftPosition.GetTop() - topLeftPosition.GetBottom());
			std::vector<SGUIData> center = GenerateQuad(centerPosition, this->m_resizableRectangle);
			this->m_data.insert(this->m_data.end(), center.begin(), center.end());
		}

		this->m_changed = false;
	}

	ITexture2DBase * IGUIButton::GetTextureToRender()
	{
		if(this->m_clicked && this->m_clickedTexture.GetTexture() != nullptr)
		{
			return this->m_clickedTexture.GetTexture();
		}
		else
		{
			if(this->m_hitTest && this->m_focusedTexture.GetTexture() != nullptr)
			{
				return this->m_focusedTexture.GetTexture();
			}
			else
			{
				return this->m_defaultTexture.GetTexture();
			}
		}
	}
}

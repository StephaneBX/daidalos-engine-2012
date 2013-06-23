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

#include <d3d9.h>
#include <d3dx9.h>
#include "../../Core/Utility/CFile.h"
#include "../../Core/Logger/ILogger.h"
#include "../../Core/Memory/CExceptionLoad.h"
#include "../../Medias/IShaderBase.h"
#include "../../Renderers/DX9/CDX9Renderer.h"
#include "../../Renderers/DX9/CDX9PixelShader.h"
#include "../../Renderers/DX9/CDX9PixelShadersLoader.h"

namespace DE
{
	CDX9PixelShadersLoader::CDX9PixelShadersLoader() {}

	
	CDX9PixelShadersLoader::~CDX9PixelShadersLoader() {}

	IShaderBase * CDX9PixelShadersLoader::LoadFromFile(const std::string & filename)
	{
		if(!CFile(filename).FileExists())
			throw CExceptionLoad(filename);

		IDirect3DPixelShader9 * shader;
		ID3DXConstantTable * constantTable;
		HRESULT hr;
		LPD3DXBUFFER pCode;
		DWORD dwShaderFlags = D3DXSHADER_OPTIMIZATION_LEVEL1;
		LPD3DXBUFFER pBufferErrors = nullptr;

		hr = D3DXCompileShaderFromFile(filename.c_str(), nullptr, nullptr, "main", "ps_3_0", dwShaderFlags, &pCode, &pBufferErrors, &constantTable);
		if(FAILED(hr))
		{
			LPVOID pCompilErrors = pBufferErrors->GetBufferPointer();
			LOG_ERROR((const char*)pCompilErrors);
			ILogger::Kill();

			throw CExceptionLoad(filename);
		}

		((CDX9Renderer*)IRenderer::GetRenderer())->GetDevice()->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &shader);
		pCode->Release();

		return new CDX9PixelShader(filename, shader, constantTable);
	}
}

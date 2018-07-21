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
#include "../../Core/Logger/ILogger.hpp"
#include "../../Renderers/DX9/CDX9Enums.hpp"

namespace daidalosengine {
	D3DPRIMITIVETYPE CDX9Enums::s_primitiveType[] = {
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_TRIANGLEFAN,
		D3DPT_LINELIST,
		D3DPT_LINESTRIP,
		D3DPT_POINTLIST
	};

	D3DFORMAT CDX9Enums::s_formatType[] = {
		D3DFMT_L8,
		D3DFMT_A8L8,
		D3DFMT_A1R5G5B5,
		D3DFMT_A4R4G4B4,
		D3DFMT_R8G8B8,
		D3DFMT_A8R8G8B8,
		D3DFMT_R16F,
		D3DFMT_R32F,
		D3DFMT_DXT1,
		D3DFMT_DXT3,
		D3DFMT_DXT5,
		D3DFMT_D16,
		D3DFMT_D32,
		D3DFMT_D24S8
	};

	D3DTEXTUREFILTERTYPE CDX9Enums::s_textureFilterType[] = {
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
		D3DTEXF_PYRAMIDALQUAD,
		D3DTEXF_GAUSSIANQUAD,
		D3DTEXF_CONVOLUTIONMONO
	};

	D3DTEXTUREADDRESS CDX9Enums::s_textureAddressType[] = {
		D3DTADDRESS_WRAP,
		D3DTADDRESS_MIRROR,
		D3DTADDRESS_CLAMP,
		D3DTADDRESS_BORDER,
		D3DTADDRESS_MIRRORONCE
	};

	D3DRENDERSTATETYPE CDX9Enums::s_renderStateType[] = {
		D3DRS_ZENABLE,
		D3DRS_ZWRITEENABLE,
		D3DRS_ALPHATESTENABLE,
		D3DRS_ALPHABLENDENABLE,
		D3DRS_SCISSORTESTENABLE,
		D3DRS_STENCILENABLE
	};

	D3DCMPFUNC CDX9Enums::s_compare[] = {
		D3DCMP_NEVER,
		D3DCMP_LESS,
		D3DCMP_EQUAL,
		D3DCMP_LESSEQUAL,
		D3DCMP_GREATER,
		D3DCMP_NOTEQUAL,
		D3DCMP_GREATEREQUAL,
		D3DCMP_ALWAYS
	};


	D3DBLEND CDX9Enums::s_renderStateBlendFunction[] = {
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_INVSRCCOLOR,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_INVDESTCOLOR,
		D3DBLEND_SRCALPHASAT,
		D3DBLEND_BOTHSRCALPHA,
		D3DBLEND_BOTHINVSRCALPHA,
		D3DBLEND_BLENDFACTOR,
		D3DBLEND_INVBLENDFACTOR,
		D3DBLEND_SRCCOLOR2,
		D3DBLEND_INVSRCCOLOR2
	};

	D3DBLENDOP CDX9Enums::s_renderStateBlendOperation[] = {
		D3DBLENDOP_ADD,
		D3DBLENDOP_SUBTRACT,
		D3DBLENDOP_REVSUBTRACT,
		D3DBLENDOP_MIN,
		D3DBLENDOP_MAX
	};

	D3DCULL CDX9Enums::s_renderStateCullType[] = {
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW
	};

	D3DFILLMODE CDX9Enums::s_fillMode[] = {
		D3DFILL_POINT,
		D3DFILL_WIREFRAME,
		D3DFILL_SOLID,
		D3DFILL_FORCE_DWORD
	};

	D3DSTENCILOP CDX9Enums::s_stencilOperation[] {
		D3DSTENCILOP_KEEP,
		D3DSTENCILOP_ZERO,
		D3DSTENCILOP_REPLACE,
		D3DSTENCILOP_INCRSAT,
		D3DSTENCILOP_DECRSAT,
		D3DSTENCILOP_INVERT,
		D3DSTENCILOP_INCR,
		D3DSTENCILOP_DECR
	};

	unsigned int CDX9Enums::s_streamSourceType[] {
		0,
		D3DSTREAMSOURCE_INDEXEDDATA,
		D3DSTREAMSOURCE_INSTANCEDATA
	};

	unsigned long CDX9Enums::BufferFlags(const unsigned long flags) {
		LOG_TRACE();

		unsigned long lockFlags = 0;

		if ((flags & BUFFER_DYNAMIC) != 0) {
			lockFlags = D3DUSAGE_DYNAMIC;
		}

		if ((flags & BUFFER_WRITEONLY) != 0) {
			lockFlags |= D3DUSAGE_WRITEONLY;
		}

		return lockFlags;
	}

	unsigned long CDX9Enums::LockFlags(const unsigned long flags) {
		LOG_TRACE();

		unsigned long lockFlags = 0;

		if ((flags & LOCK_READONLY) != 0) {
			lockFlags = D3DLOCK_READONLY;
		}

		if ((flags & LOCK_WRITEONLY) != 0) {
			lockFlags = D3DLOCK_DISCARD;
		}

		return lockFlags;
	}

	unsigned long CDX9Enums::SurfaceFlags(const unsigned long flags) {
		LOG_TRACE();

		unsigned long surfaceFlags = 0;

		if ((flags & SURFACE_TYPE_TARGET) != 0) {
			surfaceFlags |= D3DCLEAR_TARGET;
		}

		if ((flags & SURFACE_TYPE_DEPTH) != 0) {
			surfaceFlags |= D3DCLEAR_ZBUFFER;
		}

		if ((flags & SURFACE_TYPE_STENCIL) != 0) {
			surfaceFlags |= D3DCLEAR_STENCIL;
		}

		return surfaceFlags;
	}

	D3DPRIMITIVETYPE CDX9Enums::Get(const EPrimitiveType type) {
		LOG_TRACE();

		return s_primitiveType[type];
	}

	D3DFORMAT CDX9Enums::Get(const EPixelFormat type) {
		LOG_TRACE();

		return s_formatType[type];
	}

	D3DTEXTUREFILTERTYPE CDX9Enums::Get(const ETextureFilterType type) {
		LOG_TRACE();

		return s_textureFilterType[type];
	}

	D3DTEXTUREADDRESS CDX9Enums::Get(const ETextureAddress type) {
		LOG_TRACE();

		return s_textureAddressType[type];
	}

	D3DRENDERSTATETYPE CDX9Enums::Get(const ERenderStateType type) {
		LOG_TRACE();

		return s_renderStateType[type];
	}

	D3DCMPFUNC CDX9Enums::Get(const ECompare compare) {
		LOG_TRACE();

		return s_compare[compare];
	}

	D3DBLEND CDX9Enums::Get(const ERenderStateBlendFunction type) {
		LOG_TRACE();

		return s_renderStateBlendFunction[type];
	}

	D3DBLENDOP CDX9Enums::Get(const ERenderStateBlendOperation type) {
		LOG_TRACE();

		return s_renderStateBlendOperation[type];
	}

	D3DCULL CDX9Enums::Get(const ERenderStateCullType type) {
		LOG_TRACE();

		return s_renderStateCullType[type];
	}


	D3DFILLMODE CDX9Enums::Get(const ERenderStateFillMode fillMode) {
		return s_fillMode[fillMode];
	}


	D3DSTENCILOP CDX9Enums::Get(const EStencilOperation operation) {
		LOG_TRACE();

		return s_stencilOperation[operation];
	}

	unsigned int CDX9Enums::Get(const EStreamSourceType streamSourceType) {
		LOG_TRACE();

		return s_streamSourceType[streamSourceType];
	}
}

#include "core.h"
#include "gl2rendertarget.h"
#include "gl2texture.h"

#ifdef PORTMASTER
#include <string.h>

#ifndef GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT24_OES 0x81A6
#endif

// Depth format for ERTF_D24S8 on GLES. 24-bit depth renderbuffers need
// GL_OES_depth24; without it only 16-bit depth is guaranteed. The extension
// string is queried once (a GL context exists by the time targets are made).
static GLenum GetGLESDepthFormat()
{
	static GLenum sDepthFormat = 0;
	if( sDepthFormat == 0 )
	{
		const char* const Extensions = reinterpret_cast<const char*>( glGetString( GL_EXTENSIONS ) );
		const bool HasDepth24 = Extensions && strstr( Extensions, "GL_OES_depth24" );
		sDepthFormat = HasDepth24 ? GL_DEPTH_COMPONENT24_OES : GL_DEPTH_COMPONENT16;
		PRINTF( "GLES depth buffer: %s\n", HasDepth24 ? "24-bit (GL_OES_depth24)" : "16-bit" );
	}
	return sDepthFormat;
}
#endif

/*static*/ GLenum GL2RenderTarget::GetGLFormat( const ERenderTargetFormat Format )
{
	switch( Format )
	{
	case ERTF_Unknown:
		return 0;
#ifdef HAVE_GLES
	case ERTF_X8R8G8B8:
	case ERTF_A8R8G8B8:
	case ERTF_A16B16G16R16:
	case ERTF_A16B16G16R16F:
	case ERTF_A32B32G32R32F:
	case ERTF_R32F:
	case ERTF_R32G32F:
		return GL_RGBA;
#else
	case ERTF_X8R8G8B8:
		return GL_RGBA8;	// Meh?
	case ERTF_A8R8G8B8:
		return GL_RGBA8;
	case ERTF_A16B16G16R16:
		return GL_RGBA16;
	case ERTF_A16B16G16R16F:
		ASSERT( GLEW_ARB_texture_float );
		return GL_RGBA16F_ARB;
	case ERTF_A32B32G32R32F:
		ASSERT( GLEW_ARB_texture_float );
		return GL_RGBA32F_ARB;
	case ERTF_R32F:
		ASSERT( GLEW_ARB_texture_rg );
		return GL_R32F;
	case ERTF_R32G32F:
		ASSERT( GLEW_ARB_texture_rg );
		return GL_RG32F;
#endif
#ifdef HAVE_GLES
	case ERTF_D24S8:
		// Depth only: Eldritch never uses the stencil part of D24S8.
#ifdef PORTMASTER
		return GetGLESDepthFormat();
#else
		return GL_DEPTH_COMPONENT16;
#endif
#else
	case ERTF_D24S8:
		return GL_DEPTH24_STENCIL8;
#endif
	default:
		WARNDESC( "GL texture format not matched" );
		return 0;
	}
}

GL2RenderTarget::GL2RenderTarget()
:	m_FrameBufferObject( 0 )
,	m_ColorTextureObjects()
,	m_ColorTextures()
,	m_DepthStencilRenderBufferObject( 0 )
#if defined(PANDORA) || defined(__amigaos4__)
,	m_StencilRenderBufferObject( 0 )
#endif
,	m_Width( 0 )
,	m_Height( 0 )
,	m_IsChild( false )
{
}

GL2RenderTarget::~GL2RenderTarget()
{
	// Don't clean up m_ColorTextureObjects; deleting the textures does that.
	if( m_IsChild )
	{
		// Don't delete textures, we don't own them
	}
	else
	{
		FOR_EACH_ARRAY( TextureIter, m_ColorTextures, ITexture* )
		{
			ITexture* pTexture = TextureIter.GetValue();
			SafeDelete( pTexture );
		}
	}
	m_ColorTextures.Clear();
	m_ColorTextureObjects.Clear();

#if defined(PANDORA) || defined(__amigaos4__)
	if( m_StencilRenderBufferObject != 0 )
	{
		glDeleteRenderbuffers(1, &m_StencilRenderBufferObject);
	}
#endif

	if( m_DepthStencilRenderBufferObject != 0 )
	{
#ifdef HAVE_GLES
		glDeleteRenderbuffers( 1, &m_DepthStencilRenderBufferObject );
#else
		if( GLEW_ARB_framebuffer_object )
		{
			glDeleteRenderbuffers( 1, &m_DepthStencilRenderBufferObject );
			GLERRORCHECK;
		}
		else if( GLEW_EXT_framebuffer_object )
		{
			glDeleteRenderbuffersEXT( 1, &m_DepthStencilRenderBufferObject );
			GLERRORCHECK;
		}
#endif
	}

	if( m_FrameBufferObject != 0 )
	{
#ifdef HAVE_GLES
		glDeleteRenderbuffers( 1, &m_FrameBufferObject );
		GLERRORCHECK;
#else
		if( GLEW_ARB_framebuffer_object )
		{
			glDeleteFramebuffers( 1, &m_FrameBufferObject );
			GLERRORCHECK;
		}
		else if( GLEW_EXT_framebuffer_object )
		{
			glDeleteFramebuffersEXT( 1, &m_FrameBufferObject );
			GLERRORCHECK;
		}
#endif
	}
}

/*virtual*/ void GL2RenderTarget::Initialize( const SRenderTargetParams& Params )
{
	XTRACE_FUNCTION;

	m_Width		= Params.Width;
	m_Height	= Params.Height;

#ifdef HAVE_GLES
	if( Params.ColorFormat != ERTF_None )
	{
		GLGUARD_ACTIVETEXTURE;
		GLGUARD_BINDTEXTURE;

		glActiveTexture( GL_TEXTURE0 );

		uint& ColorTextureObject = m_ColorTextureObjects.PushBack();

		glGenTextures( 1, &ColorTextureObject );
		ASSERT( ColorTextureObject != 0 );
		glBindTexture( GL_TEXTURE_2D, ColorTextureObject );

		const GLenum ColorFormat = GetGLFormat( Params.ColorFormat );
		// The image format parameters don't necessarily match the color format,
		// but it doesn't matter because we're not providing image data here.
		glTexImage2D( GL_TEXTURE_2D, 0, ColorFormat, Params.Width, Params.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		GLERRORCHECK;

		m_ColorTextures.PushBack( new GL2Texture( ColorTextureObject ) );
	}

	if( Params.DepthStencilFormat != ERTF_None )
	{
		glGenRenderbuffers( 1, &m_DepthStencilRenderBufferObject );
		ASSERT( m_DepthStencilRenderBufferObject != 0 );
		glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferObject );
		const GLenum DepthStencilFormat = GetGLFormat( Params.DepthStencilFormat );
		glRenderbufferStorage( GL_RENDERBUFFER, DepthStencilFormat, Params.Width, Params.Height );
	}

	CreateFBO();
#else
	if( Params.ColorFormat != ERTF_None )
	{
		GLGUARD_ACTIVETEXTURE;
		GLGUARD_BINDTEXTURE;

		glActiveTexture( GL_TEXTURE0 );

		uint& ColorTextureObject = m_ColorTextureObjects.PushBack();

		glGenTextures( 1, &ColorTextureObject );
		ASSERT( ColorTextureObject != 0 );
		glBindTexture( GL_TEXTURE_2D, ColorTextureObject );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );

		const GLenum ColorFormat = GetGLFormat( Params.ColorFormat );
		// The image format parameters don't necessarily match the color format,
		// but it doesn't matter because we're not providing image data here.
		glTexImage2D( GL_TEXTURE_2D, 0, ColorFormat, Params.Width, Params.Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );
		GLERRORCHECK;

		m_ColorTextures.PushBack( new GL2Texture( ColorTextureObject ) );
	}

	if( Params.DepthStencilFormat != ERTF_None )
	{
		if( GLEW_ARB_framebuffer_object )
		{
			glGenRenderbuffers( 1, &m_DepthStencilRenderBufferObject );
			ASSERT( m_DepthStencilRenderBufferObject != 0 );
			glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferObject );
			const GLenum DepthStencilFormat = GetGLFormat( Params.DepthStencilFormat );
			glRenderbufferStorage( GL_RENDERBUFFER, DepthStencilFormat, Params.Width, Params.Height );
		}
		else if( GLEW_EXT_framebuffer_object )
		{
			glGenRenderbuffersEXT( 1, &m_DepthStencilRenderBufferObject );
			ASSERT( m_DepthStencilRenderBufferObject != 0 );
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_DepthStencilRenderBufferObject );
			const GLenum DepthStencilFormat = GetGLFormat( Params.DepthStencilFormat );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, DepthStencilFormat, Params.Width, Params.Height );
		}
	}

	CreateFBO();
#endif

	GLERRORCHECK;
}

void GL2RenderTarget::Release()
{
}

void GL2RenderTarget::Reset()
{
}

/*virtual*/ void* GL2RenderTarget::GetHandle()
{
	return &m_FrameBufferObject;
}

/*virtual*/ void* GL2RenderTarget::GetColorRenderTargetHandle( const uint Index )
{
	return m_ColorTextureObjects.GetData() + Index;
}

/*virtual*/ void* GL2RenderTarget::GetDepthStencilRenderTargetHandle()
{
	return &m_DepthStencilRenderBufferObject;
}

/*virtual*/ ITexture* GL2RenderTarget::GetColorTextureHandle( const uint Index )
{
	return m_ColorTextures[ Index ];
}

/*virtual*/ void GL2RenderTarget::AttachColorFrom( IRenderTarget* const pRenderTarget, const uint Index )
{
	// Fix up dimensions; other params will remain uninitialized
	if( m_Width == 0 || m_Height == 0 )
	{
		m_Width = pRenderTarget->GetWidth();
		m_Height = pRenderTarget->GetHeight();
	}

	ASSERT( m_Width == pRenderTarget->GetWidth() );
	ASSERT( m_Height == pRenderTarget->GetHeight() );

	GL2RenderTarget* const pGL2RenderTarget = static_cast<GL2RenderTarget*>( pRenderTarget );
	const GLuint Object = *static_cast<GLuint*>( pGL2RenderTarget->GetColorRenderTargetHandle( Index ) );
	m_ColorTextureObjects.PushBack( Object );
	ITexture* const pTexture = pGL2RenderTarget->GetColorTextureHandle( Index );
	m_ColorTextures.PushBack( pTexture );
}

/*virtual*/ void GL2RenderTarget::AttachDepthStencilFrom( IRenderTarget* const pRenderTarget )
{
	ASSERT( m_Width == pRenderTarget->GetWidth() );
	ASSERT( m_Height == pRenderTarget->GetHeight() );

	GL2RenderTarget* const pGL2RenderTarget = static_cast<GL2RenderTarget*>( pRenderTarget );
	m_DepthStencilRenderBufferObject = *static_cast<GLuint*>( pGL2RenderTarget->GetDepthStencilRenderTargetHandle() );
}

/*virtual*/ void GL2RenderTarget::FinishAttach()
{
#ifndef HAVE_GLES
	CreateFBO();
#endif

	m_IsChild = true;
}

void GL2RenderTarget::CreateFBO()
{
#ifdef HAVE_GLES
	glGenFramebuffers( 1, &m_FrameBufferObject );
	ASSERT( m_FrameBufferObject != 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObject );
	GLERRORCHECK;

	FOR_EACH_ARRAY( TextureObjectIter, m_ColorTextureObjects, GLuint )
	{
		const uint		TextureIndex		= TextureObjectIter.GetIndex();
		const GLuint	ColorTextureObject	= TextureObjectIter.GetValue();
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureIndex, GL_TEXTURE_2D, ColorTextureObject, 0 );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderBufferObject );
		// The renderbuffer is depth-only (16- or 24-bit, see GetGLFormat), so it is not also
		// attached as GL_STENCIL_ATTACHMENT: on GLES 2.0 drivers such as Mali that makes the
		// FBO incomplete. Eldritch does not use stencil.
		GLERRORCHECK;
	}

	const GLenum FrameBufferStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if (FrameBufferStatus != GL_FRAMEBUFFER_COMPLETE) {
		PRINTF( "Framebuffer incomplete, status 0x%04X\n", FrameBufferStatus );
	}
#else
	// FBOs were supported in GL 2.1 only by extension, but some newer drivers
	// don't still support that extension. Use whichever is available.
	ASSERT( GLEW_EXT_framebuffer_object || GLEW_ARB_framebuffer_object );

	if( GLEW_ARB_framebuffer_object )
	{
		glGenFramebuffers( 1, &m_FrameBufferObject );
		ASSERT( m_FrameBufferObject != 0 );
		glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferObject );
		GLERRORCHECK;

		FOR_EACH_ARRAY( TextureObjectIter, m_ColorTextureObjects, GLuint )
		{
			const uint		TextureIndex		= TextureObjectIter.GetIndex();
			const GLuint	ColorTextureObject	= TextureObjectIter.GetValue();
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureIndex, GL_TEXTURE_2D, ColorTextureObject, 0 );
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderBufferObject );
			GLERRORCHECK;
		}

		const GLenum FrameBufferStatus = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		ASSERT( FrameBufferStatus == GL_FRAMEBUFFER_COMPLETE );
		Unused( FrameBufferStatus );
	}
	else if( GLEW_EXT_framebuffer_object )
	{
		glGenFramebuffersEXT( 1, &m_FrameBufferObject );
		ASSERT( m_FrameBufferObject != 0 );
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_FrameBufferObject );
		GLERRORCHECK;

		FOR_EACH_ARRAY( TextureObjectIter, m_ColorTextureObjects, GLuint )
		{
			const uint		TextureIndex		= TextureObjectIter.GetIndex();
			const GLuint	ColorTextureObject	= TextureObjectIter.GetValue();
			glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + TextureIndex, GL_TEXTURE_2D, ColorTextureObject, 0 );
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_DepthStencilRenderBufferObject );
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_DepthStencilRenderBufferObject );
			GLERRORCHECK;
		}

		const GLenum FrameBufferStatus = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
		ASSERT( FrameBufferStatus == GL_FRAMEBUFFER_COMPLETE_EXT );
		Unused( FrameBufferStatus );
	}
#endif
}

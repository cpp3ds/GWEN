
#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/Renderers/cpp3ds.h"

#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Graphics/Font.hpp>

#ifdef _3DS
#include <citro3d.h>
#else
#include <GL/gl.h>
#endif

#include <cmath>

struct TextureData
{
	TextureData( cpp3ds::Image* img ): texture( NULL ), image( img ) { }
	TextureData( cpp3ds::Texture* text ): texture( text ), image( NULL ) { }
	~TextureData()
	{
		if ( texture != NULL ) delete texture;
		if ( image != NULL) delete image;
	}

	cpp3ds::Texture *texture;
	cpp3ds::Image *image;
};

Gwen::Renderer::cpp3dsRenderer::cpp3dsRenderer( cpp3ds::RenderTarget& target ):
		m_Target( target ), m_Color(), m_Buffer(), m_BufferIndex(0), m_RenderStates( cpp3ds::RenderStates::Default ), m_Height( m_Target.getSize().y )
{
	m_Buffer.setPrimitiveType( cpp3ds::Triangles );
	m_RenderStates.blendMode = cpp3ds::BlendAlpha;
}

Gwen::Renderer::cpp3dsRenderer::~cpp3dsRenderer() { }

void Gwen::Renderer::cpp3dsRenderer::Begin()
{
	m_Buffer.clear();
	m_BufferIndex = 0;

	m_OriginalView = m_Target.getView();
	cpp3ds::FloatRect vrect;
	vrect.left = 0;	vrect.top = 0;
	vrect.width = m_Target.getSize().x;	vrect.height = m_Height = m_Target.getSize().y;
	cpp3ds::FloatRect vprect;
	vprect.left = 0; vprect.top = 0;
	vprect.width = 1.0f; vprect.height = 1.0f;
	cpp3ds::View view( vrect );
	view.setViewport( vprect );
	m_Target.setView( view );
}

void Gwen::Renderer::cpp3dsRenderer::End()
{
	m_Target.setView( m_OriginalView );
	Flush();
	m_Target.resetGLStates();
}

void Gwen::Renderer::cpp3dsRenderer::StartClip()
{
	Flush();

	Gwen::Rect rect = ClipRegion();
	float x = rect.x, y = rect.y, w = rect.w, h = rect.h;

	float scale = Scale();

#ifdef _3DS
	y = 240 - y;
	x = 320 - x;
	C3D_SetScissor(GPU_SCISSOR_NORMAL, (y-h)*scale, (x-w)*scale, y*scale, x*scale);
#else
	// OpenGL's coords are from the bottom left
	// so we need to translate them here.
	y = m_Height - ( y + h );
	glEnable( GL_SCISSOR_TEST );
	glScissor( x * scale, y * scale, w * scale, h * scale );
#endif
}

void Gwen::Renderer::cpp3dsRenderer::EndClip()
{
	Flush();
#ifdef _3DS
	C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
#else
	glDisable( GL_SCISSOR_TEST );
#endif
}

void Gwen::Renderer::cpp3dsRenderer::SetDrawColor( Gwen::Color color )
{
	m_Color = cpp3ds::Color( color.r, color.g, color.b, color.a );
}

void Gwen::Renderer::cpp3dsRenderer::DrawPixel( int x, int y )
{
//	EnsurePrimitiveType( cpp3ds::Points );
//	EnsureTexture( NULL );
//	Translate( x, y );
//	AddVert( x, y+1 );
}

void Gwen::Renderer::cpp3dsRenderer::DrawLinedRect( Gwen::Rect rect )
{
//	EnsurePrimitiveType( cpp3ds::Lines );
//	EnsureTexture( NULL );
//
//	Translate ( rect );
//
//	//   (x,y) ---------- (x+w, y)
//	//         |        |
//	// (x,y+h) ---------- (x+w,y+h)
//
//	AddVert( rect.x, rect.y);
//	AddVert( rect.x+rect.w, rect.y	);
//
//	AddVert( rect.x+rect.w, rect.y	);
//	AddVert( rect.x+rect.w, rect.y+rect.h );
//
//	AddVert( rect.x+rect.w, rect.y+rect.h );
//	AddVert( rect.x, rect.y + rect.h );
//
//	AddVert( rect.x, rect.y + rect.h );
//	AddVert( rect.x, rect.y );
}

void Gwen::Renderer::cpp3dsRenderer::DrawFilledRect( Gwen::Rect rect )
{

	EnsurePrimitiveType( cpp3ds::Triangles );
	EnsureTexture( NULL );

	Translate( rect );

	AddVert( rect.x, rect.y );
	AddVert( rect.x+rect.w, rect.y );
	AddVert( rect.x, rect.y + rect.h );

	AddVert( rect.x+rect.w, rect.y );
	AddVert( rect.x+rect.w, rect.y+rect.h );
	AddVert( rect.x, rect.y + rect.h );
}

void Gwen::Renderer::cpp3dsRenderer::DrawShavedCornerRect( Gwen::Rect rect, bool bSlight )
{
	//TODO: Implement this
	Base::DrawShavedCornerRect( rect, bSlight );
}

void Gwen::Renderer::cpp3dsRenderer::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
{
	TextureData* data = reinterpret_cast<TextureData*>( pTexture->data );

	// Missing image, not loaded properly?
	if ( !data )
		return DrawMissingImage( rect );

	const cpp3ds::Texture* tex = data->texture;

	EnsurePrimitiveType( cpp3ds::Triangles );
	EnsureTexture( tex );

	Translate( rect );

	AddVert( rect.x, rect.y, u1, v1 );
	AddVert( rect.x+rect.w, rect.y,	u2, v1 );
	AddVert( rect.x, rect.y + rect.h,	u1, v2 );

	AddVert( rect.x+rect.w, rect.y, u2, v1 );
	AddVert( rect.x+rect.w, rect.y+rect.h, u2, v2 );
	AddVert( rect.x, rect.y + rect.h, u1, v2 );
}

void Gwen::Renderer::cpp3dsRenderer::LoadFont( Gwen::Font* font )
{
	font->realsize = font->size * Scale();

	cpp3ds::Font* pFont = new cpp3ds::Font();

	if ( !pFont->loadFromFile( Utility::UnicodeToString( font->facename ) ) )
	{
		// Ideally here we should be setting the font to a system default font here.
		delete pFont;
		pFont = NULL; // SFML 2 doesn't have a default font anymore
	}

	font->data = pFont;
}

void Gwen::Renderer::cpp3dsRenderer::FreeFont( Gwen::Font* pFont )
{
	if ( !pFont->data ) return;
	cpp3ds::Font* font = reinterpret_cast<cpp3ds::Font*>( pFont->data );
	delete font;
	pFont->data = NULL;
}

void Gwen::Renderer::cpp3dsRenderer::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text )
{
	Flush();

	Translate( pos.x, pos.y );

	// If the font doesn't exist, or the font size should be changed
	if ( !pFont->data || fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
	{
		FreeFont( pFont );
		LoadFont( pFont );
	}


	cpp3ds::Text sfStr;
	sfStr.setString( text );
//	sfStr.setFont( *pSFFont );
	sfStr.useSystemFont();
	sfStr.move( pos.x, pos.y );
	sfStr.setCharacterSize( pFont->realsize );
	sfStr.setFillColor( m_Color );
	m_Target.draw( sfStr );

#ifdef _3DS
	C3D_Flush();
#endif
}

Gwen::Point Gwen::Renderer::cpp3dsRenderer::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text )
{
	// If the font doesn't exist, or the font size should be changed
	if ( !pFont->data || fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
	{
		FreeFont( pFont );
		LoadFont( pFont );
	}

	const cpp3ds::Font* pSFFont = reinterpret_cast<cpp3ds::Font*>( pFont->data );

	if ( pSFFont ) {
		cpp3ds::Text sfStr;
		sfStr.setString( text );
		sfStr.useSystemFont();

		sfStr.setCharacterSize( pFont->realsize );
		return Gwen::Point( sfStr.getLocalBounds().left + sfStr.getLocalBounds().width, pSFFont->getLineSpacing( pFont->realsize ) );
	}

	return Gwen::Point();
}

void Gwen::Renderer::cpp3dsRenderer::LoadTexture( Gwen::Texture* pTexture )
{
	if ( !pTexture ) return;
	if ( pTexture->data ) FreeTexture( pTexture );

	cpp3ds::Texture* tex = new cpp3ds::Texture();
	tex->setSmooth( true );
	if ( !tex->loadFromFile( pTexture->name.Get() ) )
	{
		delete( tex );
		pTexture->failed = true;
		return;
	}

	pTexture->height = tex->getSize().x;
	pTexture->width = tex->getSize().y;
	pTexture->data = new TextureData( tex );
}

void Gwen::Renderer::cpp3dsRenderer::FreeTexture( Gwen::Texture* pTexture )
{
	TextureData* data = reinterpret_cast<TextureData*>( pTexture->data );

	if ( data )
		delete data;

	pTexture->data = NULL;
}

Gwen::Color Gwen::Renderer::cpp3dsRenderer::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default )
{
	TextureData* data = static_cast<TextureData*>( pTexture->data );

	if ( !data->texture && !data->image ) return col_default;
	if ( !data->image )
	{
		cpp3ds::Image copy = data->texture->copyToImage();
		data->image = new cpp3ds::Image( copy );
	}

	cpp3ds::Color col = data->image->getPixel( x, y );
	return Gwen::Color( col.r, col.g, col.b, col.a );
}



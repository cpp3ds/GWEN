/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#ifndef GWEN_CPP3DS_HPP
#define GWEN_CPP3DS_HPP

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"

#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include <cpp3ds/Graphics/Texture.hpp>
#include <cpp3ds/Graphics/VertexArray.hpp>
#ifdef _3DS
#include <citro3d.h>
#endif

namespace Gwen
{
	namespace Renderer
	{
		class cpp3dsRenderer: public Gwen::Renderer::Base
		{
		public:

			cpp3dsRenderer( cpp3ds::RenderTarget& target );
			virtual ~cpp3dsRenderer();

			inline void EnsurePrimitiveType( cpp3ds::PrimitiveType type )
			{
				if ( m_Buffer.getPrimitiveType() != type )
				{
					Flush();
					m_Buffer.setPrimitiveType( type );
				}
			}

			inline void EnsureTexture( const cpp3ds::Texture *texture )
			{
				if ( m_RenderStates.texture != texture )
				{
					Flush();
					m_RenderStates.texture = texture;
				}
			}

			inline void AddVert( int x, int y, float u = 0.0f , float v = 0.0f )
			{
				if ( m_RenderStates.texture )
				{
					u *= m_RenderStates.texture->getSize().x;
					v *= m_RenderStates.texture->getSize().y;
				}
				m_Buffer.append( cpp3ds::Vertex( cpp3ds::Vector2f( x, y ), m_Color, cpp3ds::Vector2f( u, v ) ) );
			}

			inline void Flush()
			{
				if ( m_Buffer.getVertexCount() > 0 )
				{
					m_Target.draw( m_Buffer, m_RenderStates );
#ifdef _3DS
					C3D_Flush();
#endif
					m_Buffer.clear();
				}
			}

			virtual void Begin();
			virtual void End();

			virtual void StartClip();
			virtual void EndClip();

			virtual void SetDrawColor( Gwen::Color color );
			virtual void DrawPixel( int x, int y );
			virtual void DrawLinedRect( Gwen::Rect rect );
			virtual void DrawFilledRect( Gwen::Rect rect );
			virtual void DrawShavedCornerRect( Gwen::Rect rect, bool bSlight = false );
			virtual void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 );

			virtual void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
			virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );
			virtual void LoadFont( Gwen::Font* font );
			virtual void FreeFont( Gwen::Font* pFont );

			virtual void LoadTexture( Gwen::Texture* pTexture );
			virtual void FreeTexture( Gwen::Texture* pTexture );
			virtual Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default );

		protected:
			cpp3ds::RenderTarget& m_Target;
			cpp3ds::Color m_Color;
			cpp3ds::VertexArray m_Buffer;
			cpp3ds::RenderStates m_RenderStates;
			cpp3ds::View m_OriginalView;
			int m_Height;
		};
	}
}

#endif // GWEN_CPP3DS_HPP

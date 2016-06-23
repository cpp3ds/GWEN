/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_INPUT_CPP3DS_H
#define GWEN_INPUT_CPP3DS_H

#include "Gwen/InputHandler.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"

#include <cpp3ds/Window/Event.hpp>

namespace Gwen
{
	namespace Input
	{
		class cpp3dsInput
		{
		public:

			cpp3dsInput()
			{
				m_Canvas = NULL;
				m_MouseX = 0;
				m_MouseY = 0;
				m_LeftMouseDown = false;
				m_RightMouseDown = false;
				m_MiddleMouseDown = false;
				m_XButton1MouseDown = false;
				m_XButton2MouseDown = false;
			}

			void Initialize( Gwen::Controls::Canvas* c )
			{
				m_Canvas = c;
			}

			unsigned char TranslateKeyCode( int iKeyCode )
			{
				switch ( iKeyCode )
				{
//					case cpp3ds::Keyboard::BackSpace:
//						return Gwen::Key::Backspace;

//					case cpp3ds::Keyboard::Return:
//						return Gwen::Key::Return;

//					case cpp3ds::Keyboard::Escape:
//						return Gwen::Key::Escape;

//					case cpp3ds::Keyboard::Tab:
//						return Gwen::Key::Tab;

//					case cpp3ds::Keyboard::Space:
//						return Gwen::Key::Space;

					case cpp3ds::Keyboard::Up:
						return Gwen::Key::Up;

					case cpp3ds::Keyboard::Down:
						return Gwen::Key::Down;

					case cpp3ds::Keyboard::Left:
						return Gwen::Key::Left;

					case cpp3ds::Keyboard::Right:
						return Gwen::Key::Right;

//					case cpp3ds::Keyboard::Home:
//						return Gwen::Key::Home;

//					case cpp3ds::Keyboard::End:
//						return Gwen::Key::End;

//					case cpp3ds::Keyboard::Delete:
//						return Gwen::Key::Delete;

//					case cpp3ds::Keyboard::LControl:
//						return Gwen::Key::Control;

//					case cpp3ds::Keyboard::LAlt:
//						return Gwen::Key::Alt;
//
//					case cpp3ds::Keyboard::LShift:
//						return Gwen::Key::Shift;
//
//					case cpp3ds::Keyboard::RControl:
//						return Gwen::Key::Control;
//
//					case cpp3ds::Keyboard::RAlt:
//						return Gwen::Key::Alt;
//
//					case cpp3ds::Keyboard::RShift:
//						return Gwen::Key::Shift;
				}

				return Gwen::Key::Invalid;
			}

			bool ProcessMessage( const cpp3ds::Event & event )
			{
				if ( !m_Canvas ) { return false; }

				switch ( event.type )
				{
					case cpp3ds::Event::TouchMoved:
					{
						int dx = event.touch.x - m_MouseX;
						int dy = event.touch.y - m_MouseY;
						m_MouseX = event.touch.x;
						m_MouseY = event.touch.y;
						return m_Canvas->InputMouseMoved( m_MouseX, m_MouseY, dx, dy );
					}

					case cpp3ds::Event::TouchBegan:
					{
//						cpp3ds::Mouse::Button button = event.mouseButton.button;
//						if      (button == cpp3ds::Mouse::Left)     m_LeftMouseDown     = true;
//						else if (button == cpp3ds::Mouse::Right)    m_RightMouseDown    = true;
//						else if (button == cpp3ds::Mouse::Middle)   m_MiddleMouseDown   = true;
//						else if (button == cpp3ds::Mouse::XButton1) m_XButton1MouseDown = true;
//						else if (button == cpp3ds::Mouse::XButton2) m_XButton2MouseDown = true;
//						return m_Canvas->InputMouseButton( button, true );
						m_LeftMouseDown     = true;
						m_Canvas->InputMouseMoved( event.touch.x, event.touch.y, 0, 0);
						return m_Canvas->InputMouseButton( 0, true );
					}

					case cpp3ds::Event::TouchEnded:
					{
//						cpp3ds::Mouse::Button button = event.mouseButton.button;
//						if      (button == cpp3ds::Mouse::Left)     m_LeftMouseDown     = false;
//						else if (button == cpp3ds::Mouse::Right)    m_RightMouseDown    = false;
//						else if (button == cpp3ds::Mouse::Middle)   m_MiddleMouseDown   = false;
//						else if (button == cpp3ds::Mouse::XButton1) m_XButton1MouseDown = false;
//						else if (button == cpp3ds::Mouse::XButton2) m_XButton2MouseDown = false;
//						return m_Canvas->InputMouseButton( button, false );
						m_LeftMouseDown     = false;
						return m_Canvas->InputMouseButton( 0, false );
					}

//					case cpp3ds::Event::MouseLeft:
//					{
//						if (m_LeftMouseDown)     { m_LeftMouseDown     = false; m_Canvas->InputMouseButton( cpp3ds::Mouse::Left,     false ); }
//						if (m_RightMouseDown)    { m_RightMouseDown    = false; m_Canvas->InputMouseButton( cpp3ds::Mouse::Right,    false ); }
//						if (m_MiddleMouseDown)   { m_MiddleMouseDown   = false; m_Canvas->InputMouseButton( cpp3ds::Mouse::Middle,   false ); }
//						if (m_XButton1MouseDown) { m_XButton1MouseDown = false; m_Canvas->InputMouseButton( cpp3ds::Mouse::XButton1, false ); }
//						if (m_XButton2MouseDown) { m_XButton2MouseDown = false; m_Canvas->InputMouseButton( cpp3ds::Mouse::XButton2, false ); }
//						break;
//					}

//					case cpp3ds::Event::MouseWheelMoved:
//					{
//						return m_Canvas->InputMouseWheel( event.mouseWheel.delta * 60 );
//					}

//					case cpp3ds::Event::TextEntered:
//					{
//						return m_Canvas->InputCharacter( event.text.unicode );
//					}

					case cpp3ds::Event::KeyPressed:
					case cpp3ds::Event::KeyReleased:
					{
						bool bPressed = ( event.type == cpp3ds::Event::KeyPressed );
//						char keyCode = event.key.code + 97; // adding 97 here turns the cpp3ds::Keyboard::Key into a lowercase ASCII character
//						bool control = event.key.control;
//
//						if ( control && bPressed && keyCode >= 'a' && keyCode <= 'z' )
//						{
//							return m_Canvas->InputCharacter( keyCode );
//						}

//						unsigned char iKey = TranslateKeyCode( event.key.code );
//						return m_Canvas->InputKey( iKey, bPressed );
					}
					default:
						break;
				}

				return false;
			}

		protected:

			Gwen::Controls::Canvas*	m_Canvas;
			int m_MouseX;
			int m_MouseY;
			bool m_LeftMouseDown;
			bool m_RightMouseDown;
			bool m_MiddleMouseDown;
			bool m_XButton1MouseDown;
			bool m_XButton2MouseDown;

		};
	}
}
#endif

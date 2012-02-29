//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#include "BaseMenu.h"
#include "glRenderer.h"
#include "Menu.h"
#include <gl/openglut.h>
#include <vector>

BaseMenu::BaseMenu(NewMenu & menu) 
{   
    m_vecItems.push_back("Return");
    m_nSelectedItem = 0;
    str_header = "Base Menu";
    menuSystem = &menu;
}
void BaseMenu::SetSelectedItem()
{
    m_nSelectedItem = m_vecItems.size() - 1;
}
void BaseMenu::Render() 
{         
    loc[0] = MENU_START_LOCATION_X;
    loc[1] = MENU_START_LOCATION_Y;
    for( uint32 i = 0; i < m_vecItems.size(); i++ )
    {
        if(i == m_nSelectedItem )
        { 
            glRender::GetInstance()->glPrint(m_vecItems[i].c_str(), MENU_FONT_SELECTED, MENU_FONT_COLOR_SELECTED, loc );
            loc[1] += FONT_HEIGHT_INCREMENT; 
        }
        else
        {
            glRender::GetInstance()->glPrint(m_vecItems[i].c_str(), MENU_FONT, MENU_FONT_COLOR, loc );
            loc[1] += FONT_HEIGHT_INCREMENT;          
        }
    }
    loc[0] -= FONT_HEIGHT_INCREMENT + FONT_HEIGHT_INCREMENT;
    glRender::GetInstance()->glPrint(str_header.c_str(), MENU_FONT_HEADER, MENU_FONT_COLOR, loc );
}

// Base class function to handle going up and down in the menu
void BaseMenu::HandleKey( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        if( m_nSelectedItem < m_vecItems.size() - 1 )
            m_nSelectedItem++;
        break;
    case GLUT_KEY_DOWN:
        if( m_nSelectedItem > 0 )
            m_nSelectedItem--;
        break;
    }
}
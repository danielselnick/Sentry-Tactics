//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef BASEMENU_H
#define BASEMENU_H
#include "RakString.h"
#include "Shared.h"
#include <string>
#include <vector>

class NewMenu;

class BaseMenu
{
public:
    BaseMenu(NewMenu &);
    void SetSelectedItem();
    virtual void Render();
    virtual void HandleKey(int key, int x, int y);
protected:
    vector<string> m_vecItems;
    uint8 m_nSelectedItem;
    string str_header;
    vec2_f loc;
    NewMenu * menuSystem;
};

#endif // BASEMENU_H
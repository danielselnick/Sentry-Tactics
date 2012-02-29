//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef IGAMESERVICE_H
#define IGAMESERVICE_H

class IGameService
{
public:
    virtual void Render() = 0;
    virtual void Update() = 0;
    virtual void HandleKey(int key, int x, int y) = 0;
    virtual void Mouse(int button, int state, int x, int y) = 0;
};

#endif // IGAMESERVICE_H
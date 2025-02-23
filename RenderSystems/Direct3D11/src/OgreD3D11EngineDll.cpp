/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreD3D11Plugin.h"

#include "OgreRoot.h"

#ifndef OGRE_STATIC_LIB
namespace Ogre
{
    static D3D11Plugin *plugin;

#    if __cplusplus >= 201103L
    extern "C" void _OgreD3D11Export dllStartPlugin( void ) noexcept( false )
#    else
    extern "C" void _OgreD3D11Export dllStartPlugin( void ) throw( Exception )
#    endif
    {
        plugin = new D3D11Plugin();
        Root::getSingleton().installPlugin( plugin );
    }

    extern "C" void _OgreD3D11Export dllStopPlugin( void )
    {
        Root::getSingleton().uninstallPlugin( plugin );
        delete plugin;
    }
}  // namespace Ogre
#endif

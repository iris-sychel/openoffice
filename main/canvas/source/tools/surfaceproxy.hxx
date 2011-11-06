/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef INCLUDED_CANVAS_SURFACEPROXY_HXX
#define INCLUDED_CANVAS_SURFACEPROXY_HXX

#include <canvas/rendering/isurfaceproxy.hxx>
#include <canvas/rendering/icolorbuffer.hxx>

#include "pagemanager.hxx"
#include "surface.hxx"

namespace canvas
{

	//////////////////////////////////////////////////////////////////////////////////
	// SurfaceProxy
	//////////////////////////////////////////////////////////////////////////////////

	/** Definition of the surface proxy class.

	    Surface proxies are the connection between *one* source image
	    and *one or more* hardware surfaces (or textures).  in a
	    logical structure surface proxies represent soley this
	    dependeny plus some simple cache management.
    */
    class SurfaceProxy : public ISurfaceProxy
	{
    public:

        SurfaceProxy( const canvas::IColorBufferSharedPtr& pBuffer,
                      const PageManagerSharedPtr &pPageManager );

        // ISurfaceProxy interface
        virtual void setColorBufferDirty();

        /** Render the surface content to screen.

            @param fAlpha
            Overall alpha for content

            @param rPos
            Output position
	            
            @param rTransform
            Output transformation (does not affect output position)
        */
        virtual bool draw( double                         fAlpha,
                           const ::basegfx::B2DPoint&     rPos,
                           const ::basegfx::B2DHomMatrix& rTransform );

        /** Render the surface content to screen.

            @param fAlpha
            Overall alpha for content

            @param rPos
            Output position
	            
            @param rArea
            Subset of the surface to render. Coordinate system are
            surface area pixel, given area will be clipped to the
            surface bounds.
	            
            @param rTransform
            Output transformation (does not affect output position)
        */
        virtual bool draw( double                         fAlpha,
                           const ::basegfx::B2DPoint&     rPos,
                           const ::basegfx::B2DRange&     rArea,
                           const ::basegfx::B2DHomMatrix& rTransform );
	        
        /** Render the surface content to screen.

            @param fAlpha
            Overall alpha for content

            @param rPos
            Output position
	            
            @param rClipPoly
            Clip polygon for the surface. The clip polygon is also
            subject to the output transformation.
	            
            @param rTransform
            Output transformation (does not affect output position)
        */
        virtual bool draw( double                           fAlpha,
                           const ::basegfx::B2DPoint&       rPos,
                           const ::basegfx::B2DPolyPolygon& rClipPoly,
                           const ::basegfx::B2DHomMatrix&   rTransform );

    private:
        PageManagerSharedPtr          mpPageManager;

        // the pagemanager will distribute the image
        // to one or more surfaces, this is why we
        // need a list here.
        std::vector<SurfaceSharedPtr> maSurfaceList;

        // pointer to the source of image data  
        // which always is stored in system memory, 
        // 32bit rgba and can have any size.    
        canvas::IColorBufferSharedPtr mpBuffer;
	};

    typedef ::boost::shared_ptr< SurfaceProxy > SurfaceProxySharedPtr;
}

#endif

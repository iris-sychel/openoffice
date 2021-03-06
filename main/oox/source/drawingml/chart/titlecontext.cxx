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



#include "oox/drawingml/chart/titlecontext.hxx"

#include "oox/drawingml/shapepropertiescontext.hxx"
#include "oox/drawingml/textbodycontext.hxx"
#include "oox/drawingml/chart/datasourcecontext.hxx"
#include "oox/drawingml/chart/titlemodel.hxx"

namespace oox {
namespace drawingml {
namespace chart {

// ============================================================================

using ::oox::core::ContextHandler2Helper;
using ::oox::core::ContextHandlerRef;
using ::rtl::OUString;

// ============================================================================

TextContext::TextContext( ContextHandler2Helper& rParent, TextModel& rModel ) :
    ContextBase< TextModel >( rParent, rModel )
{
}

TextContext::~TextContext()
{
}

ContextHandlerRef TextContext::onCreateContext( sal_Int32 nElement, const AttributeList& )
{
    // this context handler is used for <c:tx> and embedded <c:v> elements
    if( isCurrentElement( C_TOKEN( tx ) ) ) switch( nElement )
    {
        case C_TOKEN( rich ):
            return new TextBodyContext( *this, mrModel.mxTextBody.create() );

        case C_TOKEN( strRef ):
            OSL_ENSURE( !mrModel.mxDataSeq, "TextContext::onCreateContext - multiple data sequences" );
            return new StringSequenceContext( *this, mrModel.mxDataSeq.create() );

        case C_TOKEN( v ):
            OSL_ENSURE( !mrModel.mxDataSeq, "TextContext::onCreateContext - multiple data sequences" );
            return this;    // collect value in onCharacters()
    }
    return 0;
}

void TextContext::onCharacters( const OUString& rChars )
{
    // store as single string sequence element
    if( isCurrentElement( C_TOKEN( v ) ) )
        mrModel.mxDataSeq.create().maData[ 0 ] <<= rChars;
}

// ============================================================================

TitleContext::TitleContext( ContextHandler2Helper& rParent, TitleModel& rModel ) :
    ContextBase< TitleModel >( rParent, rModel )
{
}

TitleContext::~TitleContext()
{
}

ContextHandlerRef TitleContext::onCreateContext( sal_Int32 nElement, const AttributeList& rAttribs )
{
    // this context handler is used for <c:title> only
    switch( nElement )
    {
        case C_TOKEN( layout ):
            return new LayoutContext( *this, mrModel.mxLayout.create() );

        case C_TOKEN( overlay ):
            // default is 'false', not 'true' as specified
            mrModel.mbOverlay = rAttribs.getBool( XML_val, false );
            return 0;

        case C_TOKEN( spPr ):
            return new ShapePropertiesContext( *this, mrModel.mxShapeProp.create() );

        case C_TOKEN( tx ):
            return new TextContext( *this, mrModel.mxText.create() );

        case C_TOKEN( txPr ):
            return new TextBodyContext( *this, mrModel.mxTextProp.create() );
    }
    return 0;
}

// ============================================================================

LegendContext::LegendContext( ContextHandler2Helper& rParent, LegendModel& rModel ) :
    ContextBase< LegendModel >( rParent, rModel )
{
}

LegendContext::~LegendContext()
{
}

ContextHandlerRef LegendContext::onCreateContext( sal_Int32 nElement, const AttributeList& rAttribs )
{
    // this context handler is used for <c:legend> only
    switch( nElement )
    {
        case C_TOKEN( layout ):
            return new LayoutContext( *this, mrModel.mxLayout.create() );

        case C_TOKEN( legendPos ):
            mrModel.mnPosition = rAttribs.getToken( XML_val, XML_r );
            return 0;

        case C_TOKEN( overlay ):
            // default is 'false', not 'true' as specified
            mrModel.mbOverlay = rAttribs.getBool( XML_val, false );
            return 0;

        case C_TOKEN( spPr ):
            return new ShapePropertiesContext( *this, mrModel.mxShapeProp.create() );

        case C_TOKEN( txPr ):
            return new TextBodyContext( *this, mrModel.mxTextProp.create() );
    }
    return 0;
}

// ============================================================================

} // namespace chart
} // namespace drawingml
} // namespace oox

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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_svx.hxx"

// include ---------------------------------------------------------------
#include <tools/shl.hxx>
#include <vcl/svapp.hxx>
#include <svx/xtable.hxx>
#include <svx/xpool.hxx>
#include <svx/dialogs.hrc>
#include <accessibility.hrc>
#include <svx/dlgctrl.hxx>
#include <svx/dialmgr.hxx>
#include <tools/poly.hxx>
#include <vcl/region.hxx>
#include <vcl/gradient.hxx>
#include <vcl/hatch.hxx>
#ifndef _COM_SUN_STAR_ACCESSIBILITY_ACCESSIBLEEVENTOBJECT_HPP_
#include <com/sun/star/accessibility/AccessibleEventObject.hpp>
#endif
#ifndef _COM_SUN_STAR_ACCESSIBILITY_ACCESSIBLEEVENTID_HPP_
#include <com/sun/star/accessibility/AccessibleEventId.hpp>
#endif
#ifndef _COM_SUN_STAR_ACCESSIBILITY_ACCESSIBLESTATETYPE_HPP_
#include <com/sun/star/accessibility/AccessibleStateType.hpp>
#endif
#include "svxpixelctlaccessiblecontext.hxx"
#include <svtools/colorcfg.hxx>
#include <svxrectctaccessiblecontext.hxx>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <basegfx/point/b2dpoint.hxx>
#include <basegfx/polygon/b2dpolygon.hxx>
#include <svx/svdorect.hxx>
#include <svx/svdmodel.hxx>
#include <svx/svdopath.hxx>
#include <svx/sdr/contact/objectcontactofobjlistpainter.hxx>
#include <svx/sdr/contact/displayinfo.hxx>
#include <vcl/bmpacc.hxx>
#include <svx/xbtmpit.hxx>

#define OUTPUT_DRAWMODE_COLOR		(DRAWMODE_DEFAULT)
#define OUTPUT_DRAWMODE_CONTRAST	(DRAWMODE_SETTINGSLINE | DRAWMODE_SETTINGSFILL | DRAWMODE_SETTINGSTEXT | DRAWMODE_SETTINGSGRADIENT)

using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::accessibility;


/*************************************************************************
|*
|*	Control zur Darstellung und Auswahl der Eckpunkte (und Mittelpunkt)
|*	eines Objekts
|*
\************************************************************************/

Bitmap& SvxRectCtl::GetRectBitmap( void )
{
	if( !pBitmap )
		InitRectBitmap();

	return *pBitmap;
}

SvxRectCtl::SvxRectCtl( Window* pParent, const ResId& rResId, RECT_POINT eRpt,
						sal_uInt16 nBorder, sal_uInt16 nCircle, CTL_STYLE eStyle ) :

	Control( pParent, rResId ),

	pAccContext	( NULL ),
    nBorderWidth( nBorder ),
    nRadius     ( nCircle),
    eDefRP      ( eRpt ),
    eCS         ( eStyle ),
	pBitmap		( NULL ),
    m_nState    ( 0 ),
    mbCompleteDisable(sal_False)
{
	SetMapMode( MAP_100TH_MM );
    Resize_Impl();
}

// -----------------------------------------------------------------------

SvxRectCtl::~SvxRectCtl()
{
	delete pBitmap;

	if( pAccContext )
		pAccContext->release();
}

// -----------------------------------------------------------------------
void SvxRectCtl::Resize()
{
    Resize_Impl();
    Control::Resize();
}

// -----------------------------------------------------------------------

void SvxRectCtl::Resize_Impl()
{
    aSize = GetOutputSize();

	switch( eCS )
	{
		case CS_RECT:
		case CS_ANGLE:
		case CS_SHADOW:
			aPtLT = Point( 0 + nBorderWidth,  0 + nBorderWidth );
			aPtMT = Point( aSize.Width() / 2, 0 + nBorderWidth );
			aPtRT = Point( aSize.Width() - nBorderWidth, 0 + nBorderWidth );

			aPtLM = Point( 0 + nBorderWidth,  aSize.Height() / 2 );
			aPtMM = Point( aSize.Width() / 2, aSize.Height() / 2 );
			aPtRM = Point( aSize.Width() - nBorderWidth, aSize.Height() / 2 );

			aPtLB = Point( 0 + nBorderWidth,	aSize.Height() - nBorderWidth );
			aPtMB = Point( aSize.Width() / 2,	aSize.Height() - nBorderWidth );
			aPtRB = Point( aSize.Width() - nBorderWidth, aSize.Height() - nBorderWidth );
		break;

		case CS_LINE:
			aPtLT = Point( 0 + 3 * nBorderWidth,  0 + nBorderWidth );
			aPtMT = Point( aSize.Width() / 2, 0 + nBorderWidth );
			aPtRT = Point( aSize.Width() - 3 * nBorderWidth, 0 + nBorderWidth );

			aPtLM = Point( 0 + 3 * nBorderWidth,  aSize.Height() / 2 );
			aPtMM = Point( aSize.Width() / 2, aSize.Height() / 2 );
			aPtRM = Point( aSize.Width() - 3 * nBorderWidth, aSize.Height() / 2 );

			aPtLB = Point( 0 + 3 * nBorderWidth,	aSize.Height() - nBorderWidth );
			aPtMB = Point( aSize.Width() / 2,	aSize.Height() - nBorderWidth );
			aPtRB = Point( aSize.Width() - 3 * nBorderWidth, aSize.Height() - nBorderWidth );
		break;
	}
	Reset();
	InitSettings( sal_True, sal_True );
}
// -----------------------------------------------------------------------

void SvxRectCtl::InitRectBitmap( void )
{
	if( pBitmap )
		delete pBitmap;

	const StyleSettings&	rStyles = Application::GetSettings().GetStyleSettings();
	svtools::ColorConfig aColorConfig;

	pBitmap = new Bitmap( SVX_RES( RID_SVXCTRL_RECTBTNS ) );

	// set bitmap-colors
	Color	aColorAry1[7];
	Color	aColorAry2[7];
	aColorAry1[0] = Color( 0xC0, 0xC0, 0xC0 );	// light-gray
	aColorAry1[1] = Color( 0xFF, 0xFF, 0x00 );	// yellow
	aColorAry1[2] = Color( 0xFF, 0xFF, 0xFF );	// white
	aColorAry1[3] = Color( 0x80, 0x80, 0x80 );	// dark-gray
	aColorAry1[4] = Color( 0x00, 0x00, 0x00 );  // black
	aColorAry1[5] = Color( 0x00, 0xFF, 0x00 );	// green
	aColorAry1[6] = Color( 0x00, 0x00, 0xFF );	// blue
	aColorAry2[0] = rStyles.GetDialogColor();		// background
	aColorAry2[1] = rStyles.GetWindowColor();
	aColorAry2[2] = rStyles.GetLightColor();
	aColorAry2[3] = rStyles.GetShadowColor();
	aColorAry2[4] = rStyles.GetDarkShadowColor();
	aColorAry2[5] = Color( aColorConfig.GetColorValue( svtools::FONTCOLOR ).nColor );
	aColorAry2[6] = rStyles.GetDialogColor();

#ifdef DBG_UTIL
	static sal_Bool		bModify = sal_False;
	sal_Bool&			rModify = bModify;
	if( rModify )
	{
		static int		n = 0;
		static sal_uInt8	r = 0xFF;
		static sal_uInt8	g = 0x00;
		static sal_uInt8	b = 0xFF;
		int&			rn = n;
		sal_uInt8&			rr = r;
		sal_uInt8&			rg = g;
		sal_uInt8&			rb = b;
		aColorAry2[ rn ] = Color( rr, rg, rb );
	}
#endif

	pBitmap->Replace( aColorAry1, aColorAry2, 7, NULL );
}

// -----------------------------------------------------------------------

void SvxRectCtl::InitSettings( sal_Bool bForeground, sal_Bool bBackground )
{
	const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();

	if( bForeground )
	{
		svtools::ColorConfig aColorConfig;
		Color aTextColor( aColorConfig.GetColorValue( svtools::FONTCOLOR ).nColor );

		if ( IsControlForeground() )
			aTextColor = GetControlForeground();
		SetTextColor( aTextColor );
	}

	if( bBackground )
	{
		if ( IsControlBackground() )
			SetBackground( GetControlBackground() );
		else
			SetBackground( rStyleSettings.GetWindowColor() );
	}

	delete pBitmap;
	pBitmap = NULL;		// forces new creating of bitmap

	Invalidate();
}

/*************************************************************************
|*
|*	Das angeklickte Rechteck (3 x 3) wird ermittelt und der Parent (Dialog)
|*	wird benachrichtigt, dass der Punkt geaendert wurde
|*
\************************************************************************/

void SvxRectCtl::MouseButtonDown( const MouseEvent& rMEvt )
{
	// #103516# CompletelyDisabled() added to have a disabled state for SvxRectCtl
	if(!IsCompletelyDisabled())
	{
		Point aPtLast = aPtNew;

		aPtNew = GetApproxLogPtFromPixPt( rMEvt.GetPosPixel() );

		if( aPtNew == aPtMM && ( eCS == CS_SHADOW || eCS == CS_ANGLE ) )
		{
			aPtNew = aPtLast;
		}
		else
		{
			Invalidate( Rectangle( aPtLast - Point( nRadius, nRadius ),
								   aPtLast + Point( nRadius, nRadius ) ) );
			Invalidate( Rectangle( aPtNew - Point( nRadius, nRadius ),
								   aPtNew + Point( nRadius, nRadius ) ) );
			eRP = GetRPFromPoint( aPtNew );

			SetActualRP( eRP );

			if( WINDOW_TABPAGE == GetParent()->GetType() )
				( (SvxTabPage*) GetParent() )->PointChanged( this, eRP );
		}
	}
}

// -----------------------------------------------------------------------

void SvxRectCtl::KeyInput( const KeyEvent& rKeyEvt )
{
	// #103516# CompletelyDisabled() added to have a disabled state for SvxRectCtl
	if(!IsCompletelyDisabled())
	{
		RECT_POINT eNewRP = eRP;
		sal_Bool bUseMM = (eCS != CS_SHADOW) && (eCS != CS_ANGLE);

		switch( rKeyEvt.GetKeyCode().GetCode() )
		{
			case KEY_DOWN:
			{
				if( !(m_nState & CS_NOVERT) )
					switch( eNewRP )
					{
						case RP_LT: eNewRP = RP_LM; break;
						case RP_MT: eNewRP = bUseMM ? RP_MM : RP_MB; break;
						case RP_RT: eNewRP = RP_RM; break;
						case RP_LM: eNewRP = RP_LB; break;
						case RP_MM: eNewRP = RP_MB; break;
						case RP_RM: eNewRP = RP_RB; break;
                        default: ; //prevent warning
                    }
			}
			break;
			case KEY_UP:
			{
				if( !(m_nState & CS_NOVERT) )
					switch( eNewRP )
					{
						case RP_LM: eNewRP = RP_LT; break;
						case RP_MM: eNewRP = RP_MT; break;
						case RP_RM: eNewRP = RP_RT; break;
						case RP_LB: eNewRP = RP_LM; break;
						case RP_MB: eNewRP = bUseMM ? RP_MM : RP_MT; break;
						case RP_RB: eNewRP = RP_RM; break;
                        default: ; //prevent warning
                    }
			}
			break;
			case KEY_LEFT:
			{
				if( !(m_nState & CS_NOHORZ) )
					switch( eNewRP )
					{
						case RP_MT: eNewRP = RP_LT; break;
						case RP_RT: eNewRP = RP_MT; break;
						case RP_MM: eNewRP = RP_LM; break;
						case RP_RM: eNewRP = bUseMM ? RP_MM : RP_LM; break;
						case RP_MB: eNewRP = RP_LB; break;
						case RP_RB: eNewRP = RP_MB; break;
                        default: ; //prevent warning
                    }
			}
			break;
			case KEY_RIGHT:
			{
				if( !(m_nState & CS_NOHORZ) )
					switch( eNewRP )
					{
						case RP_LT: eNewRP = RP_MT; break;
						case RP_MT: eNewRP = RP_RT; break;
						case RP_LM: eNewRP = bUseMM ? RP_MM : RP_RM; break;
						case RP_MM: eNewRP = RP_RM; break;
						case RP_LB: eNewRP = RP_MB; break;
						case RP_MB: eNewRP = RP_RB; break;
                        default: ; //prevent warning
                    }
			}
			break;
			default:
				Control::KeyInput( rKeyEvt );
				return;
		}
		if( eNewRP != eRP )
		{
			SetActualRP( eNewRP );

			if( WINDOW_TABPAGE == GetParent()->GetType() )
				( (SvxTabPage*) GetParent() )->PointChanged( this, eRP );

			SetFocusRect();
		}
	}
}

// -----------------------------------------------------------------------

void SvxRectCtl::StateChanged( StateChangedType nType )
{
	if ( nType == STATE_CHANGE_CONTROLFOREGROUND )
		InitSettings( sal_True, sal_False );
	else if ( nType == STATE_CHANGE_CONTROLBACKGROUND )
		InitSettings( sal_False, sal_True );

	Window::StateChanged( nType );
}

// -----------------------------------------------------------------------

void SvxRectCtl::DataChanged( const DataChangedEvent& rDCEvt )
{
	if ( ( rDCEvt.GetType() == DATACHANGED_SETTINGS ) && ( rDCEvt.GetFlags() & SETTINGS_STYLE ) )
		InitSettings( sal_True, sal_True );
	else
		Window::DataChanged( rDCEvt );
}

/*************************************************************************
|*
|*	Zeichnet das Control (Rechteck mit 9 Kreisen)
|*
\************************************************************************/

void SvxRectCtl::Paint( const Rectangle& )
{
	Point	aPtDiff( PixelToLogic( Point( 1, 1 ) ) );

	const StyleSettings& rStyles = Application::GetSettings().GetStyleSettings();

	SetLineColor( rStyles.GetDialogColor() );
	SetFillColor( rStyles.GetDialogColor() );
	DrawRect( Rectangle( Point(0,0), GetOutputSize() ) );

	if( IsEnabled() )
		SetLineColor( rStyles.GetLabelTextColor() );
	else
		SetLineColor( rStyles.GetShadowColor() );

	SetFillColor();

	switch( eCS )
	{

		case CS_RECT:
		case CS_SHADOW:
			if( !IsEnabled() )
			{
				Color aOldCol = GetLineColor();
				SetLineColor( rStyles.GetLightColor() );
				DrawRect( Rectangle( aPtLT + aPtDiff, aPtRB + aPtDiff ) );
				SetLineColor( aOldCol );
			}
			DrawRect( Rectangle( aPtLT, aPtRB ) );
		break;

		case CS_LINE:
			if( !IsEnabled() )
			{
				Color aOldCol = GetLineColor();
				SetLineColor( rStyles.GetLightColor() );
				DrawLine( aPtLM - Point( 2 * nBorderWidth, 0) + aPtDiff,
						  aPtRM + Point( 2 * nBorderWidth, 0 ) + aPtDiff );
				SetLineColor( aOldCol );
			}
			DrawLine( aPtLM - Point( 2 * nBorderWidth, 0),
					  aPtRM + Point( 2 * nBorderWidth, 0 ) );
		break;

		case CS_ANGLE:
			if( !IsEnabled() )
			{
				Color aOldCol = GetLineColor();
				SetLineColor( rStyles.GetLightColor() );
				DrawLine( aPtLT + aPtDiff, aPtRB + aPtDiff );
				DrawLine( aPtLB + aPtDiff, aPtRT + aPtDiff );
				DrawLine( aPtLM + aPtDiff, aPtRM + aPtDiff );
				DrawLine( aPtMT + aPtDiff, aPtMB + aPtDiff );
				SetLineColor( aOldCol );
			}
			DrawLine( aPtLT, aPtRB );
			DrawLine( aPtLB, aPtRT );
			DrawLine( aPtLM, aPtRM );
			DrawLine( aPtMT, aPtMB );
		break;

		default:
			break;
	}
	SetFillColor( GetBackground().GetColor() );

	Size aBtnSize( 11, 11 );
	Size aDstBtnSize(  PixelToLogic( aBtnSize ) );
	Point aToCenter( aDstBtnSize.Width() >> 1, aDstBtnSize.Height() >> 1);
	Point aBtnPnt1( IsEnabled()?0:22,0 );
	Point aBtnPnt2( 11,0 );
	Point aBtnPnt3( 22,0 );

	sal_Bool bNoHorz = (m_nState & CS_NOHORZ) != 0;
	sal_Bool bNoVert = (m_nState & CS_NOVERT) != 0;

	Bitmap&			rBitmap = GetRectBitmap();

	// #103516# CompletelyDisabled() added to have a disabled state for SvxRectCtl
	if(IsCompletelyDisabled())
	{
		DrawBitmap( aPtLT - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtMT - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtRT - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtLM - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		if( eCS == CS_RECT || eCS == CS_LINE )
			DrawBitmap( aPtMM - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtRM - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtLB - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtMB - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
		DrawBitmap( aPtRB - aToCenter, aDstBtnSize, aBtnPnt3, aBtnSize, rBitmap );
	}
	else
	{
		DrawBitmap( aPtLT - aToCenter, aDstBtnSize, (bNoHorz | bNoVert)?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );
		DrawBitmap( aPtMT - aToCenter, aDstBtnSize, bNoVert?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );
		DrawBitmap( aPtRT - aToCenter, aDstBtnSize, (bNoHorz | bNoVert)?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );

		DrawBitmap( aPtLM - aToCenter, aDstBtnSize, bNoHorz?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );

		// Mittelpunkt bei Rechteck und Linie
		if( eCS == CS_RECT || eCS == CS_LINE )
			DrawBitmap( aPtMM - aToCenter, aDstBtnSize, aBtnPnt1, aBtnSize, rBitmap );

		DrawBitmap( aPtRM - aToCenter, aDstBtnSize, bNoHorz?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );

		DrawBitmap( aPtLB - aToCenter, aDstBtnSize, (bNoHorz | bNoVert)?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );
		DrawBitmap( aPtMB - aToCenter, aDstBtnSize, bNoVert?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );
		DrawBitmap( aPtRB - aToCenter, aDstBtnSize, (bNoHorz | bNoVert)?aBtnPnt3:aBtnPnt1, aBtnSize, rBitmap );
	}

	// draw active button, avoid center pos for angle
	// #103516# CompletelyDisabled() added to have a disabled state for SvxRectCtl
	if(!IsCompletelyDisabled())
	{
		if( IsEnabled() && (eCS != CS_ANGLE || aPtNew != aPtMM) )
		{
			Point		aCenterPt( aPtNew );
			aCenterPt -= aToCenter;

			DrawBitmap( aCenterPt, aDstBtnSize, aBtnPnt2, aBtnSize, rBitmap );
		}
	}
}

/*************************************************************************
|*
|*	Konvertiert RECT_POINT in Point
|*
\************************************************************************/

Point SvxRectCtl::GetPointFromRP( RECT_POINT _eRP) const
{
    switch( _eRP )
	{
		case RP_LT: return aPtLT;
		case RP_MT: return aPtMT;
		case RP_RT: return aPtRT;
		case RP_LM: return aPtLM;
		case RP_MM: return aPtMM;
		case RP_RM: return aPtRM;
		case RP_LB: return aPtLB;
		case RP_MB: return aPtMB;
		case RP_RB: return aPtRB;
	}
	return( aPtMM ); // default
}


void SvxRectCtl::SetFocusRect( const Rectangle* pRect )
{
	HideFocus();

	if( pRect )
		ShowFocus( *pRect );
	else
		ShowFocus( CalculateFocusRectangle() );
}

Point SvxRectCtl::SetActualRPWithoutInvalidate( RECT_POINT eNewRP )
{
	Point aPtLast = aPtNew;
	aPtNew = GetPointFromRP( eNewRP );

	if( (m_nState & CS_NOHORZ) != 0 )
		aPtNew.X() = aPtMM.X();

	if( (m_nState & CS_NOVERT) != 0 )
		aPtNew.Y() = aPtMM.Y();

	eNewRP = GetRPFromPoint( aPtNew );

	eDefRP = eNewRP;
	eRP = eNewRP;

	return aPtLast;
}

void SvxRectCtl::GetFocus()
{
	SetFocusRect();
	//Solution: Send the accessible focused event
	Control::GetFocus();
	// Send accessibility event.
	if(pAccContext)
	{
		pAccContext->FireChildFocus(GetActualRP());		
	}
}


void SvxRectCtl::LoseFocus()
{
	HideFocus();
}


Point SvxRectCtl::GetApproxLogPtFromPixPt( const Point& rPt ) const
{
	Point	aPt = PixelToLogic( rPt );
	long	x;
	long	y;

	if( ( m_nState & CS_NOHORZ ) == 0 )
	{
		if( aPt.X() < aSize.Width() / 3 )
			x = aPtLT.X();
		else if( aPt.X() < aSize.Width() * 2 / 3 )
			x = aPtMM.X();
		else
			x = aPtRB.X();
	}
	else
		x = aPtMM.X();

	if( ( m_nState & CS_NOVERT ) == 0 )
	{
		if( aPt.Y() < aSize.Height() / 3 )
			y = aPtLT.Y();
		else if( aPt.Y() < aSize.Height() * 2 / 3 )
			y = aPtMM.Y();
		else
			y = aPtRB.Y();
	}
	else
			y = aPtMM.Y();

	return Point( x, y );
}


/*************************************************************************
|*
|*	Konvertiert Point in RECT_POINT
|*
\************************************************************************/

RECT_POINT SvxRectCtl::GetRPFromPoint( Point aPt ) const
{
	if	   ( aPt == aPtLT) return RP_LT;
	else if( aPt == aPtMT) return RP_MT;
	else if( aPt == aPtRT) return RP_RT;
	else if( aPt == aPtLM) return RP_LM;
	else if( aPt == aPtRM) return RP_RM;
	else if( aPt == aPtLB) return RP_LB;
	else if( aPt == aPtMB) return RP_MB;
	else if( aPt == aPtRB) return RP_RB;

	else
		return RP_MM; // default
}

/*************************************************************************
|*
|*	Bewirkt den Ursprungszustand des Controls
|*
\************************************************************************/

void SvxRectCtl::Reset()
{
	aPtNew = GetPointFromRP( eDefRP );
	eRP = eDefRP;
	Invalidate();
}

/*************************************************************************
|*
|*	Gibt den aktuell ausgewaehlten RECT_POINT zur�ck
|*
\************************************************************************/

RECT_POINT SvxRectCtl::GetActualRP() const
{
	return( eRP );
}

/*************************************************************************
|*
|*	Gibt den aktuell ausgewaehlten RECT_POINT zur�ck
|*
\************************************************************************/
void SvxRectCtl::SetActualRP( RECT_POINT eNewRP /* MT: , sal_Bool bFireFocus */ )
{
	// MT: bFireFox as API parameter is ugly...
	Point aPtLast( SetActualRPWithoutInvalidate( eNewRP ) );

	Invalidate( Rectangle( aPtLast - Point( nRadius, nRadius ), aPtLast + Point( nRadius, nRadius ) ) );
	Invalidate( Rectangle( aPtNew - Point( nRadius, nRadius ), aPtNew + Point( nRadius, nRadius ) ) );

	// notify accessibility object about change
	if( pAccContext )
		pAccContext->selectChild( eNewRP /* MT, bFireFocus */ );
}
void SvxRectCtl::SetState( CTL_STATE nState )
{
	m_nState = nState;

	Point aPtLast( GetPointFromRP( eRP ) );
    Point _aPtNew( aPtLast );

	if( (m_nState & CS_NOHORZ) != 0 )
        _aPtNew.X() = aPtMM.X();

	if( (m_nState & CS_NOVERT) != 0 )
        _aPtNew.Y() = aPtMM.Y();

    eRP = GetRPFromPoint( _aPtNew );
	Invalidate();

	if( WINDOW_TABPAGE == GetParent()->GetType() )
		( (SvxTabPage*) GetParent() )->PointChanged( this, eRP );
}

sal_uInt8 SvxRectCtl::GetNumOfChilds( void ) const
{
	return ( eCS == CS_ANGLE )? 8 : 9;
}

Rectangle SvxRectCtl::CalculateFocusRectangle( void ) const
{
	Size		aDstBtnSize( PixelToLogic( Size( 15, 15 ) ) );
	return Rectangle( aPtNew - Point( aDstBtnSize.Width() >> 1, aDstBtnSize.Height() >> 1 ), aDstBtnSize );
}

Rectangle SvxRectCtl::CalculateFocusRectangle( RECT_POINT eRectPoint ) const
{
	Rectangle	aRet;
	RECT_POINT	eOldRectPoint = GetActualRP();

	if( eOldRectPoint == eRectPoint )
		aRet = CalculateFocusRectangle();
	else
	{
		SvxRectCtl*	pThis = const_cast< SvxRectCtl* >( this );

		pThis->SetActualRPWithoutInvalidate( eRectPoint );		// no invalidation because it's only temporary!
		aRet = CalculateFocusRectangle();

		pThis->SetActualRPWithoutInvalidate( eOldRectPoint );	// no invalidation because nothing has changed!
	}

	return aRet;
}

Reference< XAccessible > SvxRectCtl::CreateAccessible()
{
	Window*						pParent = GetAccessibleParentWindow();

	DBG_ASSERT( pParent, "-SvxRectCtl::CreateAccessible(): No Parent!" );

	Reference< XAccessible >	xAccParent  = pParent->GetAccessible();
	if( xAccParent.is() )
	{
		pAccContext = new SvxRectCtlAccessibleContext( xAccParent, *this );
		pAccContext->acquire();

        SetActualRP( GetActualRP() );

		return pAccContext;
	}
	else
		return Reference< XAccessible >();
}

RECT_POINT SvxRectCtl::GetApproxRPFromPixPt( const ::com::sun::star::awt::Point& r ) const
{
	return GetRPFromPoint( GetApproxLogPtFromPixPt( Point( r.X, r.Y ) ) );
}

// #103516# CompletelyDisabled() added to have a disabled state for SvxRectCtl
void SvxRectCtl::DoCompletelyDisable(sal_Bool bNew) 
{ 
	mbCompleteDisable = bNew; 
	Invalidate();
}

/*************************************************************************
|*
|* Konstruktor ohne Size-Parameter
|*
\************************************************************************/

SvxAngleCtl::SvxAngleCtl( Window* pParent, const ResId& rResId ) :

	SvxRectCtl( pParent, rResId ),

	aFont( Application::GetSettings().GetStyleSettings().GetAppFont() )
{
	aFontSize = Size( 250, 400 );
	Initialize();
}

/*************************************************************************
|*
|* Konstruktor mit Size-Parameter
|*
\************************************************************************/

SvxAngleCtl::SvxAngleCtl( Window* pParent, const ResId& rResId, Size _aSize ) :

	SvxRectCtl( pParent, rResId ),

	aFont( Application::GetSettings().GetStyleSettings().GetAppFont() )
{
    aFontSize = _aSize;
	Initialize();
}

/*************************************************************************
|*
|* Initialisierung
|*
\************************************************************************/

void SvxAngleCtl::Initialize()
{
	bPositive = sal_True;

	// aFont.SetName( "Helvetica" );
	aFont.SetSize( aFontSize );
	aFont.SetWeight( WEIGHT_NORMAL );
	aFont.SetTransparent( sal_False );

	SetFont( aFont );
}

/*************************************************************************
|*
|*	Zeichnet das (Mini-)Koordinatensystem
|*
\************************************************************************/

void SvxAngleCtl::Paint( const Rectangle& )
{
	SetLineColor( Color( COL_BLACK ) ); 	// PEN_DOT ???
	DrawLine( aPtLT - Point( 0, 0), aPtRB + Point( 0, 0 ) );
	DrawLine( aPtLB - Point( 0, 0), aPtRT + Point( 0, 0 ) );

	SetLineColor( Color( COL_BLACK ) );
	DrawLine( aPtLM - Point( 0, 0), aPtRM + Point( 0, 0 ) );
	DrawLine( aPtMT - Point( 0, 0), aPtMB + Point( 0, 0 ) );

	Point aDiff(aFontSize.Width() / 2, aFontSize.Height() / 2);

	DrawText( aPtLT - aDiff, UniString::CreateFromAscii(
		                     RTL_CONSTASCII_STRINGPARAM( "135" ) ) );
	DrawText( aPtLM - aDiff, UniString::CreateFromAscii(
		                     RTL_CONSTASCII_STRINGPARAM( "180" ) ) );

	if ( bPositive )
		DrawText( aPtLB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "225" ) ) );
	else
		DrawText( aPtLB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "-135" ) ) );

	aDiff.X() = aFontSize.Width();
	DrawText( aPtMT - aDiff, UniString::CreateFromAscii(
		                     RTL_CONSTASCII_STRINGPARAM( "90" ) ) );
	DrawText( aPtRT - aDiff, UniString::CreateFromAscii(
		                     RTL_CONSTASCII_STRINGPARAM( "45" ) ) );
	aDiff.X() = aDiff .X() * 3 / 2;

	if ( bPositive )
		DrawText( aPtMB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "270" ) ) );
	else
		DrawText( aPtMB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "-90" ) ) );

	DrawText( aPtRM - Point( 0, aDiff.Y() ), UniString::CreateFromAscii(
		                                     RTL_CONSTASCII_STRINGPARAM( "0" ) ) );
	aDiff.X() = aFontSize.Width() * 2;

	if ( bPositive )
		DrawText( aPtRB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "315" ) ) );
	else
		DrawText( aPtRB - aDiff, UniString::CreateFromAscii(
		                         RTL_CONSTASCII_STRINGPARAM( "-45" ) ) );
}

/*************************************************************************
|*
|*	Control zum Editieren von Bitmaps
|*
\************************************************************************/

::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SvxPixelCtl::CreateAccessible()
{
    if(!m_xAccess.is())
	{
		m_xAccess = m_pAccess =  new SvxPixelCtlAccessible(*this);	
	}	
    return m_xAccess;
}

//Logic Pixel
long SvxPixelCtl::PointToIndex(const Point &aPt) const 
{
	sal_uInt16	nX, nY;

	nX = (sal_uInt16) ( aPt.X() * nLines / aRectSize.Width() );
	nY = (sal_uInt16) ( aPt.Y() * nLines / aRectSize.Height() );

	return nX + nY * nLines ;
}
Point SvxPixelCtl::IndexToPoint(long nIndex) const 
{
	DBG_ASSERT(nIndex >= 0 && nIndex < nSquares ," Check Index");

	sal_uInt16 nXIndex = nIndex % nLines ;
	sal_uInt16 nYIndex = sal_uInt16(nIndex / nLines) ;
	
	Point aPtTl;
	aPtTl.Y() = aRectSize.Height() * nYIndex / nLines + 1;
	aPtTl.X() = aRectSize.Width() * nXIndex / nLines + 1;

	return aPtTl;
}
long SvxPixelCtl::GetFoucsPosIndex() const 
{
	return aFocusPosition.getX() + aFocusPosition.getY() * nLines ;
}
long SvxPixelCtl::ShowPosition( const Point &pt)
{
	Point aPt = PixelToLogic( pt );
	
	sal_uInt16	nX, nY;
	nX = (sal_uInt16) ( aPt.X() * nLines / aRectSize.Width() );
	nY = (sal_uInt16) ( aPt.Y() * nLines / aRectSize.Height() );

	ChangePixel( nX + nY * nLines );

	//Solution:Set new focus position and repaint
	//Invalidate( Rectangle( aPtTl, aPtBr ) );
	aFocusPosition.setX(nX);
	aFocusPosition.setY(nY);
	Invalidate(Rectangle(Point(0,0),aRectSize));

	if( WINDOW_TABPAGE == GetParent()->GetType() )
		( (SvxTabPage*) GetParent() )->PointChanged( this, RP_MM ); // RectPoint ist dummy

	return GetFoucsPosIndex();

}
SvxPixelCtl::SvxPixelCtl( Window* pParent, const ResId& rResId, sal_uInt16 nNumber ) :
						Control		( pParent, rResId ),
						nLines		( nNumber ),
						bPaintable	( sal_True )
						//Solution:Initialize it's value to Point(0,0)
						,aFocusPosition(0,0)
{
	// SetMapMode( MAP_100TH_MM );
	aRectSize = GetOutputSize();

	SetPixelColor( Color( COL_BLACK ) );
	SetBackgroundColor( Color( COL_WHITE ) );
	SetLineColor( Application::GetSettings().GetStyleSettings().GetShadowColor() );

	nSquares = nLines * nLines;
	pPixel = new sal_uInt16[ nSquares ];
	rtl_zeroMemory(pPixel, nSquares * sizeof(sal_uInt16));
	m_pAccess=NULL;
}

/*************************************************************************
|*
|*	Destruktor dealociert dyn. Array
|*
\************************************************************************/

SvxPixelCtl::~SvxPixelCtl( )
{
	delete []pPixel;
}

/*************************************************************************
|*
|*	Wechselt die Vordergrund- ,bzw. Hintergrundfarbe
|*
\************************************************************************/

void SvxPixelCtl::ChangePixel( sal_uInt16 nPixel )
{
	if( *( pPixel + nPixel) == 0 )
		*( pPixel + nPixel) = 1; // koennte erweitert werden auf mehrere Farben
	else
		*( pPixel + nPixel) = 0;
}

/*************************************************************************
|*
|*	Das angeklickte Rechteck wird ermittelt um die Farbe zu wechseln
|*
\************************************************************************/

void SvxPixelCtl::MouseButtonDown( const MouseEvent& rMEvt )
{
	//Point aPt = PixelToLogic( rMEvt.GetPosPixel() );
	//Point aPtTl, aPtBr;
	//sal_uInt16	nX, nY;

	//nX = (sal_uInt16) ( aPt.X() * nLines / aRectSize.Width() );
	//nY = (sal_uInt16) ( aPt.Y() * nLines / aRectSize.Height() );

	//ChangePixel( nX + nY * nLines );

	//aPtTl.X() = aRectSize.Width() * nX / nLines + 1;
	//aPtBr.X() = aRectSize.Width() * (nX + 1) / nLines - 1;
	//aPtTl.Y() = aRectSize.Height() * nY / nLines + 1;
	//aPtBr.Y() = aRectSize.Height() * (nY + 1) / nLines - 1;

	//Invalidate( Rectangle( aPtTl, aPtBr ) );

	//if( WINDOW_TABPAGE == GetParent()->GetType() )
	//	( (SvxTabPage*) GetParent() )->PointChanged( this, RP_MM ); // RectPoint ist dummy
	//Solution:Grab focus when click in window
	if( !HasFocus() )
	{		
		GrabFocus();
	}
	long nIndex = ShowPosition(rMEvt.GetPosPixel());

	if(m_pAccess)
	{
		m_pAccess->NotifyChild(nIndex,sal_True,sal_True);
	}
}

/*************************************************************************
|*
|*	Zeichnet das Control (Rechteck mit 9 Kreisen)
|*
\************************************************************************/

void SvxPixelCtl::Paint( const Rectangle& )
{
	sal_uInt16	i, j, nTmp;
	Point	aPtTl, aPtBr;

	if( bPaintable )
	{
		// Linien Zeichnen
		Control::SetLineColor( aLineColor );
		for( i = 1; i < nLines; i++)
		{
			// horizontal
			nTmp = (sal_uInt16) ( aRectSize.Height() * i / nLines );
			DrawLine( Point( 0, nTmp ), Point( aRectSize.Width(), nTmp ) );
			// vertikal
			nTmp = (sal_uInt16) ( aRectSize.Width() * i / nLines );
			DrawLine( Point( nTmp, 0 ), Point( nTmp, aRectSize.Height() ) );
		}

		// Rechtecke (Quadrate) zeichnen
		Control::SetLineColor();
		sal_uInt16 nLastPixel = *pPixel ? 0 : 1;

		for( i = 0; i < nLines; i++)
		{
			aPtTl.Y() = aRectSize.Height() * i / nLines + 1;
			aPtBr.Y() = aRectSize.Height() * (i + 1) / nLines - 1;

			for( j = 0; j < nLines; j++)
			{
				aPtTl.X() = aRectSize.Width() * j / nLines + 1;
				aPtBr.X() = aRectSize.Width() * (j + 1) / nLines - 1;

				if ( *( pPixel + i * nLines + j ) != nLastPixel )
				{
					nLastPixel = *( pPixel + i * nLines + j );
					// Farbe wechseln: 0 -> Hintergrundfarbe
					SetFillColor( nLastPixel ? aPixelColor : aBackgroundColor );
				}
				DrawRect( Rectangle( aPtTl, aPtBr ) );
			}
		}
		//Solution:Draw visual focus when has focus
		if( HasFocus() )
		{
			ShowFocus(implCalFocusRect(aFocusPosition));	
		}
	} // bPaintable
	else
	{
		SetBackground( Wallpaper( Color( COL_LIGHTGRAY ) ) );
		Control::SetLineColor( Color( COL_LIGHTRED ) );
		DrawLine( Point( 0, 0 ), Point( aRectSize.Width(), aRectSize.Height() ) );
		DrawLine( Point( 0, aRectSize.Height() ), Point( aRectSize.Width(), 0 ) );
	}
}
//Solution:Caculate visual focus rectangle via focus position
Rectangle SvxPixelCtl::implCalFocusRect( const Point& aPosition )
{
	long nLeft,nTop,nRight,nBottom;
	long i,j;
	i = aPosition.Y();
	j = aPosition.X();
	nLeft = aRectSize.Width() * j / nLines + 1;
	nRight = aRectSize.Width() * (j + 1) / nLines - 1;
	nTop = aRectSize.Height() * i / nLines + 1;
	nBottom = aRectSize.Height() * (i + 1) / nLines - 1;
	return Rectangle(nLeft,nTop,nRight,nBottom);
}

//Solution:Keyboard function
void SvxPixelCtl::KeyInput( const KeyEvent& rKEvt )
{
	KeyCode aKeyCode = rKEvt.GetKeyCode();
	sal_uInt16 nCode = aKeyCode.GetCode();
	sal_Bool bIsMod = aKeyCode.IsShift() || aKeyCode.IsMod1() || aKeyCode.IsMod2();
						
	if( !bIsMod )
	{
		Point pRepaintPoint( aRectSize.Width() *( aFocusPosition.getX() - 1)/ nLines - 1,
		                     aRectSize.Height() *( aFocusPosition.getY() - 1)/ nLines -1
		                    );
		Size  mRepaintSize( aRectSize.Width() *3/ nLines + 2,aRectSize.Height() *3/ nLines + 2);
		Rectangle mRepaintRect( pRepaintPoint, mRepaintSize );			
		sal_Bool bFocusPosChanged=sal_False;
		switch(nCode)
		{
			case KEY_LEFT:
				if((aFocusPosition.getX() >= 1))
				{
					aFocusPosition.setX( aFocusPosition.getX() - 1 );
					Invalidate(mRepaintRect);
					bFocusPosChanged=sal_True;
				}
				break;
			case KEY_RIGHT:
				if( aFocusPosition.getX() < (nLines - 1) )
				{
					aFocusPosition.setX( aFocusPosition.getX() + 1 );
					Invalidate(mRepaintRect);
					bFocusPosChanged=sal_True;
				}
				break;					
			case KEY_UP:
				if((aFocusPosition.getY() >= 1))
				{
					aFocusPosition.setY( aFocusPosition.getY() - 1 );
					Invalidate(mRepaintRect);
					bFocusPosChanged=sal_True;
				}
				break;
			case KEY_DOWN:
				if( aFocusPosition.getY() < ( nLines - 1 ) )
				{
					aFocusPosition.setY( aFocusPosition.getY() + 1 );
					Invalidate(mRepaintRect);
					bFocusPosChanged=sal_True;
				}
				break;
			case KEY_SPACE:
				ChangePixel( sal_uInt16(aFocusPosition.getX() + aFocusPosition.getY() * nLines) );
				Invalidate( implCalFocusRect(aFocusPosition) );
				break;
			default:
				Control::KeyInput( rKEvt );		
				return;
		}
		if(m_xAccess.is())
		{
			long nIndex = GetFoucsPosIndex();
			switch(nCode)
			{
			case KEY_LEFT:
			case KEY_RIGHT:
			case KEY_UP:
			case KEY_DOWN:
				if (bFocusPosChanged)
				{
					m_pAccess->NotifyChild(nIndex,sal_False,sal_False);
				}				
				break;
			case KEY_SPACE:
				m_pAccess->NotifyChild(nIndex,sal_False,sal_True);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		Control::KeyInput( rKEvt );
	}
}

//Draw focus when get focus
void SvxPixelCtl::GetFocus()
{
	Invalidate(implCalFocusRect(aFocusPosition));
	
	if(m_pAccess)
	{
		m_pAccess->NotifyChild(GetFoucsPosIndex(),sal_True,sal_False);
	}

	Control::GetFocus();
}

//Hide focus when lose focus
void SvxPixelCtl::LoseFocus()
{
	HideFocus();
	if (m_pAccess)
	{
		m_pAccess->LoseFocus();
	}
	Control::LoseFocus();
}

/*************************************************************************
|*
|*
|*
\************************************************************************/

void SvxPixelCtl::SetXBitmap( const BitmapEx& rBitmapEx )
{
    BitmapColor aBack;
    BitmapColor aFront;

    if(isHistorical8x8(rBitmapEx, aBack, aFront))
    {
        Bitmap aBitmap(rBitmapEx.GetBitmap());
        BitmapReadAccess* pRead = aBitmap.AcquireReadAccess();

        aBackgroundColor = aBack;
        aPixelColor = aFront;

        for(sal_uInt16 i(0); i < nSquares; i++)
        {
            const BitmapColor aColor(pRead->GetColor(i/8, i%8));

            if(aColor == aBack)
            {
                *( pPixel + i ) = 0;
            }
            else
            {
                *( pPixel + i ) = 1;
            }
        }

        aBitmap.ReleaseAccess(pRead);
    }
}

/*************************************************************************
|*
|*	Gibt ein bestimmtes Pixel zurueck
|*
\************************************************************************/

sal_uInt16 SvxPixelCtl::GetBitmapPixel( const sal_uInt16 nPixel )
{
	return( *( pPixel + nPixel ) );
}

/*************************************************************************
|*
|*	Bewirkt den Ursprungszustand des Controls
|*
\************************************************************************/

void SvxPixelCtl::Reset()
{
	// clear pixel area
	rtl_zeroMemory(pPixel, nSquares * sizeof(sal_uInt16));
	Invalidate();
}

/*************************************************************************
|*
|*	Ctor: BitmapCtl fuer SvxPixelCtl
|*
\************************************************************************/

SvxBitmapCtl::SvxBitmapCtl( Window* /*pParent*/, const Size& rSize )
{
	aSize = rSize;
	// aVD.SetOutputSizePixel( aSize );
}

/*************************************************************************
|*
|*	Dtor
|*
\************************************************************************/

SvxBitmapCtl::~SvxBitmapCtl()
{
}

/*************************************************************************
|*
|*	BitmapCtl:	Gibt die Bitmap zurueck
|*
\************************************************************************/

BitmapEx SvxBitmapCtl::GetBitmapEx()
{
    const Bitmap aRetval(createHistorical8x8FromArray(pBmpArray, aPixelColor, aBackgroundColor));

	return BitmapEx(aRetval);
}

/*************************************************************************
|*
|*	Fuellt die Listbox mit Farben und Strings
|*
\************************************************************************/

void ColorLB::Fill( const XColorListSharedPtr aColorTab )
{
	long nCount = aColorTab->Count();
	XColorEntry* pEntry;
	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aColorTab->GetColor( i );
		InsertEntry( pEntry->GetColor(), pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/************************************************************************/

void ColorLB::Append( const XColorEntry& rEntry )
{
    InsertEntry( rEntry.GetColor(), rEntry.GetName() );
    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void ColorLB::Modify( const XColorEntry& rEntry, sal_uInt16 nPos )
{
    RemoveEntry( nPos );
    InsertEntry( rEntry.GetColor(), rEntry.GetName(), nPos );
}

/*************************************************************************
|*
|*	Fuellt die Listbox mit Farben und Strings
|*
\************************************************************************/

void FillAttrLB::Fill( const XColorListSharedPtr aColorTab )
{
	long nCount = aColorTab->Count();
	XColorEntry* pEntry;
	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aColorTab->GetColor( i );
		InsertEntry( pEntry->GetColor(), pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
    SetUpdateMode( sal_True );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/

HatchingLB::HatchingLB( Window* pParent, ResId Id)
: ListBox( pParent, Id )
{
    SetEdgeBlending(true);
}

HatchingLB::HatchingLB( Window* pParent, WinBits aWB)
: ListBox( pParent, aWB )
{
    SetEdgeBlending(true);
}

void HatchingLB::Fill( const XHatchListSharedPtr aList )
{
	XHatchEntry* pEntry;
	long nCount = aList.get() ? aList->Count() : 0;

	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetHatch( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
			InsertEntry( pEntry->GetName(), aBitmap );
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/************************************************************************/

void HatchingLB::Append( const XHatchEntry& rEntry, const Bitmap& rBitmap )
{
    if(!rBitmap.IsEmpty())
    {
        InsertEntry( rEntry.GetName(), rBitmap );
    }
    else
    {
        InsertEntry( rEntry.GetName() );
    }

    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void HatchingLB::Modify( const XHatchEntry& rEntry, sal_uInt16 nPos, const Bitmap& rBitmap )
{
    RemoveEntry( nPos );

    if( !rBitmap.IsEmpty() )
    {
        InsertEntry( rEntry.GetName(), rBitmap, nPos );
    }
    else
    {
        InsertEntry( rEntry.GetName(), nPos );
    }
}

/************************************************************************/

void HatchingLB::SelectEntryByList( const XHatchListSharedPtr aList, const String& rStr, const XHatch& rHatch, sal_uInt16 nDist )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XHatchEntry* pEntry;
	sal_Bool bFound = sal_False;
	String aStr;

	long i;
	for( i = 0; i < nCount && !bFound; i++ )
	{
        pEntry = aList->GetHatch( i );

		aStr = pEntry->GetName();

		if( rStr == aStr && rHatch == pEntry->GetHatch() )
			bFound = sal_True;
	}
	if( bFound )
		SelectEntryPos( (sal_uInt16) ( i - 1 + nDist ) );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/

void FillAttrLB::Fill( const XHatchListSharedPtr aList )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XHatchEntry* pEntry;
	ListBox::SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetHatch( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
			ListBox::InsertEntry( pEntry->GetName(), aBitmap );
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	ListBox::SetUpdateMode( sal_True );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/

GradientLB::GradientLB( Window* pParent, ResId Id)
: ListBox( pParent, Id )
{
    SetEdgeBlending(true);
}

GradientLB::GradientLB( Window* pParent, WinBits aWB)
: ListBox( pParent, aWB )
{
    SetEdgeBlending(true);
}

void GradientLB::Fill( const XGradientListSharedPtr aList )
{
	XGradientEntry* pEntry;
	long nCount = aList.get() ? aList->Count() : 0;

	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetGradient( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
			InsertEntry( pEntry->GetName(), aBitmap );
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/************************************************************************/

void GradientLB::Append( const XGradientEntry& rEntry, const Bitmap& rBitmap )
{
    if(!rBitmap.IsEmpty())
    {
        InsertEntry( rEntry.GetName(), rBitmap );
    }
    else
    {
        InsertEntry( rEntry.GetName() );
    }

    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void GradientLB::Modify( const XGradientEntry& rEntry, sal_uInt16 nPos, const Bitmap& rBitmap )
{
    RemoveEntry( nPos );

    if(!rBitmap.IsEmpty())
    {
        InsertEntry( rEntry.GetName(), rBitmap, nPos );
    }
    else
    {
        InsertEntry( rEntry.GetName(), nPos );
    }
}

/************************************************************************/

void GradientLB::SelectEntryByList( const XGradientListSharedPtr aList, const String& rStr, const XGradient& rGradient, sal_uInt16 nDist )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XGradientEntry* pEntry;
	sal_Bool bFound = sal_False;
	String aStr;

	long i;
	for( i = 0; i < nCount && !bFound; i++ )
	{
        pEntry = aList->GetGradient( i );

		aStr = pEntry->GetName();

		if( rStr == aStr && rGradient == pEntry->GetGradient() )
			bFound = sal_True;
	}
	if( bFound )
		SelectEntryPos( (sal_uInt16) ( i - 1 + nDist ) );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/

void FillAttrLB::Fill( const XGradientListSharedPtr aList )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XGradientEntry* pEntry;
	ListBox::SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetGradient( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
			ListBox::InsertEntry( pEntry->GetName(), aBitmap );
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	ListBox::SetUpdateMode( sal_True );
}

/*************************************************************************
|*
|*	Konstruktor von BitmapLB
|*
\************************************************************************/

BitmapLB::BitmapLB(Window* pParent, ResId Id)
:   ListBox(pParent, Id),
    maBitmapEx()
{
    SetEdgeBlending(true);
}

/************************************************************************/

namespace
{
    void formatBitmapExToSize(BitmapEx& rBitmapEx, const Size& rSize)
    {
        if(!rBitmapEx.IsEmpty() && rSize.Width() > 0 && rSize.Height() > 0)
        {
            VirtualDevice aVirtualDevice;
            aVirtualDevice.SetOutputSizePixel(rSize);

            if(rBitmapEx.IsTransparent())
            {
                const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();

                if(rStyleSettings.GetPreviewUsesCheckeredBackground())
                {
                    const Point aNull(0, 0);
                    static const sal_uInt32 nLen(8);
                    static const Color aW(COL_WHITE);
                    static const Color aG(0xef, 0xef, 0xef);

                    aVirtualDevice.DrawCheckered(aNull, rSize, nLen, aW, aG);
                }
                else
                {
                    aVirtualDevice.SetBackground(rStyleSettings.GetFieldColor());
                    aVirtualDevice.Erase();
                }
            }

            if(rBitmapEx.GetSizePixel().Width() >= rSize.Width() && rBitmapEx.GetSizePixel().Height() >= rSize.Height())
            {
                static sal_uInt32 nScaleFlag(BMP_SCALE_FASTESTINTERPOLATE);

                rBitmapEx.Scale(rSize, nScaleFlag);
                aVirtualDevice.DrawBitmapEx(Point(0, 0), rBitmapEx);
            }
            else
            {
                const Size aBitmapSize(rBitmapEx.GetSizePixel());

                for(sal_Int32 y(0); y < rSize.Height(); y += aBitmapSize.Height())
                {
                    for(sal_Int32 x(0); x < rSize.Width(); x += aBitmapSize.Width())
                    {
                        aVirtualDevice.DrawBitmapEx(
                            Point(x, y),
                            rBitmapEx);
                    }
                }
            }

            rBitmapEx = aVirtualDevice.GetBitmap(Point(0, 0), rSize);
        }
    }
} // end of anonymous namespace

/************************************************************************/

void BitmapLB::Fill(const XBitmapListSharedPtr aList)
{
	XBitmapEntry* pEntry;
	const long nCount(aList.get() ? aList->Count() : 0);
    const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();
    const Size aSize(rStyleSettings.GetListBoxPreviewDefaultPixelSize());

	SetUpdateMode(false);

	for(long i(0); i < nCount; i++)
	{
        pEntry = aList->GetBitmap(i);
        maBitmapEx = pEntry->GetGraphicObject().GetGraphic().GetBitmapEx();
        formatBitmapExToSize(maBitmapEx, aSize);
        InsertEntry(pEntry->GetName(), maBitmapEx);
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode(true);
}

/************************************************************************/

void BitmapLB::Append(const Size& rSize, const XBitmapEntry& rEntry)
{
    maBitmapEx = rEntry.GetGraphicObject().GetGraphic().GetBitmapEx();

    if(!maBitmapEx.IsEmpty())
    {
        formatBitmapExToSize(maBitmapEx, rSize);
        InsertEntry(rEntry.GetName(), maBitmapEx);
    }
    else
    {
        InsertEntry(rEntry.GetName());
    }

    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void BitmapLB::Modify(const Size& rSize, const XBitmapEntry& rEntry, sal_uInt16 nPos)
{
    RemoveEntry(nPos);
    maBitmapEx = rEntry.GetGraphicObject().GetGraphic().GetBitmapEx();

    if(!maBitmapEx.IsEmpty())
    {
        formatBitmapExToSize(maBitmapEx, rSize);
        InsertEntry(rEntry.GetName(), maBitmapEx, nPos);
    }
    else
    {
        InsertEntry(rEntry.GetName());
    }
}

/************************************************************************/

void BitmapLB::SelectEntryByList(const XBitmapListSharedPtr aList, const String& rStr)
{
	const long nCount(aList.get() ? aList->Count() : 0);
	XBitmapEntry* pEntry;
	bool bFound(false);
    long i(0);

	for(i = 0; i < nCount && !bFound; i++)
	{
        pEntry = aList->GetBitmap(i);
		const String aStr(pEntry->GetName());

		if(rStr == aStr)
		{
			bFound = true;
		}
	}

    if(bFound)
    {
		SelectEntryPos((sal_uInt16)(i - 1));
    }
}

/*************************************************************************
|*
|*	Konstruktor von FillAttrLB
|*
\************************************************************************/

FillAttrLB::FillAttrLB( Window* pParent, ResId Id ) 
:   ColorListBox(pParent, Id),
    maBitmapEx()
{
}

/************************************************************************/

FillAttrLB::FillAttrLB(Window* pParent, WinBits aWB) 
:   ColorListBox(pParent, aWB)
{
}

/************************************************************************/

void FillAttrLB::Fill(const XBitmapListSharedPtr aList)
{
	const long nCount(aList.get() ? aList->Count() : 0);
	XBitmapEntry* pEntry;
    const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();
    const Size aSize(rStyleSettings.GetListBoxPreviewDefaultPixelSize());
	
    ListBox::SetUpdateMode(false);

	for(long i(0); i < nCount; i++)
	{
        pEntry = aList->GetBitmap( i );
		maBitmapEx = pEntry->GetGraphicObject().GetGraphic().GetBitmapEx();
        formatBitmapExToSize(maBitmapEx, aSize);
		ListBox::InsertEntry(pEntry->GetName(), maBitmapEx);
	}

    AdaptDropDownLineCountToMaximum();
    ListBox::SetUpdateMode(true);
}

/************************************************************************/

void FillAttrLB::SelectEntryByList( const XBitmapListSharedPtr aList, const String& rStr)
{
	const long nCount(aList.get() ? aList->Count() : 0);
	XBitmapEntry* pEntry;
	bool bFound(false);
	long i(0);

    for(i = 0; i < nCount && !bFound; i++)
	{
        pEntry = aList->GetBitmap(i);
		const String aStr(pEntry->GetName());

		if(rStr == aStr)
		{
			bFound = true;
		}
	}

    if(bFound)
    {
		SelectEntryPos((sal_uInt16)(i - 1));
    }
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/

void FillTypeLB::Fill()
{
	SetUpdateMode( sal_False );

    InsertEntry( String( SVX_RES( RID_SVXSTR_INVISIBLE ) ) );
	InsertEntry( String( SVX_RES( RID_SVXSTR_COLOR ) ) );
	InsertEntry( String( SVX_RES( RID_SVXSTR_GRADIENT ) ) );
	InsertEntry( String( SVX_RES( RID_SVXSTR_HATCH ) ) );
	InsertEntry( String( SVX_RES( RID_SVXSTR_BITMAP ) ) );

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/
LineLB::LineLB(Window* pParent, ResId Id) 
:   ListBox(pParent, Id),
    mbAddStandardFields(true)
{
    // No EdgeBlending for LineStyle/Dash SetEdgeBlending(true);
}

LineLB::LineLB(Window* pParent, WinBits aWB) 
:   ListBox(pParent, aWB),
    mbAddStandardFields(true)
{
    // No EdgeBlending for LineStyle/Dash SetEdgeBlending(true);
}

LineLB::~LineLB()
{
}

void LineLB::setAddStandardFields(bool bNew)
{
    if(getAddStandardFields() != bNew)
    {
        mbAddStandardFields = bNew;
    }
}

void LineLB::Fill( const XDashListSharedPtr aList )
{
	Clear();

    if(getAddStandardFields() && aList.get())
    {
        // entry for 'none'
        InsertEntry(aList->GetStringForUiNoLine());

        // entry for solid line
        InsertEntry(aList->GetStringForUiSolidLine(), aList->GetBitmapForUISolidLine());
    }

    // entries for dashed lines
	long nCount = aList.get() ? aList->Count() : 0;
	XDashEntry* pEntry;
	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetDash( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
		{
			InsertEntry( pEntry->GetName(), aBitmap );
			//delete pBitmap;
		}
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/************************************************************************/

void LineLB::Append( const XDashEntry& rEntry, const Bitmap& rBitmap )
{
    if(!rBitmap.IsEmpty())
    {
        InsertEntry( rEntry.GetName(), rBitmap );
    }
    else
    {
        InsertEntry( rEntry.GetName() );
    }

    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void LineLB::Modify( const XDashEntry& rEntry, sal_uInt16 nPos, const Bitmap& rBitmap )
{
    RemoveEntry( nPos );

    if(!rBitmap.IsEmpty())
    {
        InsertEntry( rEntry.GetName(), rBitmap, nPos );
    }
    else
    {
        InsertEntry( rEntry.GetName(), nPos );
    }
}

/************************************************************************/

void LineLB::SelectEntryByList( const XDashListSharedPtr aList, const String& rStr, const XDash& rDash, sal_uInt16 nDist )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XDashEntry* pEntry;
	sal_Bool bFound = sal_False;
	String aStr;
	XDash aDash;

	long i;
	for( i = 0; i < nCount && !bFound; i++ )
	{
        pEntry = aList->GetDash( i );

		aStr = pEntry->GetName();
		aDash = pEntry->GetDash();

		if( rStr == aStr && rDash == aDash )
			bFound = sal_True;
	}
	if( bFound )
		SelectEntryPos( (sal_uInt16) ( i - 1 + nDist ) );
}

/*************************************************************************
|*
|*	Fuellt die Listbox (vorlaeufig) mit Strings
|*
\************************************************************************/
LineEndLB::LineEndLB( Window* pParent, ResId Id )
    : ListBox( pParent, Id )
{
    // No EdgeBlending for LineEnds SetEdgeBlending(true);
}

LineEndLB::LineEndLB( Window* pParent, WinBits aWB )
    : ListBox( pParent, aWB )
{
    // No EdgeBlending for LineEnds SetEdgeBlending(true);
}

LineEndLB::~LineEndLB(void)
{
}


void LineEndLB::Fill( const XLineEndListSharedPtr aList, bool bStart )
{
	long nCount = aList.get() ? aList->Count() : 0;
	XLineEndEntry* pEntry;
	VirtualDevice aVD;
	SetUpdateMode( sal_False );

	for( long i = 0; i < nCount; i++ )
	{
        pEntry = aList->GetLineEnd( i );
		const Bitmap aBitmap = aList->GetUiBitmap( i );
		if( !aBitmap.IsEmpty() )
		{
			Size aBmpSize( aBitmap.GetSizePixel() );
			aVD.SetOutputSizePixel( aBmpSize, sal_False );
			aVD.DrawBitmap( Point(), aBitmap );
			InsertEntry( pEntry->GetName(),
				aVD.GetBitmap( bStart ? Point() : Point( aBmpSize.Width() / 2, 0 ),
					Size( aBmpSize.Width() / 2, aBmpSize.Height() ) ) );
			//delete pBitmap;
		}
		else
			InsertEntry( pEntry->GetName() );
	}

    AdaptDropDownLineCountToMaximum();
	SetUpdateMode( sal_True );
}

/************************************************************************/

void LineEndLB::Append( const XLineEndEntry& rEntry, const Bitmap& rBitmap, bool bStart )
{
    if(!rBitmap.IsEmpty())
    {
        VirtualDevice aVD;
        const Size aBmpSize(rBitmap.GetSizePixel());

        aVD.SetOutputSizePixel(aBmpSize, false);
        aVD.DrawBitmap(Point(), rBitmap);
        InsertEntry(
            rEntry.GetName(),
            aVD.GetBitmap(bStart ? Point() : Point(aBmpSize.Width() / 2, 0 ), Size(aBmpSize.Width() / 2, aBmpSize.Height())));
    }
    else
    {
        InsertEntry(rEntry.GetName());
    }

    AdaptDropDownLineCountToMaximum();
}

/************************************************************************/

void LineEndLB::Modify( const XLineEndEntry& rEntry, sal_uInt16 nPos, const Bitmap& rBitmap, bool bStart )
{
    RemoveEntry( nPos );

    if(!rBitmap.IsEmpty())
    {
        VirtualDevice aVD;
        const Size aBmpSize(rBitmap.GetSizePixel());

        aVD.SetOutputSizePixel(aBmpSize, false);
        aVD.DrawBitmap(Point(), rBitmap);
        InsertEntry( 
            rEntry.GetName(),
            aVD.GetBitmap(bStart ? Point() : Point( aBmpSize.Width() / 2, 0 ), Size( aBmpSize.Width() / 2, aBmpSize.Height())), 
            nPos);
    }
    else
    {
        InsertEntry(rEntry.GetName(), nPos);
    }
}

//////////////////////////////////////////////////////////////////////////////

void SvxPreviewBase::InitSettings(bool bForeground, bool bBackground)
{
	const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();

	if(bForeground)
	{
		svtools::ColorConfig aColorConfig;
		Color aTextColor(aColorConfig.GetColorValue(svtools::FONTCOLOR).nColor);

		if(IsControlForeground())
        {
			aTextColor = GetControlForeground();
        }

		getBufferDevice().SetTextColor(aTextColor);
	}

	if(bBackground)
	{
		if(IsControlBackground())
        {
			getBufferDevice().SetBackground(GetControlBackground());
        }
		else
        {
			getBufferDevice().SetBackground(rStyleSettings.GetWindowColor());
        }
	}

    // do not paint background self, it gets painted buffered
	SetControlBackground();
    SetBackground();

    Invalidate();
}

SvxPreviewBase::SvxPreviewBase( Window* pParent, const ResId& rResId )
:	Control( pParent, rResId ),
	mpModel( new SdrModel() ),
    mpBufferDevice( new VirtualDevice(*this) )
{
	//	Draw the control's border as a flat thin black line.
    SetBorderStyle(WINDOW_BORDER_MONO);
    SetDrawMode( GetSettings().GetStyleSettings().GetHighContrastMode() ? OUTPUT_DRAWMODE_CONTRAST : OUTPUT_DRAWMODE_COLOR );
    SetMapMode(MAP_100TH_MM);

    // init model
	mpModel->GetItemPool().FreezeIdRanges();
}

SvxPreviewBase::~SvxPreviewBase()
{
	delete mpModel;
    delete mpBufferDevice;
}

void SvxPreviewBase::LocalPrePaint()
{
    // init BufferDevice
    if(mpBufferDevice->GetOutputSizePixel() != GetOutputSizePixel())
    {
        mpBufferDevice->SetDrawMode(GetDrawMode());
        mpBufferDevice->SetSettings(GetSettings());
        mpBufferDevice->SetAntialiasing(GetAntialiasing());
        mpBufferDevice->SetOutputSizePixel(GetOutputSizePixel());
	    mpBufferDevice->SetMapMode(GetMapMode());
    }

    const StyleSettings& rStyleSettings = Application::GetSettings().GetStyleSettings();

    if(rStyleSettings.GetPreviewUsesCheckeredBackground())
    {
        const Point aNull(0, 0);
        static const sal_uInt32 nLen(8);
        static const Color aW(COL_WHITE);
        static const Color aG(0xef, 0xef, 0xef);
        const bool bWasEnabled(mpBufferDevice->IsMapModeEnabled());

        mpBufferDevice->EnableMapMode(false);
        mpBufferDevice->DrawCheckered(aNull, mpBufferDevice->GetOutputSizePixel(), nLen, aW, aG);
        mpBufferDevice->EnableMapMode(bWasEnabled);
    }
    else
    {
        mpBufferDevice->Erase();
    }
}

void SvxPreviewBase::LocalPostPaint()
{
    // copy to front (in pixel mode)
	const bool bWasEnabledSrc(mpBufferDevice->IsMapModeEnabled());
	const bool bWasEnabledDst(IsMapModeEnabled());
	const Point aEmptyPoint;
	
    mpBufferDevice->EnableMapMode(false);
    EnableMapMode(false);
	
    DrawOutDev(
        aEmptyPoint, GetOutputSizePixel(), 
        aEmptyPoint, GetOutputSizePixel(), 
        *mpBufferDevice);
	
    mpBufferDevice->EnableMapMode(bWasEnabledSrc);
    EnableMapMode(bWasEnabledDst);
}

void SvxPreviewBase::StateChanged(StateChangedType nType)
{
	Control::StateChanged(nType);

    if(STATE_CHANGE_CONTROLFOREGROUND == nType)
    {
		InitSettings(true, false);
    }
	else if(STATE_CHANGE_CONTROLBACKGROUND == nType)
    {
		InitSettings(false, true);
    }
}

void SvxPreviewBase::DataChanged(const DataChangedEvent& rDCEvt)
{
    SetDrawMode(GetSettings().GetStyleSettings().GetHighContrastMode() ? OUTPUT_DRAWMODE_CONTRAST : OUTPUT_DRAWMODE_COLOR);

    if((DATACHANGED_SETTINGS == rDCEvt.GetType()) && (rDCEvt.GetFlags() & SETTINGS_STYLE))
    {
		InitSettings(true, true);
    }
	else
    {
		Control::DataChanged(rDCEvt);
    }
}

/*************************************************************************
|*
|*    SvxXLinePreview::SvxXLinePreview()
|*
*************************************************************************/

SvxXLinePreview::SvxXLinePreview( Window* pParent, const ResId& rResId )
:	SvxPreviewBase( pParent, rResId ),
	mpLineObjA( 0L ),
	mpLineObjB( 0L ),
	mpLineObjC( 0L ),
	mpGraphic( 0L ),
	mbWithSymbol( sal_False )
{
	const Size aOutputSize(GetOutputSize());
	InitSettings( sal_True, sal_True );

	const sal_Int32 nDistance(500L);
	const sal_Int32 nAvailableLength(aOutputSize.Width() - (4 * nDistance));

	// create DrawObectA
	const sal_Int32 aYPosA(aOutputSize.Height() / 2);
	const basegfx::B2DPoint aPointA1( nDistance,  aYPosA);
	const basegfx::B2DPoint aPointA2( aPointA1.getX() + ((nAvailableLength * 14) / 20), aYPosA );
	basegfx::B2DPolygon aPolygonA;
	aPolygonA.append(aPointA1);
	aPolygonA.append(aPointA2);
	mpLineObjA = new SdrPathObj(OBJ_LINE, basegfx::B2DPolyPolygon(aPolygonA));
	mpLineObjA->SetModel(&getModel());

	// create DrawObectB
	const sal_Int32 aYPosB1((aOutputSize.Height() * 3) / 4);
	const sal_Int32 aYPosB2((aOutputSize.Height() * 1) / 4);
	const basegfx::B2DPoint aPointB1( aPointA2.getX() + nDistance,  aYPosB1);
	const basegfx::B2DPoint aPointB2( aPointB1.getX() + ((nAvailableLength * 2) / 20), aYPosB2 );
	const basegfx::B2DPoint aPointB3( aPointB2.getX() + ((nAvailableLength * 2) / 20), aYPosB1 );
	basegfx::B2DPolygon aPolygonB;
	aPolygonB.append(aPointB1);
	aPolygonB.append(aPointB2);
	aPolygonB.append(aPointB3);
	mpLineObjB = new SdrPathObj(OBJ_PLIN, basegfx::B2DPolyPolygon(aPolygonB));
	mpLineObjB->SetModel(&getModel());
	
	// create DrawObectC
	const basegfx::B2DPoint aPointC1( aPointB3.getX() + nDistance,  aYPosB1);
	const basegfx::B2DPoint aPointC2( aPointC1.getX() + ((nAvailableLength * 1) / 20), aYPosB2 );
	const basegfx::B2DPoint aPointC3( aPointC2.getX() + ((nAvailableLength * 1) / 20), aYPosB1 );
	basegfx::B2DPolygon aPolygonC;
	aPolygonC.append(aPointC1);
	aPolygonC.append(aPointC2);
	aPolygonC.append(aPointC3);
	mpLineObjC = new SdrPathObj(OBJ_PLIN, basegfx::B2DPolyPolygon(aPolygonC));
	mpLineObjC->SetModel(&getModel());
}

SvxXLinePreview::~SvxXLinePreview()
{
    SdrObject::Free( mpLineObjA );
    SdrObject::Free( mpLineObjB );
    SdrObject::Free( mpLineObjC );
}

// -----------------------------------------------------------------------

void SvxXLinePreview::SetSymbol(Graphic* p,const Size& s)
{
	mpGraphic = p;
	maSymbolSize = s;
}

// -----------------------------------------------------------------------

void SvxXLinePreview::ResizeSymbol(const Size& s)
{
	if ( s != maSymbolSize )
	{
		maSymbolSize = s;
		Invalidate();
	}
}

// -----------------------------------------------------------------------

void SvxXLinePreview::SetLineAttributes(const SfxItemSet& rItemSet)
{
	// Set ItemSet at objects
	mpLineObjA->SetMergedItemSet(rItemSet);

	// At line joints, do not use arrows
	SfxItemSet aTempSet(rItemSet);
	aTempSet.ClearItem(XATTR_LINESTART);
	aTempSet.ClearItem(XATTR_LINEEND);

	mpLineObjB->SetMergedItemSet(aTempSet);
	mpLineObjC->SetMergedItemSet(aTempSet);
}

// -----------------------------------------------------------------------

void SvxXLinePreview::Paint( const Rectangle& )
{
	LocalPrePaint();

	// paint objects to buffer device
	sdr::contact::SdrObjectVector aObjectVector;
	aObjectVector.push_back(mpLineObjA);
	aObjectVector.push_back(mpLineObjB);
	aObjectVector.push_back(mpLineObjC);

	sdr::contact::ObjectContactOfObjListPainter aPainter(getBufferDevice(), aObjectVector, 0);
	sdr::contact::DisplayInfo aDisplayInfo;

	// do processing
	aPainter.ProcessDisplay(aDisplayInfo);

	if ( mbWithSymbol && mpGraphic )
	{
    	const Size aOutputSize(GetOutputSize());
		Point aPos = Point( aOutputSize.Width() / 3, aOutputSize.Height() / 2 );
		aPos.X() -= maSymbolSize.Width() / 2;
		aPos.Y() -= maSymbolSize.Height() / 2;
		mpGraphic->Draw( &getBufferDevice(), aPos, maSymbolSize );
	}

	LocalPostPaint();
}

/*************************************************************************
|*
|*    SvxXRectPreview::SvxXRectPreview()
|*
*************************************************************************/

SvxXRectPreview::SvxXRectPreview( Window* pParent, const ResId& rResId )
:	SvxPreviewBase( pParent, rResId ),
	mpRectangleObject(0)
{
	InitSettings(true, true);

	// create RectangleObject
    const Rectangle aObjectSize(Point(), GetOutputSize());
	mpRectangleObject = new SdrRectObj(aObjectSize);
	mpRectangleObject->SetModel(&getModel());
}

SvxXRectPreview::~SvxXRectPreview()
{
    SdrObject::Free(mpRectangleObject);
}

void SvxXRectPreview::SetAttributes(const SfxItemSet& rItemSet)
{
	mpRectangleObject->SetMergedItemSet(rItemSet, true);
	mpRectangleObject->SetMergedItem(XLineStyleItem(XLINE_NONE));
}

void SvxXRectPreview::Paint( const Rectangle& )
{
	LocalPrePaint();

    sdr::contact::SdrObjectVector aObjectVector;

    aObjectVector.push_back(mpRectangleObject);

    sdr::contact::ObjectContactOfObjListPainter aPainter(getBufferDevice(), aObjectVector, 0);
	sdr::contact::DisplayInfo aDisplayInfo;

	aPainter.ProcessDisplay(aDisplayInfo);

    LocalPostPaint();
}

/*************************************************************************
|*
|*    SvxXShadowPreview::SvxXShadowPreview()
|*
*************************************************************************/

SvxXShadowPreview::SvxXShadowPreview( Window* pParent, const ResId& rResId )
:	SvxPreviewBase( pParent, rResId ),
	mpRectangleObject(0),
    mpRectangleShadow(0)
{
	InitSettings(true, true);

    // prepare size
	Size aSize = GetOutputSize();
	aSize.Width() = aSize.Width() / 3;
	aSize.Height() = aSize.Height() / 3;

    // create RectangleObject
    const Rectangle aObjectSize( Point( aSize.Width(), aSize.Height() ), aSize );
	mpRectangleObject = new SdrRectObj(aObjectSize);
	mpRectangleObject->SetModel(&getModel());

    // create ShadowObject
    const Rectangle aShadowSize( Point( aSize.Width(), aSize.Height() ), aSize );
	mpRectangleShadow = new SdrRectObj(aShadowSize);
	mpRectangleShadow->SetModel(&getModel());
}

SvxXShadowPreview::~SvxXShadowPreview()
{
    SdrObject::Free(mpRectangleObject);
    SdrObject::Free(mpRectangleShadow);
}

void SvxXShadowPreview::SetRectangleAttributes(const SfxItemSet& rItemSet)
{
	mpRectangleObject->SetMergedItemSet(rItemSet, true);
	mpRectangleObject->SetMergedItem(XLineStyleItem(XLINE_NONE));
}

void SvxXShadowPreview::SetShadowAttributes(const SfxItemSet& rItemSet)
{
	mpRectangleShadow->SetMergedItemSet(rItemSet, true);
	mpRectangleShadow->SetMergedItem(XLineStyleItem(XLINE_NONE));
}

void SvxXShadowPreview::SetShadowPosition(const Point& rPos) 
{ 
    Rectangle aObjectPosition(mpRectangleObject->GetSnapRect());
    aObjectPosition.Move(rPos.X(), rPos.Y());
    mpRectangleShadow->SetSnapRect(aObjectPosition);
}

void SvxXShadowPreview::Paint( const Rectangle& )
{
	LocalPrePaint();

    sdr::contact::SdrObjectVector aObjectVector;

    aObjectVector.push_back(mpRectangleShadow);
	aObjectVector.push_back(mpRectangleObject);

    sdr::contact::ObjectContactOfObjListPainter aPainter(getBufferDevice(), aObjectVector, 0);
	sdr::contact::DisplayInfo aDisplayInfo;

	aPainter.ProcessDisplay(aDisplayInfo);

    LocalPostPaint();
}

// -----------------------------------------------------------------------
// eof

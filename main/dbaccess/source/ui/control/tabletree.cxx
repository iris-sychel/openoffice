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
#include "precompiled_dbaccess.hxx"

#ifndef _DBAUI_TABLETREE_HXX_
#include "tabletree.hxx"
#endif
#ifndef _DBAUI_TABLETREE_HRC_
#include "tabletree.hrc"
#endif
#ifndef DBACCESS_IMAGEPROVIDER_HXX
#include "imageprovider.hxx"
#endif
#ifndef _DBAUI_MODULE_DBU_HXX_
#include "moduledbu.hxx"
#endif
#ifndef _DBU_CONTROL_HRC_
#include "dbu_control.hrc"
#endif
#ifndef _SV_MENU_HXX
#include <vcl/menu.hxx>
#endif
#ifndef _CONNECTIVITY_DBTOOLS_HXX_
#include <connectivity/dbtools.hxx>
#endif
#ifndef _COMPHELPER_TYPES_HXX_
#include <comphelper/types.hxx>
#endif
#ifndef DBACCESS_SHARED_DBUSTRINGS_HRC
#include "dbustrings.hrc"
#endif
#ifndef _COM_SUN_STAR_SDB_APPLICATION_DATABASEOBJECT_HPP_
#include <com/sun/star/sdb/application/DatabaseObject.hpp>
#endif
#ifndef _COM_SUN_STAR_SDB_APPLICATION_DATABASEOBJECTFOLDER_HPP_
#include <com/sun/star/sdb/application/DatabaseObjectContainer.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XDRIVERACCESS_HPP_
#include <com/sun/star/sdbc/XDriverAccess.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBCX_XDATADEFINITIONSUPPLIER_HPP_
#include <com/sun/star/sdbcx/XDataDefinitionSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBCX_XVIEWSSUPPLIER_HPP_
#include <com/sun/star/sdbcx/XViewsSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBCX_XTABLESSUPPLIER_HPP_
#include <com/sun/star/sdbcx/XTablesSupplier.hpp>
#endif
#ifndef _COM_SUN_STAR_SDB_SQLCONTEXT_HPP_
#include <com/sun/star/sdb/SQLContext.hpp>
#endif
#ifndef _COM_SUN_STAR_SDBC_XROW_HPP_
#include <com/sun/star/sdbc/XRow.hpp>
#endif
#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif
#ifndef _DBAUI_COMMON_TYPES_HXX_
#include "commontypes.hxx"
#endif
#ifndef _DBAUI_LISTVIEWITEMS_HXX_
#include "listviewitems.hxx"
#endif
#ifndef TOOLS_DIAGNOSE_EX_H
#include <tools/diagnose_ex.h>
#endif
#ifndef _RTL_USTRBUF_HXX_
#include <rtl/ustrbuf.hxx>
#endif
#include <connectivity/dbmetadata.hxx>

#include <algorithm>

//.........................................................................
namespace dbaui
{
//.........................................................................

using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::sdb;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::sdbc;
using namespace ::com::sun::star::sdbcx;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::container;
using namespace ::com::sun::star::sdb::application;

using namespace ::dbtools;
using namespace ::comphelper;

namespace DatabaseObject = ::com::sun::star::sdb::application::DatabaseObject;
namespace DatabaseObjectContainer = ::com::sun::star::sdb::application::DatabaseObjectContainer;

//========================================================================
//= OTableTreeListBox
//========================================================================
OTableTreeListBox::OTableTreeListBox( Window* pParent, const Reference< XMultiServiceFactory >& _rxORB, WinBits nWinStyle,sal_Bool _bVirtualRoot )
	:OMarkableTreeListBox(pParent,_rxORB,nWinStyle)
    ,m_pImageProvider( new ImageProvider )
	,m_bVirtualRoot(_bVirtualRoot)
    ,m_bNoEmptyFolders( false )
{
	implSetDefaultImages();
}
//------------------------------------------------------------------------
OTableTreeListBox::OTableTreeListBox( Window* pParent, const Reference< XMultiServiceFactory >& _rxORB, const ResId& rResId ,sal_Bool _bVirtualRoot)
	:OMarkableTreeListBox(pParent,_rxORB,rResId)
    ,m_pImageProvider( new ImageProvider )
	,m_bVirtualRoot(_bVirtualRoot)
    ,m_bNoEmptyFolders( false )
{
	implSetDefaultImages();
}

// -----------------------------------------------------------------------------
OTableTreeListBox::~OTableTreeListBox()
{
}

// -----------------------------------------------------------------------------
void OTableTreeListBox::implSetDefaultImages()
{
    ImageProvider aImageProvider;
    SetDefaultExpandedEntryBmp( aImageProvider.getFolderImage( DatabaseObject::TABLE, false ), BMP_COLOR_NORMAL );
    SetDefaultExpandedEntryBmp( aImageProvider.getFolderImage( DatabaseObject::TABLE, true ), BMP_COLOR_HIGHCONTRAST );
    SetDefaultCollapsedEntryBmp( aImageProvider.getFolderImage( DatabaseObject::TABLE, false ), BMP_COLOR_NORMAL );
    SetDefaultCollapsedEntryBmp( aImageProvider.getFolderImage( DatabaseObject::TABLE, true ), BMP_COLOR_HIGHCONTRAST );
}

// -----------------------------------------------------------------------------
bool  OTableTreeListBox::isFolderEntry( const SvLBoxEntry* _pEntry ) const
{
    sal_Int32 nEntryType = reinterpret_cast< sal_IntPtr >( _pEntry->GetUserData() );
    if  (   ( nEntryType == DatabaseObjectContainer::TABLES )
        ||  ( nEntryType == DatabaseObjectContainer::CATALOG )
        ||  ( nEntryType == DatabaseObjectContainer::SCHEMA )
        )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
void OTableTreeListBox::notifyHiContrastChanged()
{
    implSetDefaultImages();

	SvLBoxEntry* pEntryLoop = First();
	while (pEntryLoop)
	{
		sal_uInt16 nCount = pEntryLoop->ItemCount();
		for (sal_uInt16 i=0;i<nCount;++i)
		{
			SvLBoxItem* pItem = pEntryLoop->GetItem(i);
			if ( pItem && pItem->IsA() == SV_ITEM_ID_LBOXCONTEXTBMP)
			{
                SvLBoxContextBmp* pContextBitmapItem = static_cast< SvLBoxContextBmp* >( pItem );

                Image aImage, aImageHC;
                if ( isFolderEntry( pEntryLoop ) )
                {
                    aImage = m_pImageProvider->getFolderImage( DatabaseObject::TABLE, false );
                    aImageHC = m_pImageProvider->getFolderImage( DatabaseObject::TABLE, true );
                }
                else
                {
                    String sCompleteName( getQualifiedTableName( pEntryLoop ) );
                    m_pImageProvider->getImages( sCompleteName, DatabaseObject::TABLE, aImage, aImageHC );
                }

				pContextBitmapItem->SetBitmap1( aImage, BMP_COLOR_NORMAL );
				pContextBitmapItem->SetBitmap2( aImage, BMP_COLOR_NORMAL );
				pContextBitmapItem->SetBitmap1( aImageHC, BMP_COLOR_HIGHCONTRAST );
				pContextBitmapItem->SetBitmap2( aImageHC, BMP_COLOR_HIGHCONTRAST );
                // TODO: Now that we give both images to the entry item, it is not necessary anymore
                // to do this anytime HC changes - the tree control will do this itself now.
                // We would only need to properly initialize newly inserted entries.
				break;
			}
		}
		pEntryLoop = Next(pEntryLoop);
	}
}

//------------------------------------------------------------------------
void OTableTreeListBox::implOnNewConnection( const Reference< XConnection >& _rxConnection )
{
    m_xConnection = _rxConnection;
    m_pImageProvider.reset( new ImageProvider( m_xConnection  ) );
}

//------------------------------------------------------------------------
void OTableTreeListBox::UpdateTableList( const Reference< XConnection >& _rxConnection ) throw(SQLException)
{
	Sequence< ::rtl::OUString > sTables, sViews;

	String sCurrentActionError;
	try
	{
        Reference< XTablesSupplier > xTableSupp( _rxConnection, UNO_QUERY_THROW );
		sCurrentActionError = String(ModuleRes(STR_NOTABLEINFO));

		Reference< XNameAccess > xTables,xViews;

		Reference< XViewsSupplier > xViewSupp( _rxConnection, UNO_QUERY );
		if ( xViewSupp.is() )
		{
			xViews = xViewSupp->getViews();
			if (xViews.is())
				sViews = xViews->getElementNames();
		}

		xTables = xTableSupp->getTables();
		if (xTables.is())
			sTables = xTables->getElementNames();
	}
	catch(RuntimeException&)
	{
		DBG_ERROR("OTableTreeListBox::UpdateTableList : caught an RuntimeException!");
	}
    catch ( const SQLException& )
    {
        throw;
    }
	catch(Exception&)
	{
		// a non-SQLException exception occurred ... simply throw an SQLException
		SQLException aInfo;
        aInfo.Message = sCurrentActionError;
		throw aInfo;
	}

	UpdateTableList( _rxConnection, sTables, sViews );
}
// -----------------------------------------------------------------------------
namespace
{
	struct OViewSetter : public ::std::unary_function< OTableTreeListBox::TNames::value_type, bool>
	{
		const Sequence< ::rtl::OUString> m_aViews;
		::comphelper::TStringMixEqualFunctor m_aEqualFunctor;

		OViewSetter(const Sequence< ::rtl::OUString>& _rViews,sal_Bool _bCase) : m_aViews(_rViews),m_aEqualFunctor(_bCase){}
		OTableTreeListBox::TNames::value_type operator() (const ::rtl::OUString& lhs)
		{
			OTableTreeListBox::TNames::value_type aRet;
			aRet.first = lhs;
			const ::rtl::OUString* pIter = m_aViews.getConstArray();
			const ::rtl::OUString* pEnd = m_aViews.getConstArray() + m_aViews.getLength();
			aRet.second = (::std::find_if(pIter,pEnd,::std::bind2nd(m_aEqualFunctor,lhs)) != pEnd);

			return aRet;
		}
	};

}
// -----------------------------------------------------------------------------
void OTableTreeListBox::UpdateTableList(
				const Reference< XConnection >& _rxConnection,
				const Sequence< ::rtl::OUString>& _rTables,
				const Sequence< ::rtl::OUString>& _rViews
			)
{
	TNames aTables;
	aTables.resize(_rTables.getLength());
	const ::rtl::OUString* pIter = _rTables.getConstArray();
	const ::rtl::OUString* pEnd = _rTables.getConstArray() + _rTables.getLength();
	try
	{
        Reference< XDatabaseMetaData > xMeta( _rxConnection->getMetaData(), UNO_QUERY_THROW );
		::std::transform( pIter, pEnd,
            aTables.begin(), OViewSetter( _rViews, xMeta->supportsMixedCaseQuotedIdentifiers() ) );
	}
	catch(Exception&)
	{
        DBG_UNHANDLED_EXCEPTION();
	}
    UpdateTableList( _rxConnection, aTables );
}

//------------------------------------------------------------------------
namespace
{
    ::std::vector< ::rtl::OUString > lcl_getMetaDataStrings_throw( const Reference< XResultSet >& _rxMetaDataResult, sal_Int32 _nColumnIndex )
    {
        ::std::vector< ::rtl::OUString > aStrings;
        Reference< XRow > xRow( _rxMetaDataResult, UNO_QUERY_THROW );
        while ( _rxMetaDataResult->next() )
            aStrings.push_back( xRow->getString( _nColumnIndex ) );
        return aStrings;
    }

    bool lcl_shouldDisplayEmptySchemasAndCatalogs( const Reference< XConnection >& _rxConnection )
    {
        ::dbtools::DatabaseMetaData aMetaData( _rxConnection );
        return aMetaData.displayEmptyTableFolders();
    }
}

//------------------------------------------------------------------------
void OTableTreeListBox::UpdateTableList( const Reference< XConnection >& _rxConnection, const TNames& _rTables )
{
    implOnNewConnection( _rxConnection );

	// throw away all the old stuff
	Clear();

	try
	{
		// the root entry saying "all objects"
		SvLBoxEntry* pAllObjects = NULL;
		if (haveVirtualRoot())
		{
			String sRootEntryText;
			TNames::const_iterator aViews = ::std::find_if(_rTables.begin(),_rTables.end(),
			::std::compose1(::std::bind2nd(::std::equal_to<sal_Bool>(),sal_False),::std::select2nd<TNames::value_type>()));
			TNames::const_iterator aTables = ::std::find_if(_rTables.begin(),_rTables.end(),
			::std::compose1(::std::bind2nd(::std::equal_to<sal_Bool>(),sal_True),::std::select2nd<TNames::value_type>()));

			if ( aViews == _rTables.end() )
				sRootEntryText  = String(ModuleRes(STR_ALL_TABLES));
			else if ( aTables == _rTables.end() )
				sRootEntryText  = String(ModuleRes(STR_ALL_VIEWS));
			else
				sRootEntryText  = String(ModuleRes(STR_ALL_TABLES_AND_VIEWS));
            pAllObjects = InsertEntry( sRootEntryText, NULL, sal_False, LIST_APPEND, reinterpret_cast< void* >( DatabaseObjectContainer::TABLES ) );
		}

		if ( _rTables.empty() )
			// nothing to do (besides inserting the root entry)
			return;

		// get the table/view names
		TNames::const_iterator aIter = _rTables.begin();
		TNames::const_iterator aEnd = _rTables.end();

        Reference< XDatabaseMetaData > xMeta( _rxConnection->getMetaData(), UNO_QUERY_THROW );
		for ( ;	aIter != aEnd; ++aIter )
		{
			// add the entry
			implAddEntry(
				xMeta,
				aIter->first,
				sal_False
			);
		}

        if ( !m_bNoEmptyFolders && lcl_shouldDisplayEmptySchemasAndCatalogs( _rxConnection ) )
        {
            sal_Bool bSupportsCatalogs = xMeta->supportsCatalogsInDataManipulation();
            sal_Bool bSupportsSchemas = xMeta->supportsSchemasInDataManipulation();

            if ( bSupportsCatalogs || bSupportsSchemas )
            {
                // we display empty catalogs if the DB supports catalogs, and they're noted at the beginning of a
                // composed name. Otherwise, we display empty schematas. (also see the tree structure explained in
                // implAddEntry)
                bool bCatalogs = bSupportsCatalogs && xMeta->isCatalogAtStart();

                ::std::vector< ::rtl::OUString > aFolderNames( lcl_getMetaDataStrings_throw(
                    bCatalogs ? xMeta->getCatalogs() : xMeta->getSchemas(), 1 ) );
                sal_Int32 nFolderType = bCatalogs ? DatabaseObjectContainer::CATALOG : DatabaseObjectContainer::SCHEMA;

                SvLBoxEntry* pRootEntry = getAllObjectsEntry();
                for (   ::std::vector< ::rtl::OUString >::const_iterator folder = aFolderNames.begin();
                        folder != aFolderNames.end();
                        ++folder
                    )
                {
		            SvLBoxEntry* pFolder = GetEntryPosByName( *folder, pRootEntry );
		            if ( !pFolder )
			            pFolder = InsertEntry( *folder, pRootEntry, sal_False, LIST_APPEND, reinterpret_cast< void* >( nFolderType ) );
                }
            }
        }
	}
	catch ( const Exception& )
	{
        DBG_UNHANDLED_EXCEPTION();
	}
}
//------------------------------------------------------------------------
sal_Bool OTableTreeListBox::isWildcardChecked(SvLBoxEntry* _pEntry) const
{
	if (_pEntry)
	{
		OBoldListboxString* pTextItem = static_cast<OBoldListboxString*>(_pEntry->GetFirstItem(SV_ITEM_ID_BOLDLBSTRING));
		if (pTextItem)
			return pTextItem->isEmphasized();
	}
	return sal_False;
}

//------------------------------------------------------------------------
void OTableTreeListBox::checkWildcard(SvLBoxEntry* _pEntry)
{
	SetCheckButtonState(_pEntry, SV_BUTTON_CHECKED);
	checkedButton_noBroadcast(_pEntry);
}

//------------------------------------------------------------------------
SvLBoxEntry* OTableTreeListBox::getAllObjectsEntry() const
{
	return haveVirtualRoot() ? First() : NULL;
}

//------------------------------------------------------------------------
void OTableTreeListBox::checkedButton_noBroadcast(SvLBoxEntry* _pEntry)
{
	OMarkableTreeListBox::checkedButton_noBroadcast(_pEntry);

	// if an entry has children, it makes a difference if the entry is checked because alls children are checked
	// or if the user checked it explicitly.
	// So we track explicit (un)checking

	SvButtonState eState = GetCheckButtonState(_pEntry);
	DBG_ASSERT(SV_BUTTON_TRISTATE != eState, "OTableTreeListBox::CheckButtonHdl: user action which lead to TRISTATE?");
	implEmphasize(_pEntry, SV_BUTTON_CHECKED == eState);
}

//------------------------------------------------------------------------
void OTableTreeListBox::implEmphasize(SvLBoxEntry* _pEntry, sal_Bool _bChecked, sal_Bool _bUpdateDescendants, sal_Bool _bUpdateAncestors)
{
	DBG_ASSERT(_pEntry, "OTableTreeListBox::implEmphasize: invalid entry (NULL)!");

	// special emphasizing handling for the "all objects" entry
	// 89709 - 16.07.2001 - frank.schoenheit@sun.com
	sal_Bool bAllObjectsEntryAffected = haveVirtualRoot() && (getAllObjectsEntry() == _pEntry);
	if	(	GetModel()->HasChilds(_pEntry)				// the entry has children
		||	bAllObjectsEntryAffected					// or it is the "all objects" entry
		)
	{
		OBoldListboxString* pTextItem = static_cast<OBoldListboxString*>(_pEntry->GetFirstItem(SV_ITEM_ID_BOLDLBSTRING));
		if (pTextItem)
			pTextItem->emphasize(_bChecked);

		if (bAllObjectsEntryAffected)
			InvalidateEntry(_pEntry);
	}

	if (_bUpdateDescendants)
	{
		// remove the mark for all children of the checked entry
		SvLBoxEntry* pChildLoop = FirstChild(_pEntry);
		while (pChildLoop)
		{
			if (GetModel()->HasChilds(pChildLoop))
				implEmphasize(pChildLoop, sal_False, sal_True, sal_False);
			pChildLoop = NextSibling(pChildLoop);
		}
	}

	if (_bUpdateAncestors)
	{
		// remove the mark for all ancestors of the entry
		if (GetModel()->HasParent(_pEntry))
			implEmphasize(GetParent(_pEntry), sal_False, sal_False, sal_True);
	}
}

//------------------------------------------------------------------------
void OTableTreeListBox::InitEntry(SvLBoxEntry* _pEntry, const XubString& _rString, const Image& _rCollapsedBitmap, const Image& _rExpandedBitmap, SvLBoxButtonKind _eButtonKind)
{
	OMarkableTreeListBox::InitEntry(_pEntry, _rString, _rCollapsedBitmap, _rExpandedBitmap, _eButtonKind);

	// replace the text item with our own one
	SvLBoxItem* pTextItem = _pEntry->GetFirstItem(SV_ITEM_ID_LBOXSTRING);
	DBG_ASSERT(pTextItem, "OTableTreeListBox::InitEntry: no text item!?");
	sal_uInt16 nTextPos = _pEntry->GetPos(pTextItem);
	DBG_ASSERT(((sal_uInt16)-1) != nTextPos, "OTableTreeListBox::InitEntry: no text item pos!");

	_pEntry->ReplaceItem(new OBoldListboxString(_pEntry, 0, _rString), nTextPos);
}

//------------------------------------------------------------------------
SvLBoxEntry* OTableTreeListBox::implAddEntry(
		const Reference< XDatabaseMetaData >& _rxMeta,
		const ::rtl::OUString& _rTableName,
		sal_Bool _bCheckName
	)
{
    OSL_PRECOND( _rxMeta.is(), "OTableTreeListBox::implAddEntry: invalid meta data!" );
    if ( !_rxMeta.is() )
        return NULL;

	// split the complete name into it's components
	::rtl::OUString sCatalog, sSchema, sName;
	qualifiedNameComponents( _rxMeta, _rTableName, sCatalog, sSchema, sName, ::dbtools::eInDataManipulation );

    SvLBoxEntry* pParentEntry = getAllObjectsEntry();

    // if the DB uses catalog at the start of identifiers, then our hierarchy is
    //   catalog
    //   +- schema
    //      +- table
    // else it is
    //   schema
    //   +- catalog
    //      +- table
    sal_Bool bCatalogAtStart = _rxMeta->isCatalogAtStart();
    const ::rtl::OUString& rFirstName  = bCatalogAtStart ? sCatalog : sSchema;
    const sal_Int32 nFirstFolderType   = bCatalogAtStart ? DatabaseObjectContainer::CATALOG : DatabaseObjectContainer::SCHEMA;
    const ::rtl::OUString& rSecondName = bCatalogAtStart ? sSchema : sCatalog;
    const sal_Int32 nSecondFolderType  = bCatalogAtStart ? DatabaseObjectContainer::SCHEMA : DatabaseObjectContainer::CATALOG;

	if ( rFirstName.getLength() )
	{
		SvLBoxEntry* pFolder = GetEntryPosByName( rFirstName, pParentEntry );
		if ( !pFolder )
			pFolder = InsertEntry( rFirstName, pParentEntry, sal_False, LIST_APPEND, reinterpret_cast< void* >( nFirstFolderType ) );
		pParentEntry = pFolder;
	}

	if ( rSecondName.getLength() )
	{
		SvLBoxEntry* pFolder = GetEntryPosByName( rSecondName, pParentEntry );
		if ( !pFolder )
			pFolder = InsertEntry( rSecondName, pParentEntry, sal_False, LIST_APPEND, reinterpret_cast< void* >( nSecondFolderType ) );
		pParentEntry = pFolder;
	}

	SvLBoxEntry* pRet = NULL;
	if ( !_bCheckName || !GetEntryPosByName( sName, pParentEntry ) )
    {
		pRet = InsertEntry( sName, pParentEntry, sal_False, LIST_APPEND );

        Image aImage, aImageHC;
        m_pImageProvider->getImages( _rTableName, DatabaseObject::TABLE, aImage, aImageHC );

        SetExpandedEntryBmp( pRet, aImage, BMP_COLOR_NORMAL );
        SetCollapsedEntryBmp( pRet, aImage, BMP_COLOR_NORMAL );
        SetExpandedEntryBmp( pRet, aImageHC, BMP_COLOR_HIGHCONTRAST );
        SetCollapsedEntryBmp( pRet, aImageHC, BMP_COLOR_HIGHCONTRAST );
    }
	return pRet;
}

//------------------------------------------------------------------------
NamedDatabaseObject OTableTreeListBox::describeObject( SvLBoxEntry* _pEntry )
{
    NamedDatabaseObject aObject;

    sal_Int32 nEntryType = reinterpret_cast< sal_IntPtr >( _pEntry->GetUserData() );

    if  ( nEntryType == DatabaseObjectContainer::TABLES )
    {
        aObject.Type = DatabaseObjectContainer::TABLES;
    }
    else if (   ( nEntryType == DatabaseObjectContainer::CATALOG )
            ||  ( nEntryType == DatabaseObjectContainer::SCHEMA )
            )
    {
        SvLBoxEntry* pParent = GetParent( _pEntry );
        sal_Int32 nParentEntryType = pParent ? reinterpret_cast< sal_IntPtr >( pParent->GetUserData() ) : -1;

        ::rtl::OUStringBuffer buffer;
        if  ( nEntryType == DatabaseObjectContainer::CATALOG )
        {
            if ( nParentEntryType == DatabaseObjectContainer::SCHEMA )
            {
                buffer.append( GetEntryText( pParent ) );
                buffer.append( sal_Unicode( '.' ) );
            }
            buffer.append( GetEntryText( _pEntry ) );
        }
        else if ( nEntryType == DatabaseObjectContainer::SCHEMA )
        {
            if ( nParentEntryType == DatabaseObjectContainer::CATALOG )
            {
                buffer.append( GetEntryText( pParent ) );
                buffer.append( sal_Unicode( '.' ) );
            }
            buffer.append( GetEntryText( _pEntry ) );
        }
    }
    else
    {
        aObject.Type = DatabaseObject::TABLE;
        aObject.Name = getQualifiedTableName( _pEntry );
    }

    return aObject;
}

//------------------------------------------------------------------------
SvLBoxEntry* OTableTreeListBox::addedTable( const ::rtl::OUString& _rName )
{
	try
	{
        Reference< XDatabaseMetaData > xMeta;
        if ( impl_getAndAssertMetaData( xMeta ) )
		    return implAddEntry( xMeta, _rName );
	}
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION();
    }
	return NULL;
}

//------------------------------------------------------------------------
bool OTableTreeListBox::impl_getAndAssertMetaData( Reference< XDatabaseMetaData >& _out_rMetaData ) const
{
	if ( m_xConnection.is() )
        _out_rMetaData = m_xConnection->getMetaData();
    OSL_PRECOND( _out_rMetaData.is(), "OTableTreeListBox::impl_getAndAssertMetaData: invalid current connection!" );
    return _out_rMetaData.is();
}

//------------------------------------------------------------------------
String OTableTreeListBox::getQualifiedTableName( SvLBoxEntry* _pEntry ) const
{
    OSL_PRECOND( !isFolderEntry( _pEntry ), "OTableTreeListBox::getQualifiedTableName: folder entries not allowed here!" );

    try
    {
        Reference< XDatabaseMetaData > xMeta;
        if ( !impl_getAndAssertMetaData( xMeta ) )
            return String();

        ::rtl::OUString sCatalog;
	    ::rtl::OUString sSchema;
	    ::rtl::OUString sTable;

	    SvLBoxEntry* pSchema = GetParent( _pEntry );
	    if ( pSchema )
	    {
		    SvLBoxEntry* pCatalog = GetParent( pSchema );
		    if  (   pCatalog
                ||  (   xMeta->supportsCatalogsInDataManipulation()
                    &&  !xMeta->supportsSchemasInDataManipulation()
                    )   // here we support catalog but no schema
                )
		    {
			    if ( pCatalog == NULL )
			    {
				    pCatalog = pSchema;
				    pSchema = NULL;
			    }
			    sCatalog = GetEntryText( pCatalog );
		    }
		    if ( pSchema )
			    sSchema = GetEntryText(pSchema);
	    }
	    sTable = GetEntryText( _pEntry );

	    return ::dbtools::composeTableName( xMeta, sCatalog, sSchema, sTable, sal_False, ::dbtools::eInDataManipulation );
    }
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION();
    }
    return String();
}

//------------------------------------------------------------------------
SvLBoxEntry* OTableTreeListBox::getEntryByQualifiedName( const ::rtl::OUString& _rName )
{
	try
	{
        Reference< XDatabaseMetaData > xMeta;
        if ( !impl_getAndAssertMetaData( xMeta ) )
            return NULL;

		// split the complete name into it's components
		::rtl::OUString sCatalog, sSchema, sName;
		qualifiedNameComponents(xMeta, _rName, sCatalog, sSchema, sName,::dbtools::eInDataManipulation);

		SvLBoxEntry* pParent = getAllObjectsEntry();
		SvLBoxEntry* pCat = NULL;
		SvLBoxEntry* pSchema = NULL;
		if ( sCatalog.getLength() )
		{
			pCat = GetEntryPosByName(sCatalog, pParent);
			if ( pCat )
				pParent = pCat;
		}

		if ( sSchema.getLength() )
		{
			pSchema = GetEntryPosByName(sSchema, pParent);
			if ( pSchema )
				pParent = pSchema;
		}

		return GetEntryPosByName(sName, pParent);
	}
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION();
    }
	return NULL;
}
//------------------------------------------------------------------------
void OTableTreeListBox::removedTable( const ::rtl::OUString& _rName )
{
	try
	{
		SvLBoxEntry* pEntry = getEntryByQualifiedName( _rName );
		if ( pEntry )
			GetModel()->Remove( pEntry );
	}
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION();
    }
}

//.........................................................................
}	// namespace dbaui
//.........................................................................


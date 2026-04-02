//*****************************************************************************
//	Copywrite (C) 1992-2006 Barry H. Branham.  All rights reserved.
//*****************************************************************************
//**********************************************************************
//  HeaderFl.cc
//
//  09-dec-02  bhb	from GlasHeader.cc
//  Modified:
//	10-dec-02  bhb	add InitHdrDlog class from DasHeader
//	30-apr-03  bhb	remove '/f/r' from start of .hdr file in writeHdr(), change readHdr() so
//					can read either way.
//	27-jun-03  bhb	fixup readHdr(), also remove '/f/r' from read/writeSet()
//	27-aug-03  bhb	modify setFilename() to have stripDfNum arg so works if have
//					name that isn't datafile (or log) name
//	28-oct-05  bhb	fix _fileName use so don't have mismatched free()/delete
//					(don't use strdup())
//	11-aug-06  bhb	Fltk version
//	10-oct-06  bhb	fixup Sg & Ag editing, get rid of editNewSgCallback(), editNewSg()
//	15-aug-06  bhb	Fltk version
//	07-sep-07  bhb	in editAgOk(), added DG check
//	07-oct-08  bhb	revise initHdr() to fix file check bug, no setDate bool param
//	24-nov-09  bhb	removed deleteDg()
//	28-sep-12  bhb	modify dtor to hide dlogs
//	
//**********************************************************************
#include "HeaderFl.h"
#include "UI/InitHdrDlog.h"
#include "EditSgDlog.h"
#include "EditAgDlog.h"
#include "EditDgDlog.h"
#include "EditRtAgDlog.h"
#include <Data/DisplayGr.h>
#include <Data/SampleGr.h>			// for sgList
#include <Data/AMode.h>
#include <Glas/das_cmds.h>
#include <CXlib/cxlib.h>			// for checkFileTime()
#include <fltk/ask.h>
#include <fltk/Divider.h>
#include <string.h>
#include <ctype.h>
#include <string>

using namespace std;
using namespace fltk;

HeaderFl::HeaderFl() : DataHeader(), _initHdrDlog(0), _addGr(false), _updateSgCBclient(0),
	_agClient(0), _rtAgClient(0), _editRtAgDlog(0)
	
{
	sgList()->setParent( this);
	agList()->setParent( this);
	rtAgList()->setParent( this);
	_editDgDlog = new EditDgDlog();
	_editSgDlog = new EditSgDlog( _editDgDlog);
	_editAgDlog = new EditAgDlog( _editDgDlog);
}

HeaderFl::~HeaderFl()
{
	if ( _initHdrDlog != 0)
	{
		_initHdrDlog->hide();
		delete _initHdrDlog;
	}
	_editDgDlog->hide();
	delete _editDgDlog;

	_editSgDlog->hide();
	delete _editSgDlog;

	_editAgDlog->hide();
	delete _editAgDlog;

	if ( _editRtAgDlog)
	{
		_editRtAgDlog->hide();
		delete _editRtAgDlog;
	}
}

//
//	initialize Header by posting Dlog.  Resets _numGainFiles & _numBlocks but not SgList or
//	AgList.
//	
bool
HeaderFl::initHdr()
{
	bool tryagain = true;
	string hdrname;
	
	if ( _initHdrDlog == 0)
	{
		_initHdrDlog = new InitHdrDlog( this);
	}

	setDate();			// today's date
	while ( tryagain)
	{
		if ( _initHdrDlog->postAndWait( "Init Header") == Dialog::OK)
		{
			string fname;
			hdrname = _initHdrDlog->filename();
			fname = hdrname + ".hdr";
			time_t tm = checkFileTime( fname.c_str());	// returns 0 if file not found
			if ( tm > 0)
			{
				switch ( choice( 
			"Header file %s exists, you can either choose\na different name or use 'Open Dataset'.", 
			"Cancel", "Open Dataset", "*Rename", hdrname.c_str()))
				{
					case 2:							// Replace
						break;
					case 1:							// Open Dataset
					case 0:							// Cancel
						return false;
					default:    break;
				}
			}
			else
			{
				tryagain = false;
			}
		}
		else
			return false;
	}

	setFilename( hdrname);

	setDate();

	if ( _initHdrDlog->studytitleChanged())
	{
		setTitle( _initHdrDlog->studytitle());
	}

	if ( _updateSgCBclient)
		_updateAllSgCB( _updateSgCBclient);

	_numGainFiles = 0;
	_numBlocks.clear();

	return true;
}

//
//  set Sample Groups submenu of 'Edit' menu in main menu bar
//
void
HeaderFl::setEditSgMenu( Menu *em)
{
	_editSgMenu = em;					// save to update when edit done

	em->begin();
	for ( int i=em->children()-1; i>=0; i--)
	{
		Widget* o = em->child( i);
		em->remove( i);
		delete o;
	}

	em->activate();
	for ( list<SampleGr *>::iterator sg = sgList()->sgList()->begin(); 
			sg != sgList()->sgList()->end();  sg++)
		em->add( (*sg)->name(), 0, &HeaderFl::editSgCallback, (void *)this);
	new Divider();
	Widget *w = em->add( "Add SG", 0, &HeaderFl::editSgCallback, (void *)this);
	if ( sgList()->size() == DAS_MAX_SAMP_GR)
		w->deactivate();		// can't add any more sg's
	em->end();
}

//
//  set Analysis Groups submenu of 'Edit' menu in main menu bar
//
void
HeaderFl::setEditAgMenu( Menu *em)
{
	_editAgMenu = em;			// save to update when edit done
	
	em->begin();
	for ( int i=em->children()-1; i>=0; i--)
	{
		Widget* o = em->child( i);
		em->remove( i);
		delete o;
	}
	em->activate();
	for ( list<AnalysisGr *>::iterator ag = agList()->begin(); 
			ag != agList()->end();  ag++)
		em->add((*ag)->name(), 0, &HeaderFl::editAgCallback, (void *)this);
	new Divider();
	em->add("Add AG", 0, &HeaderFl::editAgCallback, (void *)this);
	em->end();
}

//
//  set Rt Analysis Groups submenu of 'Edit' menu in main menu bar
//
void
HeaderFl::setEditRtAgMenu( Menu *em)
{
	_editRtAgMenu = em;			// save to update when edit done
	
	_editRtAgMenu->begin();
	for ( int i=_editRtAgMenu->children()-1; i>=0; i--)
	{
		Widget* o = _editRtAgMenu->child( i);
		_editRtAgMenu->remove( i);
		delete o;
	}
	_editRtAgMenu->activate();
	for ( list<RtAnalysisGr *>::iterator ag = rtAgList()->List()->begin(); 
			ag != rtAgList()->List()->end();  ag++)
		_editRtAgMenu->add((*ag)->name(), 0, &HeaderFl::editRtAgCallback, (void *)this);
	new Divider();
	_editRtAgMenu->add("Add RT AG", 0, &HeaderFl::editRtAgCallback, (void *)this);
	_editRtAgMenu->end();
	
}

//----------------------------------------------------------------------
//	Edit Samp Gr Dialog static callback functions
//----------------------------------------------------------------------
void
HeaderFl::editSgCallback(Widget *w, void *clientData)
{
	((HeaderFl *)clientData)->editSg( w);
}

void
HeaderFl::editSgOkCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editSgOk();
}

void
HeaderFl::editSgDeleteCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editSgDelete();
}

void
HeaderFl::editSgCancelCB(Widget *, void *clientData)
{
	// either undo any DG changes or update menubar 'Display' Dg's
	((HeaderFl *)clientData)->editSgCancel();
}

//----------------------------------------------------------------------
//	Samp Gr Edit Dialog functions
//----------------------------------------------------------------------
void
HeaderFl::editSg( Widget *w)
{
	char str[64];
	int item = _editSgMenu->value();		// focus_index

	_SgEdit = sgList()->elem(item);			// pointer to original

	if ( _SgEdit != 0)
	{
		_editSg = new SampleGr( *_SgEdit);	// make a copy, note - has no parent list		
		_addGr = false;
	}
	else	// adding a new Sg
	{
		_SgEdit = _editSg = new SampleGr( sgList());		// has parent list but isn't in it yet
		_addGr = true;
		item--;		// so Title AG # comes out right
	}

	// post it, can't postModal because other (child) dialogs wouldn't work
	sprintf( str, "Sample Group %d", item+1);
	editSgDlog()->post( 0, &HeaderFl::editSgOkCB,
		&HeaderFl::editSgCancelCB, &HeaderFl::editSgDeleteCB, this);
	editSgDlog()->setTitle( str);

	editSgDlog()->initValues( editSg());
}

void
HeaderFl::editSgOk()
{
	unsigned short ch[NCHAN];
	int nch, rate;
	int i, setsg;
	unsigned int chgd = 0;
	
	// can't use changed() since if OK pressed, all internal values already
	// updated.  So just set everything
	editSg()->setName( editSgDlog()->name());
	nch = editSg()->gtchan( editSgDlog()->chan(), ch, NCHAN);
	if ( nch == -1)
	{
		message( "Error in channel list, try again");
//		sleep(1);
//		theInfoDialog->unpost();
		_addGr = false;
		return;
	}
	
	// see if chan have changed
	setsg = 0;
	if ( nch != editSg()->nChan())
		setsg++;
	if ( !setsg)	// compare chan #'s
		for ( i=0; i<nch && !setsg; i++)
			if ( ch[i] != editSg()->chan(i))
				setsg++;
	if ( setsg)	
	{
		chgd = SG_CHANGE_CHAN;
		editSg()->setChan( ch, nch);
		editSg()->initDefUnits( nch);		// make sure _unitsIndx is right size
	}

	rate = editSgDlog()->rate();
	if ( rate != editSg()->rate())
		chgd |= SG_CHANGE_RATE;
	editSg()->setRate( rate);
	editSg()->changed( chgd);

	if ( !_addGr)
	{
		*_SgEdit = *editSg();					// copy back to original, Sg keeps current parent
		_SgEdit->setParent( sgList());
		delete editSg();
	}
	else
		sgList()->sgList()->push_back( editSg());			// add to list

	if ( _updateSgCBclient)
		_updateSgCB( _updateSgCBclient, (void *)_SgEdit);

	// ??? display groups?
	
	setEditSgMenu( _editSgMenu);
	editSgDlog()->setDgChanged( false);
	_addGr = false;
}

void
HeaderFl::editSgDelete()
{
	if ( !ask( "Are you sure?"))
		return;

	if ( !_addGr)
	{
		sgList()->sgList()->remove( _SgEdit);			// remove, delete original
		delete _SgEdit;						// if NewSg, its not on list
	}
	delete editSg();						// delete edit copy or newSg
	
	if ( _updateSgCBclient)
		_updateAllSgCB( _updateSgCBclient);

	setEditSgMenu( _editSgMenu);
	editSgDlog()->setDgChanged( false);
	editSgDlog()->unpost();
	_addGr = false;
}

//
// either undo any DG changes or update menubar 'Display' Dg's
//
void
HeaderFl::editSgCancel()
{
	delete editSg();						// delete edit copy or newSg
	_addGr = false;
}

//----------------------------------------------------------------------
//	Edit Analysis Gr Dialog callback functions
//----------------------------------------------------------------------
void
HeaderFl::editAgCallback(Widget *w, void * clientData)
{
	((HeaderFl *)clientData)->editAg( w);
}

void
HeaderFl::editAgOkCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editAgOk();
}

void
HeaderFl::editAgDeleteCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editAgDelete();
}

void
HeaderFl::editAgCancelCB(Widget *, void *clientData)
{
	// either undo any DG changes or update menubar 'Display' Dg's
	((HeaderFl *)clientData)->editAgCancel();
}

//----------------------------------------------------------------------
//	Analysis Gr Edit Dialog functions
//----------------------------------------------------------------------
void
HeaderFl::editAg( Widget *w)
{
	char str[256];
	int item = _editAgMenu->value();

	_AgEdit = agList()->elem(item);
	
	if ( _AgEdit != 0)
	{
		_editAg = new AnalysisGr( *_AgEdit);		// edit current ag, make copy
		_addGr = false;
	}
	else	// adding a new Ag
	{
		_AgEdit = _editAg = new AnalysisGr( agList());		// make newAg
		_addGr = true;
		item--;		// so Title AG # comes out right
	}

	// post it, can't postModal because other (child) dialogs wouldn't work
	sprintf( str, "Analysis Group %d", item+1);
	editAgDlog()->post( 0, &HeaderFl::editAgOkCB,
		&HeaderFl::editAgCancelCB, &HeaderFl::editAgDeleteCB, this);
	editAgDlog()->setTitle( str);

	editAgDlog()->initValues( editAg());
}

void
HeaderFl::editAgOk()
{
	unsigned short ch[NCHAN];
	int nch;
	AnalysisGr *curAg;
	
	// set editAg from dialog
	// can't use changed() since if OK pressed, all internal values already
	// updated.  So just set everything
	editAgDlog()->initAgFromDlog( editAg());

	if ( !_addGr)
	{
		*_AgEdit = *editAg();					// copy back to original, Sg keeps current parent
		_AgEdit->setParent( agList());
		delete editAg();
		curAg = _AgEdit;
	}
	else
	{
		agList()->agList()->push_back( editAg());			// add to list
		curAg = editAg();
	}
	
	if ( _updateSgCBclient)
		_updateAllAgCB( _updateSgCBclient);

	setEditAgMenu( _editAgMenu);
	editAgDlog()->setDgChanged( false);
	int mode = editAgDlog()->getAModeId();		// returns -1 if no amode selected
	if ( mode != -1 && mode != curAg->mode())
		curAg->setAMode( agList()->AModes()->elem(mode), 1);	// init AG controls from AMode

	if ( curAg->amode()->hasVxyOutput() && curAg->dgList()->size() == 0)
		alert( "This Analysis Group needs at least one Display Group");
	_addGr = false;
}

void
HeaderFl::editAgDelete()
{
	if ( !ask( "Are you sure?"))
		return;

	if ( !_addGr)
	{
		agList()->agList()->remove( _AgEdit);
		delete _AgEdit;
	}
	delete editAg();
		
	if ( _updateSgCBclient)
		_updateAllAgCB( _updateSgCBclient);

	setEditAgMenu( _editAgMenu);
	editAgDlog()->setDgChanged( false);
	editAgDlog()->unpost();
	_addGr = false;
}

//
// either undo any DG changes or update menubar 'Display' Dg's
//
void
HeaderFl::editAgCancel()
{
	delete editAg();
	_addGr = false;
}

//----------------------------------------------------------------------
//	Edit RtAnalysis Gr Dialog callback functions
//----------------------------------------------------------------------
void
HeaderFl::editRtAgCallback(Widget *w, void * clientData)
{
	((HeaderFl *)clientData)->editRtAg();
}

void
HeaderFl::editRtAgOkCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editRtAgOk();
}

void
HeaderFl::editRtAgDeleteCB(Widget *, void *clientData)
{
	((HeaderFl *)clientData)->editRtAgDelete();
}

void
HeaderFl::editRtAgCancelCB(Widget *, void *clientData)
{
	// either undo any DG changes or update menubar 'Display' Dg's
	((HeaderFl *)clientData)->editRtAgCancel();
}


//----------------------------------------------------------------------
//	RtAnalysis Gr Edit Dialog functions
//----------------------------------------------------------------------
void
HeaderFl::editRtAg()
{
	if ( !_editRtAgDlog)
		_editRtAgDlog = new EditRtAgDlog();

	char str[256];
	int item = _editRtAgMenu->value();
	_editRtAg = rtAgList()->elem(item);
	
	if ( _editRtAg != 0)
	{
		_editRtAgCpy = new RtAnalysisGr( *_editRtAg);		// edit current ag, make copy
		_addGr = false;
	}
	else	// adding a new Ag
	{
		_editRtAg = _editRtAgCpy = new RtAnalysisGr( rtAgList());		// make newAg
		_addGr = true;
		item--;		// so Title AG # comes out right
	}

	// post it, can't postModal because other (child) dialogs wouldn't work
	sprintf( str, "Rt Analysis Group %d", item+1);
	editRtAgDlog()->post( 0, &HeaderFl::editRtAgOkCB,
		&HeaderFl::editRtAgCancelCB, &HeaderFl::editRtAgDeleteCB, this);
	editRtAgDlog()->setTitle( str);

	editRtAgDlog()->initValues( edRtAg());
}

void
HeaderFl::editRtAgOk()
{
	editRtAgDlog()->initFromDlog( edRtAg());
	
	if ( !_addGr)
	{
		*_editRtAg = *edRtAg();				// copy back to original
		_editRtAg->setParent( rtAgList());
		delete editAg();
	}
	else
	{
		rtAgList()->List()->push_back( edRtAg());			// add to list
	}
	setEditRtAgMenu( _editRtAgMenu);
	_addGr = false;

	if ( _rtAgClient)
		_updateRtAgCB( _rtAgClient);
}

void
HeaderFl::editRtAgDelete()
{
	if ( !ask( "Are you sure?"))
		return;

	if ( !_addGr)
	{
		rtAgList()->List()->remove( _editRtAg);
		delete _editRtAg;
	}
	delete _editRtAgCpy;

	if ( _rtAgClient)
		_updateRtAgCB( _rtAgClient);

	setEditRtAgMenu( _editRtAgMenu);
	editRtAgDlog()->unpost();
	_addGr = false;
}

void
HeaderFl::editRtAgCancel()
{
	delete _editRtAgCpy;
	_addGr = false;
}

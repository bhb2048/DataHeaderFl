//
//	EditAgDlog.cc
//
//	21-sep-01  bhb	add EditAgDlog::setEditAModeMenu()
//	09-dec-02  bhb	modified to use AnalyisGrVk (renamed from GlasAnalysisGr)
//	15-aug-06  bhb	Fltk version
//	07-sep-07  bhb	init _pathInput from getenv() if AG's blank
//	28-sep-12  bhb	now subclass of EditAgDlogUI
//
#include "EditAgDlog.h"
#include <Data/AnalysisGr.h>
#include <Data/DisplayGr.h>
#include "EditDgDlog.h"
#include <Data/AMode.h>
#include <fltk/ask.h>
#include <fltk/Divider.h>
#include <stdlib.h>							// getenv()
#include <string.h>							// strlen()
#include <iostream>

using namespace std;
using namespace fltk;

EditAgDlog::EditAgDlog( EditDgDlog *d) : _editDgDlog(d), _chanNamesDlog(0), _unitsDlog(0)
{
}

//
// Minimal Destructor. Base class destroys widgets.
//
EditAgDlog::~EditAgDlog()
{
	if ( chanNamesDlog())
	{
		chanNamesDlog()->hide();
		delete chanNamesDlog();
	}
	if ( unitsDlog())
	{
		unitsDlog()->hide();
		delete unitsDlog();
	}
}

void EditAgDlog::chanChanged()
{
	short unsigned int ch[NCHAN];
	int nch = ag()->gtchan( chan(), ch, NCHAN);
	_numChanOutput->value( nch);
	if ( !ag()->sameChan( ch, nch))
	{
		ag()->setChan( ch, nch);
		ag()->initDefNames( nch);
	}
}

void EditAgDlog::editAgDgCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	obj->editAgDg();
}

void EditAgDlog::editAgDg()
{
}

void EditAgDlog::editAgChanNames()
{
//	cout << "EditAgDlog::editAgChanNames" << endl;
	if ( chanNamesDlog() == 0)
		_chanNamesDlog = new ChannelNamesDlog();

	chanNamesDlog()->initChanNames( ag()->names(), ag()->numNames());
	
	if ( chanNamesDlog()->postAndWait() == Dialog::OK)
	{
//		cout << "OK" << endl;
		uint n = ag()->numNames();
		int linestart = 0;
		int pos = 0;
		for ( uint i=0; i<n; i++)
		{
			cout << i+1 << ": " << chanNamesDlog()->_textbuf->line_text(linestart) << endl;
			ag()->setChName( i, chanNamesDlog()->_textbuf->line_text(linestart));
			++pos;
			while ( chanNamesDlog()->_textbuf->line_start(pos) == linestart)
				pos++;
			linestart = chanNamesDlog()->_textbuf->line_start(pos);
		}
	}
	else
		cout << "Cancel" << endl;
}

void EditAgDlog::editAgChanUnits()
{
//	cout << "EditAgDlog::editAgChanUnits" << endl;
	if ( unitsDlog() == 0)
		_unitsDlog = new UnitsDlog();

	unitsDlog()->initUnits( ag()->units(0), ag()->units(1));

	if ( unitsDlog()->postAndWait() == Dialog::OK)
	{
		string str = unitsDlog()->unit1();
		ag()->setUnits( 0, str);
		str = unitsDlog()->unit2();
		ag()->setUnits( 1, str);
	}
}

void
EditAgDlog::initValues( AnalysisGr *ag)
{
	char str[256];

	// set values for this Ag
	// can't do Xt stuff until after createDialog()
	_ag = ag;
	_nameInput->value( (char *)ag->name());
	_chanInput->value( ag->numstr_1( str));
	_numChanOutput->value(ag->nChan());
	if ( strlen(ag->Dir()))
		setModelPath( (char *)ag->Dir());
	else
	{
		char *strptr = getenv( "GETPIC3_MODELS_DIR");
		if ( strptr)
			setModelPath( strptr);		// overridden by initValues()
	}
	setPicFile( (char *)ag->File1());
	_locInput->value( (char *)ag->File2());
	setEditDgMenu( ag);
	setEditAModeMenu( ag);
}

void
EditAgDlog::setEditDgMenu( AnalysisGr *ag)
{
	Choice *dgm = _dgMenu;		// get the option menu

	dgm->begin();
	for ( int i=dgm->children()-1; i>=0; i--)
	{
		Widget* o = dgm->child( i);
		dgm->remove( i);
		delete o;
	}
	for ( int j=0; j<ag->numDgs(); j++)
	{
		dgm->add( ag->dg(j)->name(), 0, &EditAgDlog::editDgCB, this);
	}
	new Divider();
	dgm->add("Add DG", 0, &EditAgDlog::editNewDgCB, this);
	dgm->end();
	if ( ag->numDgs() == 0)
		dgm->value(1);			// display 'Add DG'
	else
		dgm->value( ag->numDgs()-1);	// display last added Dg
	if ( ag->amode() != 0 && ag->amode()->hasVxyOutput())
		dgm->activate();
	else
		dgm->deactivate();
}

void
EditAgDlog::setEditAModeMenu( AnalysisGr *ag)
{
	Choice *menu = _modeMenu;		// get the option menu
	AModeList * amodes = ag->Parent()->AModes();

	// first menu item (index 0) is "Analysis Mode"
	for ( int i=menu->children()-1; i>=0; i--)
	{
		Widget* o = menu->child( i);
		menu->remove( i);
		delete o;
	}
	int index = 0;
	int mode = ag->mode();

	for ( unsigned int ami = 0; ami < amodes->size(); ami++)
	{
		menu->add( amodes->elem(ami)->name(), 0, &EditAgDlog::editAModeCB, this);
		if ( amodes->elem(ami)->id() == mode)
			index = ami+1;
	}
	if ( index > 0)
		menu->focus_index(index-1);
	menu->activate();
}

void
EditAgDlog::editAModeCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	obj->editAMode();
}

void
EditAgDlog::editAMode()
{
	// activate/deactivate DG menu
	int mode = getAModeId();		// returns -1 if no amode selected
	if ( mode != -1 && ag()->Parent()->AModes()->elem(mode)->hasVxyOutput())
		_dgMenu->activate();
	else
		_dgMenu->deactivate();

}

void
EditAgDlog::editDgCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	obj->postEditDg();
}

int
EditAgDlog::postEditDg()
{
	if ( editDgDlog() == 0)
		return 0;

	int i = _dgMenu->focus_index();			// get dg index
	if ( i < ag()->dgList()->size())		// _editDg already set if adding new
	{
		_addDg = false;
		_editDg = ag()->dg(i);				// save incase delete
	}
	else
		_addDg = true;						// so cancel will delete

	initAgFromDlog();		// initialize this ag before calling EditDgDlog

	editDgDlog()->setDg( editDg());	// store for initValues()->setEditDgMenu()

	// post it
	editDgDlog()->post("Edit Display Group", editDgOkCB, editDgCancelCB, editDgDeleteCB, this);

	char str[128];
	sprintf( str, "Display Group %d", editDg()->index()+1);
	editDgDlog()->setTitle( str);

	return 1;
}

void
EditAgDlog::editNewDgCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	obj->editNewDGr();
}

void
EditAgDlog::editNewDGr()
{
	_editDg = new DisplayGr();

	editDg()->setParentList( ag()->dgList());
	ag()->addDg( editDg());
	initAgFromDlog();		// initialize this ag before calling EditDgDlog
	editDg()->initDefault();
	// Cancel also deletes...
	postEditDg();
}

void
EditAgDlog::editDgOkCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	obj->editDgOk();
}

void
EditAgDlog::editDgOk()
{
	// init Dg from dialog values
	if ( !editDgDlog()->initDg())
	{
		ag()->dgList()->dglist()->remove( editDg());
		setEditDgMenu( ag());
		return;
	}

	// init _curDg to first
	 if ( ag()->dgList()->size() > 0)
		ag()->setCurDg( *ag()->dgList()->begin());

	// update Dg menu
	setEditDgMenu( ag());
	setDgChanged( true);
}

void
EditAgDlog::editDgCancelCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;
	if ( obj->addDg())
	{
		obj->ag()->dgList()->dglist()->remove( obj->editDg());
		delete obj->editDg();
	}
	obj->editDgDlog()->unpost();
}

void
EditAgDlog::editDgDeleteCB( Widget *, void *clientData)
{
	EditAgDlog *obj = (EditAgDlog *)clientData;

	if ( ask("Are you sure?"))
	{
		obj->ag()->dgList()->dglist()->remove( obj->editDg());		// STL list func
		delete obj->editDg();
	}

	// update EditSgDlog Dg menu
	obj->setEditDgMenu( obj->ag());
	obj->setDgChanged( true);
	obj->editDgDlog()->unpost();
}

//
//	Initialize AG from Dialog.
//	If Ag arg is not null, initialize it, else initialize _ag.
//
void
EditAgDlog::initAgFromDlog( AnalysisGr *Ag)
{
	const char *str;
	AnalysisGr *ag = (Ag != 0) ? Ag : _ag;

	ag->setName( name());
	int nch = ag->gtchan( chan());
	int mode = getAModeId();		// returns -1 if no amode selected
	if ( mode != -1)
	{
		ag->setAMode( ag->Parent()->AModes()->elem(mode), 1);
	}
	ag->initDefUnits( nch);
	if ( unitsDlog())
	{
		ag->setUnits( 0, unitsDlog()->unit1());
		ag->setUnits( 1, unitsDlog()->unit2());
	}
	if ( strlen( ag->units( 1)))
		ag->setnUnits( 2);
	str = modelPath();
	ag->setDir( str);
	str = picFile();
	ag->setFile1( str);
	str = _locInput->value();
	ag->setFile2( str);
}

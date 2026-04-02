
//
//	EditSgDlog.cc
//
//	07-nov-01  bhb	put editDgCB(), editNewDgCB() here from SgListVk, add _sg
//	15-aug-06  bhb	Fltk version
//
#include "EditSgDlog.h"
#include "HeaderFl.h"
#include <Data/SampleGr.h>
#include <Data/DisplayGr.h>
#include "EditDgDlog.h"
#include "ChannelNamesDlog.h"
#include <fltk/ask.h>
#include <fltk/Divider.h>
#include <iostream>

using namespace std;
using namespace fltk;

EditSgDlog::EditSgDlog( EditDgDlog *d) : EditSgDlogUI(), _editDgDlog(d), _chanNamesDlog(0)
{
}


EditSgDlog::~EditSgDlog()
{
	if ( chanNamesDlog())
	{
		_chanNamesDlog->hide();
		delete chanNamesDlog();
	}
}

void EditSgDlog::editSgChanNames()
{
	cout << "EditSgDlog::editSgChanNames" << endl;
	if ( chanNamesDlog() == 0)
		_chanNamesDlog = new ChannelNamesDlog();

	chanNamesDlog()->initChanNames( sg()->names(), sg()->numNames());
	
	if ( chanNamesDlog()->postAndWait() == Dialog::OK)
	{
		cout << "OK" << endl;
		uint n = sg()->numNames();
		int linestart = 0;
		int pos = 0;
		for ( uint i=0; i<n; i++)
		{
			cout << i+1 << ": " << chanNamesDlog()->_textbuf->line_text(linestart) << endl;
			sg()->setChName( i, chanNamesDlog()->_textbuf->line_text(linestart));
			++pos;
			while ( chanNamesDlog()->_textbuf->line_start(pos) == linestart)
				pos++;
			linestart = chanNamesDlog()->_textbuf->line_start(pos);
		}
	}
	else
		cout << "Cancel" << endl;
}

void EditSgDlog::editSgChanUnits()
{
}

void EditSgDlog::chanChanged()
{
	short unsigned int ch[NCHAN];
	int nch = sg()->gtchan( chan(), ch, NCHAN);
	_nchanOutput->value( nch);
	if ( !sg()->sameChan( ch, nch))
	{
		sg()->setChan( ch, nch);
		sg()->initDefNames( nch);
	}
}

void
EditSgDlog::initValues( SampleGr *sg)
{
	char str[256];

	// set EditSgDlog values for this Sg
	_sg = sg;
	_nameInput->value( (char *)sg->name());
	_chanInput->value( sg->numstr_1( str));
	_rateInput->value( sg->rate());

	// can't do Xt stuff until after createDialog()
	_nchanOutput->value( sg->nChan());
	setEditDgMenu( sg);
}

void
EditSgDlog::setEditDgMenu( SampleGr *sg)
{
	Choice *dgm = _dgMenu;		// get the option menu

	dgm->begin();
	for ( int i=dgm->children()-1; i>=0; i--)
	{
		Widget* o = dgm->child( i);
		dgm->remove( i);
		delete o;
	}
	for ( int j=0; j<sg->numDgs(); j++)
	{
		dgm->add( sg->dg(j)->name(), 0, &EditSgDlog::editDgCB, this);
	}
	new Divider();
	dgm->add("Add DG", 0, &EditSgDlog::editNewDgCB, this);
	dgm->end();
	if ( sg->numDgs() == 0)
		dgm->value(1);			// display 'Add DG'
	else
		dgm->value( sg->numDgs()-1);	// display last added Dg
	dgm->activate();
}


void
EditSgDlog::editDgCB( Widget *w, void *clientData)
{
	EditSgDlog *obj = (EditSgDlog *)clientData;
	obj->postEditDg();
}

int
EditSgDlog::postEditDg()
{
	if ( editDgDlog() == NULL)
		return 0;

	int i = _dgMenu->focus_index();			// get dg index
	if ( i < sg()->dgList()->size())		// _editDg already set if adding new
	{
		_addDg = false;
		_editDg = sg()->dg(i);				// save incase delete
	}
	else
		_addDg = true;						// so cancel will delete

	initSgFromDlog();		// initialize this sg before calling EditDgDlog

	editDgDlog()->setDg( editDg());	// store for prepost initValues

	// post it
	editDgDlog()->post( NULL, editDgOkCB, editDgCancelCB, editDgDeleteCB, this);

	char str[128];
	sprintf( str, "Display Group %d", editDg()->index()+1);
	editDgDlog()->setTitle( str);

	return 1;
}

void
EditSgDlog::editNewDgCB( Widget *, void *clientData)
{
	EditSgDlog *obj = (EditSgDlog *)clientData;
	obj->editNewDGr();
}

void
EditSgDlog::editNewDGr()
{
	_editDg = new DisplayGr();

	editDg()->setParentList( sg()->dgList());
	sg()->addDg( editDg());
	initSgFromDlog();		// initialize this sg before calling EditDgDlog
	editDg()->initDefault();
	// Cancel also deletes...
	postEditDg();
}

void
EditSgDlog::editDgOkCB( Widget *, void *clientData)
{
	EditSgDlog *obj = (EditSgDlog *)clientData;
	obj->editDgOk();
}

void
EditSgDlog::editDgOk()
{
	// init Dg from dialog values
	if ( !editDgDlog()->initDg())
	{
		sg()->dgList()->dglist()->remove( editDg());
		setEditDgMenu( sg());
		return;
	}

	// init _curDg to first
	if ( sg()->dgList()->size() > 0)
		sg()->setCurDg( *sg()->dgList()->begin());

	// update Dg menu
	setEditDgMenu( sg());
	setDgChanged( true);
}

void
EditSgDlog::editDgCancelCB( Widget *, void *clientData)
{
	EditSgDlog *obj = (EditSgDlog *)clientData;
	if ( obj->addDg())
	{
		obj->sg()->dgList()->dglist()->remove( obj->editDg());
		delete obj->editDg();
	}
	obj->editDgDlog()->unpost();
}

void
EditSgDlog::editDgDeleteCB( Widget *, void *clientData)
{
	EditSgDlog *obj = (EditSgDlog *)clientData;

	if ( ask("Are you sure?"))
	{
		obj->sg()->dgList()->dglist()->remove( obj->editDg());		// STL list func
		delete obj->editDg();
	}

	// update EditSgDlog Dg menu
	obj->setEditDgMenu( obj->sg());
	obj->setDgChanged( true);
	obj->editDgDlog()->unpost();
}

//
//	Initialize AG from Dialog.
//	If Ag arg is not null, initialize it, else initialize _ag.
//
void
EditSgDlog::initSgFromDlog( SampleGr *Sg)
{
	const char *str;
	SampleGr *sg = (Sg != NULL) ? Sg : _sg;

	sg->setName( name());
	int nch = sg->gtchan( chan());
	sg->initDefUnits( nch);
}

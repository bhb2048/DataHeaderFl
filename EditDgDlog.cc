//
//	EditDgDlog.h
//
//	Modified:
//	18-sep-02  bhb	fixed up initValues() to use dg Parent chan right
//	08-sep-05  bhb	updated initValues(), initDg() for large chan strings
//	15-aug-06  bhb	Fltk version
///
#include "EditDgDlog.h"
#include <Data/ChannelSet.h>
#include <fltk/ask.h>
#include <iostream>

using namespace std;
using namespace fltk;

EditDgDlog::EditDgDlog() : EditDgDlogUI()
{
}


//
// Minimal Destructor. Base class destroys widgets.
//
EditDgDlog::~EditDgDlog()
{
}

void EditDgDlog::chanChanged()
{
	short unsigned int ch[NCHAN];
	int nch = editDg()->gtchan( chan(), ch, NCHAN);
	_nchanOutput->value( nch);
	if ( nch < 8 && ncol() > 1)
		_nColInput->value( 1);
}

//
//	init dialog from DisplayGr
//
void EditDgDlog::initValues( DisplayGr *dg)
{
	char str[2048];
	unsigned short chan[NCHAN];
	int nchan = NCHAN;

	// set EditDgDlog values for this Dg
	_nameInput->value( (char *)dg->name());
	_parentOutput->value( (char *)dg->getParent()->name());
	_dynamicToggleButton->value( dg->mode() == DG_DYNAMIC);
	_staticToggleButton->value( dg->mode() == DG_STATIC);
	_gridOnToggleButton->value( dg->grid() == true);
	_gridOffToggleButton->value( dg->grid() == false);
	_nColInput->value( dg->nColumn());
	_msecInput->value( int(dg->xwind()));
	// Display group channels are indices in parent chan array but we display
	// parent channels
	nchan = dg->getParentChan( chan);
	_chanInput->value( dg->numstr_1( str, chan, nchan));
	_nchanOutput->value( nchan);
}

//
//	init DisplayGr from dialog values
//
int EditDgDlog::initDg( DisplayGr *dg)
{
	unsigned short ch[NCHAN];
	int nch, max, nc;
	char chstr[2048];
	ChannelSet *pcs;			// parent channelset

	if ( dg == NULL)
		dg = editDg();
	if ( dg == NULL)
		return 0;

	pcs = dg->getParent();
	if ( pcs == NULL)
		return 0;		// shouldn't happen

	// first check for legal values
	strcpy( chstr, chan());
	nch = dg->gtchan( chstr, ch, NCHAN);
	max = ( pcs->nChan() < DG_MAX_DISPCH) ? pcs->nChan() : DG_MAX_DISPCH;	// lessor of

	if ( nch <= 0)
	{
		alert( "Error in channel list");
		return 0;
	}
	else if ( nch > max)
	{
		alert( "Too many channels");
		return 0;
	}
	else										// check that each can in parent group
	{
		for ( int i=0; i<nch; i++)
		{											// check to see that parent contains each channel #
			if ( pcs->getIndx( ch[i]) < 0)
			{
				alert( "Error: channel %d is not in parent group.", ch[i]+1);
				return 0;
			}
		}
	}

	if ( dynValue()) 	// check that parent chan contiguous
	{
		// if dlog chan aren't contiguous, are ignored anyway, only first and nchan matter.
		// but check anyway because they get initialized
		for ( int i=0; i<nch-1; i++)
		{
			if ( ch[i] + 1 != ch[i+1])
			{
				alert( "Error - dynamic DG channels must be contiguous");
				return 0;
			}
		}
		if ( !pcs->isContiguous())
		{
			alert( "Error - may only have dynamic Dg if Sg channels are contiguous");
			return 0;
		}
	}

	nc = ncol();
	if ( nc < 0 || nc > DG_MAX_COL || nc > nch)
	{
		alert("Illegal number of columns");
		return 0;
	}

	// all ok, set dg values from dialog
	dg->setName( dgname());
	// parent Dglist set by parent EditDlog
	// parent set when added to parent's DgList
	dg->setMode( dynValue() ? DG_DYNAMIC : DG_STATIC);
	dg->setGrid( gridValue());
	dg->setnColumn( ncol());
	dg->setxWind( float(msec()));
	// convert parent channels into parent chan array indices
	dg->setChanFromParent(ch, nch);
	return 1;
}
// End user code block <tail>

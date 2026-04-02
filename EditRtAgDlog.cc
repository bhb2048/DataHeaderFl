//
//	EditRtAgDlog.cc
//
//	25-feb-13  bhb
//	Modified:
//	
#include "EditRtAgDlog.h"
#include <Data/RtAnalysisGr.h>
#include <string>

using namespace std;
using namespace fltk;

#define NUM_RTA_MODES	3
static string RtAModes[] = 
{
	"Area Ratio",
	"Rate BPM",
	"Rate Msec"
};

EditRtAgDlog::EditRtAgDlog()
{
	
}

EditRtAgDlog::~EditRtAgDlog()
{
	
}

//
//	Initialize Dialog values from RtAnalysisGr
//	
void
EditRtAgDlog::initValues( RtAnalysisGr *rtag)
{
	char str[256];

	// set values for this Ag
	// can't do Xt stuff until after createDialog()
	_RtAg = rtag;
	_nameInput->value( (char *)rtag->name());
	_chanInput->value( rtag->numstr_1( str));
	_minCycleInput->value( float(rtag->minCycle()));
	
	for ( int i=_analysisTypeChoice->children()-1; i>=0; i--)
	{
		Widget* o = _analysisTypeChoice->child( i);
		_analysisTypeChoice->remove( i);
		delete o;
	}
	int index = -1;
	int mode = rtag->type();

	for ( int ami = 0; ami < NUM_RTA_MODES; ami++)
	{
		_analysisTypeChoice->add( RtAModes[ami].c_str(), 0, 0, 0);
		if ( ami + 1 == mode)
			index = ami;
	}
	if ( index >= 0)
		_analysisTypeChoice->focus_index(index);
	_analysisTypeChoice->activate();

	setInstructions();
}

void
EditRtAgDlog::initFromDlog( RtAnalysisGr *rtag)
{
	short unsigned int ch[NCHAN];
	int nch = rtag->gtchan( chan(), ch, NCHAN);

	rtag->setName( name());
	rtag->setChan( ch, nch);
	rtag->type( analysisType());
	rtag->minCycle( minCycle());
}

void
EditRtAgDlog::setInstructions( bool gui)
{
	int type;
	
	type = gui ? _analysisTypeChoice->value()+1 : _RtAg->type();
		
	switch ( type)
	{
		case 0:
		case 1:			// Area Ratio
			_chanInstructionsBox->label( "Two channels, first numerator, second denominator");
			break;
		case 2:			// Heart Rate BPM
		case 3:			// Heart Rate Msec
			_chanInstructionsBox->label( "Heart rate channel");
			break;
		default:
			break;
	}
	window()->redraw();
}

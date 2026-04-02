//
//	ChannelNamesDlog.cc - subclass of ChannelNamesDlogUI for editing channel names
//	
//	23-oct-12  bhb
//	Modified:
//	
#include "ChannelNamesDlog.h"
#include <Data/ChannelSet.h>
#include <sstream>

using namespace std;

void
ChannelNamesDlog::initChanNames( std::string *names, uint n)
{
	uint i;
	uint max = n > NCHANNAMES ? NCHANNAMES : n;

	// erase all
	_textbuf->remove( 0, _textbuf->length());
//	_textbuf->text(0);	// supposed to replace entire contents of the text buffer, didn't work
	
	for ( i = 0; i < max; i++)
	{
		ostringstream oss;
		oss << names[i] << endl;
		_textbuf->append( oss.str().c_str());
	}
	for ( ; i < n; i++)
	{
		ostringstream oss;
		oss << i+1 << endl;
		_textbuf->append( oss.str().c_str());
	}
}
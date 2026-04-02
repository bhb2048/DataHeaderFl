//
//	InitHdrDlog.cc - Class for initialize Header Dialog
//
//	08-nov-05  bhb	split from HeaderVk
//	Modified:
//	03-oct-12  bhb	now subclass of InitHdrDlogUI
//
#include "InitHdrDlog.h"
#include <string.h>

using namespace std;
using namespace fltk;

string	InitHdrDlog::_retstr;			// return string

InitHdrDlog::InitHdrDlog( HeaderFl *hdr) : InitHdrDlogUI( hdr)
{
	_showApply = false;
}

InitHdrDlog::~InitHdrDlog()
{
}

const char *
InitHdrDlog::filename()
{
_retstr = _filename->value();
return _retstr.c_str();
}

const char *
InitHdrDlog::studytitle()
{
_retstr = _studytitle->value();
return _retstr.c_str();
}

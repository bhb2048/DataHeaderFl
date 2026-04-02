//
//	InitHdrDlog.h - Class for initialize Header Dialog
//
//	08-nov-05  bhb	split from HeaderVk
//	Modified:
//	03-oct-12  bhb	now subclass of InitHdrDlogUI
//	
#ifndef INITHDRDLOG_H
#define INITHDRDLOG_H

#include "UI/InitHdrDlogUI.h"
#include <string>

class InitHdrDlog : public InitHdrDlogUI
{
	public:
		InitHdrDlog( HeaderFl *hdr);
		virtual ~InitHdrDlog();

		const char *	filename();
		bool			filenameChanged()	{ return _filename->changed();	}
		const char *	studytitle();
		bool			studytitleChanged()	{ return _studytitle->changed();	}

	private:
		static std::string		_retstr;

};
#endif

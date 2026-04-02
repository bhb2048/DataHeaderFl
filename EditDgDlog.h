//
//	EditDgDlog.h
//
//
//	Modified:
//	13-jun-02  bhb	add initDg()
//	15-aug-06  bhb	Fltk version
//	03-oct-12  bhb	now subclass of EditDgDlogUI
//	
#ifndef EditDgDlog_H
#define EditDgDlog_H
#include "UI/EditDgDlogUI.h"

class DisplayGr;

class EditDgDlog : public EditDgDlogUI
{
	public:

		EditDgDlog();
		virtual ~EditDgDlog();

		const char *	dgname()				{ return _nameInput->value();	}
		int				msec()					{ return _msecInput->ivalue();	}
		const char *	chan()					{ return _chanInput->value();	}
		int				ncol()					{ return int(_nColInput->value());	}
		int				nchan()					{ return int(_nchanOutput->value());	}
		int				dynValue()				{ return _dynamicToggleButton->value();	}
		int				gridValue()				{ return _gridOnToggleButton->value();	}
		virtual void	initValues( DisplayGr *);
		int				initDg( DisplayGr *dg=NULL);

	protected:

		virtual void chanChanged();
};
#endif

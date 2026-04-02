//
//	EditRtAgDlog.h
//
//	25-feb-13  bhb
//	Modified:
//	
#ifndef EditRtAgDlog_H
#define EditRtAgDlog_H

#include "UI/EditRtAgDlogUI.h"

class RtAnalysisGr;

class EditRtAgDlog : public EditRtAgDlogUI
{
	public:
		EditRtAgDlog();
		virtual ~EditRtAgDlog();

		RtAnalysisGr * 	RtAg()		{ return _RtAg;	}
		void			initValues( RtAnalysisGr *ag);
		void			initFromDlog( RtAnalysisGr *rtag);
		virtual void	setInstructions( bool gui=false);
		
	protected:
		RtAnalysisGr *		_RtAg;				// analysis group we're editing
	
};
#endif
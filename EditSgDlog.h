//
// 	EditSgDlog.h
//
//  Modified:
//  03-feb-01  bhb	added _DgChanged var & fcns, & setEditDgMenu()
//	07-nov-01  bhb	put editDgCB(), editNewDgCB() here from GlasSgList, add _sg
//	09-nov-05  bhb	rework EditDgDlog, make pointer to a member
//	15-aug-06  bhb	Fltk version
//	03-oct-12  bhb	now subclass of EditSgDlogUI
//	
#ifndef EditSgDlog_H
#define EditSgDlog_H
#include "UI/EditSgDlogUI.h"
#include "ChannelNamesDlog.h"

class SampleGr;
class DisplayGr;
class ChanNamesDlog;
class EditDgDlog;

class EditSgDlog : public EditSgDlogUI
{
	public:

		EditSgDlog( EditDgDlog *d);
		virtual ~EditSgDlog();

		SampleGr *		sg()			{ return _sg;	}
		void        	setDgChanged( bool val) { _DgChanged = val; }
		bool     		DgChanged() 	{ return _DgChanged; }
		void			initValues( SampleGr *sg);
		void        	setEditDgMenu( SampleGr *sg);
		DisplayGr *		editDg()		{ return _editDg;	}
		bool			addDg()			{ return _addDg;	}
		EditDgDlog *	editDgDlog()    { return _editDgDlog; }
		ChannelNamesDlog *chanNamesDlog()	{ return _chanNamesDlog;	}
		int				postEditDg();
		void			editNewDGr();
		void			initSgFromDlog( SampleGr *Sg = NULL);

	protected:

		virtual void editSgChanNames();
		virtual void editSgChanUnits();
		virtual void chanChanged();

		SampleGr *		_sg;						// the SampleGr we are editing
		bool     		_DgChanged;					// keep track of whether any Dg's changed
		DisplayGr *		_editDg;
		bool			_addDg;						// so editDgCancelCB can delete
		EditDgDlog *	_editDgDlog;
		ChannelNamesDlog *	_chanNamesDlog;

	private:

		static void editDgCB( fltk::Widget *, void *);
		static void editNewDgCB( fltk::Widget *, void *);
		static void editDgOkCB( fltk::Widget *, void *);
		static void editDgCancelCB( fltk::Widget *, void *);
		static void editDgDeleteCB( fltk::Widget *, void *);

		void editDgOk();
};
#endif

//
//	EditAgDlog.h
//
//	07-nov-01  bhb	put editDgCB(), editNewDgCB() here from GlasSgList, add _sg
//	30-sep-02  bhb	added _pathLabeledText, _picLabeledText, _locLabeledText
//					and access functions.
//	02-oct-02  bhb	add initAgFromDlog()
//	15-aug-06  bhb	Fltk version
//	28-sep-12  bhb	now subclass of EditAgDlogUI
//	15-nov-12  bhb	add UnitsDlog
//	
#ifndef EditAgDlog_H
#define EditAgDlog_H
#include "UI/EditAgDlogUI.h"
#include "ChannelNamesDlog.h"
#include "UnitsDlog.h"

class AnalysisGr;
class DisplayGr;
class EditDgDlog;

class EditAgDlog : public EditAgDlogUI
{
	public:

		EditAgDlog( EditDgDlog *d);
		virtual ~EditAgDlog();

		AnalysisGr * 	ag()		{ return _ag;	}
//		char *		modelDir();
//		char *		picFile();
//		char *		locFile();

		void        setDgChanged( bool val) { _DgChanged = val; }
		bool     	DgChanged() 			{ return _DgChanged; }
		void		initValues( AnalysisGr *ag);
		void		setEditDgMenu( AnalysisGr *ag);
		void		setEditAModeMenu( AnalysisGr *ag);
		DisplayGr *	editDg()				{ return _editDg;	}
		bool		addDg()					{ return _addDg;	}
		EditDgDlog *editDgDlog()    		{ return _editDgDlog; }
		ChannelNamesDlog *chanNamesDlog()	{ return _chanNamesDlog;	}
		UnitsDlog *	unitsDlog()				{ return _unitsDlog;	}
		
		int			postEditDg();
		void		editNewDGr();
		void		initAgFromDlog( AnalysisGr *Ag = 0);

	protected:
		virtual void chanChanged();
		virtual void editAgDg();
		virtual void editAgChanNames();
		virtual void editAgChanUnits();
		
		AnalysisGr *		_ag;				// analysis group we're editing
		bool				_DgChanged;			// keep track of whether any Dg's changed
		DisplayGr *			_editDg;
		bool				_addDg;				// so editDgCancelCB can delete
		EditDgDlog *		_editDgDlog;
		ChannelNamesDlog *	_chanNamesDlog;
		UnitsDlog *			_unitsDlog;
		
		void		editAMode();

	private:

		static bool        _initAppDefaults;

		static void editAgDgCB( fltk::Widget *, void *);

		static void editAModeCB( fltk::Widget *, void *);
		static void editDgCB( fltk::Widget *, void *);
		static void editNewDgCB( fltk::Widget *, void *);
		static void editDgOkCB( fltk::Widget *, void *);
		static void editDgCancelCB( fltk::Widget *, void *);
		static void editDgDeleteCB( fltk::Widget *, void *);
		void	editDgOk();
};
#endif

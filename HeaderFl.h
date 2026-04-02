//*****************************************************************************
//	Copywrite (C) 1992-2006 Barry H. Branham.  All rights reserved.
//*****************************************************************************
//**********************************************************************
//	HeaaderFl.h -- subclass of DataHeader for Glas for editing SampleGr and AnalysisGr
//
//  09-dec-02  bhb	from GlasHeader.cc
//	Version 3.0
//	Modified:
//	09-dec-02  bhb	moved to libHeaderVk from GlasHeader
//	10-dec-02  bhb	add InitHdrDlog class from DasHeader
//	05-nov-05  bhb	change _filename to string
//	09-nov-05  bhb	major redo: putting edit group functions here instead of grpLists
//	07-aug-06  bhb	Fltk version
//	10-oct-06  bhb	fixup Sg & Ag editing, get rid of editNewSgCallback(), editNewSg()
//	04-mar-08  bhb	change initHdr() to use postAndWait(), and to bool return,
//					remove initHdrChgCB(), initHdrChg()
//	07-oct-08  bhb	revise initHdr() to fix file check bug, no setDate bool param
//	24-nov-09  bhb	removed deleteDg()
//	09-mar-13  bhb	removed setCheckSgFunc(), add setUpdateRtAgCB(), _updateRtAgCB,
//					_rtAgClient
//	
//**********************************************************************
#ifndef HEADERFL_H
#define HEADERFL_H

#include <Data/DataHeader.h>
#include <fltk/Widget.h>
#include <fltk/Menu.h>

class SampleGr;
class SgList;
class AnalysisGr;
class AgList;
class RtAnalysisGr;
class EditSgDlog;
class EditAgDlog;
class EditDgDlog;
class EditRtAgDlog;
class InitHdrDlog;

//////////////////////////////////////////////////////////////////////////
//  GUI for DataHeader
//////////////////////////////////////////////////////////////////////////

class   HeaderFl : public DataHeader
{
	friend class SampleGr;		// need to avoid 'callCallbacks() protected'
//	friend class SgList;		// error in SampleGr::editOK()
	friend class AnalysisGr;	// need to avoid 'callCallbacks() protected'

	
	public:
		HeaderFl();
		virtual ~HeaderFl();

		//  access

		void        	setEditSgMenu( fltk::Menu *em);
		void        	setEditAgMenu( fltk::Menu *em);
		void			setEditRtAgMenu( fltk::Menu *em);
		EditSgDlog  *   editSgDlog()    { return _editSgDlog; }
		EditAgDlog  *   editAgDlog()    { return _editAgDlog; }
		EditDgDlog  *   editDgDlog()    { return _editDgDlog; }
		EditRtAgDlog*	editRtAgDlog()	{ return _editRtAgDlog;	}
		
		SampleGr *		editSg()		{ return _editSg;	}
		AnalysisGr *	editAg()		{ return _editAg;	}
		RtAnalysisGr*	edRtAg()		{ return _editRtAgCpy;	}

		// actions

		void 			editSg( fltk::Widget *w);
		void 			editAg( fltk::Widget *w);

		bool        	initHdr();
		
		// callbacks for Sg editing
		static void editSgCallback( fltk::Widget* o, void* v);
		static void editSgOkCB( fltk::Widget* o, void* v);
		static void editSgDeleteCB( fltk::Widget* o, void* v);
		static void editSgCancelCB( fltk::Widget* o, void* v);

		void			editSgOk();
		void        	editSgDelete();
		void			editSgCancel();

		// callbacks for Ag editing
		static void editAgCallback( fltk::Widget* o, void* v);
		static void editAgOkCB( fltk::Widget* o, void* v);
		static void editAgDeleteCB( fltk::Widget* o, void* v);
		static void editAgCancelCB( fltk::Widget* o, void* v);
	
		void        	editAgOk();
		void        	editAgDelete();
		void        	editAgCancel();

		void			setUpdateSgCB( void (*cb)( void *, void *), void *clientData)	
							{ _updateSgCB = cb;	_updateSgCBclient = clientData;	}
		void			setUpdateAllSgCB( void (*cb)( void *), void *clientData)	
							{ _updateAllSgCB = cb; _updateSgCBclient = clientData; }
		void			setUpdateAllAgCB( void (*cb)( void *), void *clientData)	
							{ _updateAllAgCB = cb; _agClient = clientData; }
		void			setUpdateRtAgCB( void (*cb)( void *), void *clientData)
							{ _updateRtAgCB = cb; _rtAgClient = clientData; }

	protected:
		InitHdrDlog *	_initHdrDlog;
		EditSgDlog  *	_editSgDlog;
		EditAgDlog  *	_editAgDlog;
		EditDgDlog  *	_editDgDlog;
		EditRtAgDlog*	_editRtAgDlog;
		
		fltk::Menu *	_editSgMenu;
		SampleGr *		_editSg;			// copy to edit
		SampleGr *		_SgEdit;			// original
		bool			_addGr;				// true if adding an SG or AG

		fltk::Menu *	_editAgMenu;
		AnalysisGr *	_editAg;			// copy to edit
		AnalysisGr *	_AgEdit;			// original

		fltk::Menu *	_editRtAgMenu;
		RtAnalysisGr *	_editRtAgCpy;		// copy to edit
		RtAnalysisGr *	_editRtAg;			// original
		
		void 			(*_updateSgCB)( void *, void *);
		void *			_updateSgCBclient;
		void 			(*_updateAllSgCB)( void *);
		void 			(*_updateAllAgCB)( void *);
		void *			_agClient;
		void 			(*_updateRtAgCB)( void *);
		void *			_rtAgClient;

		static void editRtAgCallback( fltk::Widget *w, void * clientData);
		static void editRtAgOkCB( fltk::Widget* o, void* v);
		static void editRtAgDeleteCB( fltk::Widget* o, void* v);
		static void editRtAgCancelCB( fltk::Widget* o, void* v);
		
		virtual void	editRtAg();
		void			editRtAgOk();
		void			editRtAgDelete();
		void			editRtAgCancel();
};

#endif

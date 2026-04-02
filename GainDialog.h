//////////////////////////////////////////////////////////////////////////
//  GainDialog.h - Gain Dialog Class
//
//  05-feb-01  bhb	split out of Das.h
//  Modified:
//  11-dec-02  bhb	moved Glas/Dash versions to HeaderVk
//  12-dec-02  bhb	added expand Button
//	03-oct-12  bhb	now subclass of GainDialogUI
//	
//////////////////////////////////////////////////////////////////////////
#ifndef GAINDIALOG_H
#define GAINDIALOG_H

#include <HdrFl/GainDialogUI.h>
#include <Data/AdGain.h>

#define MAX_BOARDS	8

class GainDialog : public GainDialogUI
{
	public:
		GainDialog( int nBank, int nChBk, int nBd, int nChBd);
		virtual ~GainDialog();

		int		ampGains(int i)  		{ return _ampGains[i]->value();	}
		int		adcGains(int i)    		{ return _adcGains[i]->value();	}
		void	setValues( AdGain *gn)	{ _adGain = gn;	}

	protected:

		int				_nBank;
		int				_nChanBank;			// channels per amp bank
		int				_nBoard;
		int				_nChanBoard;		// channels per adc board
		AdGain *		_adGain;

		virtual int		initValues();
		int     getAmpGainIndex( float gn);
		int     getAdcGainIndex( int gn);

};
#endif

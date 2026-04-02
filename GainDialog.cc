//////////////////////////////////////////////////////////////////////////
//  GainDialog.cc - Gain Dialog Class
//
//  Callback: DAS::setGainsCB()
//
//  05-feb-01  bhb	split out of Das.h
//  Modified:
//	05-feb-01  bhb	add _nChanBank, _nChanBoard, make bank gain and adcVrange labels
//					adjustable
//  11-dec-02  bhb	moved Glas/Dash versions to HeaderVk
//  12-dec-02  bhb	added expand Button
//
//////////////////////////////////////////////////////////////////////////
#include "GainDialog.h"
#include <stdio.h>

using namespace fltk;

GainDialog::GainDialog( int nBank, int nChBk, int nBd, int nChBd)
						: _nBank(nBank), _nChanBank(nChBk), _nBoard(nBd), _nChanBoard(nChBd),
						_adGain(0), GainDialogUI()
{
}

GainDialog::~GainDialog()
{
}

//
//  Set Dialog values from AdGain object
//  Call only after dialog has been created. from prepost().
//
int
GainDialog::initValues()
{
	if ( _adGain == NULL)
		return 0;

	if ( _nBank != _adGain->numBanks())
		return false;

	for ( int bank = 0; bank < _nBank; bank++)
	{
		int bi = getAmpGainIndex(_adGain->bankGain( bank));
		if ( bi >= 0)
			_ampGains[bank]->value( bi);
	}

	if ( _nBoard > 4)
		_nBoard = 4;

	if ( _nBoard > 0)
	{
		for ( int adc = 0; adc < _nBoard; adc++)
		{
			// assume same gain for all chan on board!
			int ai = getAdcGainIndex(_adGain->adcGain(adc * _nChanBoard));
			if ( ai >= 0)
				_adcGains[adc]->value( ai);
		}
	}
	return 1;
}

int
GainDialog::getAmpGainIndex( float gn)
{
	float gains[] = {1.0, 18.75, 31.25, 62.5, 125, 250, 500, 1000, 2000, 4000};

	for ( int i=0; i<9; i++)
		if ( gn == gains[i])
			return i;
	return -1;
}

int
GainDialog::getAdcGainIndex( int gn)
{
	float gains[] = { 1, 2, 4, 8};

	for ( int i=0; i<4; i++)
		if ( gn == gains[i])
			return i;
	return -1;
}

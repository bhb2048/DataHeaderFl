//
//	ModuleNode.h - FlNode subclass for AModule's
//
//	11-nov-05  bhb	split from AModule.cc
//	Modified:
//	16-nov-12  bhb	add VolCathFlowControls
//	28-nov-12  bhb	add VolCathTauControls
//	01-feb-13  bhb	add Beat2Controls
//	
#include "ModuleNode.h"
#include <AModules/AModuleControlsInc.h>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//	ModuleNode class - subclass FlNode so EditAMod graph nodes can point to AModule
///////////////////////////////////////////////////////////////////////////////
void
ModuleNode::print()
{
	int i;
	
	cout << "Node: " << label() << endl;
	cout << "  Parents: ";
	for ( i=0; i<nParents(); i++)
		cout << parent(i)->label() << ", ";
	cout << endl;
	cout << "  Children: ";
	for ( i=0; i<nChildren(); i++)
		cout << child(i)->label() << ", ";
	cout << endl;
}

///////////////////////////////////////////////////////////////////////////////
//	factory method for creating module Control Dialogs
///////////////////////////////////////////////////////////////////////////////
ModuleControls *
ModuleNode::createModuleControls( AModule *am)
{
	ModuleControls *ctrl = 0;
	
	switch( am->getId())
	{
		case AModule::DS_MODULE:
			break;
		case AModule::ACTIVITY_MODULE:
			ctrl = new ActivityControls();	break;
		case AModule::AREA_MODULE:
			ctrl = new AreaControls();			break;
		case AModule::BACT_MODULE:
			ctrl = new BiActControls();			break;
		case AModule::BASEV_MODULE:
			ctrl = new BaseControls();			break;
		case AModule::BEAT_MODULE:
			ctrl = new BeatControls();			break;
		case AModule::DVDT1_MODULE:
			ctrl = new DvdtControls();			break;
		// filter
		case AModule::FILTER_MODULE:
			ctrl = new FilterControls();		break;
		case AModule::SMOOTH_MODULE:
			ctrl = new SmoothControls();		break;
		case AModule::SIGAVG_MODULE:
			ctrl = new SignalAvgControls();		break;
		// hemo
		case AModule::PEAKINT_MODULE:
			ctrl = new PeakIntControls();		break;
		case AModule::SYSDIA_MODULE:
			ctrl = new SysDiaControls();		break;
		// operations (ops)
		case AModule::OPER1_MODULE:
			ctrl = new Oper1Controls();			break;
		case AModule::OPER2_MODULE:
			ctrl = new Oper2Controls();			break;
		case AModule::OPINT_MODULE:
			ctrl = new OpIntControls();			break;
		case AModule::OPPICK_MODULE:
			ctrl = new OpPickControls();		break;
		case AModule::OPINTPICK_MODULE:
			ctrl = new OpIntPickControls();		break;
		case AModule::OPVEC_MODULE:
			ctrl = new OpVecControls();			break;
		case AModule::OPVEC2_MODULE:
			ctrl = new OpVec2Controls();		break;
		case AModule::OPRANGE_MODULE:
			ctrl = new OpRangeControls();		break;
		// fft
		case AModule::FFT_MODULE:
			ctrl = new FftControls();			break;
		case AModule::PWELCH_MODULE:
			ctrl = new PwelchControls();		break;
		case AModule::MAXFREQ_MODULE:
			ctrl = new MaxFreqControls();		break;
		// qrst
		case AModule::QWAVE_MODULE:
			ctrl = new QWaveControls();			break;
		case AModule::TWAVE_MODULE:
			ctrl = new TWaveControls();			break;
		case AModule::UPPA_MODULE:
			ctrl = new UppaControls();			break;
		case AModule::SPATIALAVG_MODULE:
			ctrl = new SpatialControls();		break;
		case AModule::ACTPOTDUR_MODULE:
			break;
		case AModule::PVLOOP_MODULE:
			ctrl = new PVLoopControls();		break;
		case AModule::VOLCATH_CH_MODULE:
			ctrl = new VolCathChanControls();	break;
		case AModule::VOLCATH_DVDT_MODULE:
			break;
		case AModule::VOLCATH_DATA_MODULE:
			break;
		case AModule::VOLCATH_FLOW_MODULE:
			break;
		case AModule::VOLCATH_TAU_MODULE:
			ctrl = new VolCathTauControls();	break;
		case AModule::VOLCATH_OCL_MODULE:
			ctrl = new VolCathOclControls();	break;
		case AModule::BEAT2_MODULE:
			ctrl = new Beat2Controls();	break;
		default:
			cerr << "ModuleNode::createModuleControls: INVALID MODULE" << endl;;
			break;
	}
	if ( ctrl != 0)
	{
		ctrl->setMod( am);
		am->setControls( ctrl);
	}
	return ctrl;
}

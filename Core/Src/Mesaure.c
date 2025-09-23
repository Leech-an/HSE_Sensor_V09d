



#define _USE_MATH_DEFINES
#include <math.h>

#include <Const.h>
#include <Config.h>
#include <Parameter.h>
#include <G_Value.h>

#include <Measure.h>

void Calc_Init(void)
{
	uint8_t i = 0;
	for(i = PM_CH1; i < PM_CH_MAX; i++)
	{
		tPmResult1[i].fMass_Concet = 0.0f;
		tPmResult1[i].fTmpMassConcet = 0.0f;
		tPmResult1[i].C = 0.0f;
		tPmResult1[i].M = 0.0f;
		tPmResult1[i].fNum_Concet = 0.0f;
		tPmResult1[i].nTmpNumConcet = 0;

		tPmResult1[i].fTps = 0.0f;
	}

	tPmResult2.fCFU = 0.0f;
	tPmResult2.fNumConcetBio = 0.0f;
	tPmResult2.fResultTps = 0.0f;
	tPmResult2.fReverved = 0.0f;
}

void Calc_GetSelNumConnect(void)
{
	tPmResult1[PM_CH1].C = (float)( (tPmResult1[PM_CH1].nTmpNumConcet * tParam[PM_CH1].Cal));
	tPmResult1[PM_CH2].C = (float)( (tPmResult1[PM_CH2].nTmpNumConcet * tParam[PM_CH2].Cal));
	tPmResult1[PM_CH3].C = (float)( (tPmResult1[PM_CH3].nTmpNumConcet * tParam[PM_CH3].Cal));
	tPmResult1[PM_CH4].C = (float)( (tPmResult1[PM_CH4].nTmpNumConcet * tParam[PM_CH4].Cal));
	tPmResult1[PM_CH5].C = (float)( (tPmResult1[PM_CH5].nTmpNumConcet * tParam[PM_CH5].Cal));
}

// Number Concentration Calc
void Calc_GetNumberConcent(void)
{
	tPmResult1[PM_CH1].fNum_Concet = tPmResult1[PM_CH1].C;
	tPmResult1[PM_CH2].fNum_Concet = tPmResult1[PM_CH1].C + tPmResult1[PM_CH2].C;
	tPmResult1[PM_CH3].fNum_Concet = tPmResult1[PM_CH1].C + tPmResult1[PM_CH2].C + tPmResult1[PM_CH3].C;
	tPmResult1[PM_CH4].fNum_Concet = tPmResult1[PM_CH1].C + tPmResult1[PM_CH2].C + tPmResult1[PM_CH3].C + tPmResult1[PM_CH4].C;
	tPmResult1[PM_CH5].fNum_Concet = tPmResult1[PM_CH1].C + tPmResult1[PM_CH2].C + tPmResult1[PM_CH3].C + tPmResult1[PM_CH4].C + tPmResult1[PM_CH5].C;
}

void Calc_GetSelMassConnect(void) //PM Count * Calibration  * Density * (4/3*Pi*(Dpv/2)^3;
{
	tPmResult1[PM_CH1].M = (float)( tPmResult1[PM_CH1].C * tParam[PM_CH1].den ) * (float)( (4.0/3.0) * M_PI*( powf(DEFAULT_PARAM_PDCH1_DPV_VAL / 2.0, 3) ) );
	tPmResult1[PM_CH2].M = (float)( tPmResult1[PM_CH2].C * tParam[PM_CH1].den ) * (float)( (4.0/3.0) * M_PI*( powf(DEFAULT_PARAM_PDCH2_DPV_VAL / 2.0, 3) ) );
	tPmResult1[PM_CH3].M = (float)( tPmResult1[PM_CH3].C * tParam[PM_CH1].den ) * (float)( (4.0/3.0) * M_PI*( powf(DEFAULT_PARAM_PDCH3_DPV_VAL / 2.0, 3) ) );
	tPmResult1[PM_CH4].M = (float)( tPmResult1[PM_CH4].C * tParam[PM_CH1].den ) * (float)( (4.0/3.0) * M_PI*( powf(DEFAULT_PARAM_PDCH4_DPV_VAL / 2.0, 3) ) );
	tPmResult1[PM_CH5].M = (float)( tPmResult1[PM_CH5].C * tParam[PM_CH1].den ) * (float)( (4.0/3.0) * M_PI*( powf(DEFAULT_PARAM_PDCH5_DPV_VAL / 2.0, 3) ) );
}


// Mass Concentration
void Calc_GetMassConcentration(void)
{
	tPmResult1[PM0_5].fMass_Concet = (float)(tPmResult1[PM_CH1].M + tPmResult1[PM_CH2].M);
	tPmResult1[PM1_0].fMass_Concet = (float)(tPmResult1[PM_CH1].M + tPmResult1[PM_CH2].M + tPmResult1[PM_CH3].M);
	tPmResult1[PM2_5].fMass_Concet = (float)(tPmResult1[PM_CH1].M + tPmResult1[PM_CH2].M + tPmResult1[PM_CH3].M + tPmResult1[PM_CH4].M);
	tPmResult1[PM4_0].fMass_Concet = (float)(tPmResult1[PM_CH1].M + tPmResult1[PM_CH2].M + tPmResult1[PM_CH3].M + tPmResult1[PM_CH4].M + tPmResult1[PM_CH5].M);
}

// TPS
void Calc_GetTypicalPaticleSize(void)
{
	tPmResult1[PM_CH1].fTps = (float)( tPmResult1[PM_CH1].C * DEFAULT_PARAM_PDCH1_DP_VAL );
	tPmResult1[PM_CH2].fTps = (float)( tPmResult1[PM_CH2].C * DEFAULT_PARAM_PDCH2_DP_VAL );
	tPmResult1[PM_CH3].fTps = (float)( tPmResult1[PM_CH3].C * DEFAULT_PARAM_PDCH3_DP_VAL );
	tPmResult1[PM_CH4].fTps = (float)( tPmResult1[PM_CH4].C * DEFAULT_PARAM_PDCH4_DP_VAL );
	tPmResult1[PM_CH5].fTps = (float)( tPmResult1[PM_CH5].C * DEFAULT_PARAM_PDCH5_DP_VAL );
}

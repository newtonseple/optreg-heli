/*
 * E2d.c
 *
 * Code generation for model "E2d".
 *
 * Model version              : 1.164
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Tue Feb 21 12:34:48 2017
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "E2d.h"
#include "E2d_private.h"
#include "E2d_dt.h"

/* Block signals (auto storage) */
B_E2d_T E2d_B;

/* Continuous states */
X_E2d_T E2d_X;

/* Block states (auto storage) */
DW_E2d_T E2d_DW;

/* Real-time model */
RT_MODEL_E2d_T E2d_M_;
RT_MODEL_E2d_T *const E2d_M = &E2d_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  E2d_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void E2d_output(void)
{
  /* local block i/o variables */
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* set solver stop time */
    if (!(E2d_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&E2d_M->solverInfo, ((E2d_M->Timing.clockTickH0 + 1)
        * E2d_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&E2d_M->solverInfo, ((E2d_M->Timing.clockTick0 + 1) *
        E2d_M->Timing.stepSize0 + E2d_M->Timing.clockTickH0 *
        E2d_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(E2d_M)) {
    E2d_M->Timing.t[0] = rtsiGetT(&E2d_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(E2d_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

    /* S-Function Block: E2d/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(E2d_DW.HILReadEncoderTimebase_Task, 1,
        &E2d_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 = E2d_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 = E2d_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 = E2d_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S5>/Travel: Count to rad' */
    E2d_B.TravelCounttorad = E2d_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S12>/Gain' */
    E2d_B.Gain = E2d_P.Gain_Gain * E2d_B.TravelCounttorad;
  }

  /* TransferFcn: '<S5>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.TravelTransferFcn_C * E2d_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.TravelTransferFcn_D * E2d_B.TravelCounttorad;

  /* Gain: '<S13>/Gain' */
  E2d_B.Gain_d = E2d_P.Gain_Gain_l * rtb_Backgain;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* Gain: '<S5>/Pitch: Count to rad' */
    E2d_B.PitchCounttorad = E2d_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S9>/Gain' */
    E2d_B.Gain_i = E2d_P.Gain_Gain_a * E2d_B.PitchCounttorad;
  }

  /* TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.PitchTransferFcn_C * E2d_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.PitchTransferFcn_D * E2d_B.PitchCounttorad;

  /* Gain: '<S10>/Gain' */
  E2d_B.Gain_b = E2d_P.Gain_Gain_ae * rtb_Backgain;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* Gain: '<S5>/Elevation: Count to rad' */
    E2d_B.ElevationCounttorad = E2d_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S7>/Gain' */
    E2d_B.Gain_e = E2d_P.Gain_Gain_lv * E2d_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    E2d_B.Sum = E2d_B.Gain_e + E2d_P.elavation_offsetdeg_Value;
  }

  /* TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.ElevationTransferFcn_C *
    E2d_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.ElevationTransferFcn_D * E2d_B.ElevationCounttorad;

  /* Gain: '<S8>/Gain' */
  E2d_B.Gain_dg = E2d_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  E2d_B.Gain1[0] = E2d_P.Gain1_Gain * E2d_B.Gain;
  E2d_B.Gain1[1] = E2d_P.Gain1_Gain * E2d_B.Gain_d;
  E2d_B.Gain1[2] = E2d_P.Gain1_Gain * E2d_B.Gain_i;
  E2d_B.Gain1[3] = E2d_P.Gain1_Gain * E2d_B.Gain_b;
  E2d_B.Gain1[4] = E2d_P.Gain1_Gain * E2d_B.Sum;
  E2d_B.Gain1[5] = E2d_P.Gain1_Gain * E2d_B.Gain_dg;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* FromWorkspace: '<Root>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) E2d_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E2d_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = E2d_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = E2d_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      E2d_B.FromWorkspace = pDataValues[100];
    } else {
      /* Get index */
      if (t <= pTimeValues[0]) {
        currTimeIndex = 0;
      } else if (t >= pTimeValues[100]) {
        currTimeIndex = 99;
      } else {
        if (t < pTimeValues[currTimeIndex]) {
          while (t < pTimeValues[currTimeIndex]) {
            currTimeIndex--;
          }
        } else {
          while (t >= pTimeValues[currTimeIndex + 1]) {
            currTimeIndex++;
          }
        }
      }

      E2d_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            E2d_B.FromWorkspace = pDataValues[currTimeIndex];
          } else {
            E2d_B.FromWorkspace = pDataValues[currTimeIndex + 1];
          }
        } else {
          real_T f1 = (t2 - t) / (t2 - t1);
          real_T f2 = 1.0 - f1;
          real_T d1;
          real_T d2;
          int_T TimeIndex= currTimeIndex;
          d1 = pDataValues[TimeIndex];
          d2 = pDataValues[TimeIndex + 1];
          E2d_B.FromWorkspace = (real_T) rtInterpolate(d1, d2, f1, f2);
          pDataValues += 101;
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Sum: '<S6>/Sum' incorporates:
   *  Constant: '<S6>/Vd_bias'
   *  Gain: '<S6>/K_pd'
   *  Gain: '<S6>/K_pp'
   *  Sum: '<S6>/Sum2'
   *  Sum: '<S6>/Sum3'
   */
  E2d_B.Sum_k = ((E2d_B.FromWorkspace - E2d_B.Gain1[2]) * E2d_P.K_pp -
                 E2d_P.K_pd * E2d_B.Gain1[3]) + E2d_P.Vd_ff;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Integrator: '<S4>/Integrator'
   *
   * Regarding '<S4>/Integrator':
   *  Limited Integrator
   */
  if (E2d_X.Integrator_CSTATE >= E2d_P.Integrator_UpperSat ) {
    E2d_X.Integrator_CSTATE = E2d_P.Integrator_UpperSat;
  } else if (E2d_X.Integrator_CSTATE <= (E2d_P.Integrator_LowerSat) ) {
    E2d_X.Integrator_CSTATE = (E2d_P.Integrator_LowerSat);
  }

  rtb_Backgain = E2d_X.Integrator_CSTATE;

  /* Sum: '<S4>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   */
  rtb_Derivative = E2d_P.elevation_ref_Value - E2d_B.Gain1[4];

  /* Sum: '<S4>/Sum2' incorporates:
   *  Constant: '<S4>/Vs_bias'
   *  Gain: '<S4>/K_ed'
   *  Gain: '<S4>/K_ep'
   *  Sum: '<S4>/Sum1'
   */
  E2d_B.Sum2 = ((E2d_P.K_ep * rtb_Derivative + rtb_Backgain) - E2d_P.K_ed *
                E2d_B.Gain1[5]) + E2d_P.Vs_ff;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (E2d_B.Sum2 - E2d_B.Sum_k) * E2d_P.Backgain_Gain;

  /* Gain: '<S4>/K_ei' */
  E2d_B.K_ei = E2d_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Derivative: '<S5>/Derivative' */
  if ((E2d_DW.TimeStampA >= E2d_M->Timing.t[0]) && (E2d_DW.TimeStampB >=
       E2d_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = E2d_DW.TimeStampA;
    lastU = &E2d_DW.LastUAtTimeA;
    if (E2d_DW.TimeStampA < E2d_DW.TimeStampB) {
      if (E2d_DW.TimeStampB < E2d_M->Timing.t[0]) {
        rtb_Derivative = E2d_DW.TimeStampB;
        lastU = &E2d_DW.LastUAtTimeB;
      }
    } else {
      if (E2d_DW.TimeStampA >= E2d_M->Timing.t[0]) {
        rtb_Derivative = E2d_DW.TimeStampB;
        lastU = &E2d_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (E2d_B.PitchCounttorad - *lastU) / (E2d_M->Timing.t[0] -
      rtb_Derivative);
  }

  /* End of Derivative: '<S5>/Derivative' */

  /* Gain: '<S11>/Gain' */
  E2d_B.Gain_l = E2d_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Saturate: '<S5>/Back motor: Saturation' */
  if (rtb_Backgain > E2d_P.BackmotorSaturation_UpperSat) {
    E2d_B.BackmotorSaturation = E2d_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < E2d_P.BackmotorSaturation_LowerSat) {
    E2d_B.BackmotorSaturation = E2d_P.BackmotorSaturation_LowerSat;
  } else {
    E2d_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S5>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (E2d_B.Sum_k + E2d_B.Sum2) * E2d_P.Frontgain_Gain;

  /* Saturate: '<S5>/Front motor: Saturation' */
  if (rtb_Derivative > E2d_P.FrontmotorSaturation_UpperSat) {
    E2d_B.FrontmotorSaturation = E2d_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < E2d_P.FrontmotorSaturation_LowerSat) {
    E2d_B.FrontmotorSaturation = E2d_P.FrontmotorSaturation_LowerSat;
  } else {
    E2d_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S5>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* S-Function (hil_write_analog_block): '<S5>/HIL Write Analog' */

    /* S-Function Block: E2d/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      E2d_DW.HILWriteAnalog_Buffer[0] = E2d_B.FrontmotorSaturation;
      E2d_DW.HILWriteAnalog_Buffer[1] = E2d_B.BackmotorSaturation;
      result = hil_write_analog(E2d_DW.HILInitialize_Card,
        E2d_P.HILWriteAnalog_channels, 2, &E2d_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
      }
    }

    /* Logic: '<S3>/Logical Operator' incorporates:
     *  Constant: '<S3>/Start'
     */
    E2d_B.LogicalOperator = !(E2d_P.Start_Value != 0.0);

    /* DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
    if (E2d_B.LogicalOperator || (E2d_DW.DiscreteTimeIntegrator_PrevRese != 0))
    {
      E2d_DW.DiscreteTimeIntegrator_DSTATE = E2d_P.DiscreteTimeIntegrator_IC;
    }

    E2d_B.DiscreteTimeIntegrator = E2d_DW.DiscreteTimeIntegrator_DSTATE;

    /* End of DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
  }
}

/* Model update function */
void E2d_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S5>/Derivative' */
  if (E2d_DW.TimeStampA == (rtInf)) {
    E2d_DW.TimeStampA = E2d_M->Timing.t[0];
    lastU = &E2d_DW.LastUAtTimeA;
  } else if (E2d_DW.TimeStampB == (rtInf)) {
    E2d_DW.TimeStampB = E2d_M->Timing.t[0];
    lastU = &E2d_DW.LastUAtTimeB;
  } else if (E2d_DW.TimeStampA < E2d_DW.TimeStampB) {
    E2d_DW.TimeStampA = E2d_M->Timing.t[0];
    lastU = &E2d_DW.LastUAtTimeA;
  } else {
    E2d_DW.TimeStampB = E2d_M->Timing.t[0];
    lastU = &E2d_DW.LastUAtTimeB;
  }

  *lastU = E2d_B.PitchCounttorad;

  /* End of Update for Derivative: '<S5>/Derivative' */
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* Update for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' incorporates:
     *  Constant: '<S3>/Start'
     */
    if (!E2d_B.LogicalOperator) {
      E2d_DW.DiscreteTimeIntegrator_DSTATE +=
        E2d_P.DiscreteTimeIntegrator_gainval * E2d_P.Start_Value;
    }

    if (E2d_B.LogicalOperator) {
      E2d_DW.DiscreteTimeIntegrator_PrevRese = 1;
    } else {
      E2d_DW.DiscreteTimeIntegrator_PrevRese = 0;
    }

    /* End of Update for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
  }

  if (rtmIsMajorTimeStep(E2d_M)) {
    rt_ertODEUpdateContinuousStates(&E2d_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++E2d_M->Timing.clockTick0)) {
    ++E2d_M->Timing.clockTickH0;
  }

  E2d_M->Timing.t[0] = rtsiGetSolverStopTime(&E2d_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++E2d_M->Timing.clockTick1)) {
      ++E2d_M->Timing.clockTickH1;
    }

    E2d_M->Timing.t[1] = E2d_M->Timing.clockTick1 * E2d_M->Timing.stepSize1 +
      E2d_M->Timing.clockTickH1 * E2d_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void E2d_derivatives(void)
{
  XDot_E2d_T *_rtXdot;
  _rtXdot = ((XDot_E2d_T *) E2d_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += E2d_P.TravelTransferFcn_A *
    E2d_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += E2d_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += E2d_P.PitchTransferFcn_A *
    E2d_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += E2d_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += E2d_P.ElevationTransferFcn_A *
    E2d_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += E2d_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S4>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( E2d_X.Integrator_CSTATE <= (E2d_P.Integrator_LowerSat) );
    usat = ( E2d_X.Integrator_CSTATE >= E2d_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (E2d_B.K_ei > 0)) ||
        (usat && (E2d_B.K_ei < 0)) ) {
      ((XDot_E2d_T *) E2d_M->ModelData.derivs)->Integrator_CSTATE = E2d_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_E2d_T *) E2d_M->ModelData.derivs)->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void E2d_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E2d/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &E2d_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E2d_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(E2d_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E2d_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(E2d_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E2d_M, _rt_error_message);
      return;
    }

    if ((E2d_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (E2d_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &E2d_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = E2d_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &E2d_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = E2d_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_analog_input_chan, 8U,
        &E2d_DW.HILInitialize_AIMinimums[0], &E2d_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_analog_output && !is_switching) ||
        (E2d_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &E2d_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = E2d_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &E2d_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = E2d_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_analog_output_cha, 8U,
        &E2d_DW.HILInitialize_AOMinimums[0], &E2d_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (E2d_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E2d_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E2d_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_analog_output_cha, 8U,
        &E2d_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if (E2d_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E2d_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E2d_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (E2d_DW.HILInitialize_Card, E2d_P.HILInitialize_analog_output_cha, 8U,
         &E2d_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_encoder_param && !is_switching) ||
        (E2d_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes = &E2d_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = E2d_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_encoder_channels, 8U, (t_encoder_quadrature_mode *)
        &E2d_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_encoder_count && !is_switching) ||
        (E2d_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts = &E2d_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = E2d_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_encoder_channels, 8U,
        &E2d_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (E2d_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &E2d_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E2d_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &E2d_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          E2d_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &E2d_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            E2d_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            E2d_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              E2d_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            E2d_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              p_HILInitialize_pwm_channels[i1];
            E2d_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              E2d_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(E2d_DW.HILInitialize_Card,
          &E2d_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &E2d_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E2d_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(E2d_DW.HILInitialize_Card,
          &E2d_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &E2d_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E2d_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &E2d_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E2d_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &E2d_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = E2d_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals = &E2d_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = E2d_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &E2d_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &E2d_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &E2d_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &E2d_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = E2d_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &E2d_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E2d_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_pwm_channels, 8U,
        &E2d_DW.HILInitialize_POSortedFreqs[0], &E2d_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if ((E2d_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (E2d_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E2d_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E2d_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_pwm_channels, 8U, &E2d_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }

    if (E2d_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &E2d_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E2d_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state(E2d_DW.HILInitialize_Card,
        E2d_P.HILInitialize_pwm_channels, 8U, &E2d_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E2d_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S5>/HIL Read Encoder Timebase' */

  /* S-Function Block: E2d/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(E2d_DW.HILInitialize_Card,
      E2d_P.HILReadEncoderTimebase_samples_,
      E2d_P.HILReadEncoderTimebase_channels, 3,
      &E2d_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E2d_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/From Workspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75,
      2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75, 5.0,
      5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0, 8.25,
      8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0, 11.25,
      11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75, 14.0,
      14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5, 16.75,
      17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25, 19.5,
      19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0, 22.25,
      22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75, 25.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877571639024,
      0.52359877475735661, 0.52359877437507374, 0.5235987734393639,
      0.52359877338514127, 0.5235987732763776, 0.523598772766669,
      0.52359877160644508, 0.52359877005689826, 0.52359876838743658,
      0.52359876522080528, 0.52359875923808408, 0.52359874213477853,
      0.045875298816182117, -0.52359874022185349, -0.5235987468798371,
      -0.52359875092931463, -0.52359875287440816, -0.52359875370003062,
      -0.52359875389829191, -0.52359875339798734, -0.52359875077128248,
      -0.52359874427811293, -0.52359874014877172, -0.52359874232347081,
      -0.52359870737198444, -0.52359865410958728, -0.52359863899290748,
      -0.523598416433402, -0.52359858409304627, -0.52359757806604235,
      0.33693131075453636, 0.52358881817158165, 0.52358945916561517,
      0.52358615204327119, 0.52357906509081664, 0.5235884152207102,
      -0.14045062193491328, -0.52293495801486134, -0.52170554925248169,
      -0.30908910535876449, 0.517797872876735, 0.43232607136813117,
      -0.40990156039747183, -0.013103000494241054, 0.079159452348270376,
      -0.0159118387766419, -0.00504911755491423, 0.0031328175613986996,
      -0.00048866794564060058, -0.0003313547391516152, 0.00024205760358268812,
      1.5639234460155952E-5, -6.923764977602708E-5, -1.1418317548424857E-6,
      1.9311378628101438E-5, 5.6722381122437039E-6, -7.0139869201468892E-7,
      -3.0950904367877244E-7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E2d_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    E2d_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    E2d_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* InitializeConditions for TransferFcn: '<S5>/Travel: Transfer Fcn' */
  E2d_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Pitch: Transfer Fcn' */
  E2d_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S5>/Elevation: Transfer Fcn' */
  E2d_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  E2d_X.Integrator_CSTATE = E2d_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S5>/Derivative' */
  E2d_DW.TimeStampA = (rtInf);
  E2d_DW.TimeStampB = (rtInf);

  /* InitializeConditions for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
  E2d_DW.DiscreteTimeIntegrator_DSTATE = E2d_P.DiscreteTimeIntegrator_IC;
  E2d_DW.DiscreteTimeIntegrator_PrevRese = 0;
}

/* Model terminate function */
void E2d_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E2d/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(E2d_DW.HILInitialize_Card);
    hil_monitor_stop_all(E2d_DW.HILInitialize_Card);
    is_switching = false;
    if ((E2d_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (E2d_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E2d_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E2d_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((E2d_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (E2d_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E2d_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E2d_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(E2d_DW.HILInitialize_Card
                         , E2d_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , E2d_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &E2d_DW.HILInitialize_AOVoltages[0]
                         , &E2d_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(E2d_DW.HILInitialize_Card,
            E2d_P.HILInitialize_analog_output_cha, num_final_analog_outputs,
            &E2d_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(E2d_DW.HILInitialize_Card,
            E2d_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &E2d_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E2d_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(E2d_DW.HILInitialize_Card);
    hil_monitor_delete_all(E2d_DW.HILInitialize_Card);
    hil_close(E2d_DW.HILInitialize_Card);
    E2d_DW.HILInitialize_Card = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  E2d_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  E2d_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  E2d_initialize();
}

void MdlTerminate(void)
{
  E2d_terminate();
}

/* Registration function */
RT_MODEL_E2d_T *E2d(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  E2d_P.Integrator_UpperSat = rtInf;
  E2d_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)E2d_M, 0,
                sizeof(RT_MODEL_E2d_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&E2d_M->solverInfo, &E2d_M->Timing.simTimeStep);
    rtsiSetTPtr(&E2d_M->solverInfo, &rtmGetTPtr(E2d_M));
    rtsiSetStepSizePtr(&E2d_M->solverInfo, &E2d_M->Timing.stepSize0);
    rtsiSetdXPtr(&E2d_M->solverInfo, &E2d_M->ModelData.derivs);
    rtsiSetContStatesPtr(&E2d_M->solverInfo, (real_T **)
                         &E2d_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&E2d_M->solverInfo, &E2d_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&E2d_M->solverInfo, (&rtmGetErrorStatus(E2d_M)));
    rtsiSetRTModelPtr(&E2d_M->solverInfo, E2d_M);
  }

  rtsiSetSimTimeStep(&E2d_M->solverInfo, MAJOR_TIME_STEP);
  E2d_M->ModelData.intgData.f[0] = E2d_M->ModelData.odeF[0];
  E2d_M->ModelData.contStates = ((real_T *) &E2d_X);
  rtsiSetSolverData(&E2d_M->solverInfo, (void *)&E2d_M->ModelData.intgData);
  rtsiSetSolverName(&E2d_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = E2d_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    E2d_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    E2d_M->Timing.sampleTimes = (&E2d_M->Timing.sampleTimesArray[0]);
    E2d_M->Timing.offsetTimes = (&E2d_M->Timing.offsetTimesArray[0]);

    /* task periods */
    E2d_M->Timing.sampleTimes[0] = (0.0);
    E2d_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    E2d_M->Timing.offsetTimes[0] = (0.0);
    E2d_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(E2d_M, &E2d_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = E2d_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    E2d_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(E2d_M, -1);
  E2d_M->Timing.stepSize0 = 0.002;
  E2d_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  E2d_M->Sizes.checksums[0] = (2823223217U);
  E2d_M->Sizes.checksums[1] = (170623278U);
  E2d_M->Sizes.checksums[2] = (223299359U);
  E2d_M->Sizes.checksums[3] = (617870590U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    E2d_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(E2d_M->extModeInfo,
      &E2d_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(E2d_M->extModeInfo, E2d_M->Sizes.checksums);
    rteiSetTPtr(E2d_M->extModeInfo, rtmGetTPtr(E2d_M));
  }

  E2d_M->solverInfoPtr = (&E2d_M->solverInfo);
  E2d_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&E2d_M->solverInfo, 0.002);
  rtsiSetSolverMode(&E2d_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  E2d_M->ModelData.blockIO = ((void *) &E2d_B);
  (void) memset(((void *) &E2d_B), 0,
                sizeof(B_E2d_T));

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      E2d_B.Gain1[i] = 0.0;
    }

    E2d_B.TravelCounttorad = 0.0;
    E2d_B.Gain = 0.0;
    E2d_B.Gain_d = 0.0;
    E2d_B.PitchCounttorad = 0.0;
    E2d_B.Gain_i = 0.0;
    E2d_B.Gain_b = 0.0;
    E2d_B.ElevationCounttorad = 0.0;
    E2d_B.Gain_e = 0.0;
    E2d_B.Sum = 0.0;
    E2d_B.Gain_dg = 0.0;
    E2d_B.FromWorkspace = 0.0;
    E2d_B.Sum_k = 0.0;
    E2d_B.Sum2 = 0.0;
    E2d_B.K_ei = 0.0;
    E2d_B.Gain_l = 0.0;
    E2d_B.BackmotorSaturation = 0.0;
    E2d_B.FrontmotorSaturation = 0.0;
    E2d_B.DiscreteTimeIntegrator = 0.0;
  }

  /* parameters */
  E2d_M->ModelData.defaultParam = ((real_T *)&E2d_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &E2d_X;
    E2d_M->ModelData.contStates = (x);
    (void) memset((void *)&E2d_X, 0,
                  sizeof(X_E2d_T));
  }

  /* states (dwork) */
  E2d_M->ModelData.dwork = ((void *) &E2d_DW);
  (void) memset((void *)&E2d_DW, 0,
                sizeof(DW_E2d_T));
  E2d_DW.DiscreteTimeIntegrator_DSTATE = 0.0;

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E2d_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  E2d_DW.TimeStampA = 0.0;
  E2d_DW.LastUAtTimeA = 0.0;
  E2d_DW.TimeStampB = 0.0;
  E2d_DW.LastUAtTimeB = 0.0;
  E2d_DW.HILWriteAnalog_Buffer[0] = 0.0;
  E2d_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    E2d_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  E2d_M->Sizes.numContStates = (4);    /* Number of continuous states */
  E2d_M->Sizes.numY = (0);             /* Number of model outputs */
  E2d_M->Sizes.numU = (0);             /* Number of model inputs */
  E2d_M->Sizes.sysDirFeedThru = (0);   /* The model is not direct feedthrough */
  E2d_M->Sizes.numSampTimes = (2);     /* Number of sample times */
  E2d_M->Sizes.numBlocks = (63);       /* Number of blocks */
  E2d_M->Sizes.numBlockIO = (20);      /* Number of block outputs */
  E2d_M->Sizes.numBlockPrms = (144);   /* Sum of parameter "widths" */
  return E2d_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/

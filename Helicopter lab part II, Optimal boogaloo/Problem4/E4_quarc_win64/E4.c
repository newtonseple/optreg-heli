/*
 * E4.c
 *
 * Code generation for model "E4".
 *
 * Model version              : 1.176
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Tue Mar 21 10:20:15 2017
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "E4.h"
#include "E4_private.h"
#include "E4_dt.h"

/* Block signals (auto storage) */
B_E4_T E4_B;

/* Continuous states */
X_E4_T E4_X;

/* Block states (auto storage) */
DW_E4_T E4_DW;

/* Real-time model */
RT_MODEL_E4_T E4_M_;
RT_MODEL_E4_T *const E4_M = &E4_M_;

/*
 * Writes out MAT-file header.  Returns success or failure.
 * Returns:
 *      0 - success
 *      1 - failure
 */
int_T rt_WriteMat4FileHeader(FILE *fp, int32_T m, int32_T n, const char *name)
{
  typedef enum { ELITTLE_ENDIAN, EBIG_ENDIAN } ByteOrder;

  int16_T one = 1;
  ByteOrder byteOrder = (*((int8_T *)&one)==1) ? ELITTLE_ENDIAN : EBIG_ENDIAN;
  int32_T type = (byteOrder == ELITTLE_ENDIAN) ? 0: 1000;
  int32_T imagf = 0;
  int32_T name_len = (int32_T)strlen(name) + 1;
  if ((fwrite(&type, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&m, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&n, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&imagf, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&name_len, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(name, sizeof(char), name_len, fp) == 0)) {
    return(1);
  } else {
    return(0);
  }
}                                      /* end rt_WriteMat4FileHeader */

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
  E4_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void E4_output(void)
{
  /* local block i/o variables */
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  int32_T i;
  real_T tmp[6];
  int32_T i_0;
  if (rtmIsMajorTimeStep(E4_M)) {
    /* set solver stop time */
    if (!(E4_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&E4_M->solverInfo, ((E4_M->Timing.clockTickH0 + 1) *
        E4_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&E4_M->solverInfo, ((E4_M->Timing.clockTick0 + 1) *
        E4_M->Timing.stepSize0 + E4_M->Timing.clockTickH0 *
        E4_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(E4_M)) {
    E4_M->Timing.t[0] = rtsiGetT(&E4_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(E4_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: E4/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(E4_DW.HILReadEncoderTimebase_Task, 1,
        &E4_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 = E4_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 = E4_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 = E4_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }
  }

  /* FromWorkspace: '<Root>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *) E4_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E4_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = E4_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = E4_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      {
        int_T elIdx;
        for (elIdx = 0; elIdx < 6; ++elIdx) {
          (&E4_B.FromWorkspace1[0])[elIdx] = pDataValues[100];
          pDataValues += 101;
        }
      }
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

      E4_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 6; ++elIdx) {
                (&E4_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex];
                pDataValues += 101;
              }
            }
          } else {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 6; ++elIdx) {
                (&E4_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex + 1];
                pDataValues += 101;
              }
            }
          }
        } else {
          real_T f1 = (t2 - t) / (t2 - t1);
          real_T f2 = 1.0 - f1;
          real_T d1;
          real_T d2;
          int_T TimeIndex= currTimeIndex;

          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 6; ++elIdx) {
              d1 = pDataValues[TimeIndex];
              d2 = pDataValues[TimeIndex + 1];
              (&E4_B.FromWorkspace1[0])[elIdx] = (real_T) rtInterpolate(d1, d2,
                f1, f2);
              pDataValues += 101;
            }
          }
        }
      }
    }
  }

  /* FromWorkspace: '<Root>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) E4_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E4_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = E4_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = E4_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      {
        int_T elIdx;
        for (elIdx = 0; elIdx < 2; ++elIdx) {
          (&E4_B.FromWorkspace[0])[elIdx] = pDataValues[100];
          pDataValues += 101;
        }
      }
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

      E4_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 2; ++elIdx) {
                (&E4_B.FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex];
                pDataValues += 101;
              }
            }
          } else {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 2; ++elIdx) {
                (&E4_B.FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex + 1];
                pDataValues += 101;
              }
            }
          }
        } else {
          real_T f1 = (t2 - t) / (t2 - t1);
          real_T f2 = 1.0 - f1;
          real_T d1;
          real_T d2;
          int_T TimeIndex= currTimeIndex;

          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 2; ++elIdx) {
              d1 = pDataValues[TimeIndex];
              d2 = pDataValues[TimeIndex + 1];
              (&E4_B.FromWorkspace[0])[elIdx] = (real_T) rtInterpolate(d1, d2,
                f1, f2);
              pDataValues += 101;
            }
          }
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(E4_M)) {
    /* Gain: '<S4>/Travel: Count to rad' */
    E4_B.TravelCounttorad = E4_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S11>/Gain' */
    E4_B.Gain = E4_P.Gain_Gain * E4_B.TravelCounttorad;

    /* Sum: '<Root>/Sum4' incorporates:
     *  Constant: '<Root>/travel_offset [deg]'
     */
    E4_B.Sum4 = E4_P.travel_offsetdeg_Value + E4_B.Gain;
  }

  /* TransferFcn: '<S4>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E4_P.TravelTransferFcn_C * E4_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += E4_P.TravelTransferFcn_D * E4_B.TravelCounttorad;

  /* Gain: '<S12>/Gain' */
  E4_B.Gain_d = E4_P.Gain_Gain_l * rtb_Backgain;
  if (rtmIsMajorTimeStep(E4_M)) {
    /* Gain: '<S4>/Pitch: Count to rad' */
    E4_B.PitchCounttorad = E4_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S8>/Gain' */
    E4_B.Gain_i = E4_P.Gain_Gain_a * E4_B.PitchCounttorad;
  }

  /* TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E4_P.PitchTransferFcn_C * E4_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += E4_P.PitchTransferFcn_D * E4_B.PitchCounttorad;

  /* Gain: '<S9>/Gain' */
  E4_B.Gain_b = E4_P.Gain_Gain_ae * rtb_Backgain;
  if (rtmIsMajorTimeStep(E4_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' */
    E4_B.ElevationCounttorad = E4_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S6>/Gain' */
    E4_B.Gain_e = E4_P.Gain_Gain_lv * E4_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    E4_B.Sum = E4_B.Gain_e + E4_P.elavation_offsetdeg_Value;
  }

  /* TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E4_P.ElevationTransferFcn_C * E4_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += E4_P.ElevationTransferFcn_D * E4_B.ElevationCounttorad;

  /* Gain: '<S7>/Gain' */
  E4_B.Gain_dg = E4_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  E4_B.Gain1[0] = E4_P.Gain1_Gain * E4_B.Sum4;
  E4_B.Gain1[1] = E4_P.Gain1_Gain * E4_B.Gain_d;
  E4_B.Gain1[2] = E4_P.Gain1_Gain * E4_B.Gain_i;
  E4_B.Gain1[3] = E4_P.Gain1_Gain * E4_B.Gain_b;
  E4_B.Gain1[4] = E4_P.Gain1_Gain * E4_B.Sum;
  E4_B.Gain1[5] = E4_P.Gain1_Gain * E4_B.Gain_dg;

  /* Sum: '<Root>/Sum2' incorporates:
   *  Gain: '<Root>/LQR'
   */
  for (i = 0; i < 6; i++) {
    tmp[i] = E4_B.Gain1[i] - E4_B.FromWorkspace1[i];
  }

  /* End of Sum: '<Root>/Sum2' */

  /* Sum: '<Root>/Sum1' incorporates:
   *  Gain: '<Root>/LQR'
   */
  for (i = 0; i < 2; i++) {
    rtb_Derivative = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_Derivative += E4_P.K[(i_0 << 1) + i] * tmp[i_0];
    }

    E4_B.Sum1[i] = E4_B.FromWorkspace[i] - rtb_Derivative;
  }

  /* End of Sum: '<Root>/Sum1' */
  if (rtmIsMajorTimeStep(E4_M)) {
    /* SignalConversion: '<Root>/TmpSignal ConversionAtTo FileInport1' */
    for (i = 0; i < 6; i++) {
      E4_B.TmpSignalConversionAtToFileInpo[i] = E4_B.FromWorkspace1[i];
    }

    E4_B.TmpSignalConversionAtToFileInpo[6] = E4_B.FromWorkspace[0];
    E4_B.TmpSignalConversionAtToFileInpo[7] = E4_B.FromWorkspace[1];
    for (i = 0; i < 6; i++) {
      E4_B.TmpSignalConversionAtToFileInpo[i + 8] = E4_B.Gain1[i];
    }

    E4_B.TmpSignalConversionAtToFileInpo[14] = E4_B.Sum1[0];
    E4_B.TmpSignalConversionAtToFileInpo[15] = E4_B.Sum1[1];

    /* End of SignalConversion: '<Root>/TmpSignal ConversionAtTo FileInport1' */

    /* ToFile: '<Root>/To File' */
    {
      if (!(++E4_DW.ToFile_IWORK.Decimation % 1) && (E4_DW.ToFile_IWORK.Count*17)
          +1 < 100000000 ) {
        FILE *fp = (FILE *) E4_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[17];
          E4_DW.ToFile_IWORK.Decimation = 0;
          u[0] = E4_M->Timing.t[1];
          u[1] = E4_B.TmpSignalConversionAtToFileInpo[0];
          u[2] = E4_B.TmpSignalConversionAtToFileInpo[1];
          u[3] = E4_B.TmpSignalConversionAtToFileInpo[2];
          u[4] = E4_B.TmpSignalConversionAtToFileInpo[3];
          u[5] = E4_B.TmpSignalConversionAtToFileInpo[4];
          u[6] = E4_B.TmpSignalConversionAtToFileInpo[5];
          u[7] = E4_B.TmpSignalConversionAtToFileInpo[6];
          u[8] = E4_B.TmpSignalConversionAtToFileInpo[7];
          u[9] = E4_B.TmpSignalConversionAtToFileInpo[8];
          u[10] = E4_B.TmpSignalConversionAtToFileInpo[9];
          u[11] = E4_B.TmpSignalConversionAtToFileInpo[10];
          u[12] = E4_B.TmpSignalConversionAtToFileInpo[11];
          u[13] = E4_B.TmpSignalConversionAtToFileInpo[12];
          u[14] = E4_B.TmpSignalConversionAtToFileInpo[13];
          u[15] = E4_B.TmpSignalConversionAtToFileInpo[14];
          u[16] = E4_B.TmpSignalConversionAtToFileInpo[15];
          if (fwrite(u, sizeof(real_T), 17, fp) != 17) {
            rtmSetErrorStatus(E4_M, "Error writing to MAT-file o4_out.mat");
            return;
          }

          if (((++E4_DW.ToFile_IWORK.Count)*17)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file o4_out.mat.\n");
          }
        }
      }
    }
  }

  /* Sum: '<S5>/Sum' incorporates:
   *  Constant: '<S5>/Vd_bias'
   *  Gain: '<S5>/K_pd'
   *  Gain: '<S5>/K_pp'
   *  Sum: '<S5>/Sum2'
   *  Sum: '<S5>/Sum3'
   */
  E4_B.Sum_k = ((E4_B.Sum1[0] - E4_B.Gain1[2]) * E4_P.K_pp - E4_P.K_pd *
                E4_B.Gain1[3]) + E4_P.Vd_ff;
  if (rtmIsMajorTimeStep(E4_M)) {
  }

  /* Integrator: '<S3>/Integrator'
   *
   * Regarding '<S3>/Integrator':
   *  Limited Integrator
   */
  if (E4_X.Integrator_CSTATE >= E4_P.Integrator_UpperSat ) {
    E4_X.Integrator_CSTATE = E4_P.Integrator_UpperSat;
  } else if (E4_X.Integrator_CSTATE <= (E4_P.Integrator_LowerSat) ) {
    E4_X.Integrator_CSTATE = (E4_P.Integrator_LowerSat);
  }

  rtb_Backgain = E4_X.Integrator_CSTATE;

  /* Sum: '<S3>/Sum' */
  rtb_Derivative = E4_B.Sum1[1] - E4_B.Gain1[4];

  /* Sum: '<S3>/Sum2' incorporates:
   *  Constant: '<S3>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  E4_B.Sum2 = ((E4_P.K_ep * rtb_Derivative + rtb_Backgain) - E4_P.K_ed *
               E4_B.Gain1[5]) + E4_P.Vs_ff;
  if (rtmIsMajorTimeStep(E4_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (E4_B.Sum2 - E4_B.Sum_k) * E4_P.Backgain_Gain;

  /* Gain: '<S3>/K_ei' */
  E4_B.K_ei = E4_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(E4_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((E4_DW.TimeStampA >= E4_M->Timing.t[0]) && (E4_DW.TimeStampB >=
       E4_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = E4_DW.TimeStampA;
    lastU = &E4_DW.LastUAtTimeA;
    if (E4_DW.TimeStampA < E4_DW.TimeStampB) {
      if (E4_DW.TimeStampB < E4_M->Timing.t[0]) {
        rtb_Derivative = E4_DW.TimeStampB;
        lastU = &E4_DW.LastUAtTimeB;
      }
    } else {
      if (E4_DW.TimeStampA >= E4_M->Timing.t[0]) {
        rtb_Derivative = E4_DW.TimeStampB;
        lastU = &E4_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (E4_B.PitchCounttorad - *lastU) / (E4_M->Timing.t[0] -
      rtb_Derivative);
  }

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S10>/Gain' */
  E4_B.Gain_l = E4_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(E4_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > E4_P.BackmotorSaturation_UpperSat) {
    E4_B.BackmotorSaturation = E4_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < E4_P.BackmotorSaturation_LowerSat) {
    E4_B.BackmotorSaturation = E4_P.BackmotorSaturation_LowerSat;
  } else {
    E4_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(E4_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (E4_B.Sum_k + E4_B.Sum2) * E4_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Derivative > E4_P.FrontmotorSaturation_UpperSat) {
    E4_B.FrontmotorSaturation = E4_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < E4_P.FrontmotorSaturation_LowerSat) {
    E4_B.FrontmotorSaturation = E4_P.FrontmotorSaturation_LowerSat;
  } else {
    E4_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(E4_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: E4/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      E4_DW.HILWriteAnalog_Buffer[0] = E4_B.FrontmotorSaturation;
      E4_DW.HILWriteAnalog_Buffer[1] = E4_B.BackmotorSaturation;
      result = hil_write_analog(E4_DW.HILInitialize_Card,
        E4_P.HILWriteAnalog_channels, 2, &E4_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void E4_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (E4_DW.TimeStampA == (rtInf)) {
    E4_DW.TimeStampA = E4_M->Timing.t[0];
    lastU = &E4_DW.LastUAtTimeA;
  } else if (E4_DW.TimeStampB == (rtInf)) {
    E4_DW.TimeStampB = E4_M->Timing.t[0];
    lastU = &E4_DW.LastUAtTimeB;
  } else if (E4_DW.TimeStampA < E4_DW.TimeStampB) {
    E4_DW.TimeStampA = E4_M->Timing.t[0];
    lastU = &E4_DW.LastUAtTimeA;
  } else {
    E4_DW.TimeStampB = E4_M->Timing.t[0];
    lastU = &E4_DW.LastUAtTimeB;
  }

  *lastU = E4_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(E4_M)) {
    rt_ertODEUpdateContinuousStates(&E4_M->solverInfo);
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
  if (!(++E4_M->Timing.clockTick0)) {
    ++E4_M->Timing.clockTickH0;
  }

  E4_M->Timing.t[0] = rtsiGetSolverStopTime(&E4_M->solverInfo);

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
    if (!(++E4_M->Timing.clockTick1)) {
      ++E4_M->Timing.clockTickH1;
    }

    E4_M->Timing.t[1] = E4_M->Timing.clockTick1 * E4_M->Timing.stepSize1 +
      E4_M->Timing.clockTickH1 * E4_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void E4_derivatives(void)
{
  XDot_E4_T *_rtXdot;
  _rtXdot = ((XDot_E4_T *) E4_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += E4_P.TravelTransferFcn_A *
    E4_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += E4_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += E4_P.PitchTransferFcn_A *
    E4_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += E4_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += E4_P.ElevationTransferFcn_A *
    E4_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += E4_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( E4_X.Integrator_CSTATE <= (E4_P.Integrator_LowerSat) );
    usat = ( E4_X.Integrator_CSTATE >= E4_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (E4_B.K_ei > 0)) ||
        (usat && (E4_B.K_ei < 0)) ) {
      ((XDot_E4_T *) E4_M->ModelData.derivs)->Integrator_CSTATE = E4_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_E4_T *) E4_M->ModelData.derivs)->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void E4_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E4/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &E4_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E4_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(E4_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E4_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(E4_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E4_M, _rt_error_message);
      return;
    }

    if ((E4_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (E4_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &E4_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = E4_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &E4_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = E4_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_analog_input_chan, 8U,
        &E4_DW.HILInitialize_AIMinimums[0], &E4_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_analog_output && !is_switching) ||
        (E4_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &E4_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = E4_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &E4_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = E4_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_analog_output_cha, 8U,
        &E4_DW.HILInitialize_AOMinimums[0], &E4_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (E4_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E4_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_analog_output_cha, 8U,
        &E4_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if (E4_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E4_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_analog_output_cha, 8U,
        &E4_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_encoder_param && !is_switching) ||
        (E4_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes = &E4_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = E4_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_encoder_channels, 8U, (t_encoder_quadrature_mode *)
        &E4_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_encoder_count && !is_switching) ||
        (E4_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts = &E4_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = E4_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_encoder_channels, 8U,
        &E4_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (E4_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &E4_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E4_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &E4_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          E4_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &E4_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            E4_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            E4_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              E4_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            E4_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              p_HILInitialize_pwm_channels[i1];
            E4_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              E4_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(E4_DW.HILInitialize_Card,
          &E4_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &E4_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E4_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(E4_DW.HILInitialize_Card,
          &E4_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &E4_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E4_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &E4_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E4_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &E4_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = E4_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals = &E4_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = E4_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &E4_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &E4_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &E4_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &E4_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = E4_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &E4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E4_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_pwm_channels, 8U,
        &E4_DW.HILInitialize_POSortedFreqs[0], &E4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if ((E4_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (E4_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E4_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_pwm_channels, 8U, &E4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }

    if (E4_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &E4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E4_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state(E4_DW.HILInitialize_Card,
        E4_P.HILInitialize_pwm_channels, 8U, &E4_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E4_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: E4/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(E4_DW.HILInitialize_Card,
      E4_P.HILReadEncoderTimebase_samples_, E4_P.HILReadEncoderTimebase_channels,
      3,
      &E4_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E4_M, _rt_error_message);
    }
  }

  /* Start for FromWorkspace: '<Root>/From Workspace1' */
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

    static real_T pDataValues0[] = { 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1378421413625261, 3.1262155534579983,
      3.1033093000299643, 3.0666274151911783, 3.0144539223941584,
      2.9456562771175667, 2.8595077632935446, 2.7555515879651526,
      2.633505110490284, 2.4931956060320961, 2.334518576064299,
      2.1584884848384447, 1.9680877648863644, 1.7678780801903806,
      1.5631854413834849, 1.3593701329795407, 1.1613174821398544,
      0.9730888744012266, 0.79767402238784213, 0.63713304086142208,
      0.492827667991044, 0.36556386826227016, 0.25555358547213614,
      0.16248171062403072, 0.085609900762324539, 0.023885615067388028,
      -0.023957680565069809, -0.059314466522070286, -0.083636635067064882,
      -0.098377837221242284, -0.10591794811265257, -0.1084351009482412,
      -0.10760533002941308, -0.10461338779261257, -0.10025550154175353,
      -0.095049133699116717, -0.0893226341697771, -0.083280655466577119,
      -0.07704898950735839, -0.070704100016611759, -0.064292045036130133,
      -0.0578403581352272, -0.051365393682505651, -0.044876812578711081,
      -0.038380294618161316, -0.031879165152130427, -0.025375363929238746,
      -0.018870018754208236, -0.012363783438275402, -0.0058570360128720076,
      0.00065000546532605, 0.0071572154884833924, 0.013664521960125563,
      0.02017188354025086, 0.026679276564040937, 0.033186687505546125,
      0.039694108644829341, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, -0.015002048909068423, -0.046506351618112111,
      -0.091625013712135384, -0.14672753935514371, -0.20869397118807925,
      -0.27519058110636674, -0.34459405529608839, -0.41582470131356869,
      -0.48818590989947458, -0.56123801783275185, -0.63470811987118858,
      -0.70412036490341712, -0.7616028798083212, -0.8008387387839353,
      -0.81877055522758235, -0.81526123361577763, -0.79221060335874482,
      -0.75291443095451116, -0.7016594080535381, -0.64216392610568007,
      -0.57722149148151214, -0.50905519891509554, -0.44004113116053606,
      -0.37228749939242173, -0.30748723944682471, -0.246897142779746,
      -0.19137318252983135, -0.14142714382800192, -0.0972886741799784,
      -0.058964808616709628, -0.030160443565641095, -0.010068611342354518,
      0.0033190836753124839, 0.011967768947201998, 0.017431545003436195,
      0.020825471370547233, 0.022905998117358533, 0.024167914812799871,
      0.024926663836874925, 0.025379557962986483, 0.02564821992192649,
      0.025806747603611752, 0.025899857810886186, 0.025954324415178278,
      0.025986071842199056, 0.026004517864123555, 0.02601520489156672,
      0.02602138070012204, 0.026024941263731338, 0.026026989701613577,
      0.02602816591279223, 0.02602884009262937, 0.026029225886568683,
      0.026029446320501189, 0.026029572095160308, 0.026029643766020752,
      0.026029684557132858, 0.026029707746920892, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.10602875205865551, 0.22266037932317656,
      0.31888147181640641, 0.38944360631144165, 0.43795507377677839,
      0.46997264230390062, 0.4905172487754707, 0.50343100141474328,
      0.5114213858602934, 0.51630439857701838, 0.51925862127063693,
      0.49057923774052126, 0.40626446144138845, 0.27730406615452668,
      0.12673523004684303, -0.024802544860869261, -0.16291305108606302,
      -0.27773033843291184, -0.36225092638132445, -0.42049134370215657,
      -0.45898832490067715, -0.48177332157032404, -0.48776507281509918,
      -0.47885679265265968, -0.45798348857234772, -0.42822766247884336,
      -0.39242214515739882, -0.35299952599322182, -0.3119538459663232,
      -0.27085844502751966, -0.20357825112545883, -0.14200139661718,
      -0.0946191153134467, -0.061125604368094423, -0.038615882422586234,
      -0.023986975343496295, -0.014704368445013587, -0.0089187452481073182,
      -0.005362548318301439, -0.0032008827125791189, -0.0018988001175508458,
      -0.0011204131087507467, -0.00065806738406669048, -0.00038494915707625867,
      -0.00022437868910371063, -0.00013036943799851343, -7.5531828344492315E-5,
      -4.3648256184391232E-5, -2.5164703728621429E-5, -1.4477576605705223E-5,
      -8.313011388367764E-6, -4.76484560397871E-6, -2.7266442193953322E-6,
      -1.5579428461313057E-6, -8.8892725437734815E-7, -5.0654226886036308E-7,
      -2.882960012972514E-7, -1.6389656509671672E-7, -9.3076681213969867E-8, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823462206,
      0.46652650905808424, 0.38488436997291947, 0.28224853798014093,
      0.19404586986134692, 0.12807027410848898, 0.082178425886280437,
      0.051655010557090625, 0.031961537782199866, 0.0195320508668999,
      0.011816890774474429, -0.11471753412046258, -0.33725910519653124,
      -0.51584158114744683, -0.60227534443073472, -0.60615109963084912,
      -0.5524420249007751, -0.45926914938739544, -0.33808235179365048,
      -0.23296166928332851, -0.1539879247940821, -0.091139986678587678,
      -0.023967004979100374, 0.035633120649758009, 0.083493216321247912,
      0.11902330437401747, 0.1432220692857781, 0.15769047665670791,
      0.16418272010759458, 0.16438160375521427, 0.26912077560824327,
      0.24630741803311526, 0.1895291252149332, 0.13397404378140912,
      0.090038887782032742, 0.058515628316359758, 0.037130427593930822,
      0.02314249278762508, 0.014224787719223515, 0.0086466624228892786,
      0.0052083303801130935, 0.0031135480352003958, 0.0018493828987362249,
      0.0010924729079617274, 0.00064228187189019207, 0.00037603700442078873,
      0.0002193504386160845, 0.00012753428864040433, 7.3934209823079214E-5,
      4.2748508491664822E-5, 2.4658260869349841E-5, 1.4192663137556215E-5,
      8.1528055383335121E-6, 4.6748054930561066E-6, 2.67606236701583E-6,
      1.5295399420679407E-6, 8.7298507025244673E-7, 4.9759774480213859E-7,
      2.8327953553098752E-7, 1.6108417128143948E-7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0044620443233024847, 0.012543840846505042,
      0.023530877531464538, 0.036808458609793378, 0.051836152368947529,
      0.068124331734327623, 0.08521183824230176, 0.10264432945041417,
      0.11995259495182002, 0.136630320265057, 0.15211037762015475,
      0.16573931915224258, 0.17674901657159064, 0.18422505460973462,
      0.18707037393417725, 0.18632584871935362, 0.1828444506851262,
      0.17732207248045723, 0.17032332432037231, 0.16230361389806108,
      0.15362755060592864, 0.14458450834749623, 0.13540178322992824,
      0.12625562926656289, 0.11728035660370198, 0.10857616088887428,
      0.10021550698711247, 0.092248513897369044, 0.084707391955674491,
      0.077610141817510636, 0.070963651214580328, 0.064766124796484,
      0.059009172932683936, 0.053679488334888822, 0.0487602065594693,
      0.044232003222672026, 0.040073972492582356, 0.03626432439466086,
      0.032780932501023138, 0.029601758512128266, 0.026705175950128626,
      0.024070211551876892, 0.021676719878967333, 0.019505504068787087,
      0.017538393463043041, 0.015758287007660205, 0.014149169768263033,
      0.012696108604220459, 0.011385231953559423, 0.010203697768622701,
      0.0091396528806458148, 0.0081821864370284283, 0.0073212795280553017,
      0.0065477526832468883, 0.0058532125570147139, 0.0052299988266648516,
      0.004671132082699099, 0.0041702632930581468, 0.0037216252620352595, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.017848177293209939,
      0.032327186092810226, 0.043948146739837983, 0.053110324313315346,
      0.060110775036616625, 0.06515271746152039, 0.068350026031896549,
      0.069729964832449684, 0.0692330620056234, 0.066710901252947943,
      0.061920229420391042, 0.054515766128351295, 0.044038789677392234,
      0.029904152152575914, 0.011381277297770522, -0.0029781008592945132,
      -0.013925592136909712, -0.02208951281867589, -0.027994992640339719,
      -0.0320788416892449, -0.034704253168529826, -0.036172169033729641,
      -0.03673090047027202, -0.036584615853461358, -0.035901090651443653,
      -0.03481678285931078, -0.033442615607047241, -0.031867972358973692,
      -0.030164487766778238, -0.028389000552655422, -0.026585962411721221,
      -0.024790105672385328, -0.023027807455200267, -0.021318738391180449,
      -0.019677127101678085, -0.018112813347189113, -0.016632122920358663,
      -0.015238592391685998, -0.013933567574550897, -0.012716695955579475,
      -0.011586330247998553, -0.01053985759300693, -0.0095739666916382377,
      -0.0086848632407209841, -0.0078684424229761955, -0.00712042582153134,
      -0.006436468957588694, -0.0058122446561702833, -0.0052435066026441509,
      -0.0047261367397468911, -0.0042561795519075437, -0.0038298657744695428,
      -0.0034436276358925082, -0.0030941073792336536, -0.0027781605049286952,
      -0.0024928549213994516, -0.0022354669758630086, -0.0020034751585638094,
      -0.0017945521240915509, -0.0016065555488847974, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    } ;

    E4_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    E4_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    E4_DW.FromWorkspace1_IWORK.PrevIndex = 0;
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.52359877559829882, 0.52359877559829882,
      0.52359877559829882, 0.37321893783885784, 0.11705201412929744,
      -0.1046252938987302, -0.27360120067711774, -0.39667564395383836,
      -0.48045849209845348, -0.52359877559829882, -0.52359877559829882,
      -0.52359877559829882, -0.52359877559829882, -0.51399899086163736,
      -0.47732536972937051, -0.4348229250917216, -0.38908617867155337,
      -0.34222224605773049, -0.29586035419163614, -0.25122979861695433,
      -0.20919538905625831, -0.17032874606179618, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.28557083669135908, 0.30305684996644439,
      0.31970615904698751, 0.33493126898149483, 0.34797938635754655,
      0.35792263755472009, 0.36360395934104772, 0.36360345667076405,
      0.35618125234287995, 0.33922200543010045, 0.31014545064270144,
      0.26583982527398531, 0.20254181891821496, 0.11574027746475032,
      -3.7249499199886696E-7, 1.1328777613672802E-7, -1.8899448440083989E-6,
      7.492634559451711E-7, -1.2777361986521606E-6, 5.1713661549031216E-7,
      1.3738948337965592E-6, -5.2619255167109755E-8, -8.2851366804940724E-7,
      1.4603353786316757E-6, -2.7695163381657263E-7, 1.9133491426367113E-7,
      -9.8797324449131615E-8, -9.57013793485276E-9, -6.28973654996212E-7,
      -1.6417437788144485E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E4_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    E4_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    E4_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    char fileName[509] = "o4_out.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(E4_M, "Error creating .mat file o4_out.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,17,0,"oppg4_x123456o_u12o_x123456_u12_out")) {
      rtmSetErrorStatus(E4_M, "Error writing mat file header to file o4_out.mat");
      return;
    }

    E4_DW.ToFile_IWORK.Count = 0;
    E4_DW.ToFile_IWORK.Decimation = -1;
    E4_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  E4_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  E4_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  E4_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  E4_X.Integrator_CSTATE = E4_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  E4_DW.TimeStampA = (rtInf);
  E4_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void E4_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E4/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(E4_DW.HILInitialize_Card);
    hil_monitor_stop_all(E4_DW.HILInitialize_Card);
    is_switching = false;
    if ((E4_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (E4_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E4_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E4_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((E4_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (E4_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E4_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E4_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(E4_DW.HILInitialize_Card
                         , E4_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , E4_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &E4_DW.HILInitialize_AOVoltages[0]
                         , &E4_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(E4_DW.HILInitialize_Card,
            E4_P.HILInitialize_analog_output_cha, num_final_analog_outputs,
            &E4_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(E4_DW.HILInitialize_Card,
            E4_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &E4_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E4_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(E4_DW.HILInitialize_Card);
    hil_monitor_delete_all(E4_DW.HILInitialize_Card);
    hil_close(E4_DW.HILInitialize_Card);
    E4_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) E4_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "o4_out.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E4_M, "Error closing MAT-file o4_out.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(E4_M, "Error reopening MAT-file o4_out.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 17, E4_DW.ToFile_IWORK.Count,
           "oppg4_x123456o_u12o_x123456_u12_out")) {
        rtmSetErrorStatus(E4_M,
                          "Error writing header for oppg4_x123456o_u12o_x123456_u12_out to MAT-file o4_out.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E4_M, "Error closing MAT-file o4_out.mat");
        return;
      }

      E4_DW.ToFile_PWORK.FilePtr = (NULL);
    }
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
  E4_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  E4_update();
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
  E4_initialize();
}

void MdlTerminate(void)
{
  E4_terminate();
}

/* Registration function */
RT_MODEL_E4_T *E4(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  E4_P.Integrator_UpperSat = rtInf;
  E4_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)E4_M, 0,
                sizeof(RT_MODEL_E4_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&E4_M->solverInfo, &E4_M->Timing.simTimeStep);
    rtsiSetTPtr(&E4_M->solverInfo, &rtmGetTPtr(E4_M));
    rtsiSetStepSizePtr(&E4_M->solverInfo, &E4_M->Timing.stepSize0);
    rtsiSetdXPtr(&E4_M->solverInfo, &E4_M->ModelData.derivs);
    rtsiSetContStatesPtr(&E4_M->solverInfo, (real_T **)
                         &E4_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&E4_M->solverInfo, &E4_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&E4_M->solverInfo, (&rtmGetErrorStatus(E4_M)));
    rtsiSetRTModelPtr(&E4_M->solverInfo, E4_M);
  }

  rtsiSetSimTimeStep(&E4_M->solverInfo, MAJOR_TIME_STEP);
  E4_M->ModelData.intgData.f[0] = E4_M->ModelData.odeF[0];
  E4_M->ModelData.contStates = ((real_T *) &E4_X);
  rtsiSetSolverData(&E4_M->solverInfo, (void *)&E4_M->ModelData.intgData);
  rtsiSetSolverName(&E4_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = E4_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    E4_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    E4_M->Timing.sampleTimes = (&E4_M->Timing.sampleTimesArray[0]);
    E4_M->Timing.offsetTimes = (&E4_M->Timing.offsetTimesArray[0]);

    /* task periods */
    E4_M->Timing.sampleTimes[0] = (0.0);
    E4_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    E4_M->Timing.offsetTimes[0] = (0.0);
    E4_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(E4_M, &E4_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = E4_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    E4_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(E4_M, -1);
  E4_M->Timing.stepSize0 = 0.002;
  E4_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  E4_M->Sizes.checksums[0] = (3092171698U);
  E4_M->Sizes.checksums[1] = (1265290741U);
  E4_M->Sizes.checksums[2] = (2099696264U);
  E4_M->Sizes.checksums[3] = (1478139468U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    E4_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(E4_M->extModeInfo, &E4_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(E4_M->extModeInfo, E4_M->Sizes.checksums);
    rteiSetTPtr(E4_M->extModeInfo, rtmGetTPtr(E4_M));
  }

  E4_M->solverInfoPtr = (&E4_M->solverInfo);
  E4_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&E4_M->solverInfo, 0.002);
  rtsiSetSolverMode(&E4_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  E4_M->ModelData.blockIO = ((void *) &E4_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      E4_B.FromWorkspace1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      E4_B.Gain1[i] = 0.0;
    }

    for (i = 0; i < 16; i++) {
      E4_B.TmpSignalConversionAtToFileInpo[i] = 0.0;
    }

    E4_B.FromWorkspace[0] = 0.0;
    E4_B.FromWorkspace[1] = 0.0;
    E4_B.TravelCounttorad = 0.0;
    E4_B.Gain = 0.0;
    E4_B.Sum4 = 0.0;
    E4_B.Gain_d = 0.0;
    E4_B.PitchCounttorad = 0.0;
    E4_B.Gain_i = 0.0;
    E4_B.Gain_b = 0.0;
    E4_B.ElevationCounttorad = 0.0;
    E4_B.Gain_e = 0.0;
    E4_B.Sum = 0.0;
    E4_B.Gain_dg = 0.0;
    E4_B.Sum1[0] = 0.0;
    E4_B.Sum1[1] = 0.0;
    E4_B.Sum_k = 0.0;
    E4_B.Sum2 = 0.0;
    E4_B.K_ei = 0.0;
    E4_B.Gain_l = 0.0;
    E4_B.BackmotorSaturation = 0.0;
    E4_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  E4_M->ModelData.defaultParam = ((real_T *)&E4_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &E4_X;
    E4_M->ModelData.contStates = (x);
    (void) memset((void *)&E4_X, 0,
                  sizeof(X_E4_T));
  }

  /* states (dwork) */
  E4_M->ModelData.dwork = ((void *) &E4_DW);
  (void) memset((void *)&E4_DW, 0,
                sizeof(DW_E4_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E4_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  E4_DW.TimeStampA = 0.0;
  E4_DW.LastUAtTimeA = 0.0;
  E4_DW.TimeStampB = 0.0;
  E4_DW.LastUAtTimeB = 0.0;
  E4_DW.HILWriteAnalog_Buffer[0] = 0.0;
  E4_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    E4_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  E4_M->Sizes.numContStates = (4);     /* Number of continuous states */
  E4_M->Sizes.numY = (0);              /* Number of model outputs */
  E4_M->Sizes.numU = (0);              /* Number of model inputs */
  E4_M->Sizes.sysDirFeedThru = (0);    /* The model is not direct feedthrough */
  E4_M->Sizes.numSampTimes = (2);      /* Number of sample times */
  E4_M->Sizes.numBlocks = (65);        /* Number of blocks */
  E4_M->Sizes.numBlockIO = (22);       /* Number of block outputs */
  E4_M->Sizes.numBlockPrms = (153);    /* Sum of parameter "widths" */
  return E4_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/

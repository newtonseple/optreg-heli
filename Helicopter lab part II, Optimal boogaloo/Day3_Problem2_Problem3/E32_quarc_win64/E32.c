/*
 * E32.c
 *
 * Code generation for model "E32".
 *
 * Model version              : 1.173
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Tue Mar 14 12:29:56 2017
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "E32.h"
#include "E32_private.h"
#include "E32_dt.h"

/* Block signals (auto storage) */
B_E32_T E32_B;

/* Continuous states */
X_E32_T E32_X;

/* Block states (auto storage) */
DW_E32_T E32_DW;

/* Real-time model */
RT_MODEL_E32_T E32_M_;
RT_MODEL_E32_T *const E32_M = &E32_M_;

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
  E32_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void E32_output(void)
{
  /* local block i/o variables */
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T *lastU;
  real_T rtb_Derivative;
  if (rtmIsMajorTimeStep(E32_M)) {
    /* set solver stop time */
    if (!(E32_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&E32_M->solverInfo, ((E32_M->Timing.clockTickH0 + 1)
        * E32_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&E32_M->solverInfo, ((E32_M->Timing.clockTick0 + 1) *
        E32_M->Timing.stepSize0 + E32_M->Timing.clockTickH0 *
        E32_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(E32_M)) {
    E32_M->Timing.t[0] = rtsiGetT(&E32_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(E32_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: E32/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(E32_DW.HILReadEncoderTimebase_Task, 1,
        &E32_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 = E32_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 = E32_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 = E32_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }

    /* Gain: '<S4>/Travel: Count to rad' */
    E32_B.TravelCounttorad = E32_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S11>/Gain' */
    E32_B.Gain = E32_P.Gain_Gain * E32_B.TravelCounttorad;

    /* Sum: '<Root>/Sum4' incorporates:
     *  Constant: '<Root>/travel_offset [deg]'
     */
    E32_B.Sum4 = E32_P.travel_offsetdeg_Value + E32_B.Gain;
  }

  /* TransferFcn: '<S4>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E32_P.TravelTransferFcn_C * E32_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += E32_P.TravelTransferFcn_D * E32_B.TravelCounttorad;

  /* Gain: '<S12>/Gain' */
  E32_B.Gain_d = E32_P.Gain_Gain_l * rtb_Backgain;
  if (rtmIsMajorTimeStep(E32_M)) {
    /* Gain: '<S4>/Pitch: Count to rad' */
    E32_B.PitchCounttorad = E32_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S8>/Gain' */
    E32_B.Gain_i = E32_P.Gain_Gain_a * E32_B.PitchCounttorad;
  }

  /* TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E32_P.PitchTransferFcn_C * E32_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += E32_P.PitchTransferFcn_D * E32_B.PitchCounttorad;

  /* Gain: '<S9>/Gain' */
  E32_B.Gain_b = E32_P.Gain_Gain_ae * rtb_Backgain;
  if (rtmIsMajorTimeStep(E32_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' */
    E32_B.ElevationCounttorad = E32_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S6>/Gain' */
    E32_B.Gain_e = E32_P.Gain_Gain_lv * E32_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    E32_B.Sum = E32_B.Gain_e + E32_P.elavation_offsetdeg_Value;
  }

  /* TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E32_P.ElevationTransferFcn_C *
    E32_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += E32_P.ElevationTransferFcn_D * E32_B.ElevationCounttorad;

  /* Gain: '<S7>/Gain' */
  E32_B.Gain_dg = E32_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  E32_B.Gain1[0] = E32_P.Gain1_Gain * E32_B.Sum4;
  E32_B.Gain1[1] = E32_P.Gain1_Gain * E32_B.Gain_d;
  E32_B.Gain1[2] = E32_P.Gain1_Gain * E32_B.Gain_i;
  E32_B.Gain1[3] = E32_P.Gain1_Gain * E32_B.Gain_b;
  E32_B.Gain1[4] = E32_P.Gain1_Gain * E32_B.Sum;
  E32_B.Gain1[5] = E32_P.Gain1_Gain * E32_B.Gain_dg;

  /* FromWorkspace: '<Root>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *) E32_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E32_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = E32_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = E32_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      {
        int_T elIdx;
        for (elIdx = 0; elIdx < 4; ++elIdx) {
          (&E32_B.FromWorkspace1[0])[elIdx] = pDataValues[100];
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

      E32_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 4; ++elIdx) {
                (&E32_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex];
                pDataValues += 101;
              }
            }
          } else {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 4; ++elIdx) {
                (&E32_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex +
                  1];
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
            for (elIdx = 0; elIdx < 4; ++elIdx) {
              d1 = pDataValues[TimeIndex];
              d2 = pDataValues[TimeIndex + 1];
              (&E32_B.FromWorkspace1[0])[elIdx] = (real_T) rtInterpolate(d1, d2,
                f1, f2);
              pDataValues += 101;
            }
          }
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* FromWorkspace: '<Root>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) E32_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E32_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = E32_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = E32_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      E32_B.FromWorkspace = pDataValues[100];
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

      E32_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            E32_B.FromWorkspace = pDataValues[currTimeIndex];
          } else {
            E32_B.FromWorkspace = pDataValues[currTimeIndex + 1];
          }
        } else {
          real_T f1 = (t2 - t) / (t2 - t1);
          real_T f2 = 1.0 - f1;
          real_T d1;
          real_T d2;
          int_T TimeIndex= currTimeIndex;
          d1 = pDataValues[TimeIndex];
          d2 = pDataValues[TimeIndex + 1];
          E32_B.FromWorkspace = (real_T) rtInterpolate(d1, d2, f1, f2);
          pDataValues += 101;
        }
      }
    }
  }

  /* Gain: '<Root>/LQR' incorporates:
   *  Sum: '<Root>/Sum2'
   */
  rtb_Backgain = (((E32_B.Gain1[0] - E32_B.FromWorkspace1[0]) * E32_P.K[0] +
                   (E32_B.Gain1[1] - E32_B.FromWorkspace1[1]) * E32_P.K[1]) +
                  (E32_B.Gain1[2] - E32_B.FromWorkspace1[2]) * E32_P.K[2]) +
    (E32_B.Gain1[3] - E32_B.FromWorkspace1[3]) * E32_P.K[3];

  /* Sum: '<Root>/Sum1' */
  E32_B.Sum1 = E32_B.FromWorkspace - rtb_Backgain;
  if (rtmIsMajorTimeStep(E32_M)) {
    /* SignalConversion: '<Root>/TmpSignal ConversionAtTo FileInport1' */
    E32_B.TmpSignalConversionAtToFileInpo[0] = E32_B.FromWorkspace1[0];
    E32_B.TmpSignalConversionAtToFileInpo[1] = E32_B.FromWorkspace1[1];
    E32_B.TmpSignalConversionAtToFileInpo[2] = E32_B.FromWorkspace1[2];
    E32_B.TmpSignalConversionAtToFileInpo[3] = E32_B.FromWorkspace1[3];
    E32_B.TmpSignalConversionAtToFileInpo[4] = E32_B.FromWorkspace;
    E32_B.TmpSignalConversionAtToFileInpo[5] = E32_B.Gain1[0];
    E32_B.TmpSignalConversionAtToFileInpo[6] = E32_B.Gain1[1];
    E32_B.TmpSignalConversionAtToFileInpo[7] = E32_B.Gain1[2];
    E32_B.TmpSignalConversionAtToFileInpo[8] = E32_B.Gain1[3];
    E32_B.TmpSignalConversionAtToFileInpo[9] = E32_B.Sum1;

    /* ToFile: '<Root>/To File' */
    {
      if (!(++E32_DW.ToFile_IWORK.Decimation % 1) && (E32_DW.ToFile_IWORK.Count*
           11)+1 < 100000000 ) {
        FILE *fp = (FILE *) E32_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[11];
          E32_DW.ToFile_IWORK.Decimation = 0;
          u[0] = E32_M->Timing.t[1];
          u[1] = E32_B.TmpSignalConversionAtToFileInpo[0];
          u[2] = E32_B.TmpSignalConversionAtToFileInpo[1];
          u[3] = E32_B.TmpSignalConversionAtToFileInpo[2];
          u[4] = E32_B.TmpSignalConversionAtToFileInpo[3];
          u[5] = E32_B.TmpSignalConversionAtToFileInpo[4];
          u[6] = E32_B.TmpSignalConversionAtToFileInpo[5];
          u[7] = E32_B.TmpSignalConversionAtToFileInpo[6];
          u[8] = E32_B.TmpSignalConversionAtToFileInpo[7];
          u[9] = E32_B.TmpSignalConversionAtToFileInpo[8];
          u[10] = E32_B.TmpSignalConversionAtToFileInpo[9];
          if (fwrite(u, sizeof(real_T), 11, fp) != 11) {
            rtmSetErrorStatus(E32_M, "Error writing to MAT-file o3_Q1_out.mat");
            return;
          }

          if (((++E32_DW.ToFile_IWORK.Count)*11)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file o3_Q1_out.mat.\n");
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
  E32_B.Sum_k = ((E32_B.Sum1 - E32_B.Gain1[2]) * E32_P.K_pp - E32_P.K_pd *
                 E32_B.Gain1[3]) + E32_P.Vd_ff;
  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* Integrator: '<S3>/Integrator'
   *
   * Regarding '<S3>/Integrator':
   *  Limited Integrator
   */
  if (E32_X.Integrator_CSTATE >= E32_P.Integrator_UpperSat ) {
    E32_X.Integrator_CSTATE = E32_P.Integrator_UpperSat;
  } else if (E32_X.Integrator_CSTATE <= (E32_P.Integrator_LowerSat) ) {
    E32_X.Integrator_CSTATE = (E32_P.Integrator_LowerSat);
  }

  rtb_Backgain = E32_X.Integrator_CSTATE;

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   */
  rtb_Derivative = E32_P.elevation_ref_Value - E32_B.Gain1[4];

  /* Sum: '<S3>/Sum2' incorporates:
   *  Constant: '<S3>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  E32_B.Sum2 = ((E32_P.K_ep * rtb_Derivative + rtb_Backgain) - E32_P.K_ed *
                E32_B.Gain1[5]) + E32_P.Vs_ff;
  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (E32_B.Sum2 - E32_B.Sum_k) * E32_P.Backgain_Gain;

  /* Gain: '<S3>/K_ei' */
  E32_B.K_ei = E32_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((E32_DW.TimeStampA >= E32_M->Timing.t[0]) && (E32_DW.TimeStampB >=
       E32_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = E32_DW.TimeStampA;
    lastU = &E32_DW.LastUAtTimeA;
    if (E32_DW.TimeStampA < E32_DW.TimeStampB) {
      if (E32_DW.TimeStampB < E32_M->Timing.t[0]) {
        rtb_Derivative = E32_DW.TimeStampB;
        lastU = &E32_DW.LastUAtTimeB;
      }
    } else {
      if (E32_DW.TimeStampA >= E32_M->Timing.t[0]) {
        rtb_Derivative = E32_DW.TimeStampB;
        lastU = &E32_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (E32_B.PitchCounttorad - *lastU) / (E32_M->Timing.t[0] -
      rtb_Derivative);
  }

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S10>/Gain' */
  E32_B.Gain_l = E32_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > E32_P.BackmotorSaturation_UpperSat) {
    E32_B.BackmotorSaturation = E32_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < E32_P.BackmotorSaturation_LowerSat) {
    E32_B.BackmotorSaturation = E32_P.BackmotorSaturation_LowerSat;
  } else {
    E32_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(E32_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (E32_B.Sum_k + E32_B.Sum2) * E32_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Derivative > E32_P.FrontmotorSaturation_UpperSat) {
    E32_B.FrontmotorSaturation = E32_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < E32_P.FrontmotorSaturation_LowerSat) {
    E32_B.FrontmotorSaturation = E32_P.FrontmotorSaturation_LowerSat;
  } else {
    E32_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(E32_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: E32/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      E32_DW.HILWriteAnalog_Buffer[0] = E32_B.FrontmotorSaturation;
      E32_DW.HILWriteAnalog_Buffer[1] = E32_B.BackmotorSaturation;
      result = hil_write_analog(E32_DW.HILInitialize_Card,
        E32_P.HILWriteAnalog_channels, 2, &E32_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void E32_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (E32_DW.TimeStampA == (rtInf)) {
    E32_DW.TimeStampA = E32_M->Timing.t[0];
    lastU = &E32_DW.LastUAtTimeA;
  } else if (E32_DW.TimeStampB == (rtInf)) {
    E32_DW.TimeStampB = E32_M->Timing.t[0];
    lastU = &E32_DW.LastUAtTimeB;
  } else if (E32_DW.TimeStampA < E32_DW.TimeStampB) {
    E32_DW.TimeStampA = E32_M->Timing.t[0];
    lastU = &E32_DW.LastUAtTimeA;
  } else {
    E32_DW.TimeStampB = E32_M->Timing.t[0];
    lastU = &E32_DW.LastUAtTimeB;
  }

  *lastU = E32_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(E32_M)) {
    rt_ertODEUpdateContinuousStates(&E32_M->solverInfo);
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
  if (!(++E32_M->Timing.clockTick0)) {
    ++E32_M->Timing.clockTickH0;
  }

  E32_M->Timing.t[0] = rtsiGetSolverStopTime(&E32_M->solverInfo);

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
    if (!(++E32_M->Timing.clockTick1)) {
      ++E32_M->Timing.clockTickH1;
    }

    E32_M->Timing.t[1] = E32_M->Timing.clockTick1 * E32_M->Timing.stepSize1 +
      E32_M->Timing.clockTickH1 * E32_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void E32_derivatives(void)
{
  XDot_E32_T *_rtXdot;
  _rtXdot = ((XDot_E32_T *) E32_M->ModelData.derivs);

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += E32_P.TravelTransferFcn_A *
    E32_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += E32_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += E32_P.PitchTransferFcn_A *
    E32_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += E32_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += E32_P.ElevationTransferFcn_A *
    E32_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += E32_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( E32_X.Integrator_CSTATE <= (E32_P.Integrator_LowerSat) );
    usat = ( E32_X.Integrator_CSTATE >= E32_P.Integrator_UpperSat );
    if ((!lsat && !usat) ||
        (lsat && (E32_B.K_ei > 0)) ||
        (usat && (E32_B.K_ei < 0)) ) {
      ((XDot_E32_T *) E32_M->ModelData.derivs)->Integrator_CSTATE = E32_B.K_ei;
    } else {
      /* in saturation */
      ((XDot_E32_T *) E32_M->ModelData.derivs)->Integrator_CSTATE = 0.0;
    }
  }
}

/* Model initialize function */
void E32_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E32/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &E32_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E32_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(E32_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E32_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(E32_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E32_M, _rt_error_message);
      return;
    }

    if ((E32_P.HILInitialize_set_analog_input_ && !is_switching) ||
        (E32_P.HILInitialize_set_analog_inpu_m && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &E32_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = E32_P.HILInitialize_analog_input_mini;
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &E32_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = E32_P.HILInitialize_analog_input_maxi;
        }
      }

      result = hil_set_analog_input_ranges(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_analog_input_chan, 8U,
        &E32_DW.HILInitialize_AIMinimums[0], &E32_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_analog_output && !is_switching) ||
        (E32_P.HILInitialize_set_analog_outp_b && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &E32_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = E32_P.HILInitialize_analog_output_min;
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &E32_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = E32_P.HILInitialize_analog_output_max;
        }
      }

      result = hil_set_analog_output_ranges(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_analog_output_cha, 8U,
        &E32_DW.HILInitialize_AOMinimums[0], &E32_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_analog_outp_e && !is_switching) ||
        (E32_P.HILInitialize_set_analog_outp_j && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E32_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E32_P.HILInitialize_initial_analog_ou;
        }
      }

      result = hil_write_analog(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_analog_output_cha, 8U,
        &E32_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if (E32_P.HILInitialize_set_analog_outp_p) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E32_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E32_P.HILInitialize_watchdog_analog_o;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (E32_DW.HILInitialize_Card, E32_P.HILInitialize_analog_output_cha, 8U,
         &E32_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_encoder_param && !is_switching) ||
        (E32_P.HILInitialize_set_encoder_par_m && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes = &E32_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = E32_P.HILInitialize_quadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_encoder_channels, 8U, (t_encoder_quadrature_mode *)
        &E32_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_encoder_count && !is_switching) ||
        (E32_P.HILInitialize_set_encoder_cou_k && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts = &E32_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = E32_P.HILInitialize_initial_encoder_c;
        }
      }

      result = hil_set_encoder_counts(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_encoder_channels, 8U,
        &E32_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_pwm_params_at && !is_switching) ||
        (E32_P.HILInitialize_set_pwm_params__f && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &E32_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E32_P.HILInitialize_pwm_modes;
        }
      }

      result = hil_set_pwm_mode(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_pwm_channels, 8U, (t_pwm_mode *)
        &E32_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_pwm_channels =
          E32_P.HILInitialize_pwm_channels;
        int32_T *dw_POModeValues = &E32_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE) {
            E32_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              p_HILInitialize_pwm_channels[i1];
            E32_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              E32_P.HILInitialize_pwm_frequency;
            num_duty_cycle_modes++;
          } else {
            E32_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              p_HILInitialize_pwm_channels[i1];
            E32_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              E32_P.HILInitialize_pwm_frequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(E32_DW.HILInitialize_Card,
          &E32_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &E32_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E32_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(E32_DW.HILInitialize_Card,
          &E32_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &E32_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E32_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &E32_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = E32_P.HILInitialize_pwm_configuration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &E32_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = E32_P.HILInitialize_pwm_alignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals = &E32_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = E32_P.HILInitialize_pwm_polarity;
        }
      }

      result = hil_set_pwm_configuration(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_pwm_channels, 8U,
        (t_pwm_configuration *) &E32_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &E32_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &E32_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &E32_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = E32_P.HILInitialize_pwm_leading_deadb;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &E32_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E32_P.HILInitialize_pwm_trailing_dead;
        }
      }

      result = hil_set_pwm_deadband(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_pwm_channels, 8U,
        &E32_DW.HILInitialize_POSortedFreqs[0], &E32_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if ((E32_P.HILInitialize_set_pwm_outputs_a && !is_switching) ||
        (E32_P.HILInitialize_set_pwm_outputs_g && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E32_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E32_P.HILInitialize_initial_pwm_outpu;
        }
      }

      result = hil_write_pwm(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_pwm_channels, 8U, &E32_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }

    if (E32_P.HILInitialize_set_pwm_outputs_o) {
      {
        int_T i1;
        real_T *dw_POValues = &E32_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E32_P.HILInitialize_watchdog_pwm_outp;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state(E32_DW.HILInitialize_Card,
        E32_P.HILInitialize_pwm_channels, 8U, &E32_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(E32_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: E32/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(E32_DW.HILInitialize_Card,
      E32_P.HILReadEncoderTimebase_samples_,
      E32_P.HILReadEncoderTimebase_channels, 3,
      &E32_DW.HILReadEncoderTimebase_Task);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(E32_M, _rt_error_message);
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
      3.1415926535897931, 3.1378421413625315, 3.1262155534580218,
      3.1033093000300256, 3.0666274151913049, 3.0144539223943858,
      2.9456562771179411, 2.859507763294125, 2.7555515879660177,
      2.6335051104915435, 2.4931956060339213, 2.3345185760670661,
      2.1583785949524832, 1.9678012313071425, 1.7674134694343919,
      1.5625848879166107, 1.3586888733371518, 1.1605971079001447,
      0.97235833778319269, 0.79695258937503832, 0.6364331459848972,
      0.49215716028041168, 0.36485057122540471, 0.25462570247136257,
      0.1610697945325211, 0.0833559445461416, 0.0203537233730774,
      -0.029270545186106545, -0.066967644850820046, -0.09423426722840518,
      -0.11255613093570045, -0.12336415429614711, -0.12800215751999192,
      -0.1277045054421416, -0.12358213763329481, -0.11661552225618226,
      -0.10765318889593076, -0.097414629856000051, -0.086496500391681844,
      -0.075381190105649, -0.064446975938248663, -0.053979098655188448,
      -0.04418122714289846, -0.035186886560758031, -0.0270705264634332,
      -0.019857992882105949, -0.013536243912080921, -0.008062211743757524,
      -0.0033707656039267003, 0.00061822990462685333, 0.0039937378049206534,
      0.0068482122759090684, 0.0092730559033079446, 0.011354830197230105,
      0.013172198763827241, 0.014793616719452464, 0.016275827235902849,
      0.017663278364013317, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, -0.015002048909046481, -0.0465063516180402,
      -0.091625013711984282, -0.14672753935488311, -0.2086939711876756,
      -0.2751905811057796, -0.34459405529526432, -0.41582470131243032,
      -0.48818590989789662, -0.56123801783048888, -0.63470811986742037,
      -0.70455992445833227, -0.76230945458136246, -0.80155104749100226,
      -0.8193143260711242, -0.8155840583178352, -0.79236706174802785,
      -0.75295508046780846, -0.7016229936326176, -0.64207777356056417,
      -0.57710394281794208, -0.50922635622002788, -0.44089947501616861,
      -0.37422363175536577, -0.31085539994551803, -0.2520088846922568,
      -0.19849707423673579, -0.15078839865885396, -0.10906648951034059,
      -0.0732874548291811, -0.043232093441786627, -0.018552012895379142,
      0.0011906083114012777, 0.016489471235387122, 0.027866461508450246,
      0.035849333441005972, 0.040954236159722865, 0.043672517857272795,
      0.044461241144131358, 0.0437368566696014, 0.041871509132240865,
      0.039191486049159924, 0.035977362328561718, 0.032465440389299341,
      0.028850134325309, 0.025286995880100107, 0.021896128673293585,
      0.018765784559323297, 0.015955982034214215, 0.013502031601175203,
      0.01141789788395366, 0.0096993745095955031, 0.00832709717568864,
      0.007269474266388542, 0.0064856718225008913, 0.0059288420658015371,
      0.0055498045124418756, 0.0053013042051486253, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.10602875205850044, 0.22266037932282337,
      0.3188814718158467, 0.38944360631066777, 0.43795507377576759,
      0.46997264230260377, 0.49051724877379604, 0.50343100141252184,
      0.51142138585718611, 0.51630439857217725, 0.51925862125999867,
      0.49368587682329873, 0.40815162302379465, 0.27734459141029827,
      0.12554406879596819, -0.026364107804420094, -0.16408886464559813,
      -0.27854883133828229, -0.36279558483587121, -0.4208428737678257,
      -0.45921022067500472, -0.4797328580484595, -0.48290830072724128,
      -0.4712393951154264, -0.44786246063365037, -0.41590469496018784,
      -0.37820103889732826, -0.33718669793406114, -0.29487451929632275,
      -0.25287255228274463, -0.21241981544607708, -0.17442938340648173,
      -0.13953330652430604, -0.10812652015593119, -0.0804082221133921,
      -0.056419889975249431, -0.036079502735070136, -0.019211776863417789,
      -0.0055743949597886746, 0.0051196728067357195, 0.013183563974549473,
      0.01894137958820372, 0.022716198908747071, 0.02482092298221459,
      0.025551602490995244, 0.025182901685472294, 0.023965354366828284,
      0.022124076646477076, 0.019858611118677847, 0.017343584440099032,
      0.014729861134287493, 0.012145866866003956, 0.009698732091492605,
      0.0074748747921991243, 0.0055396163210544129, 0.0039354600541442672,
      0.0026788926639079022, 0.0017563052639142371, 0.0011215707594713395, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823400178,
      0.46652650905729171, 0.3848843699720933, 0.28224853797928429,
      0.19404586986039934, 0.12807027410734459, 0.082178425884769035,
      0.05165501055490325, 0.031961537778656866, 0.019532050859964685,
      0.011816890751285343, -0.10229097774679971, -0.34213701519801626,
      -0.52322812645398575, -0.60720209045732021, -0.6076327064015532,
      -0.55089902736471219, -0.45783986677073663, -0.33698701399035563,
      -0.23218915572781798, -0.15346938762871595, -0.082090549493819262,
      -0.012701770715127247, 0.046675622447259764, 0.093507737927104026,
      0.12783106269385006, 0.15081462425143849, 0.16405736385306818,
      0.16924871455095361, 0.1680078680543125, 0.16181094734667037,
      0.15196172815838138, 0.13958430752870277, 0.12562714547349943,
      0.11087319217015637, 0.09595332855257066, 0.081361548960717164,
      0.0674709034866094, 0.054549527614516452, 0.042776271066097576,
      0.03225556467125501, 0.023031262454616994, 0.015099277282173409,
      0.0084188962938700779, 0.0029227180351226153, -0.0014748032220917877,
      -0.0048701892745760429, -0.0073651108814048391, -0.0090618621111969184,
      -0.010060106714315257, -0.010454893223246157, -0.010335977073134144,
      -0.0097885390980454023, -0.0088954291971739229, -0.0077410338845788455,
      -0.0064166250676405837, -0.00502626956094546, -0.00369034959997466,
      -0.0025389380177715908, -0.0016765010655476911, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    } ;

    E32_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    E32_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    E32_DW.FromWorkspace1_IWORK.PrevIndex = 0;
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559753309,
      0.523598775597397, 0.52359877559722157, 0.52359877559699,
      0.523598775596675, 0.52359877559622914, 0.52359877559556334,
      0.5235987755944882, 0.5235987755925201, 0.52359877558795276,
      0.52359877556741929, 0.38856036545868561, 0.10949576024353411,
      -0.11003563544112602, -0.27688452242302208, -0.39785602260923403,
      -0.47956180370348472, -0.52358628562450082, -0.52359877490890916,
      -0.523598774954063, -0.52359863675677765, -0.50324239318874864,
      -0.4647567833487643, -0.42054051296355521, -0.37322907148973761,
      -0.32496743350908108, -0.2774531791743251, -0.23198382899978745,
      -0.1895060041249248, -0.1506644799103363, -0.11584962973923742,
      -0.0852421406485294, -0.058854217035133112, -0.036566772640272888,
      -0.018162346496149957, -0.0033536691243817932, 0.0081920446834722275,
      0.016832886062451407, 0.022935851914134192, 0.026863924747256195,
      0.028966268700267195, 0.029571161680182007, 0.028981281303823186,
      0.027470970409997228, 0.025285122866445353, 0.022639348876530577,
      0.019721097611529976, 0.016691430137389288, 0.01368714322725244,
      0.010822940222778847, 0.00819332401767213, 0.0058738464761366512,
      0.0039212915160998525, 0.0023723171285108917, 0.0012401012706759953,
      0.00050879864505998967, 0.00012652119243539915, -2.871272556091978E-14,
      -1.0770196013524689E-14, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E32_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    E32_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    E32_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    char fileName[509] = "o3_Q1_out.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(E32_M, "Error creating .mat file o3_Q1_out.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,11,0,
         "oppg3_Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
      rtmSetErrorStatus(E32_M,
                        "Error writing mat file header to file o3_Q1_out.mat");
      return;
    }

    E32_DW.ToFile_IWORK.Count = 0;
    E32_DW.ToFile_IWORK.Decimation = -1;
    E32_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  E32_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  E32_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  E32_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  E32_X.Integrator_CSTATE = E32_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  E32_DW.TimeStampA = (rtInf);
  E32_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void E32_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: E32/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(E32_DW.HILInitialize_Card);
    hil_monitor_stop_all(E32_DW.HILInitialize_Card);
    is_switching = false;
    if ((E32_P.HILInitialize_set_analog_out_ex && !is_switching) ||
        (E32_P.HILInitialize_set_analog_outp_c && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &E32_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = E32_P.HILInitialize_final_analog_outp;
        }
      }

      num_final_analog_outputs = 8U;
    }

    if ((E32_P.HILInitialize_set_pwm_output_ap && !is_switching) ||
        (E32_P.HILInitialize_set_pwm_outputs_p && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &E32_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = E32_P.HILInitialize_final_pwm_outputs;
        }
      }

      num_final_pwm_outputs = 8U;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(E32_DW.HILInitialize_Card
                         , E32_P.HILInitialize_analog_output_cha,
                         num_final_analog_outputs
                         , E32_P.HILInitialize_pwm_channels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &E32_DW.HILInitialize_AOVoltages[0]
                         , &E32_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(E32_DW.HILInitialize_Card,
            E32_P.HILInitialize_analog_output_cha, num_final_analog_outputs,
            &E32_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(E32_DW.HILInitialize_Card,
            E32_P.HILInitialize_pwm_channels, num_final_pwm_outputs,
            &E32_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(E32_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(E32_DW.HILInitialize_Card);
    hil_monitor_delete_all(E32_DW.HILInitialize_Card);
    hil_close(E32_DW.HILInitialize_Card);
    E32_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) E32_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "o3_Q1_out.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E32_M, "Error closing MAT-file o3_Q1_out.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(E32_M, "Error reopening MAT-file o3_Q1_out.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 11, E32_DW.ToFile_IWORK.Count,
           "oppg3_Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
        rtmSetErrorStatus(E32_M,
                          "Error writing header for oppg3_Q1_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out to MAT-file o3_Q1_out.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E32_M, "Error closing MAT-file o3_Q1_out.mat");
        return;
      }

      E32_DW.ToFile_PWORK.FilePtr = (NULL);
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
  E32_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  E32_update();
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
  E32_initialize();
}

void MdlTerminate(void)
{
  E32_terminate();
}

/* Registration function */
RT_MODEL_E32_T *E32(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  E32_P.Integrator_UpperSat = rtInf;
  E32_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)E32_M, 0,
                sizeof(RT_MODEL_E32_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&E32_M->solverInfo, &E32_M->Timing.simTimeStep);
    rtsiSetTPtr(&E32_M->solverInfo, &rtmGetTPtr(E32_M));
    rtsiSetStepSizePtr(&E32_M->solverInfo, &E32_M->Timing.stepSize0);
    rtsiSetdXPtr(&E32_M->solverInfo, &E32_M->ModelData.derivs);
    rtsiSetContStatesPtr(&E32_M->solverInfo, (real_T **)
                         &E32_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&E32_M->solverInfo, &E32_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&E32_M->solverInfo, (&rtmGetErrorStatus(E32_M)));
    rtsiSetRTModelPtr(&E32_M->solverInfo, E32_M);
  }

  rtsiSetSimTimeStep(&E32_M->solverInfo, MAJOR_TIME_STEP);
  E32_M->ModelData.intgData.f[0] = E32_M->ModelData.odeF[0];
  E32_M->ModelData.contStates = ((real_T *) &E32_X);
  rtsiSetSolverData(&E32_M->solverInfo, (void *)&E32_M->ModelData.intgData);
  rtsiSetSolverName(&E32_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = E32_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    E32_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    E32_M->Timing.sampleTimes = (&E32_M->Timing.sampleTimesArray[0]);
    E32_M->Timing.offsetTimes = (&E32_M->Timing.offsetTimesArray[0]);

    /* task periods */
    E32_M->Timing.sampleTimes[0] = (0.0);
    E32_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    E32_M->Timing.offsetTimes[0] = (0.0);
    E32_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(E32_M, &E32_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = E32_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    E32_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(E32_M, -1);
  E32_M->Timing.stepSize0 = 0.002;
  E32_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  E32_M->Sizes.checksums[0] = (3170637004U);
  E32_M->Sizes.checksums[1] = (3467527198U);
  E32_M->Sizes.checksums[2] = (4131639906U);
  E32_M->Sizes.checksums[3] = (783170660U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[1];
    E32_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(E32_M->extModeInfo,
      &E32_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(E32_M->extModeInfo, E32_M->Sizes.checksums);
    rteiSetTPtr(E32_M->extModeInfo, rtmGetTPtr(E32_M));
  }

  E32_M->solverInfoPtr = (&E32_M->solverInfo);
  E32_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&E32_M->solverInfo, 0.002);
  rtsiSetSolverMode(&E32_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  E32_M->ModelData.blockIO = ((void *) &E32_B);

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      E32_B.Gain1[i] = 0.0;
    }

    for (i = 0; i < 10; i++) {
      E32_B.TmpSignalConversionAtToFileInpo[i] = 0.0;
    }

    E32_B.TravelCounttorad = 0.0;
    E32_B.Gain = 0.0;
    E32_B.Sum4 = 0.0;
    E32_B.Gain_d = 0.0;
    E32_B.PitchCounttorad = 0.0;
    E32_B.Gain_i = 0.0;
    E32_B.Gain_b = 0.0;
    E32_B.ElevationCounttorad = 0.0;
    E32_B.Gain_e = 0.0;
    E32_B.Sum = 0.0;
    E32_B.Gain_dg = 0.0;
    E32_B.FromWorkspace1[0] = 0.0;
    E32_B.FromWorkspace1[1] = 0.0;
    E32_B.FromWorkspace1[2] = 0.0;
    E32_B.FromWorkspace1[3] = 0.0;
    E32_B.FromWorkspace = 0.0;
    E32_B.Sum1 = 0.0;
    E32_B.Sum_k = 0.0;
    E32_B.Sum2 = 0.0;
    E32_B.K_ei = 0.0;
    E32_B.Gain_l = 0.0;
    E32_B.BackmotorSaturation = 0.0;
    E32_B.FrontmotorSaturation = 0.0;
  }

  /* parameters */
  E32_M->ModelData.defaultParam = ((real_T *)&E32_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &E32_X;
    E32_M->ModelData.contStates = (x);
    (void) memset((void *)&E32_X, 0,
                  sizeof(X_E32_T));
  }

  /* states (dwork) */
  E32_M->ModelData.dwork = ((void *) &E32_DW);
  (void) memset((void *)&E32_DW, 0,
                sizeof(DW_E32_T));

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 8; i++) {
      E32_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  E32_DW.TimeStampA = 0.0;
  E32_DW.LastUAtTimeA = 0.0;
  E32_DW.TimeStampB = 0.0;
  E32_DW.LastUAtTimeB = 0.0;
  E32_DW.HILWriteAnalog_Buffer[0] = 0.0;
  E32_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    E32_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.B = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.P = &rtPTransTable;
  }

  /* Initialize Sizes */
  E32_M->Sizes.numContStates = (4);    /* Number of continuous states */
  E32_M->Sizes.numY = (0);             /* Number of model outputs */
  E32_M->Sizes.numU = (0);             /* Number of model inputs */
  E32_M->Sizes.sysDirFeedThru = (0);   /* The model is not direct feedthrough */
  E32_M->Sizes.numSampTimes = (2);     /* Number of sample times */
  E32_M->Sizes.numBlocks = (67);       /* Number of blocks */
  E32_M->Sizes.numBlockIO = (22);      /* Number of block outputs */
  E32_M->Sizes.numBlockPrms = (146);   /* Sum of parameter "widths" */
  return E32_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/

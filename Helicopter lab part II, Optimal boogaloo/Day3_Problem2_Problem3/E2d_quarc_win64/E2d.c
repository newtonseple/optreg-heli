/*
 * E2d.c
 *
 * Code generation for model "E2d".
 *
 * Model version              : 1.172
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Tue Feb 28 12:55:27 2017
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
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

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

    /* Gain: '<S4>/Travel: Count to rad' */
    E2d_B.TravelCounttorad = E2d_P.TravelCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o1;

    /* Gain: '<S11>/Gain' */
    E2d_B.Gain = E2d_P.Gain_Gain * E2d_B.TravelCounttorad;

    /* Sum: '<Root>/Sum4' incorporates:
     *  Constant: '<Root>/travel_offset [deg]'
     */
    E2d_B.Sum4 = E2d_P.travel_offsetdeg_Value + E2d_B.Gain;
  }

  /* TransferFcn: '<S4>/Travel: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.TravelTransferFcn_C * E2d_X.TravelTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.TravelTransferFcn_D * E2d_B.TravelCounttorad;

  /* Gain: '<S12>/Gain' */
  E2d_B.Gain_d = E2d_P.Gain_Gain_l * rtb_Backgain;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* Gain: '<S4>/Pitch: Count to rad' */
    E2d_B.PitchCounttorad = E2d_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S8>/Gain' */
    E2d_B.Gain_i = E2d_P.Gain_Gain_a * E2d_B.PitchCounttorad;
  }

  /* TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.PitchTransferFcn_C * E2d_X.PitchTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.PitchTransferFcn_D * E2d_B.PitchCounttorad;

  /* Gain: '<S9>/Gain' */
  E2d_B.Gain_b = E2d_P.Gain_Gain_ae * rtb_Backgain;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' */
    E2d_B.ElevationCounttorad = E2d_P.ElevationCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o3;

    /* Gain: '<S6>/Gain' */
    E2d_B.Gain_e = E2d_P.Gain_Gain_lv * E2d_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    E2d_B.Sum = E2d_B.Gain_e + E2d_P.elavation_offsetdeg_Value;
  }

  /* TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  rtb_Backgain = 0.0;
  rtb_Backgain += E2d_P.ElevationTransferFcn_C *
    E2d_X.ElevationTransferFcn_CSTATE;
  rtb_Backgain += E2d_P.ElevationTransferFcn_D * E2d_B.ElevationCounttorad;

  /* Gain: '<S7>/Gain' */
  E2d_B.Gain_dg = E2d_P.Gain_Gain_n * rtb_Backgain;

  /* Gain: '<S2>/Gain1' */
  E2d_B.Gain1[0] = E2d_P.Gain1_Gain * E2d_B.Sum4;
  E2d_B.Gain1[1] = E2d_P.Gain1_Gain * E2d_B.Gain_d;
  E2d_B.Gain1[2] = E2d_P.Gain1_Gain * E2d_B.Gain_i;
  E2d_B.Gain1[3] = E2d_P.Gain1_Gain * E2d_B.Gain_b;
  E2d_B.Gain1[4] = E2d_P.Gain1_Gain * E2d_B.Sum;
  E2d_B.Gain1[5] = E2d_P.Gain1_Gain * E2d_B.Gain_dg;

  /* FromWorkspace: '<Root>/From Workspace1' */
  {
    real_T *pDataValues = (real_T *) E2d_DW.FromWorkspace1_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) E2d_DW.FromWorkspace1_PWORK.TimePtr;
    int_T currTimeIndex = E2d_DW.FromWorkspace1_IWORK.PrevIndex;
    real_T t = E2d_M->Timing.t[0];
    if (t >= pTimeValues[100]) {
      {
        int_T elIdx;
        for (elIdx = 0; elIdx < 4; ++elIdx) {
          (&E2d_B.FromWorkspace1[0])[elIdx] = pDataValues[100];
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

      E2d_DW.FromWorkspace1_IWORK.PrevIndex = currTimeIndex;

      /* Post output */
      {
        real_T t1 = pTimeValues[currTimeIndex];
        real_T t2 = pTimeValues[currTimeIndex + 1];
        if (t1 == t2) {
          if (t < t1) {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 4; ++elIdx) {
                (&E2d_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex];
                pDataValues += 101;
              }
            }
          } else {
            {
              int_T elIdx;
              for (elIdx = 0; elIdx < 4; ++elIdx) {
                (&E2d_B.FromWorkspace1[0])[elIdx] = pDataValues[currTimeIndex +
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
              (&E2d_B.FromWorkspace1[0])[elIdx] = (real_T) rtInterpolate(d1, d2,
                f1, f2);
              pDataValues += 101;
            }
          }
        }
      }
    }
  }

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

  /* Sum: '<Root>/Sum1' */
  E2d_B.Sum1 = E2d_B.FromWorkspace - 0.0;
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* SignalConversion: '<Root>/TmpSignal ConversionAtTo FileInport1' */
    E2d_B.TmpSignalConversionAtToFileInpo[0] = E2d_B.FromWorkspace1[0];
    E2d_B.TmpSignalConversionAtToFileInpo[1] = E2d_B.FromWorkspace1[1];
    E2d_B.TmpSignalConversionAtToFileInpo[2] = E2d_B.FromWorkspace1[2];
    E2d_B.TmpSignalConversionAtToFileInpo[3] = E2d_B.FromWorkspace1[3];
    E2d_B.TmpSignalConversionAtToFileInpo[4] = E2d_B.FromWorkspace;
    E2d_B.TmpSignalConversionAtToFileInpo[5] = E2d_B.Gain1[0];
    E2d_B.TmpSignalConversionAtToFileInpo[6] = E2d_B.Gain1[1];
    E2d_B.TmpSignalConversionAtToFileInpo[7] = E2d_B.Gain1[2];
    E2d_B.TmpSignalConversionAtToFileInpo[8] = E2d_B.Gain1[3];
    E2d_B.TmpSignalConversionAtToFileInpo[9] = E2d_B.Sum1;

    /* ToFile: '<Root>/To File' */
    {
      if (!(++E2d_DW.ToFile_IWORK.Decimation % 1) && (E2d_DW.ToFile_IWORK.Count*
           11)+1 < 100000000 ) {
        FILE *fp = (FILE *) E2d_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[11];
          E2d_DW.ToFile_IWORK.Decimation = 0;
          u[0] = E2d_M->Timing.t[1];
          u[1] = E2d_B.TmpSignalConversionAtToFileInpo[0];
          u[2] = E2d_B.TmpSignalConversionAtToFileInpo[1];
          u[3] = E2d_B.TmpSignalConversionAtToFileInpo[2];
          u[4] = E2d_B.TmpSignalConversionAtToFileInpo[3];
          u[5] = E2d_B.TmpSignalConversionAtToFileInpo[4];
          u[6] = E2d_B.TmpSignalConversionAtToFileInpo[5];
          u[7] = E2d_B.TmpSignalConversionAtToFileInpo[6];
          u[8] = E2d_B.TmpSignalConversionAtToFileInpo[7];
          u[9] = E2d_B.TmpSignalConversionAtToFileInpo[8];
          u[10] = E2d_B.TmpSignalConversionAtToFileInpo[9];
          if (fwrite(u, sizeof(real_T), 11, fp) != 11) {
            rtmSetErrorStatus(E2d_M, "Error writing to MAT-file o3_Q01_out.mat");
            return;
          }

          if (((++E2d_DW.ToFile_IWORK.Count)*11)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file o3_Q01_out.mat.\n");
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
  E2d_B.Sum_k = ((E2d_B.Sum1 - E2d_B.Gain1[2]) * E2d_P.K_pp - E2d_P.K_pd *
                 E2d_B.Gain1[3]) + E2d_P.Vd_ff;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Integrator: '<S3>/Integrator'
   *
   * Regarding '<S3>/Integrator':
   *  Limited Integrator
   */
  if (E2d_X.Integrator_CSTATE >= E2d_P.Integrator_UpperSat ) {
    E2d_X.Integrator_CSTATE = E2d_P.Integrator_UpperSat;
  } else if (E2d_X.Integrator_CSTATE <= (E2d_P.Integrator_LowerSat) ) {
    E2d_X.Integrator_CSTATE = (E2d_P.Integrator_LowerSat);
  }

  rtb_Backgain = E2d_X.Integrator_CSTATE;

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   */
  rtb_Derivative = E2d_P.elevation_ref_Value - E2d_B.Gain1[4];

  /* Sum: '<S3>/Sum2' incorporates:
   *  Constant: '<S3>/Vs_bias'
   *  Gain: '<S3>/K_ed'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  E2d_B.Sum2 = ((E2d_P.K_ep * rtb_Derivative + rtb_Backgain) - E2d_P.K_ed *
                E2d_B.Gain1[5]) + E2d_P.Vs_ff;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (E2d_B.Sum2 - E2d_B.Sum_k) * E2d_P.Backgain_Gain;

  /* Gain: '<S3>/K_ei' */
  E2d_B.K_ei = E2d_P.K_ei * rtb_Derivative;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
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

  /* End of Derivative: '<S4>/Derivative' */

  /* Gain: '<S10>/Gain' */
  E2d_B.Gain_l = E2d_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > E2d_P.BackmotorSaturation_UpperSat) {
    E2d_B.BackmotorSaturation = E2d_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < E2d_P.BackmotorSaturation_LowerSat) {
    E2d_B.BackmotorSaturation = E2d_P.BackmotorSaturation_LowerSat;
  } else {
    E2d_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(E2d_M)) {
  }

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Derivative = (E2d_B.Sum_k + E2d_B.Sum2) * E2d_P.Frontgain_Gain;

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Derivative > E2d_P.FrontmotorSaturation_UpperSat) {
    E2d_B.FrontmotorSaturation = E2d_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Derivative < E2d_P.FrontmotorSaturation_LowerSat) {
    E2d_B.FrontmotorSaturation = E2d_P.FrontmotorSaturation_LowerSat;
  } else {
    E2d_B.FrontmotorSaturation = rtb_Derivative;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(E2d_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

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
  }
}

/* Model update function */
void E2d_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
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

  /* End of Update for Derivative: '<S4>/Derivative' */
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

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += E2d_P.TravelTransferFcn_A *
    E2d_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += E2d_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += E2d_P.PitchTransferFcn_A *
    E2d_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += E2d_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += E2d_P.ElevationTransferFcn_A *
    E2d_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += E2d_B.ElevationCounttorad;

  /* Derivatives for Integrator: '<S3>/Integrator' */
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

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

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
      3.1415926535897931, 3.1378421244928791, 3.1262155372552249,
      3.1033092853388951, 3.066627403238944, 3.0144539148522034,
      2.9456562762271612, 2.859507772142782, 2.7555516108187761,
      2.6335051530889912, 2.4931956758185327, 2.33451868238738, 2.15741147779644,
      1.961836714722041, 1.751194813461691, 1.5333159850301696,
      1.3157848458922028, 1.1045758379674173, 0.90396650183202987,
      0.71685486372635565, 0.54513479636074036, 0.39001290640785741,
      0.25224356470192494, 0.13229158057657525, 0.030440041839026186,
      -0.053140266241372032, -0.11834709923984459, -0.1651196735988435,
      -0.1934221111123148, -0.20323325739795225, -0.194541004948583,
      -0.16733811751745878, -0.12780535326285716, -0.084057831790902857,
      -0.043152381305306228, -0.010395688317916924, 0.010511850376226107,
      0.017103981559829239, 0.012807023449454809, 0.0044744981911832857,
      -0.0015210590178536178, -0.0021622529212779826, -0.00046228628168167507,
      0.00041850398572202644, 0.00023549643681915574, -5.7414157268506638E-5,
      -7.563872998988619E-5, 2.4937265357580739E-6, 2.4442094508944081E-5,
      4.3871961953326044E-6, -7.69478448012423E-6, -3.1847677285789066E-6,
      2.8034550394372964E-6, 2.6520166452144674E-6, -3.3060528026798973E-7,
      -1.7283992320510481E-6, -7.645757857369411E-7, 1.5221041489157947E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -0.015002040209024481, -0.046506340987446861, -0.091624999702148976,
      -0.14672752043663534, -0.20869394558379092, -0.27519054653699909,
      -0.3445940083743475, -0.41582463733285346, -0.48818582295596985,
      -0.56123790111866445, -0.6347079657614414, -0.70842881040059125,
      -0.78229904433442765, -0.84256759707822937, -0.871515305762916,
      -0.87012454858869814, -0.84483602373597255, -0.80243733657838046,
      -0.7484465444595273, -0.68688026149929149, -0.620487551848362,
      -0.55107735886056042, -0.47980792853822912, -0.40740614698702665,
      -0.33432122435842326, -0.26082732403072062, -0.18709028947282605,
      -0.11320974209071566, -0.039244577179380222, 0.034769017760646587,
      0.10881155768766659, 0.15813106498157598, 0.17499009385098682,
      0.16362180990555608, 0.13102677991272682, 0.08363016273974172,
      0.026368532697582121, -0.017187824478328128, -0.0333300930699165,
      -0.023982220872978022, -0.0025647676505278641, 0.0067998745215548245,
      0.0035231690327844002, -0.00073202223244188836, -0.0011716344131810553,
      -7.2890327715924E-5, 0.00031253778927217138, 8.7801435062338377E-5,
      -8.02116300848516E-5, -4.831995953223303E-5, 1.8048030175775643E-5,
      2.3960854241659147E-5, -5.9779040729699437E-7, -1.1922524532335483E-5,
      -5.5832126375378915E-6, 3.8632569548507686E-6, 9.15468290820529E-6,
      1.193841594584312E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602874726765964, 0.22266036609523307, 0.31888144834960575,
      0.38944357203683372, 0.43795502694118971, 0.46997257935909453,
      0.4905171618905651, 0.50343088126537894, 0.5114212239850171,
      0.51630418858686156, 0.51925835738934556, 0.52103077462500946,
      0.52208660110461569, 0.42595511325115876, 0.20459134959856212,
      -0.0098293401279802486, -0.17872963496758607, -0.29965772737031343,
      -0.3815862979425953, -0.43512697395372768, -0.46923831447918679,
      -0.49056473425728436, -0.50370511365185389, -0.51170813966630169,
      -0.51653631989393944, -0.51942681807309865, -0.52114519795349035,
      -0.52215949178416932, -0.52275753616333376, -0.52309982110957254,
      -0.52330439322287814, -0.3485714408719775, -0.11915317677766217,
      0.0803466895317856, 0.23036922360786066, 0.33498118872754862,
      0.40470333200951586, 0.3078396977526397, 0.11408738971529854,
      -0.066067190101676776, -0.15137037913689352, -0.066185713881153352,
      0.023158503361709271, 0.030074067123272971, 0.0031070114123208439,
      -0.0077655031432514975, -0.0027240583099785295, 0.0015883511364764178,
      0.0011874525933221617, -0.00022539773034620984, -0.00046906318683010806,
      -4.1789165302346331E-5, 0.00017357153781306803, 8.0039312567574644E-5,
      -4.4803418391511592E-5, -6.6763622302450765E-5, -3.7397360769351873E-5,
      -1.9674361937773306E-5, -9.9203533384135137E-6, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411498774056616, 0.4665264832734633,
      0.38488433698066038, 0.28224850271208152, 0.19404582758059352,
      0.12807021763478882, 0.082178338089051881, 0.051654885462425221,
      0.0319613788417223, 0.019531866370547706, 0.01181668317310525,
      0.0070896769058250908, 0.0042233138815946239, -0.38452594345065794,
      -0.88545504664721686, -0.85768275094299984, -0.67560117139525366,
      -0.48371236164773984, -0.32771427432595784, -0.21416269608135982,
      -0.13644535413866687, -0.085305671149220774, -0.052561509615108352,
      -0.032012096094621544, -0.019312712947381547, -0.011561984753467298,
      -0.0068735115583970522, -0.0040571673595462812, -0.00239216955348841,
      -0.0013691318217854171, -0.0008182804900530306, 0.69893181736677223,
      0.91767306434043094, 0.79799947320096065, 0.60009014426746987,
      0.41844786844192144, 0.27888858109103837, -0.38745452906433486,
      -0.77500922418619511, -0.72061831130473164, -0.34121274817769742,
      0.34073866898613037, 0.35737687693462006, 0.027662263009424412,
      -0.10786821488063893, -0.043490050259119784, 0.020165787296261471,
      0.017249645748989382, -0.0016035862094474305, -0.0056513933315038914,
      -0.00097465386276599907, 0.0017091040492806416, 0.00086145077563125165,
      -0.00037412093781237911, -0.00049936296066675052, -8.7832852474162369E-5,
      0.00011747300930198994, 7.0899958495908579E-5, 3.9023997567033531E-5,
      1.9838632926299731E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E2d_DW.FromWorkspace1_PWORK.TimePtr = (void *) pTimeValues0;
    E2d_DW.FromWorkspace1_PWORK.DataPtr = (void *) pDataValues0;
    E2d_DW.FromWorkspace1_IWORK.PrevIndex = 0;
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359875029699521,
      0.52359873692313585, 0.52359872441143918, 0.52359871405469338,
      0.52359869543915227, 0.52359866797495325, 0.52359861849516542,
      0.523598560206832, 0.5235984990864011, 0.523598438449975,
      0.52359837135439713, 0.52359801750249357, 0.52359704754262892,
      0.04578598202541681, -0.5235954476225495, -0.52359705149591629,
      -0.523597420114779, -0.52359755207250747, -0.52359759216269286,
      -0.52359757405626, -0.52359752226035272, -0.52359752074657551,
      -0.52359751159898793, -0.52359677413378225, -0.523595867361449,
      -0.52359791821640267, -0.52359473995554806, -0.5235870938392212,
      -0.52359761768730073, -0.52355445725736582, -0.52359873921855238,
      0.33987997919774221, 0.52333742830416019, 0.523320114189831,
      0.52318039085261092, 0.522863784406257, 0.52301587432390206,
      -0.17778843391478627, -0.50426429570235354, -0.48613239613990938,
      -0.21819770190701868, 0.396722855129246, 0.2477691305781119,
      -0.07615538283286967, -0.11342723534006108, -0.010300298232169644,
      0.0333721782707416, 0.011040743588243068, -0.0068333891139279179,
      -0.0051907129097109681, 0.00068187025200334007, 0.0020049260581779947,
      0.000383446941380451, -0.00061002766188029587, -0.00039674702235748438,
      3.3244592493349612E-5, 0.00011105970222178622, 6.2555020346784709E-6,
      2.0184847427564921E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E2d_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    E2d_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    E2d_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    char fileName[509] = "o3_Q01_out.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(E2d_M, "Error creating .mat file o3_Q01_out.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,11,0,
         "oppg3Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
      rtmSetErrorStatus(E2d_M,
                        "Error writing mat file header to file o3_Q01_out.mat");
      return;
    }

    E2d_DW.ToFile_IWORK.Count = 0;
    E2d_DW.ToFile_IWORK.Decimation = -1;
    E2d_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  E2d_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  E2d_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  E2d_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  E2d_X.Integrator_CSTATE = E2d_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  E2d_DW.TimeStampA = (rtInf);
  E2d_DW.TimeStampB = (rtInf);
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

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) E2d_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "o3_Q01_out.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E2d_M, "Error closing MAT-file o3_Q01_out.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(E2d_M, "Error reopening MAT-file o3_Q01_out.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 11, E2d_DW.ToFile_IWORK.Count,
           "oppg3Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
        rtmSetErrorStatus(E2d_M,
                          "Error writing header for oppg3Q01_x1o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out to MAT-file o3_Q01_out.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E2d_M, "Error closing MAT-file o3_Q01_out.mat");
        return;
      }

      E2d_DW.ToFile_PWORK.FilePtr = (NULL);
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
  E2d_M->Sizes.checksums[0] = (1644053438U);
  E2d_M->Sizes.checksums[1] = (1878177205U);
  E2d_M->Sizes.checksums[2] = (3655643196U);
  E2d_M->Sizes.checksums[3] = (638514708U);

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

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      E2d_B.Gain1[i] = 0.0;
    }

    for (i = 0; i < 10; i++) {
      E2d_B.TmpSignalConversionAtToFileInpo[i] = 0.0;
    }

    E2d_B.TravelCounttorad = 0.0;
    E2d_B.Gain = 0.0;
    E2d_B.Sum4 = 0.0;
    E2d_B.Gain_d = 0.0;
    E2d_B.PitchCounttorad = 0.0;
    E2d_B.Gain_i = 0.0;
    E2d_B.Gain_b = 0.0;
    E2d_B.ElevationCounttorad = 0.0;
    E2d_B.Gain_e = 0.0;
    E2d_B.Sum = 0.0;
    E2d_B.Gain_dg = 0.0;
    E2d_B.FromWorkspace1[0] = 0.0;
    E2d_B.FromWorkspace1[1] = 0.0;
    E2d_B.FromWorkspace1[2] = 0.0;
    E2d_B.FromWorkspace1[3] = 0.0;
    E2d_B.FromWorkspace = 0.0;
    E2d_B.Sum1 = 0.0;
    E2d_B.Sum_k = 0.0;
    E2d_B.Sum2 = 0.0;
    E2d_B.K_ei = 0.0;
    E2d_B.Gain_l = 0.0;
    E2d_B.BackmotorSaturation = 0.0;
    E2d_B.FrontmotorSaturation = 0.0;
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
  E2d_M->Sizes.numBlocks = (65);       /* Number of blocks */
  E2d_M->Sizes.numBlockIO = (22);      /* Number of block outputs */
  E2d_M->Sizes.numBlockPrms = (142);   /* Sum of parameter "widths" */
  return E2d_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/

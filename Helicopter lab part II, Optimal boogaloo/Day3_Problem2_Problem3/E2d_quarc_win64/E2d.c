/*
 * E2d.c
 *
 * Code generation for model "E2d".
 *
 * Model version              : 1.177
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Tue Mar 14 12:02:06 2017
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
            rtmSetErrorStatus(E2d_M, "Error writing to MAT-file o2_Q10_out.mat");
            return;
          }

          if (((++E2d_DW.ToFile_IWORK.Count)*11)+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file o2_Q10_out.mat.\n");
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
      3.1415926535897931, 3.1378421413615754, 3.1262155534570493,
      3.1033093000290193, 3.0666274151902377, 3.014453922393344,
      2.94596191845575, 2.8614638334598128, 2.7621883174032629, 2.64996194020731,
      2.5269296317567673, 2.3953467285769516, 2.2574382158417352,
      2.1153103203800194, 1.9708994141298755, 1.8259461320331625,
      1.6819859114895255, 1.5403499080501386, 1.402172269884099,
      1.2684011575495706, 1.1398118307763707, 1.0170207318181297,
      0.90049988420219551, 0.79059117320527439, 0.687520231717906,
      0.59140975587480638, 0.50229214022408819, 0.42012136559107044,
      0.34478410237177864, 0.27611001279325648, 0.21388125076710868,
      0.157841169209555, 0.1077022532549082, 0.063153304366598481,
      0.023865905409100434, -0.010499799410390155, -0.04028997297816389,
      -0.065852563338510042, -0.087533351365261677, -0.10567252737481911,
      -0.12060175591594398, -0.13264168834762141, -0.14209988360845593,
      -0.14926909874179919, -0.15442591223606378, -0.15782964505336972,
      -0.1597215463611858, -0.160324213497931, -0.15984121869784584,
      -0.15845691875998386, -0.15633642848041623, -0.15362574475572258,
      -0.15045201650808185, -0.14692396690967968, -0.14313248985726232,
      -0.13915146302539871, -0.13503884434763302, -0.13083814214510991, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -0.015002048908628312, -0.04650635161766116, -0.091625013711676,
      -0.14672753935468319, -0.20869397118713179, -0.27396801574993085,
      -0.337992339983306, -0.39710206422575534, -0.4489055087833691,
      -0.49212923380172646, -0.52633161271882067, -0.55163405094042084,
      -0.56851158184642092, -0.57764362500013222, -0.57981312838640886,
      -0.5758408821741039, -0.56654401375710406, -0.552710552663715,
      -0.53508444933766974, -0.51435730709235561, -0.49116439583252119,
      -0.46608339046329317, -0.439634843987241, -0.41228376594902966,
      -0.38444190337195505, -0.35647046260242904, -0.3286830985316278,
      -0.30134905287672359, -0.27469635831364531, -0.2489150481041478,
      -0.22416032622977125, -0.20055566381814369, -0.17819579555279541,
      -0.15714959582954874, -0.13746281927751888, -0.11916069427065148,
      -0.10225036144094111, -0.086723152106563076, -0.072556704037786232,
      -0.059716914164056049, -0.048159729726266282, -0.03783278104289458,
      -0.028676860532929588, -0.020627253976614787, -0.0136149312687804,
      -0.007567605230820903, -0.0024106685465372884, 0.00193197920078409,
      0.0055371997518914324, 0.0084819611187139533, 0.010842734899218078,
      0.012694912991006377, 0.01411219839405212, 0.015165908210112976,
      0.015924107327897927, 0.016450474711506159, 0.016802808810535964,
      0.017031150185923962, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875205870245, 0.22266037932312319, 0.31888147181637,
      0.38944360631145691, 0.43795507377336035, 0.46133201729741352,
      0.45249947130624274, 0.41776495557017096, 0.36612696119552673,
      0.30548878028606913, 0.24172935150378116, 0.17882796976192719,
      0.11928394252289179, 0.064541793271241779, 0.015333221350534356,
      -0.028074319135533373, -0.065706715281525066, -0.097769619688217055,
      -0.12457456649054512, -0.14649152521320574, -0.16391863887342462,
      -0.17726296693165972, -0.18692822517900651, -0.19330697356259841,
      -0.196775651241279, -0.19769146041660662, -0.19639047661318298,
      -0.19318659518244624, -0.1883710658892154, -0.18221245408001349,
      -0.17495691980528838, -0.16682873875697754, -0.15803100914684892,
      -0.14874650161177794, -0.13913861783287546, -0.12935242954222687,
      -0.1195157739918135, -0.10974038537376653, -0.10012304445480469,
      -0.090746731013768, -0.081681765649323446, -0.072986929192681932,
      -0.06471054930587955, -0.056891544808453785, -0.049560418730168865,
      -0.0427401908221166, -0.036447258930693456, -0.030692175720087096,
      -0.025480321937806451, -0.020812448667971491, -0.016685047446285232,
      -0.013090487362938587, -0.010016831934516857, -0.0074472185436620791,
      -0.00535866179064172, -0.0037201636354002496, -0.0024901628473501911,
      -0.0016138296323718504, -0.0010219383342808684, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.42411500823516041, 0.46652650905812648,
      0.38488436997343078, 0.28224853798079091, 0.19404586984805708,
      0.093507774096656254, -0.0353301839642395, -0.1389380629438437,
      -0.20655197749813364, -0.24255272363738672, -0.25503771512870843,
      -0.25160552696697241, -0.23817610895569818, -0.21896859700615651,
      -0.19683428768238623, -0.17363016194382744, -0.15052958458352328,
      -0.1282516176263245, -0.10721978720886879, -0.087667834890199067,
      -0.069708454640432016, -0.053377312232496824, -0.038661032988943744,
      -0.025514993533924047, -0.013874710714278976, -0.0036632367008670387,
      0.0052039352141379979, 0.012815525723390394, 0.019262117173366889,
      0.024634447237251087, 0.02902213709934397, 0.032512724193686907,
      0.0351909184409579, 0.037138030140727356, 0.038431535116053413,
      0.039144753163037838, 0.039346622202096976, 0.03910155447263132,
      0.038469363676290877, 0.037505253764590192, 0.036259861458221646,
      0.03477934582700952, 0.033105519547653041, 0.031276017990146496,
      0.029324504313583171, 0.027280911632652505, 0.025171727566136053,
      0.023020332842868908, 0.02084741512956605, 0.018671493079783309,
      0.016509604887188504, 0.014378240333830046, 0.012294621714130385,
      0.010278453563862578, 0.0083542270125249035, 0.0065539926214093491,
      0.0049200031526437007, 0.0035053328603568304, 0.0023675651928073945,
      0.0015439585215019382, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
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
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.52359877559843915,
      0.523598775597815, 0.523598775598479, 0.523598775598492,
      0.52359877558128221, 0.480929022498434, 0.38279339306434268,
      0.29416529563343963, 0.2146498772270472, 0.14381739274551367,
      0.081210365188898445, 0.026350810589154745, -0.021252816717700355,
      -0.062099173584968859, -0.096688190788428829, -0.12551590200775567,
      -0.14906994950074914, -0.16782575020759671, -0.18224328990700092,
      -0.19276450384835445, -0.19981119754216395, -0.20378345949574,
      -0.20505851759699648, -0.20398999191367054, -0.20090749846454442,
      -0.19611656075992956, -0.18989878842174129, -0.18251228485910356,
      -0.1741922487111722, -0.16515173651637802, -0.15558255678400587,
      -0.14565626829809569, -0.13552525805073271, -0.1253238766628634,
      -0.11516961149057538, -0.10516427982037871, -0.095395226616339576,
      -0.085936513181705268, -0.076850084820667033, -0.068186907106406056,
      -0.059988061638306521, -0.052285793136251653, -0.045104500260986446,
      -0.038461662482827522, -0.032368694350114857, -0.026831716165373453,
      -0.021852225640697654, -0.017427647506244769, -0.013551725788866254,
      -0.010214704650423411, -0.0074032162849533021, -0.0050997575016602887,
      -0.0032815935199293528, -0.0019188930517652304, -0.00097191631014707422,
      -0.00038725567976363625, -9.371871643968957E-5, 2.3626763836856066E-14,
      2.3378284728842427E-14, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    E2d_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    E2d_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    E2d_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    char fileName[509] = "o2_Q10_out.mat";
    FILE *fp = (NULL);
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(E2d_M, "Error creating .mat file o2_Q10_out.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp,11,0,
         "oppg2Q10_x10o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
      rtmSetErrorStatus(E2d_M,
                        "Error writing mat file header to file o2_Q10_out.mat");
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
      char fileName[509] = "o2_Q10_out.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E2d_M, "Error closing MAT-file o2_Q10_out.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(E2d_M, "Error reopening MAT-file o2_Q10_out.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 11, E2d_DW.ToFile_IWORK.Count,
           "oppg2Q10_x10o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out")) {
        rtmSetErrorStatus(E2d_M,
                          "Error writing header for oppg2Q10_x10o_x2o_x3o_x4o_u1o_x1_x2_x3_x4_u1_out to MAT-file o2_Q10_out.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(E2d_M, "Error closing MAT-file o2_Q10_out.mat");
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
  E2d_M->Sizes.checksums[0] = (566854785U);
  E2d_M->Sizes.checksums[1] = (1653391276U);
  E2d_M->Sizes.checksums[2] = (2151003284U);
  E2d_M->Sizes.checksums[3] = (1367028721U);

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

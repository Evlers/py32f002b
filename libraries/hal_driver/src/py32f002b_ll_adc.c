/**
  ******************************************************************************
  * @file    py32f002b_ll_adc.c
  * @author  MCU Application Team
  * @brief   ADC LL module driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "py32f002b_ll_adc.h"
#include "py32f002b_ll_bus.h"


#ifdef  USE_FULL_ASSERT
#include "py32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup PY32F002B_LL_Driver
  * @{
  */

#if defined (ADC1)

/** @addtogroup ADC_LL ADC
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup ADC_LL_Private_Constants
  * @{
  */

/* Definitions of ADC hardware constraints delays */
/* Note: Only ADC IP HW delays are defined in ADC LL driver driver,           */
/*       not timeout values:                                                  */
/*       Timeout values for ADC operations are dependent to device clock      */
/*       configuration (system clock versus ADC clock),                       */
/*       and therefore must be defined in user application.                   */
/*       Refer to @ref ADC_LL_EC_HW_DELAYS for description of ADC timeout     */
/*       values definition.                                                   */
/* Unit: CPU cycles.                                                          */
#define ADC_CLOCK_RATIO_VS_CPU_HIGHEST          ((uint32_t) 512U * 16U * 4U)
#define ADC_TIMEOUT_DISABLE_CPU_CYCLES          (ADC_CLOCK_RATIO_VS_CPU_HIGHEST * 1U)
#define ADC_TIMEOUT_STOP_CONVERSION_CPU_CYCLES  (ADC_CLOCK_RATIO_VS_CPU_HIGHEST * 1U)

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/** @addtogroup ADC_LL_Private_Macros ADC Private Macros
  * @{
  */

/* Check of parameters for configuration of ADC hierarchical scope:           */
/* common to several ADC instances.                                           */
/* Check of parameters for configuration of ADC hierarchical scope:           */
/* ADC instance.                                                              */
#define IS_LL_ADC_CLOCK(__CLOCK__)                                             \
  (   ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV64)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV32)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV16)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV8)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV4)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV2)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_SYNC_PCLK_DIV1)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV64)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV32)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV16)                            \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV8)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV4)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV2)                             \
   || ((__CLOCK__) == LL_ADC_CLOCK_ASYNC_HSI_DIV1)                             \
  )

#define IS_LL_ADC_RESOLUTION(__RESOLUTION__)                                   \
  (   ((__RESOLUTION__) == LL_ADC_RESOLUTION_12B)                              \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_10B)                              \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_8B)                               \
   || ((__RESOLUTION__) == LL_ADC_RESOLUTION_6B)                               \
  )

#define IS_LL_ADC_DATA_ALIGN(__DATA_ALIGN__)                                   \
  (   ((__DATA_ALIGN__) == LL_ADC_DATA_ALIGN_RIGHT)                            \
   || ((__DATA_ALIGN__) == LL_ADC_DATA_ALIGN_LEFT)                             \
  )

#define IS_LL_ADC_LOW_POWER(__LOW_POWER__)                                     \
  (   ((__LOW_POWER__) == LL_ADC_LP_MODE_NONE)                                 \
   || ((__LOW_POWER__) == LL_ADC_LP_AUTOWAIT)                                  \
  )

/* Check of parameters for configuration of ADC hierarchical scope:           */
/* ADC group regular                                                          */
#define IS_LL_ADC_REG_TRIG_SOURCE(__REG_TRIG_SOURCE__)                         \
  (   ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_SOFTWARE)                      \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM1_TRGO)                 \
   || ((__REG_TRIG_SOURCE__) == LL_ADC_REG_TRIG_EXT_TIM1_CH4)                  \
  )

#define IS_LL_ADC_REG_CONTINUOUS_MODE(__REG_CONTINUOUS_MODE__)                 \
  (   ((__REG_CONTINUOUS_MODE__) == LL_ADC_REG_CONV_SINGLE)                    \
   || ((__REG_CONTINUOUS_MODE__) == LL_ADC_REG_CONV_CONTINUOUS)                \
  )

#define IS_LL_ADC_REG_OVR_DATA_BEHAVIOR(__REG_OVR_DATA_BEHAVIOR__)             \
  (   ((__REG_OVR_DATA_BEHAVIOR__) == LL_ADC_REG_OVR_DATA_PRESERVED)           \
   || ((__REG_OVR_DATA_BEHAVIOR__) == LL_ADC_REG_OVR_DATA_OVERWRITTEN)         \
  )

#define IS_LL_ADC_REG_SEQ_SCAN_DISCONT_MODE(__REG_SEQ_DISCONT_MODE__)          \
  (   ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_DISABLE)           \
   || ((__REG_SEQ_DISCONT_MODE__) == LL_ADC_REG_SEQ_DISCONT_1RANK)             \
  )

/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup ADC_LL_Exported_Functions ADC Exported Functions
  * @{
  */

/** @addtogroup ADC_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize registers of all ADC instances belonging to
  *         the same ADC common instance to their default reset values.
  * @note   This function is performing a hard reset, using high level
  *         clock source RCC ADC reset.
  * @param  ADCxy_COMMON ADC common instance
  *         (can be set directly from CMSIS definition or by using helper macro @ref __LL_ADC_COMMON_INSTANCE() )
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ADC common registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_ADC_CommonDeInit(ADC_Common_TypeDef *ADCxy_COMMON)
{
  /* Check the parameters */
  assert_param(IS_ADC_COMMON_INSTANCE(ADCxy_COMMON));

  /* Force reset of ADC clock (core clock) */
  LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_ADC1);

  /* Release reset of ADC clock (core clock) */
  LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_ADC1);

  return SUCCESS;
}

/**
  * @brief  De-initialize registers of the selected ADC instance
  *         to their default reset values.
  * @note   To reset all ADC instances quickly (perform a hard reset),
  *         use function @ref LL_ADC_CommonDeInit().
  * @note   If this functions returns error status, it means that ADC instance
  *         is in an unknown state.
  *         In this case, perform a hard reset using high level
  *         clock source RCC ADC reset.
  *         Refer to function @ref LL_ADC_CommonDeInit().
  * @param  ADCx ADC instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ADC registers are de-initialized
  *          - ERROR: ADC registers are not de-initialized  
  */
ErrorStatus LL_ADC_DeInit(ADC_TypeDef *ADCx)
{
  ErrorStatus status = SUCCESS;

  __IO uint32_t timeout_cpu_cycles = 0U;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));

  /* Disable ADC instance if not already disabled.                            */
  if(LL_ADC_IsEnabled(ADCx) == 1U)
  {
     /* Set ADC group regular trigger source to SW start to ensure to not      */
    /* have an external trigger event occurring during the conversion stop    */
    /* ADC disable process.                                                   */
    LL_ADC_REG_SetTriggerSource(ADCx, LL_ADC_REG_TRIG_SOFTWARE);
    
    /* Stop potential ADC conversion on going on ADC group regular.           */
    if(LL_ADC_REG_IsConversionOngoing(ADCx) != 0U)
    {
      if(LL_ADC_REG_IsStopConversionOngoing(ADCx) == 0U)
      {
        LL_ADC_REG_StopConversion(ADCx);
      }
    }
    
    /* Wait for ADC conversions are effectively stopped                       */
    timeout_cpu_cycles = ADC_TIMEOUT_STOP_CONVERSION_CPU_CYCLES;
    while (LL_ADC_REG_IsStopConversionOngoing(ADCx) == 1U)
    {
      if(timeout_cpu_cycles-- == 0U)
      {
        /* Time-out error */
        status = ERROR;
      }
    }
    
    /* Disable the ADC instance */
    LL_ADC_Disable(ADCx);
    
    /* Wait for ADC instance is effectively disabled */
    timeout_cpu_cycles = ADC_TIMEOUT_DISABLE_CPU_CYCLES;
    while (LL_ADC_IsDisableOngoing(ADCx) == 1U)
    {
      if(timeout_cpu_cycles-- == 0U)
      {
        /* Time-out error */
        status = ERROR;
      }
    }
  }

  /* Check whether ADC state is compliant with expected state */
  if(READ_BIT(ADCx->CR,(ADC_CR_ADSTP | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN ))== 0U)
  {
    /* ========== Reset ADC registers ========== */
    /* Reset register IER */
    CLEAR_BIT(ADCx->IER,
              (  LL_ADC_IT_EOC
                 | LL_ADC_IT_EOS
                 | LL_ADC_IT_OVR
                 | LL_ADC_IT_EOSMP
                 | LL_ADC_IT_AWD )
             );

    /* Reset register ISR */
    SET_BIT(ADCx->ISR,
            (  LL_ADC_FLAG_EOC
               | LL_ADC_FLAG_EOS
               | LL_ADC_FLAG_OVR
               | LL_ADC_FLAG_EOSMP
               | LL_ADC_FLAG_AWD )
           );
    
    /* Reset register CR */
    /* Bits ADC_CR_VREFBUFF_SEL, ADC_CR_VREF_BUFFERE */
    CLEAR_BIT(ADCx->CR,(ADC_CR_VREFBUFF_SEL | ADC_CR_VREF_BUFFERE) );

    /* Reset register CFGR1 */
    CLEAR_BIT(ADCx->CFGR1,
              (  ADC_CFGR1_AWDCH   | ADC_CFGR1_AWDEN  | ADC_CFGR1_AWDSGL  | ADC_CFGR1_DISCEN
                 | ADC_CFGR1_WAIT   | ADC_CFGR1_CONT    | ADC_CFGR1_OVRMOD
                 | ADC_CFGR1_EXTEN   | ADC_CFGR1_EXTSEL | ADC_CFGR1_ALIGN   | ADC_CFGR1_RESSEL
                 | ADC_CFGR1_SCANDIR                     )
             );

    /* Reset register CFGR2 */
    /* Note: Update of ADC clock mode is conditioned to ADC state disabled:   */
    /*       already done above.                                              */
    CLEAR_BIT(ADCx->CFGR2, ADC_CFGR2_CKMODE);

    /* Reset register SMPR */
    CLEAR_BIT(ADCx->SMPR, ADC_SMPR_SMP);

    /* Reset register TR */
    MODIFY_REG(ADCx->TR, ADC_TR_HT | ADC_TR_LT, ADC_TR_HT);

    /* Reset register CHSELR */
#if defined(ADC_CHSELR_CHSEL10)
    CLEAR_BIT(ADCx->CHSELR,
              (   ADC_CHSELR_CHSEL10 | ADC_CHSELR_CHSEL9  | ADC_CHSELR_CHSEL8
                | ADC_CHSELR_CHSEL7  | ADC_CHSELR_CHSEL6  | ADC_CHSELR_CHSEL5  | ADC_CHSELR_CHSEL4
                | ADC_CHSELR_CHSEL3  | ADC_CHSELR_CHSEL2  | ADC_CHSELR_CHSEL1  | ADC_CHSELR_CHSEL0 )
             );
#else
    CLEAR_BIT(ADCx->CHSELR,
              (   ADC_CHSELR_CHSEL9  | ADC_CHSELR_CHSEL8
                | ADC_CHSELR_CHSEL7  | ADC_CHSELR_CHSEL6  | ADC_CHSELR_CHSEL5  | ADC_CHSELR_CHSEL4
                | ADC_CHSELR_CHSEL3  | ADC_CHSELR_CHSEL2  | ADC_CHSELR_CHSEL1  | ADC_CHSELR_CHSEL0 )
             );


#endif


    /* Reset register CCR */
    CLEAR_BIT(__LL_ADC_COMMON_INSTANCE(ADC1)->CCR,ADC_CCR_TSEN | ADC_CCR_VREFEN);

    /* Reset register DR */
    /* bits in access mode read only, no direct reset applicable */

  }
  else
  {
    /* ADC instance is in an unknown state */
    /* Need to performing a hard reset of ADC instance, using high level      */
    /* clock source RCC ADC reset.                                            */
    status = ERROR;
  }

  return status;
}


/**
  * @brief  Initialize some features of ADC instance.
  * @note   These parameters have an impact on ADC scope: ADC instance.
  *         Refer to corresponding unitary functions into
  *         @ref ADC_LL_EF_Configuration_ADC_Instance .
  * @note   After using this function, some other features must be configured
  *         using LL unitary functions.
  *         The minimum configuration remaining to be done is:
  *          - Set ADC group regular sequencer:
  *            map channel on rank corresponding to channel number.
  *            Refer to function @ref LL_ADC_REG_SetSequencerChannels();
  *          - Set ADC channel sampling time
  *            Refer to function LL_ADC_SetChannelSamplingTime();
  * @param  ADCx ADC instance
  * @param  ADC_InitStruct Pointer to a @ref LL_ADC_REG_InitTypeDef structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ADC registers are initialized
  *          - ERROR: ADC registers are not initialized
  */
ErrorStatus LL_ADC_Init(ADC_TypeDef *ADCx, LL_ADC_InitTypeDef *ADC_InitStruct)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));

  assert_param(IS_LL_ADC_CLOCK(ADC_InitStruct->Clock));
  assert_param(IS_LL_ADC_RESOLUTION(ADC_InitStruct->Resolution));
  assert_param(IS_LL_ADC_DATA_ALIGN(ADC_InitStruct->DataAlignment));
  assert_param(IS_LL_ADC_LOW_POWER(ADC_InitStruct->LowPowerMode));

  /* Note: Hardware constraint (refer to description of this function):       */
  /*       ADC instance must be disabled.                                     */
  if(LL_ADC_IsEnabled(ADCx) == 0U)
  {
    /* Configuration of ADC hierarchical scope:                               */
    /*  - ADC instance                                                        */
    /*    - Set ADC data resolution                                           */
    /*    - Set ADC conversion data alignment                                 */
    /*    - Set ADC low power mode                                            */
    MODIFY_REG(ADCx->CFGR1,
               ADC_CFGR1_RESSEL
               | ADC_CFGR1_ALIGN
               | ADC_CFGR1_WAIT
               ,
               ADC_InitStruct->Resolution
               | ADC_InitStruct->DataAlignment
               | ADC_InitStruct->LowPowerMode
              );

    MODIFY_REG(ADCx->CFGR2,
               ADC_CFGR2_CKMODE
               ,
               ADC_InitStruct->Clock
              );
  }
  else
  {
    /* Initialization error: ADC instance is not disabled. */
    status = ERROR;
  }
  return status;
}

/**
  * @brief  Set each @ref LL_ADC_InitTypeDef field to default value.
  * @param  ADC_InitStruct Pointer to a @ref LL_ADC_InitTypeDef structure
  *                        whose fields will be set to default values.
  * @retval None
  */
void LL_ADC_StructInit(LL_ADC_InitTypeDef *ADC_InitStruct)
{
  /* Set ADC_InitStruct fields to default values */
  /* Set fields of ADC instance */
  ADC_InitStruct->Clock         = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  ADC_InitStruct->Resolution    = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct->DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct->LowPowerMode  = LL_ADC_LP_MODE_NONE;
}


/**
  * @brief  Initialize some features of ADC group regular.
  * @note   These parameters have an impact on ADC scope: ADC group regular.
  *         Refer to corresponding unitary functions into
  *         @ref ADC_LL_EF_Configuration_ADC_Group_Regular
  *         (functions with prefix "REG").
  * @note   After using this function, other features must be configured
  *         using LL unitary functions.
  *         The minimum configuration remaining to be done is:
  *          - Set ADC group regular sequencer:
  *            map channel on rank corresponding to channel number.
  *            Refer to function @ref LL_ADC_REG_SetSequencerChannels();
  *          - Set ADC channel sampling time
  *            Refer to function LL_ADC_SetChannelSamplingTime();
  * @note   Depending on devices and packages, DMA may not be available.
  *         Refer to device datasheet for DMA availability.
  * @param  ADCx ADC instance
  * @param  ADC_REG_InitStruct Pointer to a @ref LL_ADC_REG_InitTypeDef structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: ADC registers are initialized
  *          - ERROR: ADC registers are not initialized
  */
ErrorStatus LL_ADC_REG_Init(ADC_TypeDef *ADCx, LL_ADC_REG_InitTypeDef *ADC_REG_InitStruct)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(ADCx));
  assert_param(IS_LL_ADC_REG_TRIG_SOURCE(ADC_REG_InitStruct->TriggerSource));
  assert_param(IS_LL_ADC_REG_SEQ_SCAN_DISCONT_MODE(ADC_REG_InitStruct->SequencerDiscont));
  assert_param(IS_LL_ADC_REG_CONTINUOUS_MODE(ADC_REG_InitStruct->ContinuousMode));
  assert_param(IS_LL_ADC_REG_OVR_DATA_BEHAVIOR(ADC_REG_InitStruct->Overrun));

  /* Note: Hardware constraint (refer to description of this function):       */
  /*       ADC instance must be disabled.                                     */
  if(LL_ADC_IsEnabled(ADCx) == 0U)
  {
    /* Configuration of ADC hierarchical scope:                               */
    /*  - ADC group regular                                                   */
    /*    - Set ADC group regular trigger source                              */
    /*    - Set ADC group regular sequencer discontinuous mode                */
    /*    - Set ADC group regular continuous mode                             */
    /*    - Set ADC group regular overrun behavior                            */

    MODIFY_REG(ADCx->CFGR1,
               ADC_CFGR1_EXTSEL
               | ADC_CFGR1_EXTEN
               | ADC_CFGR1_DISCEN
               | ADC_CFGR1_CONT
               | ADC_CFGR1_OVRMOD
               ,
               ADC_REG_InitStruct->TriggerSource
               | ADC_REG_InitStruct->SequencerDiscont
               | ADC_REG_InitStruct->ContinuousMode
               | ADC_REG_InitStruct->Overrun
              );
  }
  else
  {
    /* Initialization error: ADC instance is not disabled. */
    status = ERROR;
  }
  return status;
}

/**
  * @brief  Set each @ref LL_ADC_REG_InitTypeDef field to default value.
  * @param  ADC_REG_InitStruct Pointer to a @ref LL_ADC_REG_InitTypeDef structure
  *                            whose fields will be set to default values.
  * @retval None
  */
void LL_ADC_REG_StructInit(LL_ADC_REG_InitTypeDef *ADC_REG_InitStruct)
{
  /* Set ADC_REG_InitStruct fields to default values */
  /* Set fields of ADC group regular */
  ADC_REG_InitStruct->TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct->SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct->ContinuousMode   = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct->Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* ADC1 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

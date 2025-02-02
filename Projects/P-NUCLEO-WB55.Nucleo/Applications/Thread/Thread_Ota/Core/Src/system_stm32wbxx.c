/**
  ******************************************************************************
  * @file    system_stm32wbxx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex Device Peripheral Access Layer System Source File
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32wbxx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *   After each device reset the MSI (4 MHz) is used as system clock source.
  *   Then SystemInit() function is called, in "startup_stm32wbxx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  *   This file configures the system clock as follows:
  *=============================================================================
  *-----------------------------------------------------------------------------
  *        System Clock source                    | MSI
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 4000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 4000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 1
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 8
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 7
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_R                                  | 2
  *-----------------------------------------------------------------------------
  *        PLLSAI1_P                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI1_Q                              | NA
  *-----------------------------------------------------------------------------
  *        PLLSAI1_R                              | NA
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32WBxx_system
  * @{
  */

/** @addtogroup stm32WBxx_System_Private_Includes
  * @{
  */

#include "app_common.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    (32000000UL) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
   #define MSI_VALUE    (4000000UL) /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    (16000000UL) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#if !defined  (LSI_VALUE) 
 #define LSI_VALUE  (32000UL)       /*!< Value of LSI in Hz*/
#endif /* LSI_VALUE */ 

#if !defined  (LSE_VALUE)
  #define LSE_VALUE    (32768UL)    /*!< Value of LSE in Hz*/
#endif /* LSE_VALUE */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_TypesDefinitions
  * @{
  */
typedef void (*fct_t)(void);
/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Defines
  * @{
  */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET         0x0U            /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */

#define VECT_TAB_BASE_ADDRESS   SRAM1_BASE       /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
  uint32_t SystemCoreClock  = 4000000UL ; /*CPU1: M4 on MSI clock after startup (4MHz)*/

  const uint32_t AHBPrescTable[16UL] = {1UL, 3UL, 5UL, 1UL, 1UL, 6UL, 10UL, 32UL, 2UL, 4UL, 8UL, 16UL, 64UL, 128UL, 256UL, 512UL};

  const uint32_t APBPrescTable[8UL]  = {0UL, 0UL, 0UL, 0UL, 1UL, 2UL, 3UL, 4UL};

  const uint32_t MSIRangeTable[16UL] = {100000UL, 200000UL, 400000UL, 800000UL, 1000000UL, 2000000UL, \
                                      4000000UL, 8000000UL, 16000000UL, 24000000UL, 32000000UL, 48000000UL, 0UL, 0UL, 0UL, 0UL}; /* 0UL values are incorrect cases */

  const uint32_t SmpsPrescalerTable[4UL][6UL]={{1UL,3UL,2UL,2UL,1UL,2UL}, \
                                        {2UL,6UL,4UL,3UL,2UL,4UL}, \
                                        {4UL,12UL,8UL,6UL,4UL,8UL}, \
                                        {4UL,12UL,8UL,6UL,4UL,8UL}};

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Private_Functions
  * @{
  */
static void JumpFwApp( void );
static void BootModeCheck( void );
static void JumpSelectionOnPowerUp( void );
static uint8_t  CheckFwAppValidity( void );

/**
 * Check the Boot mode request
 * Depending on the result, the CPU may either jump to an existing application in the user flash
 * or keep on running the code to start the OTA loader
 */
static void BootModeCheck( void )
{
  if(LL_RCC_IsActiveFlag_SFTRST( ) || LL_RCC_IsActiveFlag_OBLRST( ))
  {
    /**
     * The SRAM1 content is kept on Software Reset.
     * In the Thread_Ota application, the first address of the SRAM1 indicates which kind of action has been requested
     */

    /**
     * Check Boot Mode from SRAM1
     */
    if(((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_FW_APP) && (CheckFwAppValidity( ) != 0))
    {
      /**
       * The user has requested to start on the firmware application and it has been checked
       * a valid application is ready
       * Jump now on the application
       */
      JumpFwApp();
    }
    else if(((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_FW_APP) && (CheckFwAppValidity( ) == 0))
    {
      /**
       * The user has requested to start on the firmware application but there is no valid application
       * Erase all sectors specified by byte1 and byte1 in SRAM1 to download a new App.
       */
      *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_THREAD_OTA_APP;     /* Request to reboot on Thread_Ota application */
      *((uint8_t*)SRAM1_BASE+1) = CFG_APP_START_SECTOR_INDEX;
      *((uint8_t*)SRAM1_BASE+2) = 0xFF;
    }
    else if((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_THREAD_OTA_APP)
    {
      /**
       * It has been requested to reboot on Thread_Ota application to download data
       */
    }
    else if((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_CPU2_UPGRADE)
    {
      /**
       * It has been requested to reboot on Thread_Ota application to keep running the firmware upgrade process
       *
       */
    }
    else
    {
      /**
       * There should be no use case to be there because the device already starts from power up
       * and the SRAM1 is then filled with the value define by the user
       * However, it could be that a reset occurs just after a power up and in that case, the Thread_Ota
       * will be running but the sectors to download a new App may not be erased
       */
      JumpSelectionOnPowerUp( );
    }
  }
  else
  {
    /**
     * On Power up, the content of SRAM1 is random
     * The only thing that could be done is to jump on either the firmware application
     * or the Thread_Ota application
     */
    JumpSelectionOnPowerUp( );
  }

  /**
   * Return to the startup file and run the Thread_Ota application
   */
  return;
}

static void JumpSelectionOnPowerUp( void )
{
  /**
   * Check if there is a FW App
   */
  if(CheckFwAppValidity( ) != 0)
  {
    /**
     * The SRAM1 is random
     * Initialize SRAM1 to indicate we requested to reboot of firmware application
     */
    *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_FW_APP;

    /**
     * A valid application is available
     * Jump now on the application
     */
    JumpFwApp();
  }
  else
  {
    /**
     * The SRAM1 is random
     * Initialize SRAM1 to indicate we requested to reboot of Thread_Ota application
     */
    *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_THREAD_OTA_APP;

    /**
     * There is no valid application available
     * Erase all sectors specified by byte1 and byte1 in SRAM1 to download a new App.
     */
    *((uint8_t*)SRAM1_BASE+1) = CFG_APP_START_SECTOR_INDEX;
    *((uint8_t*)SRAM1_BASE+2) = 0xFF;
  }
  return;
}

/**
 * Jump to existing FW App in flash
 * It never returns
 */
static void JumpFwApp( void )
{
  fct_t app_reset_handler;

  SCB->VTOR = FLASH_BASE + (CFG_APP_START_SECTOR_INDEX * 0x1000);
  __set_MSP(*(uint32_t*)(FLASH_BASE + (CFG_APP_START_SECTOR_INDEX * 0x1000)));
  app_reset_handler = (fct_t)(*(uint32_t*)(FLASH_BASE + (CFG_APP_START_SECTOR_INDEX * 0x1000) + 4));
  app_reset_handler();

  /**
   * app_reset_handler() never returns.
   * However, if for any reason a PUSH instruction is added at the entry of  JumpFwApp(),
   * we need to make sure the POP instruction is not there before app_reset_handler() is called
   * The way to ensure this is to add a dummy code after app_reset_handler() is called
   * This prevents app_reset_handler() to be the last code in the function.
   */
  __WFI();


  return;
}

static uint8_t CheckFwAppValidity( void )
{
  uint8_t status;
  uint32_t magic_keyword_address;
  uint32_t last_user_flash_address;

  magic_keyword_address = *(uint32_t*)(FLASH_BASE + (CFG_APP_START_SECTOR_INDEX * 0x1000 + 0x140));
  last_user_flash_address = (((READ_BIT(FLASH->SFR, FLASH_SFR_SFSA) >> FLASH_SFR_SFSA_Pos) << 12) + FLASH_BASE) - 4;
  if( (magic_keyword_address < FLASH_BASE) || (magic_keyword_address > last_user_flash_address) )
  {
    /**
     * The address is not valid
     */
    status = 0;
  }
  else
  {
    if( (*(uint32_t*)magic_keyword_address) != 0x94448A29  )
    {
      /**
       * A firmware update procedure did not complete
       */
      status = 0;
    }
    else
    {
      /**
       * The firmware application is available
       */
      status = 1;
    }
  }

  return status;
}

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  BootModeCheck();

  /* Configure the Vector Table location add offset address ------------------*/
#if defined(VECT_TAB_SRAM) && defined(VECT_TAB_BASE_ADDRESS)  
  /* program in SRAMx */
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET;  /* Vector Table Relocation in Internal SRAMx for CPU1 */
#else    /* program in FLASH */
  SCB->VTOR = VECT_TAB_OFFSET;              /* Vector Table Relocation in Internal FLASH */
#endif

  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << (10UL*2UL))|(3UL << (11UL*2UL)));  /* set CP10 and CP11 Full Access */
  #endif
  
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00070000U;

  /* Reset PLLSAI1ON, PLLON, HSECSSON, HSEON, HSION, and MSIPLLON bits */
  RCC->CR &= (uint32_t)0xFAF6FEFBU;

  /*!< Reset LSI1 and LSI2 bits */
  RCC->CSR &= (uint32_t)0xFFFFFFFAU;
  
  /*!< Reset HSI48ON  bit */
  RCC->CRRCR &= (uint32_t)0xFFFFFFFEU;
    
  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x22041000U;

  /* Reset PLLSAI1CFGR register */
  RCC->PLLSAI1CFGR = 0x22041000U;

  /* Reset HSEBYP bit */
  RCC->CR &= 0xFFFBFFFFU;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI_VALUE(*)
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(***)
  *             or HSI_VALUE(*) or MSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) MSI_VALUE is a constant defined in stm32wbxx_hal.h file (default value
  *             4 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSI_VALUE is a constant defined in stm32wbxx_hal_conf.h file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  *
  *         (***) HSE_VALUE is a constant defined in stm32wbxx_hal_conf.h file (default value
  *              32 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp, msirange, pllvco, pllr, pllsource , pllm;

  /* Get MSI Range frequency--------------------------------------------------*/

  /*MSI frequency range in Hz*/
  msirange = MSIRangeTable[(RCC->CR & RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos];

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
    case 0x00:   /* MSI used as system clock source */
      SystemCoreClock = msirange;
      break;

    case 0x04:  /* HSI used as system clock source */
      /* HSI used as system clock source */
        SystemCoreClock = HSI_VALUE;
      break;

    case 0x08:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;

    case 0x0C: /* PLL used as system clock  source */
      /* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
         SYSCLK = PLL_VCO / PLLR
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
      pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1UL ;

      if(pllsource == 0x02UL) /* HSI used as PLL clock source */
      {
        pllvco = (HSI_VALUE / pllm);
      }
      else if(pllsource == 0x03UL) /* HSE used as PLL clock source */
      {
        pllvco = (HSE_VALUE / pllm);
      }
      else /* MSI used as PLL clock source */
      {
        pllvco = (msirange / pllm);
      }
      
      pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);
      pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1UL);
      
      SystemCoreClock = pllvco/pllr;
      break;

    default:
      SystemCoreClock = msirange;
      break;
  }
  
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK1 prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
  /* HCLK clock frequency */
  SystemCoreClock = SystemCoreClock / tmp;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

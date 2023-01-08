/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */

//=========================================================================
//  File Name: main.c
//
//  Description:
//      main for the OCA Pi
//  
//  Modification History:
//      $Id: $
//      $Log:  $
//
//=========================================================================
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "stm32f2x7_eth.h"
#include "stm32f2x7_eth_bsp.h"
#include "main.h"
#include "task.h"
#include "semphr.h"
#include "netconf.h"
#include "tcpip.h"
#include "stdio.h"

#include "core_cm3.h"
#include "i2c_handler.h"
#include "persist.h"

// Test junk
#include "lwip/sockets.h"
#include "lwip/stats.h"
#include "mdnsd.h"
#include "mdns.h"

#ifdef Rowley_Crossworks
#include "rowley_locks.h"   
#endif


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/*--------------- Tasks Priority -------------*/

/* Global function prototypes ------------------------------------------------*/
static void Master_Task(void * pvParameters);


/* Global variables ----------------------------------------------------------*/
int errno;

static const int8_t masterTask[] = "Master Task";
static const int8_t readSwitches[] = "readSwitches";
static const int8_t dbgConsole[] = "dbgConsole";

void __assert(const char *expression, const char *filename, int line)
{
    printf("Assertion failed: %s at %s line %d\n\r", expression, filename, line);
}



static int tryToSend(const uint8_t *Data, uint16_t length)
{
    int retVal = -1;  // return -1 on failure, success sets to 0
    uint32_t txRetVal;
    uint8_t tries = 5;
    
    while (tries)
    {
        // the call to USART1_TxData can never "fail" putchar only tries to send one char at at time
        // and USART1_TxData only fails if length is > the txBuff size
        txRetVal = USART1_TxData(Data, length);    
        if (0 == txRetVal)
        {
            // no room currently in TX buff, wait a bit and try again
            vTaskDelay(2);
            tries--;
        }
        else
        {
            // data went set tries to 0
            tries = 0;
            retVal = 0;
        }    
    }
    return retVal;
}

int __putchar(int ch)
{
    int retVal = 0;
    uint8_t data = (uint8_t)ch;
        
    data = (uint8_t)ch;
    retVal = tryToSend(&data, 1);
    
    // putchar returns the character sent on success, or -1 (EOF) on fail
    if (0 == retVal)
    {
        retVal = ch;
    }
    else
    {
        retVal = -1;
    }
    return retVal;
}

void vApplicationIdleHook( void )
{
    portTickType currTime;
    static portTickType m_prevTime;
    
    currTime = xTaskGetTickCount();
    
    if ((currTime - m_prevTime) > 1000)
    {
        m_prevTime = currTime;
      
    }
}



/* Extern function prototypes ------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if 0
static void mdnsRegisterService(void);
#endif

/* Private functions ---------------------------------------------------------*/
static void GPIO_Config(void);
extern int OCA_main(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
#ifdef Rowley_Crossworks
  // At this stage the microcontroller clock setting is configured to 64 MHz, 
  //     this is done through SystemInit() function which is called from
  //     startup file (startup_stm32f2xx.s) before the branch to application main.
  //     To reconfigure the default setting of SystemInit() function, refer to
  //     system_stm32f2xx.c file
  //     
  //     sysTick timer is running at 120 MHz
#else
  // Configure the microcontroller clock settings to 64 MHz, 
  //     this is done through SystemInit().
  //     To reconfigure the default setting of SystemInit() function, refer to
  //     system_stm32f2xx.c file
  //     
  //     sysTick timer is running at 120 MHz
  SystemInit();
#endif

  // see important notes on configuring STM32 processors at
  // http://www.freertos.org/RTOS-Cortex-M3-M4.html
  // set the NVIC_PriorityGroupConfig once here
  // 4 bits for pre-emption priority, 0 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
  
  // just prior to jumping to the application, the bootloader issued a
  // __disable_fault_irq, we need to issue a __enable_fault_irq to undo that.
  __enable_fault_irq();
  
  // configure the GPIOs  
  GPIO_Config();


  USART1_Init();
#ifdef Rowley_Crossworks
    // call the init Rowley Locks function on startup, it
    // configures the low level locks around the heap, etc
    // this is used only with Rowley_Crossworks builds.
    initRowleyLocks();
#endif

  //Create the Master task.
  //This task will initialize the rest of the System
  //and the remaining tasks.
  (void) xTaskCreate(Master_Task, masterTask, configMINIMAL_STACK_SIZE * 2, NULL, (tskIDLE_PRIORITY + 4), NULL);
  

  /* Start scheduler */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; )
  {
    ;
  }
}


void vApplicationTickHook( void )
{
    // the LED Array uses a row / column approach and needs to be 
    // updated every millisecond for it to look correct
    processLedArray();
}


static void Master_Task(void * pvParameters)
{
    // since alot of these items use semaphores, have taskDelays, etc they
    // need to be called in the context of a task.  
    // Hence the use of the "Master task"
    (void) pvParameters;
    
    // Init I2C
    I2C_1_Init();
    
    // initialize all the persistent data
    persist_Init();
    
    /* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
    ETH_BSP_Config();
                
    // start the task to read the switches
    (void) xTaskCreate(OCA_Pi_ReadSwitches, readSwitches , configMINIMAL_STACK_SIZE * 2, NULL,(tskIDLE_PRIORITY + 3), NULL);

    (void) xTaskCreate(serialDebugPortTask, dbgConsole , configMINIMAL_STACK_SIZE * 2, NULL,(tskIDLE_PRIORITY + 2), NULL);
    OCA_main();

    encoder_init();

    LED51_on();  // using this as a Power ON LED
    RELAY1_off();  // should be off, just making sure
    RELAY2_off();

    int runLed = 0;
for (;;)
 {
    vTaskDelay(50);

    runLed = !runLed;
    if (runLed)
    {
        LED51_off();
    }
    else
    {
        LED51_on(); 
    }
}
    //delete ourself.
;;    vTaskDelete(NULL);
}


static void GPIO_Config(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC |    
                           RCC_AHB1Periph_GPIOD |    
                           RCC_AHB1Periph_GPIOE,
                           ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // configure all the output pins
    // configure the Port A output pins
    GPIO_InitStructure.GPIO_Pin   = PORTA_OUT_PINS;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, & GPIO_InitStructure);

    // configure the Port B output pins
    GPIO_InitStructure.GPIO_Pin   = PORTB_OUT_PINS;
    GPIO_Init(GPIOB, & GPIO_InitStructure);

    // Port C does not have any output pins
    
    // configure the Port D output pins
    GPIO_InitStructure.GPIO_Pin   = PORTD_OUT_PINS;
    GPIO_Init(GPIOD, & GPIO_InitStructure);

    // to keep the LEDs from flickering, set the column outputs to
    // all ones prior to configuring the pins as outputs
    GPIOE->ODR = 0xFFFF;
    // configure the Port E output pins
    GPIO_InitStructure.GPIO_Pin   = PORTE_OUT_PINS;
    GPIO_Init(GPIOE, & GPIO_InitStructure);

    // configure all the input pins
    
    // Port A does not have any input pins
    
    // Port B does not have any input pins

    // configure the Port C input pins
    // port C input pins are used by the encoders and are initialized
    // as part of the encoder init function

    // configure the Port D input pins
    GPIO_InitStructure.GPIO_Pin  = PORTD_IN_PINS;
    GPIO_Init(GPIOD, & GPIO_InitStructure);

    // Port E does not have any input pins

}


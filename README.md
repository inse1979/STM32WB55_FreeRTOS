# STM32WB55 FreeRTOS Example

## Hardware
  ### [P-NUCLEO-WB5](https://www.st.com/en/evaluation-tools/p-nucleo-wb55.html)
  ![P-NUCLEO-WB55](https://www.st.com/bin/ecommerce/api/image.PF265562.en.feature-description-include-personalized-no-cpn-medium.jpg)
---
## Hardware
```
  - support IWDOG
  - support UASRT1
  - support I2C1
  - support GPIO
  - support USB Virtual COM Port
```
## Software
```
  - support FreeRTOS 10.3.1
  - support FreeRTOS-Plus-Cli
```
---
## FLASH
|        | Start Sectors |  length | End Sectors | Description |
|  ----  | ------------  | ------- | ------------|-------------|
|    1   |  0x08000000   |   64K   | 0x080FFFFF  | bootloader  |
|    2   |  0x08010000   |   64K   | 0x0801FFFF  | user date   |
|    3   |  0x08020000   |  192K   | 0x0804FFFF  | freertos    |
|    4   |  0x08050000   |  192K   | 0x0807FFFF  | empty       |

## Modify the FLASH start address
```
  1. Modify /STM32WB55RGVX_FLASH.ld
  2. Modify /Drivers/CMSIS/Device/ST/STM32WBxx/Include
  3. Modify /Core/Src/system_stm32wbxx.c
```
### `/STM32WB55RGVX_FLASH.ld`
#### Modify the start address and length of FLASH
```
...
/* Specify the memory areas */
MEMORY
{
FLASH (rx)                 : ORIGIN = 0x08020000, LENGTH = 384K
RAM1 (xrw)                 : ORIGIN = 0x20000004, LENGTH = 0x2FFFC
RAM_SHARED (xrw)           : ORIGIN = 0x20030000, LENGTH = 10K
}
...
```  
### `/Drivers/CMSIS/Device/ST/STM32WBxx/Include/stm32wb55xx.h`
#### Modify the start address and length of FLASH
```
...
/** @addtogroup Peripheral_memory_map
  * @{
  */

/*!< Boundary memory map */
#define FLASH_BASE             (0x08020000UL)/*!< FLASH(up to 1 MB) base address */
#define SRAM_BASE              (0x20000000UL)/*!< SRAM(up to 256 KB) base address */
#define PERIPH_BASE            (0x40000000UL)/*!< Peripheral base address */
...
```
### `/Core/Src/system_stm32wbxx.c`

#### enable VECT_TAB_SRAM MACRO

```
#if defined(USER_VECT_TAB_ADDRESS)
/*!< Uncomment this line for user vector table remap in Sram else user remap
     will be done in Flash. */
#define VECT_TAB_SRAM
#if defined(VECT_TAB_SRAM)
#define VECT_TAB_BASE_ADDRESS   SRAM1_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#else
#define VECT_TAB_BASE_ADDRESS   FLASH_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif /* VECT_TAB_SRAM */
#endif /* USER_VECT_TAB_ADDRESS */

```

//Define Clock source
#define MCU_CLOCK_SOURCE      
#define MCU_CLOCK_SOURCE_HXT  // HXT, LXT, HIRC, LIRC
#define MCU_CLOCK_FREQUENCY   50000000  //Hz

//Define MCU Interfaces
//Define MCU Interfaces
#define MCU_INTERFACE_SPI3
#define SPI3_CLOCK_SOURCE_HCLK // HCLK, PLL
#define PIN_SPI3_SS0_PD8
#define PIN_SPI3_SCLK_PD9
#define PIN_SPI3_MISO0_PD10
#define PIN_SPI3_MOSI0_PD11



//Define MCU Interfaces SERVO
#define MCU_INTERFACE_PWM0
#define PWM0_CH01_CLOCK_SOURCE_HXT // HXT, LXT, HCLK, HIRC
#define PIN_PWM0_CH0_PA12  

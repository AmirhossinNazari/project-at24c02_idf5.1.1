

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define  I2C_FREQ       100000
#define  I2C_GPIO_SDA   GPIO_NUM_17
#define  I2C_GPIO_SCL   GPIO_NUM_18
#define  I2C_PORT_NUM   I2C_NUM_1
#define  ACK_CHECK_EN    0x1
#define  ACK_CHECK_DES   0x0
#define  ACK_VAL         0x0
#define  NACK_VAL        0x1

#define EEPROM_BUS_ADDRESS   0XA0 
static const char *TAG = "eeprom";
uint8_t *buffer;
uint8_t eep_data;
int ret;

//+++++++++++++++++++++++++++++++++++++

void p_w(){

      i2c_cmd_handle_t cmd = i2c_cmd_link_create();

      ret = i2c_master_start(cmd);

      ret = i2c_master_write_byte(cmd, EEPROM_BUS_ADDRESS  | I2C_MASTER_WRITE, ACK_CHECK_EN  );

      ret = i2c_master_write_byte(cmd, 6 , ACK_CHECK_EN );     //address

      ret = i2c_master_write_byte(cmd, 45 , ACK_CHECK_EN );    //data
    
      ret = i2c_master_stop(cmd);
     
      ret = i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);

      if(ret == ESP_OK){ESP_LOGE(TAG, "i2c_ok_write : %d",ret);}

      i2c_cmd_link_delete(cmd);

      usleep(1000);


}

void p_r(uint8_t *data)
{

     i2c_cmd_handle_t cmd = i2c_cmd_link_create();

     ret = i2c_master_start(cmd);
    
     ret = i2c_master_write_byte(cmd, EEPROM_BUS_ADDRESS , ACK_CHECK_EN  );

     ret = i2c_master_write_byte(cmd, 6 , ACK_CHECK_EN );        // address

     ret = i2c_master_start(cmd);

     ret = i2c_master_write_byte(cmd, EEPROM_BUS_ADDRESS | I2C_MASTER_READ , ACK_CHECK_EN  );

     ret = i2c_master_read_byte(cmd, data  , 0);

     ret = i2c_master_stop(cmd);

     ret = i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);

     if(ret == ESP_OK){ESP_LOGE(TAG, "i2c_okread : %d",ret); }
     
     i2c_cmd_link_delete(cmd);

     usleep(1000);

}

void app_main()
{

i2c_config_t  i2c_conf ={


        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_GPIO_SDA,
        .scl_io_num = I2C_GPIO_SCL,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_FREQ,
		.clk_flags = 0,

        
    };
//___________________________________________________________________
i2c_param_config(I2C_PORT_NUM ,  &i2c_conf);
//___________________________________________________________________
i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0);
//___________________________________________________________________

p_w();

ESP_LOGE(TAG, "____________________: ");

p_r(&eep_data);

printf("data >>>>>>>>>>>>>>>>>>>>  : %d\n",eep_data);

ESP_LOGE(TAG, "_________end___________: ");


}

 






























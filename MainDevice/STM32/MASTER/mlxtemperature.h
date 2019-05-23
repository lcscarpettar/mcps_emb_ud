#ifndef MLXTEMPERATURE_H
#define MLXTEMPERATURE_H

#include "stm32f1xx_hal.h"

static const uint16_t MLX_I2C_ADDRESS = 0x5A;
static const uint8_t MLX_RAM_TEMP_AMB_ADDRESS [] = {0x06};
static const uint8_t MLX_RAM_TEMP_OBJ1_ADDRESS [] = {0x07};
static const uint8_t MLX_RAM_TEMP_OBJ2_ADDRESS [] = {0x08};
static const uint32_t MLX_I2C_TIME_OUT = 500;

void mlx_write_to_memory(I2C_HandleTypeDef * i2c_handle, const uint8_t memory_address []);
double MLX_get_ambient_temperature(I2C_HandleTypeDef * i2c_handle);
double MLX_get_object_temperature(I2C_HandleTypeDef * i2c_handle);
double mlx_read_from_memory(I2C_HandleTypeDef * i2c_handle);
int mlx_detect_device(I2C_HandleTypeDef * i2c_handle);


#endif /* GRANDPARENT_H */

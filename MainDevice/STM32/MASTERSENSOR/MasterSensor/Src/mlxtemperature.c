#include "mlxtemperature.h"

double MLX_get_ambient_temperature(I2C_HandleTypeDef * i2c_handle)
{
	mlx_write_to_memory(i2c_handle, MLX_RAM_TEMP_AMB_ADDRESS);
	return mlx_read_from_memory(i2c_handle);
}

double MLX_get_object_temperature(I2C_HandleTypeDef * i2c_handle)
{
	mlx_write_to_memory(i2c_handle, MLX_RAM_TEMP_OBJ1_ADDRESS);
	double temp_obj1 = mlx_read_from_memory(i2c_handle);
	mlx_write_to_memory(i2c_handle, MLX_RAM_TEMP_OBJ2_ADDRESS);
	double temp_obj2 = mlx_read_from_memory(i2c_handle);
	return (temp_obj1 + temp_obj2) / 2.0;
}

void mlx_write_to_memory(I2C_HandleTypeDef * i2c_handle, const uint8_t memory_address [])
{
	HAL_I2C_Master_Transmit(i2c_handle, MLX_I2C_ADDRESS, (uint8_t *)memory_address, 1, MLX_I2C_TIME_OUT);
}

double mlx_read_from_memory(I2C_HandleTypeDef * i2c_handle)
{
	uint8_t i2c_received_data [2];
	HAL_I2C_Master_Receive(i2c_handle, MLX_I2C_ADDRESS, i2c_received_data, 2, MLX_I2C_TIME_OUT);
	return ((i2c_received_data[0] << 8) + i2c_received_data[0]) * 0.01;	
}

int mlx_detect_device(I2C_HandleTypeDef * i2c_handle)
{
	HAL_StatusTypeDef result = HAL_I2C_IsDeviceReady(i2c_handle, MLX_I2C_ADDRESS, 2, 2);
	return result == HAL_OK;
}

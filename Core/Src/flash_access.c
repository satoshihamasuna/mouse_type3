/*
 * flash_access.c
 *
 *  Created on: 2020/07/24
 *      Author: sato1
 */

#include "stm32f4xx_hal.h"
#include "flash_access.h"

#include "index.h"
#include "glob_var.h"
#include "mymacro.h"

#include <string.h>
#include <stdint.h>

#define START_ADDRESS  	0x8060000
#define END_ADDRESS    	0x807FFFF

#define MAP_START_ADDRESS 	0x8060000
#define MAP_END_ADDRESS	  	0x806FFFF

#define WALL_START_ADDRESS  0x8070000
#define WALL_END_ADDRESS	0x807FFFF

void write_map_flash();
void write_wall_flash();
void read_map_flash();
void read_wall_flash();

void eraseALL(){
	HAL_FLASH_Unlock();
	eraseFlash();
	HAL_FLASH_Lock();
}

void save_data(){
	for( uint32_t x = 0 ; x < MAZE_SIZE_X ; x++ ){
		for( uint32_t y = 0 ; y < MAZE_SIZE_Y ; y++ ){
			save_map [x][y] = map[x][y];
			save_wall[x][y] = wall[x][y];
		}
	}
}

void write_save_data(){
	HAL_FLASH_Unlock();
	eraseFlash();
	write_map_flash();
	write_wall_flash();
	HAL_FLASH_Lock();
}

void read_save_data(){
	read_map_flash();
	read_wall_flash();
	save_data();
}

void read_map_flash(){
	for( uint32_t x = 0 ; x < MAZE_SIZE_X ; x++ ){
		for( uint32_t y = 0 ; y < MAZE_SIZE_Y ; y++ ){
			uint32_t address = MAP_START_ADDRESS + (x << 4) + y;
			loadFlash(address,&map[x][y],1);
		}
	}
}

void write_map_flash(){
	for( uint32_t x = 0 ; x < MAZE_SIZE_X ; x++ ){
		for( uint32_t y = 0 ; y < MAZE_SIZE_Y ; y++ ){
			uint32_t address = MAP_START_ADDRESS + (x << 4) + y;
			writeFlash(address,&save_map[x][y]);
		}
	}

}

void read_wall_flash(){
	for(uint32_t x = 0 ; x < MAZE_SIZE_X ; x++ ){
		for(uint32_t y = 0 ; y < MAZE_SIZE_Y ; y++ ){
			uint32_t address = WALL_START_ADDRESS + (x << 4) + y;
			uint16_t data = 0;
			loadFlash(address,&data,2);
			wall[x][y].north = (data >> 6) & 0x03;
			wall[x][y].east  = (data >> 4) & 0x03;
			wall[x][y].south = (data >> 2) & 0x03;
			wall[x][y].west  = (data >> 0) & 0x03;
		}
	}
}

void write_wall_flash(){
	for(uint32_t x = 0 ; x < MAZE_SIZE_X ; x++ ){
		for(uint32_t y = 0 ; y < MAZE_SIZE_Y ; y++ ){
			uint32_t address = WALL_START_ADDRESS + (x << 4) + y;
			uint16_t data =((uint8_t)(save_wall[x][y].north) << 6) | ((uint8_t)(save_wall[x][y].east) << 4) | ((uint8_t)(save_wall[x][y].south) << 2) | ((uint8_t)(save_wall[x][y].west) << 0);
			writeFlash(address,&data);
		}
	}
}

void eraseFlash( void )
{
	FLASH_EraseInitTypeDef erase;
	erase.TypeErase = FLASH_TYPEERASE_SECTORS;	// select sector
	erase.Sector = FLASH_SECTOR_7;		       // set selector7
	erase.NbSectors = 1;		// set to erase one sector
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;	// set voltage range (2.7 to 3.6V)

	uint32_t pageError = 0;

	HAL_FLASHEx_Erase(&erase, &pageError);	// erase sector
}

/*
 * @brief write flash(sector11)
 * @param uint32_t address sector11 start address
 * @param uint8_t * data write data
 * @param uint32_t size write data size
*/
void writeFlash(uint32_t address, uint16_t *data )
{
	//HAL_FLASH_Unlock();		// unlock flash

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, *data); // write byte

	//HAL_FLASH_Lock();		// lock flash
}

/*
 * @brief write flash(sector11)
 * @param uint32_t address sector11 start address
 * @param uint8_t * data read data
 * @param uint32_t size read data size
*/
void loadFlash(uint32_t address,uint16_t *data, uint32_t size )
{
	memcpy(data, (uint16_t*) address, size); // copy data
}

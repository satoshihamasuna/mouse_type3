/*
 * flash_access.h
 *
 *  Created on: 2020/07/24
 *      Author: sato1
 */

#ifndef INC_FLASH_ACCESS_H_
#define INC_FLASH_ACCESS_H_


void eraseFlash( void );
void writeFlash(uint32_t address, uint16_t *data );
void loadFlash(uint32_t address,uint16_t *data, uint32_t size );

/*
 * writeFlash( start_address, (uint8_t*)maze_data, sizeof(MAZE_DATA) );
 *
 * loadFlash( start_address, (uint8_t*)maze_data, sizeof(MAZE_DATA) );
 *
 */

#endif /* INC_FLASH_ACCESS_H_ */

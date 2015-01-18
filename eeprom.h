#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_START (uint8_t *)(0x10)
#define EEPROM_PERIOD (uint8_t *)(0x11)
#define EEPROM_RUN (uint8_t *)(0x12)
#define EEPROM_COUNTER (uint16_t *)(0x13) // TWO bytes

#endif /* EEPROM_H_ */

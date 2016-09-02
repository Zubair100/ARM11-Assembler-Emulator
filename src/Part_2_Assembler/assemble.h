#ifndef ASSEMBLE_H
#define ASSEMBLE_H

/* ******************************************************************** */
/*                  PROTOTYPES FOR ASSEMBLE FUNCTIONS                   */
/* ******************************************************************** */

void to_little_endian(uint32_t *bin_array, uint32_t length);
uint32_t endian_switcher(uint32_t word);
void write_file(char *filename, uint32_t *bin_array, uint32_t no_of_instr);
char *read_file(char *filename);

/* ******************************************************************** */
/*                  MACROS USED TO REPRESENT BIT MASKS                  */
/* ******************************************************************** */

#define BITMASK_RIGHT_8_BITS 0x000000FF
#define BITMASK_MIDDLE_RIGHT_8_BITS 0x0000FF00
#define BITMASK_LEFT_8_BITS 0xFF000000
#define BITMASK_MIDDLE_LEFT_8_BITS 0x00FF0000
#define BITMASK_RIGHT_1_BIT 0x00000001
#define BITMASK_RIGHT_2_BITS 0x00000003
#define BITMASK_RIGHT_4_BITS 0x0000000F
#define BITMASK_RIGHT_5_BITS 0x0000001F
#define BITMASK_RIGHT_12_BITS 0x00000FFF
#define BITMASK_RIGHT_24_BITS 0x00FFFFFF
#define BITMASK_RIGHT_32_BITS 0xFFFFFFFF
#define BITMASK_LEFT_6_BITS 0xFC000000

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#endif

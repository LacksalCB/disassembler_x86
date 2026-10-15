#define COUNT 256  
#define OPCODE_LENGTH 0  

// FLAGS:
#define F_NONE  0
#define F_MODRM  1<<0
#define F_SIB  1<<1
#define F_IMM8  1<<2
#define F_IMM32  1<<3
#define F_GROUP  1<<4
#define F_REX  1<<5
#define F_REG_OPCODE  1<<6

// F_MODRM:
char F_MODRM[16] = {$RM, $RM, DISP, $RM, DISP, $REG_MODE, $REG_MODE, 
// RM:
char RM[16] = {$REG_MODE[0], $REG_MODE[1], $REG_MODE[2], $REG_MODE[3], $SIB, $REG_MODE[5], $REG_MODE[6], $REG_MODE[7], $REG_MODE[7], $REG_MODE[7], $REG_MODE[7], 
// SIB_I:
char SIB_I[16] = {
// SIB_S:
char SIB_S[16] = {$I*1, $I*2, $I*4, $I*8, $I*8, 
// SIB_B:
char SIB_B[16] = {
// REX_W:
char REX_W[16] = {
// REX_R:
char REX_R[16] = {$REX_R$REG, $REX_R$REG, 
// REX_X:
char REX_X[16] = { $REX_X$SIB_I,  $REX_X$SIB_I, 
// REX_B:
char REX_B[16] = {$REX_B$SIB_B, $REX_B$RM, $REX_B$RM, $REX_B$REG, $REX_B$REG, $REX_B$REG, $REX_B$REG, 
// REG_8:
char REG_8[16] = {$8_REX[index], $8_SHORT[index], $8_SHORT[index], $8_SHORT[index], 
// 8_SHORT:
char 8_SHORT[16] = {
// 8_REX:
char 8_REX[16] = {
// REG_16:
char REG_16[16] = {
// REG_32:
char REG_32[16] = {
// REG_64:
char REG_64[16] = {
// OPCODES_1BYTE:
char OPCODES_1BYTE[16] = {
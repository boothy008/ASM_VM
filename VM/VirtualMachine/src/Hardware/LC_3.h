#pragma once

#include <Windows.h>
#include <cstdint>
#include <cstdio>

class LC_3
{

    uint8_t m_running = 1;
    HANDLE m_hstdin;
	
    enum
    {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC,
        R_PSR,
        R_COUNT
    };

	//R_PSR
	//MSB
	//[15] privilege mode - 0 = supervisor mode, 1 = user mode
	//[10:8] current priority level
	//[2:0] condition code [2] = Negative, [1] = Zero, [0] = Positive

    enum
    {
        OP_BR = 0, /* branch */
        OP_ADD,    /* add  */
        OP_LD,     /* load */
        OP_ST,     /* store */
        OP_JSR,    /* jump register */
        OP_AND,    /* bitwise and */
        OP_LDR,    /* load register */
        OP_STR,    /* store register */
        OP_RTI,    /* unused */
        OP_NOT,    /* bitwise not */
        OP_LDI,    /* load indirect */
        OP_STI,    /* store indirect */
        OP_JMP,    /* jump */
        OP_RES,    /* reserved (unused) */
        OP_LEA,    /* load effective address */
        OP_TRAP    /* execute trap */
    };

    enum
    {
        FL_POS = 1 << 0, // P
        FL_ZRO = 1 << 1, // Z
        FL_NEG = 1 << 2  // N
    };

	enum
	{
		MR_KBSR = 0xFE00, // Keyboard status
		MR_KBDR = 0xFE02, // keyboard data
	};

	enum
	{
		TRAP_GETC = 0x20, //Get char for keyboard not echoed to terminal
		TRAP_OUT = 0x21, //output a char
		TRAP_PUTS = 0x22, //output a word string
		TRAP_IN = 0x23, //Get charactor from keyboard echoed into terminal
		TRAP_PUTSP = 0x24, //Output a byte string
		TRAP_HALT = 0x25
	};

    uint16_t m_memory[UINT16_MAX] = {};
    uint16_t m_reg[R_COUNT] = {};

    //num is converted from smaller data size
    //will convert to a 16 bit number and then set all new bits to 1 if negative
    //or just return the number if positive
    uint16_t sign_extend(uint16_t num, size_t bitSize);

    uint16_t swap16(uint16_t x);

    void update_flags(uint16_t r);

    void add(uint16_t instr);

    //Bitwise and
    void bitwise_and(uint16_t instr);

    //Bitwise not
    void bitwise_not(uint16_t instr);

    //Conditional branch
    void br(uint16_t instr);

    //Jump
    void jmp(uint16_t instr);

    //Jump to subroutine
    void jsr(uint16_t instr);

    //Load
    void ld(uint16_t instr);

    //Load Inderect	
    void ldi(uint16_t instr);

    //Load base + offset
    void ldr(uint16_t instr);

    //Load Effictive Address
    void lea(uint16_t instr);

    //Return from subroutine
    void ret(uint16_t instr);

	//Store
    void st(uint16_t instr);

	//Store indirect
    void sti(uint16_t instr);

	//store base + offset
    void str(uint16_t instr);

	//Get char
    void trap_getc();

    void trap_out();

    void trap_puts();

    void trap_in();

    void trap_putsp();

    void trap_halt();

    uint16_t check_key();
	
    uint16_t mem_read(uint16_t address);

    void mem_write(uint16_t address, uint16_t data);

    void read_image_file(FILE* file);

	
public:

    LC_3(HANDLE hstd) : m_hstdin(hstd)
    {    }
	
    int read_image(const char* image_path);

    void start();
	
};


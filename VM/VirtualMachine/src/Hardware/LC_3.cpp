#include "LC_3.h"

#include <cstdio>
#include <signal.h>
#include <conio.h>

uint16_t LC_3::sign_extend(uint16_t num, size_t bitSize)
{
	if ((num >> (bitSize - 1)) & 0x01)
		num |= (0xFFFF << bitSize);
	return num;
}

uint16_t LC_3::swap16(uint16_t x)
{
	return (x << 8) | (x >> 8);
}

void LC_3::update_flags(uint16_t r)
{
	//Clear condition bits
	m_reg[R_PSR] &= ~(0x07);
	
	if (m_reg[r] == 0)
	{
		m_reg[R_PSR] |= FL_ZRO;
	}
	else if (m_reg[r] >> 15) // If the MSB is 1 the number is negative
	{
		m_reg[R_PSR] |= FL_NEG;
	}
	else
	{
		m_reg[R_PSR] |= FL_POS;
	}
}

void LC_3::add(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t r1 = (instr >> 6) & 0x07;
	uint16_t imm_flag = (instr >> 5) & 0x01;

	if (imm_flag)
	{
		uint16_t imm5 = sign_extend(instr & 0x1F, 5);
		m_reg[r0] = m_reg[r1] + imm5;
	}
	else
	{
		uint16_t r2 = instr & 0x07;
		m_reg[r0] = m_reg[r1] + m_reg[r2];
	}

	update_flags(r0);
}

void LC_3::bitwise_and(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t r1 = (instr >> 6) & 0x07;
	uint16_t imm_flag = (instr >> 5) & 0x01;

	if (imm_flag)
	{
		uint16_t imm5 = sign_extend(instr & 0x1F, 5);
		m_reg[r0] = m_reg[r1] & imm5;
	}
	else
	{
		uint16_t r2 = instr & 0x07;
		m_reg[r0] = m_reg[r1] & m_reg[r2];
	}
	update_flags(r0);
}

void LC_3::bitwise_not(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t r1 = (instr >> 6) & 0x07;

	m_reg[r0] = ~m_reg[r1];
	update_flags(r0);
}

void LC_3::br(uint16_t instr)
{
	uint16_t pc_offset = sign_extend(instr & 0x01FF, 9);
	uint16_t cond_flag = (instr >> 9) & 0x07;

	if (cond_flag & m_reg[R_PSR])
	{
		m_reg[R_PC] += pc_offset;
	}
}

void LC_3::jmp(uint16_t instr)
{
	uint16_t r1 = (instr >> 6) & 0x07;
	m_reg[R_PC] = m_reg[r1];
}

void LC_3::jsr(uint16_t instr)
{
	uint16_t long_flag = instr >> 11 & 1;
	m_reg[R_R7] = m_reg[R_PC];

	if (long_flag)
	{
		uint16_t long_pc_offset = sign_extend(instr & 0x07FF, 11);
		m_reg[R_PC] += long_pc_offset;
	}
	else
	{
		uint16_t r1 = (instr >> 6) & 0x07;
		m_reg[R_PC] = m_reg[r1];
	}
}

void LC_3::ld(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t pc_Offset = sign_extend(instr & 0x01FF, 9);
	m_reg[r0] = mem_read(m_reg[R_PC] + pc_Offset);
	update_flags(r0);
}

void LC_3::ldi(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	m_reg[r0] = mem_read(mem_read(m_reg[R_PC] + pc_offset));
	update_flags(r0);
}

void LC_3::ldr(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t r1 = (instr >> 6) & 0x07;
	uint16_t offset = sign_extend(instr & 0x3F, 6);
	m_reg[r0] = mem_read(m_reg[r1] + offset);
	update_flags(r0);
}

void LC_3::lea(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t offset = sign_extend(instr & 0x01FF, 9);
	m_reg[r0] = m_reg[R_PC] + offset;
	update_flags(r0);
}

void LC_3::ret(uint16_t instr)
{
	m_reg[R_PC] = m_reg[R_R7];
}

void LC_3::st(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t offset = sign_extend(instr & 0x01FF, 9);
	mem_write(m_reg[R_PC] + offset, m_reg[r0]);
}

void LC_3::sti(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t offset = sign_extend(instr & 0x01FF, 9);
	mem_write(mem_read(m_reg[R_PC] + offset), m_reg[r0]);
}

void LC_3::str(uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x07;
	uint16_t r1 = (instr >> 6) & 0x07;
	uint16_t offset = sign_extend(instr & 0x3f, 6);
	mem_write(m_reg[r1] + offset, m_reg[r0]);
}

void LC_3::trap_getc()
{
	m_reg[R_R0] = (uint16_t)getchar();
}

void LC_3::trap_out()
{
	putc((char)m_reg[R_R0], stdout);
	fflush(stdout);
}

void LC_3::trap_puts()
{
	uint16_t* c = m_memory + m_reg[R_R0];
	while(*c)
	{
		putc((char)*c, stdout);
		++c;
	}
	fflush(stdout);
}

void LC_3::trap_in()
{
	printf("Enter a character: ");
	char c = getchar();
	putc(c, stdout);
	m_reg[R_R0] = (uint16_t)c;
}

void LC_3::trap_putsp()
{
	uint8_t* c = (uint8_t*)m_memory;
	c += m_reg[R_R0] * 2;

	while (*c)
	{
		putc((char)*c, stdout);
		++c;
	}
	fflush(stdout);
}

void LC_3::trap_halt()
{
	puts("HALT");
	fflush(stdout);
	m_running = 0;
}

uint16_t LC_3::check_key()
{
	return WaitForSingleObject(m_hstdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}

uint16_t LC_3::mem_read(uint16_t address)
{
	if(address == MR_KBSR)
	{
		if(check_key())
		{
			m_memory[MR_KBSR] = (1 << 15);
			m_memory[MR_KBDR] = getchar();
		}
		else
		{
			m_memory[MR_KBSR] = 0;
		}
	}
	
	return m_memory[address];
}

void LC_3::mem_write(uint16_t address, uint16_t val)
{
	m_memory[address] = val;
}

void LC_3::read_image_file(FILE* file)
{
	uint16_t origin;
	fread(&origin, sizeof(origin), 1, file);
	origin = swap16(origin);

	uint16_t max_read = UINT16_MAX - origin;
	uint16_t* p = m_memory + origin;
	size_t read = fread(p, sizeof(uint16_t), max_read, file);

	while(read-- > 0)
	{
		*p = swap16(*p);
		++p;
	}
}

int LC_3::read_image(const char* image_path)
{
	FILE* file;
	errno_t err;
	if ((err = fopen_s(&file, image_path, "rb")) != 0)
		return 0;

	read_image_file(file);
	fclose(file);
	return 1;
}

void LC_3::start()
{
	//set the PC to starting position
	m_reg[R_PC] = 0x3000;

	while (m_running)
	{
		uint16_t instr = mem_read(m_reg[R_PC]++);
		uint16_t op = instr >> 12;

		switch (op)
		{
		case OP_BR:
			br(instr);
			break;
		case OP_ADD:
			add(instr);
			break;
		case OP_LD:
			ld(instr);
			break;
		case OP_ST:
			st(instr);
			break;
		case OP_JSR:
			jsr(instr);
			break;
		case OP_AND:
			bitwise_and(instr);
			break;
		case OP_LDR:
			ldr(instr);
			break;
		case OP_STR:
			str(instr);
			break;
		case OP_RTI:

			break;
		case OP_NOT:
			bitwise_not(instr);
			break;
		case OP_LDI:
			ldi(instr);
			break;
		case OP_STI:
			sti(instr);
			break;
		case OP_JMP:
			jmp(instr);
			break;
		case OP_RES:
			abort();
			break;
		case OP_LEA:
			lea(instr);
			break;
		case OP_TRAP:
			switch(instr & 0xFF)
			{
			case TRAP_GETC:
				trap_getc();
				break;
			case TRAP_OUT:
				trap_out();
				break;
			case TRAP_PUTS:
				trap_puts();
				break;
			case TRAP_IN:
				trap_in();
				break;
			case TRAP_PUTSP:
				trap_putsp();
				break;
			case TRAP_HALT:
				trap_halt();
				break;
			}
			break;
		}
	}
}

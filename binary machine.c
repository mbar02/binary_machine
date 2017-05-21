/*
	Binary Machine
	by mbar02
*/

#include <stdio.h>

#define VERSION "0.1"

int main(int arg_c, char* arg_v[]);
int execute();
int w_prog();
int r_file(char* filename);

char string[16]={'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
short int ram[256];
short int ir;
short int acc;
unsigned char pc;
int i;

int main(int arg_c, char* arg_v[])
{
	printf("\nBinary Machine,\n\tby mbar02\n");
	if(arg_c==2&& *arg_v[1]=='h')
	{
		printf("\n\t\tBinary Machine Guide\n");
		printf("To execute a .bmf file, digit \"bm.exe [namefile.bmf]\"\n");
		printf("To create a new programm in the BM Editor, digit \"bm.exe\"\n\n");
		printf("BM Language:\n");
		printf("FEDCBA\t\t98\t\t76543210\n");
		printf("Op.Code\t\tAddr.Mode\tOperand\n\n");
		printf("Operand Codes:\t\t00 0001  ADD\n");
		printf("\t\t\t00 0010  SUB\n");
		printf("\t\t\t00 0100  MUL\n");
		printf("\t\t\t00 1000  DIV\n");
		printf("\t\t\t00 1001  NOT\n");
		printf("\t\t\t00 1010  AND\n");
		printf("\t\t\t00 1011  NAND\n");
		printf("\t\t\t00 1100  OR\n");
		printf("\t\t\t00 1101  NOR\n");
		printf("\t\t\t00 1110  XOR\n");
		printf("\t\t\t00 1111  XNOR\n");
		printf("\t\t\t01 0001  LDA\n");
		printf("\t\t\t01 0010  STA\n");
		printf("\t\t\t10 0001  IN\n");
		printf("\t\t\t10 0010  OUT\n");
		printf("\t\t\t11 0001  JMP\n");
		printf("\t\t\t11 0010  JZ\n");
		printf("\t\t\t11 0011  JNZ\n");
		printf("\t\t\t11 0100  JG\n");
		printf("\t\t\t11 0101  JL\n");
		printf("\t\t\t11 0110  JNL\n");
		printf("\t\t\t11 0111  JNG\n");
		printf("\t\t\t11 1111  HLT\n");
		printf("\nAddressing Mode:\t00  From/To the pointed by Operand RAM word\n");
		printf("\t\t\t01  From the value of Operand\n");
		return 0;
	}
	else if(arg_c==2 && *arg_v[1]=='v')
	{
		printf("\nVersion no: %s\n", VERSION);
		return 0;
	}
	else if(arg_c==1)
	{
		printf("\n\t\t\tSource Editor:\n");
		i=w_prog();
		printf("\n");
		getch();
		return i;
	}
	else if(arg_c==2)
	{
		i=r_file(arg_v[1]);
		printf("\n");
		getch();
		return i;
	}
	return;
}

int w_prog()
{
	for(pc=1; pc!=0; pc++)
	{
		printf("%d:\t", pc);
		scanf("%s", &string[0]);
		for(i=0; i<16; i++)
		{
			ram[pc]*=2;
			ram[pc]+=string[i]-'0';
		}
		if(ram[pc]==-1)
		{
			ram[pc]=0;
			break;
		}
	}
	return execute();
}

int r_file(char* filename)
{
	FILE *prog = fopen(filename, "r");
	for(pc=1; pc!=0; pc++)
	{
		fscanf(prog, "%s", &string[0]);
		if(string[0]==NULL)
			break;
		for(i=0; i<16; i++)
		{
			ram[pc]*=2;
			ram[pc]+=string[i]-'0';
		}
	}
	fflush(prog);
	fclose(prog);
	return execute();
}

int execute()
{
	for(pc=1; pc!=0; pc++)
	{
		ir = ram[pc];
		switch (ir&0xFC00)
		{
			/*
				addressing mode:
					00 op, mem
					01 op, val
					10 error
					11 error
			*/
			case 0x0400:	// +
				if( (ir&0x0300) == 0x0000 )
					acc += ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc += ir&0x00FF;
				else
					return;
			break;

			case 0x0800:	// -
				if( (ir&0x0300) == 0x0000 )
					acc -= ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc -= ir&0x00FF;
				else
					return;
			break;

			case 0x1000:	// *
				if( (ir&0x0300) == 0x0000 )
					acc *= ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc *= ir&0x00FF;
				else
					return;
			break;

			case 0x2000:	// /
				if( (ir&0x0300) == 0x0000 )
				{
					acc /= ram[ir&0x00FF];
					ram[0] = acc%ram[ir&0x00FF];
				}
				else if ( (ir&0x0300) == 0x0100 )
				{
					acc /= ir&0x00FF;
					ram[0] = acc%ir&0x00FF;
				}
				else
					return;
			break;

			case 0x2400:	// NOT
				if( (ir&0x0300) == 0x0000 )
					acc = ~ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc = ~(ir&0x00FF);
				else
					return;
			break;

			case 0x2800:	// AND
				if( (ir&0x0300) == 0x0000 )
					acc &= ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc &= ir&0x00FF;
				else
					return;
			break;

			case 0x2C00:	// NAND
				if( (ir&0x0300) == 0x0000 )
					acc = ~ ( acc & ram[ir&0x00FF] );
				else if ( (ir&0x0300) == 0x0100 )
					acc = ~ ( acc & (ir&0x00FF) );
				else
					return;
			break;

			case 0x3000:	// OR
				if( (ir&0x0300) == 0x0000 )
					acc |= ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc |= ir&0x00FF;
				else
					return;
			break;

			case 0x3400:	// NOR
				if( (ir&0x0300) == 0x0000 )
					acc = ~ ( acc | ram[ir&0x00FF] );
				else if ( (ir&0x0300) == 0x0100 )
					acc = ~ ( acc | (ir&0x00FF) );
				else
					return;
			break;

			case 0x3800:	// XOR
				if( (ir&0x0300) == 0x0000 )
					acc ^= ram[ir&0x00FF];
				else if ( (ir&0x0300) == 0x0100 )
					acc ^= ir&0x00FF;
				else
					return;
			break;

			case 0x3C00:	// XNOR
				if( (ir&0x0300) == 0x0000 )
					acc = ~ ( acc ^ ram[ir&0x00FF] );
				else if ( (ir&0x0300) == 0x0100 )
					acc = ~ ( acc ^ (ir&0x00FF) );
				else
					return;
			break;

			case 0x4400:	// load
				if( (ir&0x0300) == 0x0000 )
					acc = ram[ir&0x00FF];
				else if( (ir&0x0300) == 0x0100 )
					acc = ir&0x00FF;
				else
					return;
			break;

			case 0x4800:	// store
				if( (ir&0x0300) == 0x0000 )
					ram[ir&0x00FF] = acc;
				else
					return;
			break;

			case 0x8400:	// input
				if( (ir&0x0300) == 0x0000 )
				{
					printf("\nInserisci qui un valore: ");
					scanf( "%d", &(ram[ir&0x00FF]) );
				}
				else
					return;
			break;

			case 0x8800:	// output
				if( (ir&0x0300) == 0x0000 )
					printf("\nResult: %d\n", ram[ir&0x00FF]);
				else if( (ir&0x0300) == 0x0100 )
					printf("\nResult: %d", ir&0x00FF);
				else
					return;
			break;

			case 0xC400:	// jump
				if( (ir&0x0300) == 0x0000 )
					pc = ram[ir&0x00FF]-1;
				else if( (ir&0x0300) == 0x0100 )
					pc = (ir&0x00FF)-1;
				else
					return;
			break;

			case 0xC800:	// jz
				if(acc==0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xCC00:	// jnz
				if(acc!=0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xD000:	// jg
				if(acc>0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xD400:	// jl
				if(acc<0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xD800:	// jnl
				if(acc>=0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xDC00:	// jng
				if(acc<=0)
				{
					if( (ir&0x0300) == 0x0000 )
						pc = ram[ir&0x00FF]-1;
					else if( (ir&0x0300) == 0x0100 )
						pc = (ir&0x00FF)-1;
					else
						return;
				}
			break;

			case 0xFC00:	// halt
				return 0;
			break;

			default:
				return;
		}
	}
	return;
}
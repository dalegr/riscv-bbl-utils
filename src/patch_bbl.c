/*
 *   Author: Nikita Ermakov <arei@basealt.ru> <coffe92@gmail.com>
 *   Version: v0.1
 *   Date: 24 Aug 2018
 *   License: GNU GPL v3
 *   Description: this little util allows one to patch a Berkeley Bootloader
 *                binary blob to change the PAYLOAD_END address.
 *                This is a part of riscv-bbl package.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//-----------------------------------
#define BBL_SIZE 0x00200000
#define DWORD_1A_POS 0x31A
#define DWORD_1B_POS 0x31E
#define DWORD_2A_POS 0x338
#define DWORD_2B_POS 0x33C

//-----------------------------------
FILE *fBBL = 0;    // input  file descriptor
FILE *fBBLout = 0; // output file descriptor

//-----------------------------------
unsigned int  read_dword(size_t offset);
unsigned long file_size();
void          print_usage();
void          close_files();

//-----------------------------------
int main(int argc, char **argv)
{
  char *fnameBBL = 0;
  char *onameBBL = 0;

  // read input arguments from the command line
  if (argc > 4)
  {
    for (unsigned int i = 0; i < argc; i++)
    {
      if (strstr(argv[i], "-f")) fnameBBL = argv[i + 1];
      if (strstr(argv[i], "-o")) onameBBL = argv[i + 1];
    }
    if (!fnameBBL || !onameBBL) print_usage();
  }
  else
    print_usage();

  // open input BBL file
  if (!(fBBL = fopen(fnameBBL, "rb")))
  {
    printf("[ERR] can't open a BBL file!\n");
    return -1;
  }
  if (!(fBBLout = fopen(onameBBL, "wb+")))
  {
    printf("[ERR] can't open an output BBL file!\n");
    fclose(fBBL);
    return -1;
  }

  // read the double words from the BBL file
  unsigned int dword1A = read_dword(DWORD_1A_POS);
  unsigned int dword1B = read_dword(DWORD_1B_POS);
  unsigned int dword2A = read_dword(DWORD_2A_POS);
  unsigned int dword2B = read_dword(DWORD_2B_POS);

  // edit double words
  // addi: <-- 12 bit -->   <-- 5 bit --> 000 <-- 5 bit --> 0010011
  //              J       +    src reg     =      dst reg     7 bit instr code
  //
  // auipc: <-- 20 bit -->   <-- 5 bit -->   0010111
  //         offset << 12  =  dst reg = pc     7 bit instr code
  //
  // auipc rd, delta[31:12] + delta[11]   Load absolute address,
  // addi  rd, rd, delta[11:0]            where delta = symbol - pc
  //                                            symbol = BBL_SIZE + fsize - 1
  // For more info one can see RISC-V User-Level ISA
  //                (for ex: V20180801-draft page150)
  unsigned long fsize = file_size();
  unsigned int delta1 = (BBL_SIZE + fsize - 1) - DWORD_1A_POS;
  unsigned int delta2 = (BBL_SIZE + fsize - 1) - DWORD_2A_POS;
  dword1A = (dword1A << 20) >> 20 | ((delta1 >> 12) << 12 + (delta1 & 0x800));
  dword2A = (dword2A << 20) >> 20 | ((delta2 >> 12) << 12 + (delta2 & 0x800));
  dword1B = (dword1B << 12) >> 12 | (delta1 << 20);
  dword2B = (dword2B << 12) >> 12 | (delta2 << 20);

  // write changes to BBL file
  char *buf = (char *)malloc(fsize);
  if (!buf)
  {
    printf("[ERR] can't allocate memory: %lu bytes\n", fsize);
    close_files();
    return -1;
  }
  fseek(fBBL, 0, SEEK_SET);
  fread(buf, fsize, 1, fBBL);
  *((unsigned int *)(buf + DWORD_1A_POS)) = dword1A;
  *((unsigned int *)(buf + DWORD_1B_POS)) = dword1B;
  *((unsigned int *)(buf + DWORD_2A_POS)) = dword2A;
  *((unsigned int *)(buf + DWORD_2B_POS)) = dword2B;
  fwrite(buf, fsize, 1, fBBLout);

  // close BBL file and get the hell out of here!
  close_files();
  free(buf);
  return 0;
}

//-----------------------------------
unsigned int read_dword(size_t offset)
{
  unsigned int dword;
  fseek(fBBL, offset, SEEK_SET);
  fread(&dword, sizeof(unsigned int), 1, fBBL);
  return dword;
}

//-----------------------------------
unsigned long file_size()
{
  fseek(fBBL, 0, SEEK_SET);
  fseek(fBBL, 0, SEEK_END);
  return ftell(fBBL);
}

//-----------------------------------
void print_usage()
{
  printf("Usage: patch_bbl -f PATH_TO_THE_BBL -o OUT_BBL\n");
  exit(-1);
}

//-----------------------------------
void close_files()
{
  fclose(fBBL);
  fclose(fBBLout);
}

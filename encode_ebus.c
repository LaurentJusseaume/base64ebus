
/**
 * `test.c' - b64
 *
 * copyright (c) 2014 joseph werle
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "b64.h"

#define INDEX_OF_NN 4

typedef enum
{
  ENCODE,
  DECODE
} Operation_type_e;

typedef enum
{
  FORMAT_NONE,
  FORMAT_EBUS
} Format_type_e;

static char* prompt[] =
{
    "QQ",
    "ZZ",
    "PB",
    "SB",
    "NN",
    "DATA[0]",
    "DATA[1]",
    "DATA[2]",
    "DATA[3]",
    "DATA[4]",
    "DATA[5]",
    "DATA[6]",
    "DATA[7]",
    "DATA[8]",
    "DATA[9]",
    "DATA[10]",
    "DATA[11]",
    "DATA[12]",
    "DATA[13]",
    "DATA[14]",
    "DATA[15]",
};

unsigned char nibble_to_int(char nibble)
{
  unsigned char value = 0;

  if ((nibble >= '0') && (nibble <= '9'))
  {
    value = nibble - '0';
  }
  else if ((nibble >= 'a') && (nibble <= 'f'))
  {
    value = nibble - 'a' + 0xa;
  }
  else if ((nibble >= 'A') && (nibble <= 'F'))
  {
    value = nibble - 'A' + 0xa;
  }

  return value;
}

int read_param(int argc, char *argv[], Operation_type_e* operation, Format_type_e* format)
{
  int idx;
  int is_operation_set = 0;

  if (argc < 2)
  {
    return -1;
  }

  for (idx = 1; idx < argc; idx++)
  {
    if (argv[idx][0] == '-')
    {
      switch(argv[idx][1])
      {
      case 'd':
        *operation = DECODE;
        is_operation_set = 1;
        break;
      case 'e':
        *operation = ENCODE;
        is_operation_set = 1;
        break;
      case 'f':
        if (argc > idx+1)
        {
          if (strcmp(argv[idx+1], "ebus") == 0)
          {
            *format = FORMAT_EBUS;
            printf("format = ebus\n");
          }
        }
      }
    }
  }

  if (is_operation_set == 0)
  {
    return -1;
  }

  return 0;
}

static unsigned char NN = 10;

int is_end_of_encode_loop(Format_type_e format, int idx, char value)
{
  if (format == FORMAT_EBUS)
  {
    if (idx == INDEX_OF_NN)
    {
      if (value>0x10)
      {
        printf("NN cannot be greater than 0x10\n");
        //TODO: que faire
        return 1;
      }
      NN = value;
      printf("NN=%d\n", NN);
    }

    if (idx == INDEX_OF_NN + NN)
    {
      //printf("idx>4+NN\n");
      return 1;
    }
  }

  return 0;
}

int
main (int argc, char *argv[]) {

  Operation_type_e operation = ENCODE;
  Format_type_e    format    = FORMAT_NONE;

  if (read_param(argc, argv, &operation, &format) != 0)
  {
    printf("Usage:\n %s [option]\n\n", argv[0]);
    printf("options:\n");
    printf("  -e : encode\n");
    printf("  -d : decode\n");
    printf("  -f <format> : optional. Specify the format (accpeted value=ebus)\n");
    return 0;
  }

  if (operation == ENCODE)
  {
    char          string[10];
    unsigned char buffer[100];
    int           idx         = 0;
    int           idx2        = 0;
    unsigned char value       = 0;
    char          *new_buffer = NULL;

    memset(buffer, 0, 100);

    printf("encode\nenter values in hexadecimal (without 0x)\n");
    printf("type '.' to stop entering data\n");

    do
    {
      if (format == FORMAT_EBUS)
      {
        printf("%s: ", prompt[idx]);
      }
      else
      {
        printf("byte %d: ", idx);
      }
      scanf("%s", string);

      if (string[0]=='.')
      {
        break;
      }

      if (strlen(string) == 1)
      {
        value = nibble_to_int(string[0]);
      }
      else
      {
        value = 0x10 * nibble_to_int(string[0]);
        value += nibble_to_int(string[1]);
      }

      buffer[idx++] = (unsigned char)value;
    //}while(idx < 4 + NN);
    } while (!is_end_of_encode_loop(format, idx-1, value));

    printf("buffer to encode: ");
    for (idx2=0; idx2<idx; idx2++)
    {
      printf("%02x ", buffer[idx2]);
    }
    printf("\n");

    new_buffer = b64_encode(buffer, idx);
    printf("encoded buffer : %s\n", new_buffer);
  }
  else if (operation == DECODE)
  {
     char         buffer[100];
    unsigned char *new_buffer = NULL;
    int           idx         = 0;
    size_t        size        = 0;

    printf("enter string to decode : ");
    scanf("%s", buffer);
    new_buffer = b64_decode_ex(buffer, strlen(buffer), &size);
    if (new_buffer != NULL)
    {
      printf("decoded buffer: ");
      for (idx=0; idx<size; idx++)
      {
        printf("%02x ", new_buffer[idx]);
      }
      printf("\n");
    }
  }

  return 0;
}

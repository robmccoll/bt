#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

double
time_now()
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);

  double rtn = ((double)tp.tv_sec) + (((double)tp.tv_nsec) * 0.000000001); 

  return rtn;
}

double
time_since_replace(double * time)
{
  double new = time_now();
  double out = new - *time;
  *time = new;

  return out;
}

int
cmp_end(const char * hst_beg, const char * hst_end,
        const char * ndl_beg, const char * ndl_end)
{
  if((hst_end - hst_beg) < (ndl_end - ndl_beg)) {
    return 0;
  }

  int i = -1;
  for(; ndl_end + i >= ndl_beg; i--) {
    if(hst_end[i] != ndl_end[i]) {
      return 0;
    }
  }

  return 1;
}

int
main(int argc, char *argv[])
{
  if(argc > 1 && (
      (0 == strcmp(argv[1], "-?")) ||
      (0 == strcmp(argv[1], "--help")))) {
    printf("bt - counts the time between occurrences of the given string on stdin\n"
           "   - stdin is consumed.\n"
           "   - output will be the times in floating point seconds, one per line\n"
           "\n"
           "   Usage: bt <string>\n"
           "\n"
           "   If the string is not given, \"\\n\" is used.\n"
           "   Note that bt disables buffering of stdin and operates one\n"
           "   character at a time, but buffering may need to be disabled for the\n"
           "   program generating input data.\n");
    return -1;
  }

  char * string = "\n";

  long inbuf_size = 65536;
  long inbuf_stop = 0;
  char * inbuf = calloc(sizeof(char), inbuf_size);

  if(argc > 1) {
    string = argv[1];
  }

  char * string_end = string;

  while(string_end[0] != '\0') {
    string_end++;
  }

  /* disable buffer on stdin */
  struct termios old_tio, new_tio;
  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

  double last_time = time_now();

  while(!feof(stdin)) {
    if(inbuf_stop >= inbuf_size - 1) {
      inbuf_size *= 1.3;
      inbuf = realloc(inbuf, inbuf_size);
    }
    inbuf[inbuf_stop++] = getchar();
    inbuf[inbuf_stop] = '\0';

    if(cmp_end(inbuf, inbuf + inbuf_stop, string, string_end)) {
      printf("%lf\n", time_since_replace(&last_time));
      inbuf_stop = 0;
    }
  }

  /* reset buffer on stdin */
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

  free(inbuf);
}

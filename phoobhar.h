
int
phoo();

int
bhar();

#ifdef __has_include_next
#  if __has_include_next("phoobhar.h")
#    include_next "phoobhar.h"
#  else
#    error "phoobhar.h"
#  endif
#else
#  error "phoobhar.h"
#endif

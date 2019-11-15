#include "common.h"
#include <unistd.h>
#include "model.h"
#include "snapshot-interface.h"
#include <dlfcn.h>
#include <errno.h>

static int (*pipe_init_p)(int filep[2]) = NULL;

int pipe(int fildes[2]) {
  if (!model) {
    snapshot_system_init(10000, 1024, 1024, 40000);
    model = new ModelChecker();
    model->startChecker();
  }
  if (!pipe_init_p) {
    pipe_init_p = (int (*)(int fildes[2])) dlsym(RTLD_NEXT, "pipe");
    char *error = dlerror();
    if (error != NULL) {
      fputs(error, stderr);
      exit(EXIT_FAILURE);
    }
  }
  return pipe_init_p(fildes);
}

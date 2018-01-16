#include "esp_attr.h"

#include <limits.h>
#include <reent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/mount.h>

extern int __real__stat_r(struct _reent *r, const char *path, int flags, int mode);

int IRAM_ATTR __wrap__stat_r(struct _reent *r, const char *path, int flags, int mode) {
	char *ppath;
	int res;

	if (!path || !*path) {
		errno = ENOENT;
		return -1;
	}

	ppath = mount_resolve_to_physical(path);
	if (ppath) {
		res = __real__stat_r(r, ppath, flags, mode);
		free(ppath);
		return res;
	} else {
		return -1;
	}
}

/******************************************************************************
 *  @file         path.c
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-05-07
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        
 *****************************************************************************/

#include "path.h"
#include <stdlib.h>
#include <string.h>
#include "muggle/c/base/macro.h"
#include "muggle/c/os/path.h"
#include "muggle/c/os/os.h"

#if _WIN32

const char *hpkg_path_home(char *buf, size_t bufsize)
{
	const char *homedir = getenv("USERPROFILE");
	if (homedir == NULL) {
		return NULL;
	}

	memset(buf, 0, bufsize);
	strncpy(buf, homedir, bufsize - 1);

	return buf;
}

#else

const char *hpkg_path_home(char *buf, size_t bufsize)
{
	const char *homedir = getenv("HOME");
	if (homedir == NULL) {
		return NULL;
	}

	memset(buf, 0, bufsize);
	strncpy(buf, homedir, bufsize - 1);

	return buf;
}

#endif

const char *hpkg_path_process_dir(char *buf, size_t bufsize)
{
	char exepath[MUGGLE_MAX_PATH];
	if (muggle_os_process_path(exepath, sizeof(exepath)) != 0) {
		return NULL;
	}

	if (muggle_path_dirname(exepath, buf, bufsize) != 0) {
		return NULL;
	}

	return buf;
}

const char *hpkg_path_expand(const char *filepath, char *buf, size_t bufsize)
{
	const char *curpath = filepath;

	// expand user
	char tmpbuf[MUGGLE_MAX_PATH];
	if (filepath[0] == '~') {
		if (hpkg_path_home(tmpbuf, sizeof(tmpbuf)) == NULL) {
			return NULL;
		}

		size_t len = strlen(tmpbuf);
		if (filepath[1] != '\0') {
			size_t remain = sizeof(tmpbuf) - 1 - len;
			strncpy((char *)tmpbuf + len, &filepath[1], remain);
		}

		curpath = tmpbuf;
	}

	// expand relative to abs path
	if (muggle_path_abspath(curpath, buf, bufsize) != 0) {
		return NULL;
	}

	return buf;
}

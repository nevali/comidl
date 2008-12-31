/*
 * COM IDL Compiler
 * @(#) $Id$
 */

/*
 * Copyright (c) 2008 Mo McRoberts.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the author(s) of this software may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_comidl.h"

/* This is horribly inefficient, but it doesn't really need to be optimised */

typedef struct incpath_struct incpath_t;

struct incpath_struct
{
	char *path;
	int is_framework;
};

static incpath_t **incpath;
static size_t nincpath;

static incpath_t *
incpath_add(const char *path, int isfw)
{
	incpath_t **q, *p;
	size_t l;
	
	if(NULL == (q = (incpath_t **) realloc(incpath, sizeof(incpath_t *) * (nincpath + 1))))
	{
		return NULL;
	}
	incpath = q;
	if(NULL == (p = (incpath_t *) calloc(1, sizeof(incpath_t))))
	{
		return NULL;
	}
	l = strlen(path);
	if(NULL == (p->path = (char *) calloc(1, l + 2)))
	{
		free(p);
		return NULL;
	}
	strcpy(p->path, path);
	if(0 == l || '/' != p->path[l - 1])
	{
		p->path[l] = '/';
		p->path[l + 1] = 0;
	}
	p->is_framework = isfw;
	q[nincpath] = p;
	nincpath++;
	return p;
}

int
idl_incpath_add_includedir(const char *path)
{
	if(NULL == incpath_add(path, 0))
	{
		return -1;
	}
	return 0;
}

int
idl_incpath_add_frameworkdir(const char *path)
{
	if(NULL == incpath_add(path, 1))
	{
		return -1;
	}
	return 0;
}

int
idl_incpath_locate(char *buf, size_t buflen, const char *path)
{
	size_t c;
	char pathbuf[PATH_MAX + 1];
	struct stat sbuf;
	
	for(c = 0; c < nincpath; c++)
	{
		if(strlen(incpath[c]->path) + strlen(path) > PATH_MAX)
		{
			fprintf(stderr, "%s: %s%s: path too long\n", progname, incpath[c]->path, path);
			continue;
		}
		strcpy(pathbuf, incpath[c]->path);
		strcat(pathbuf, path);
		if(0 == stat(pathbuf, &sbuf))
		{
			if(strlen(pathbuf) + 1 > buflen)
			{
				fprintf(stderr, "%s: %s: path too long\n", progname, pathbuf);
				return -1;
			}
			strcpy(buf, pathbuf);
			return 0;
		}
	}
	return -1;
}

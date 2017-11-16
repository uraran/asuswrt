/* 
   Unix SMB/CIFS implementation.
   SMB client library implementation (Old interface compatibility)
   Copyright (C) Andrew Tridgell 1998
   Copyright (C) Richard Sharpe 2000
   Copyright (C) John Terpstra 2000
   Copyright (C) Tom Jansen (Ninja ISD) 2002 
   Copyright (C) Derrell Lipman 2003, 2008
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "includes.h"
#include "libsmb_internal.h"

struct smbc_compat_fdlist {
	SMBCFILE * file;
	int fd;
	struct smbc_compat_fdlist *next, *prev;
};

static SMBCCTX * statcont = NULL;
static int smbc_compat_initialized = 0;
static int smbc_compat_nextfd = 0;
static struct smbc_compat_fdlist * smbc_compat_fd_in_use = NULL;
static struct smbc_compat_fdlist * smbc_compat_fd_avail = NULL;

/* Find an fd and return the SMBCFILE * or NULL on failure */
static SMBCFILE *
find_fd(int fd)
{
	struct smbc_compat_fdlist * f = smbc_compat_fd_in_use;
	while (f) {
		if (f->fd == fd) 
			return f->file;
		f = f->next;
	}
	return NULL;
}

/* Add an fd, returns 0 on success, -1 on error with errno set */
static int
add_fd(SMBCFILE * file)
{
        struct smbc_compat_fdlist * f = smbc_compat_fd_avail;
        
	if (f) {
                /* We found one that's available */
                DLIST_REMOVE(smbc_compat_fd_avail, f);
                
	} else {
                /*
                 * None were available, so allocate one.  Keep the number of
                 * file descriptors determinate.  This allows the application
                 * to allocate bitmaps or mapping of file descriptors based on
                 * a known maximum number of file descriptors that will ever
                 * be returned.
                 */
                if (smbc_compat_nextfd >= FD_SETSIZE) {
                        errno = EMFILE;
                        return -1;
                }
                
                f = SMB_MALLOC_P(struct smbc_compat_fdlist);
                if (!f) {
                        errno = ENOMEM;
                        return -1;
                }
                
                f->fd = SMBC_BASE_FD + smbc_compat_nextfd++;
        }
        
	f->file = file;
	DLIST_ADD(smbc_compat_fd_in_use, f);
        
	return f->fd;
}



/* Delete an fd, returns 0 on success */
static int
del_fd(int fd)
{
	struct smbc_compat_fdlist * f = smbc_compat_fd_in_use;
        
	while (f) {
		if (f->fd == fd) 
			break;
		f = f->next;
	}
        
	if (f) {
		/* found */
		DLIST_REMOVE(smbc_compat_fd_in_use, f);
                f->file = NULL;
                DLIST_ADD(smbc_compat_fd_avail, f);
		return 0;
	}
	return 1;
}



int
smbc_init(smbc_get_auth_data_fn fn,
          int debug)
{
	if (!smbc_compat_initialized) {
		statcont = smbc_new_context();
		if (!statcont) 
			return -1;
                
                smbc_setDebug(statcont, debug);
                smbc_setFunctionAuthData(statcont, fn);
                
		if (!smbc_init_context(statcont)) {
			smbc_free_context(statcont, False);
			return -1;
		}
                
		smbc_compat_initialized = 1;
                
		return 0;
	}
	return 0;
}


SMBCCTX *
smbc_set_context(SMBCCTX * context)
{
        SMBCCTX *old_context = statcont;
        
        if (context) {
                /* Save provided context.  It must have been initialized! */
                statcont = context;
                
                /* You'd better know what you're doing.  We won't help you. */
		smbc_compat_initialized = 1;
        }
        
        return old_context;
}


int
smbc_open(const char *furl,
          int flags,
          mode_t mode)
{
	SMBCFILE * file;
	int fd;
      
        file = smbc_getFunctionOpen(statcont)(statcont, furl, flags, mode);
	if (!file)
		return -1;
        
	fd = add_fd(file);
	if (fd == -1) 
                smbc_getFunctionClose(statcont)(statcont, file);
	return fd;
}


int
smbc_creat(const char *furl,
           mode_t mode)
{
	SMBCFILE * file;
	int fd;
        
        file = smbc_getFunctionCreat(statcont)(statcont, furl, mode);
	if (!file)
		return -1;
        
	fd = add_fd(file);
	if (fd == -1) {
		/* Hmm... should we delete the file too ? I guess we could try */
                smbc_getFunctionClose(statcont)(statcont, file);
                smbc_getFunctionUnlink(statcont)(statcont, furl);
	}
	return fd;
}


ssize_t
smbc_read(int fd,
          void *buf,
          size_t bufsize)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionRead(statcont)(statcont, file, buf, bufsize);
}

ssize_t
smbc_write(int fd,
           const void *buf,
           size_t bufsize)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionWrite(statcont)(statcont, file, buf, bufsize);
}

off_t
smbc_lseek(int fd,
           off_t offset,
           int whence)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionLseek(statcont)(statcont, file, offset, whence);
}

int
smbc_close(int fd)
{
	SMBCFILE * file = find_fd(fd);
	del_fd(fd);
        return smbc_getFunctionClose(statcont)(statcont, file);
}

int
smbc_unlink(const char *fname)
{
        return smbc_getFunctionUnlink(statcont)(statcont, fname);
}

int
smbc_rename(const char *ourl,
            const char *nurl)
{
        return smbc_getFunctionRename(statcont)(statcont, ourl,
                                                statcont, nurl);
}

int
smbc_opendir(const char *durl)
{
	SMBCFILE * file;
	int fd;
        
        file = smbc_getFunctionOpendir(statcont)(statcont, durl);
	if (!file)
		return -1;
        
	fd = add_fd(file);
	if (fd == -1) 
                smbc_getFunctionClosedir(statcont)(statcont, file);
        
	return fd;
}

int
smbc_closedir(int dh) 
{
	SMBCFILE * file = find_fd(dh);
	del_fd(dh);
        return smbc_getFunctionClosedir(statcont)(statcont, file);
}

int
smbc_getdents(unsigned int dh,
              struct smbc_dirent *dirp,
              int count)
{
	SMBCFILE * file = find_fd(dh);
        return smbc_getFunctionGetdents(statcont)(statcont, file, dirp, count);
}

struct smbc_dirent *
smbc_readdir(unsigned int dh)
{
	SMBCFILE * file = find_fd(dh);
        return smbc_getFunctionReaddir(statcont)(statcont, file);
}

off_t
smbc_telldir(int dh)
{
	SMBCFILE * file = find_fd(dh);
        return smbc_getFunctionTelldir(statcont)(statcont, file);
}

int
smbc_lseekdir(int fd,
              off_t offset)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionLseekdir(statcont)(statcont, file, offset);
}

int
smbc_mkdir(const char *durl,
           mode_t mode)
{
        return smbc_getFunctionMkdir(statcont)(statcont, durl, mode);
}

int
smbc_rmdir(const char *durl)
{
        return smbc_getFunctionRmdir(statcont)(statcont, durl);
}

int
smbc_stat(const char *url,
          struct stat *st)
{
        return smbc_getFunctionStat(statcont)(statcont, url, st);
}

int
smbc_fstat(int fd,
           struct stat *st)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionFstat(statcont)(statcont, file, st);
}

int
smbc_statvfs(char *path,
             struct statvfs *st)
{
        return smbc_getFunctionStatVFS(statcont)(statcont, path, st);
}

int
smbc_fstatvfs(int fd,
              struct statvfs *st)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionFstatVFS(statcont)(statcont, file, st);
}

int
smbc_ftruncate(int fd,
               off_t size)
{
	SMBCFILE * file = find_fd(fd);
        return smbc_getFunctionFtruncate(statcont)(statcont, file, size);
}

int
smbc_chmod(const char *url,
           mode_t mode)
{
        return smbc_getFunctionChmod(statcont)(statcont, url, mode);
}

int
smbc_utimes(const char *fname,
            struct timeval *tbuf)
{
        return smbc_getFunctionUtimes(statcont)(statcont, fname, tbuf);
}

#ifdef HAVE_UTIME_H
int
smbc_utime(const char *fname,
           struct utimbuf *utbuf)
{
        struct timeval tv[2];
        
        if (utbuf == NULL)
                return smbc_getFunctionUtimes(statcont)(statcont, fname, NULL);
        
        tv[0].tv_sec = utbuf->actime;
        tv[1].tv_sec = utbuf->modtime;
        tv[0].tv_usec = tv[1].tv_usec = 0;
        
        return smbc_getFunctionUtimes(statcont)(statcont, fname, tv);
}
#endif

int
smbc_setxattr(const char *fname,
              const char *name,
              const void *value,
              size_t size,
              int flags)
{
        return smbc_getFunctionSetxattr(statcont)(statcont,
                                                  fname, name,
                                                  value, size, flags);
}

int
smbc_lsetxattr(const char *fname,
               const char *name,
               const void *value,
               size_t size,
               int flags)
{
        return smbc_getFunctionSetxattr(statcont)(statcont,
                                                  fname, name,
                                                  value, size, flags);
}

int
smbc_fsetxattr(int fd,
               const char *name,
               const void *value,
               size_t size,
               int flags)
{
	SMBCFILE * file = find_fd(fd);
	if (file == NULL) {
		errno = EBADF;
		return -1;
	}
        return smbc_getFunctionSetxattr(statcont)(statcont,
                                                  file->fname, name,
                                                  value, size, flags);
}

int
smbc_getxattr(const char *fname,
              const char *name,
              const void *value,
              size_t size)
{
        return smbc_getFunctionGetxattr(statcont)(statcont,
                                                  fname, name,
                                                  value, size);
}

int
smbc_lgetxattr(const char *fname,
               const char *name,
               const void *value,
               size_t size)
{
        return smbc_getFunctionGetxattr(statcont)(statcont,
                                                  fname, name,
                                                  value, size);
}

int
smbc_fgetxattr(int fd,
               const char *name,
               const void *value,
               size_t size)
{
	SMBCFILE * file = find_fd(fd);
	if (file == NULL) {
		errno = EBADF;
		return -1;
	}
        return smbc_getFunctionGetxattr(statcont)(statcont,
                                                  file->fname, name,
                                                  value, size);
}

int
smbc_removexattr(const char *fname,
                 const char *name)
{
        return smbc_getFunctionRemovexattr(statcont)(statcont, fname, name);
}

int
smbc_lremovexattr(const char *fname,
                  const char *name)
{
        return smbc_getFunctionRemovexattr(statcont)(statcont, fname, name);
}

int
smbc_fremovexattr(int fd,
                  const char *name)
{
	SMBCFILE * file = find_fd(fd);
	if (file == NULL) {
		errno = EBADF;
		return -1;
	}
        return smbc_getFunctionRemovexattr(statcont)(statcont,
                                                     file->fname, name);
}

int
smbc_listxattr(const char *fname,
               char *list,
               size_t size)
{
        return smbc_getFunctionListxattr(statcont)(statcont,
                                                   fname, list, size);
}

int
smbc_llistxattr(const char *fname,
                char *list,
                size_t size)
{
        return smbc_getFunctionListxattr(statcont)(statcont,
                                                   fname, list, size);
}

int
smbc_flistxattr(int fd,
                char *list,
                size_t size)
{
	SMBCFILE * file = find_fd(fd);
	if (file == NULL) {
		errno = EBADF;
		return -1;
	}
        return smbc_getFunctionListxattr(statcont)(statcont,
                                                   file->fname, list, size);
}

int
smbc_print_file(const char *fname,
                const char *printq)
{
        return smbc_getFunctionPrintFile(statcont)(statcont, fname,
                                                   statcont, printq);
}

int
smbc_open_print_job(const char *fname)
{
        SMBCFILE * file;
        
        file = smbc_getFunctionOpenPrintJob(statcont)(statcont, fname);
	if (!file) return -1;
	return file->cli_fd;
}

int
smbc_list_print_jobs(const char *purl,
                     smbc_list_print_job_fn fn)
{
        return smbc_getFunctionListPrintJobs(statcont)(statcont, purl, fn);
}

int
smbc_unlink_print_job(const char *purl,
                      int id)
{
        return smbc_getFunctionUnlinkPrintJob(statcont)(statcont, purl, id);
}


int smbc_parse_path(const char *puri, 
			char *pWorkgroup,
			char *pServer,
			char *pShare,
			char *pPath)
{
	SMBCCTX *context = statcont;
	char *server = NULL;
	char *share = NULL;
	char *user = NULL;
	char *password = NULL;
	char *workgroup = NULL;
	char *path = NULL;
	TALLOC_CTX *frame = talloc_stackframe();

	if (!context || !context->internal->initialized) {
		errno = EINVAL;  /* Best I can think of ... */
		TALLOC_FREE(frame);
		return -1;
	}

	if (!puri) {
		errno = EINVAL;
		TALLOC_FREE(frame);
		return -1;
	}
		
	DEBUG(4, ("smbc_parse_path(%s)\n", puri));
		
	if (SMBC_parse_path(frame,
						context,
						puri,
						&workgroup,
						&server,
						&share,
						&path,
						&user,
						&password,
						NULL)) 
	{
		errno = EINVAL;
		TALLOC_FREE(frame);
		return -1;
	}

	memcpy(pWorkgroup, workgroup, strlen(workgroup));
	memcpy(pServer, server, strlen(server));
	memcpy(pShare, share, strlen(share));
	memcpy(pPath, path, strlen(path));

	TALLOC_FREE(frame);
	return 0;
}

int smbc_check_connectivity(char *puri)
{
	SMBCCTX *context = statcont;
	char *server = NULL;
	char *share = NULL;
	char *user = NULL;
	char *password = NULL;
	char *workgroup = NULL;
	char *path = NULL;
	TALLOC_CTX *frame = talloc_stackframe();

	if (!context || !context->internal->initialized) {
		errno = EINVAL;  /* Best I can think of ... */
		TALLOC_FREE(frame);
		return -1;
	}

	if (SMBC_parse_path(frame,
						context,
						puri,
						&workgroup,
						&server,
						&share,
						&path,
						&user,
						&password,
						NULL)) 
	{
		errno = EINVAL;
		TALLOC_FREE(frame);
		return -1;
	}

	if(server[0] == '\0') {
		//perform LAN NBNS scan
		TALLOC_FREE(frame);
		return 1;
	}

	NTSTATUS status;
	struct cli_state *cli = cli_initialise();
	struct sockaddr_storage ss;

	//- JerryLin add
	cli->timeout = 300;

	zero_sockaddr(&ss);
	cli->port = SMB_PORT1;
	status = cli_connect(cli, server, &ss);

	if(!NT_STATUS_IS_OK(status)) {
		zero_sockaddr(&ss);
		cli->port = SMB_PORT2;
		status = cli_connect(cli, server, &ss);
	}
	cli_shutdown(cli);
//fprintf(stderr, "JerryLin: libsmb_compat.c-->smbc_check_connectivity: %s can %sbe connected..\n", puri, (NT_STATUS_V(status)) ? "not " : "");

	TALLOC_FREE(frame);
	return NT_STATUS_IS_OK(status);
}



int smbc_server_check_creds(
            const char *server,
            const char *share,
            char *workgroup,
            char *username,
            char *password)
{
	SMBCCTX *context = statcont;
//	TALLOC_CTX *frame = talloc_stackframe();
	SMBCSRV *srv=NULL;
	struct cli_state *c;
	struct sockaddr_storage ss;
 	NTSTATUS status;

	zero_sockaddr(&ss);
//fprintf(stderr, "JerryLin: libsmb_compat.c-->smbc_server_check_creds: with server=[%s], share=[%s], user=[%s], passwd=[%s]\n", server, share, username, password);
	/* have to open a new connection */
	if ((c = cli_initialise()) == NULL) {
		errno = ENOMEM;
		return NT_STATUS_V(NT_STATUS_NO_MEMORY);
	}

        if (smbc_getOptionUseKerberos(context)) {
		c->use_kerberos = True;
	}

        if (smbc_getOptionFallbackAfterKerberos(context)) {
		c->fallback_after_kerberos = True;
	}

        if (smbc_getOptionUseCCache(context)) {
		c->use_ccache = True;
	}

	c->timeout = smbc_getTimeout(context);
	c->port = 445;

	status = cli_connect(c, server, &ss);
	if (!NT_STATUS_IS_OK(status)) {		
		cli_shutdown(c);
		errno = ETIMEDOUT;
		return NT_STATUS_V(status);
	}

	DEBUG(4,(" session request ok\n"));

	//- JerryLin add
	//c->use_spnego = True;
	
	status = cli_negprot(c);

	if (!NT_STATUS_IS_OK(status)) {
		fprintf(stderr, "JerryLin: cli_negprot fail: %s\n", nt_errstr(status));
		cli_shutdown(c);
		errno = ETIMEDOUT;
		return NT_STATUS_V(status);
	}
	
	status = cli_session_setup(c, username,
						password,
						strlen(password),
						password,
						strlen(password),
						workgroup);
	if (!NT_STATUS_IS_OK(status)) 
	{
fprintf(stderr, "JerryLin: libsmb_compat.c-->SMBC_server_internal: cli_session_setup fail with username=[%s], password=[%s]\n", username, password);
		cli_shutdown(c);
		errno = EPERM;
		return NT_STATUS_V(status);
	}
	
#if 1
	status = cli_init_creds(c, username, workgroup, password);
	if (!NT_STATUS_IS_OK(status)) {
		
		errno = map_errno_from_nt_status(status);
		cli_shutdown(c);
		return NT_STATUS_V(status);
	}

	DEBUG(4,(" session setup ok\n"));
		
	if(share && share[0] != '\0') {
		/* must be a normal share */
		status = cli_tcon_andx(c, share, "?????", password, strlen(password)+1);
		if (!NT_STATUS_IS_OK(status)) {
			errno = map_errno_from_nt_status(status);
			cli_shutdown(c);
			return NT_STATUS_V(status);
		}

		DEBUG(4,(" tconx ok\n"));
	}
#endif	

	cli_shutdown(c);

	return NT_STATUS_V(NT_STATUS_OK);
}

#include "libsmb_nmblookup.c"

//- JerryLin add
const char* smbc_nmblookup(const char* ip)
{
	TALLOC_CTX *frame = talloc_stackframe();
	char* hostname = NULL;
	query_hostname_byip(ip, &hostname);
	TALLOC_FREE(frame);

	return hostname;
}

/*==== add by Andrew to support SMB-WebDAV ====*/
#include "libsmb_webdav.c"
/*==== end ====*/

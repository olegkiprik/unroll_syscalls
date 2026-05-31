/* x86-64 Linux */

/* compile with -fno-builtin to prevent surprises */

/*
	unsigned char
	unsigned short
	unsigned int
	unsigned long

	rationale: macroing keywords if necessary in future
*/

#if defined(KERNEL_MITIGATION_ERRNO)
#if KERNEL_MITIGATION_ERRNO == 0
#warning "KERNEL_MITIGATION_ERRNO == 0"
#endif
#endif

#define LONG_MAX 0x7fffFFFFffffFFFF
#define ULONG_MAX 0xffffFFFFffffFFFF
#define INT_MAX 0x7fFFffFF
#define UINT_MAX 0xffFFffFF
#define SHRT_MAX 0x7fff
#define USHRT_MAX 0xffff
#define SCHAR_MAX 0x7f
#define UCHAR_MAX 0xff

#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define STDIN_FILENO 0

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define NULL ((void *)0ul)
#if defined(NDEBUG)
#define NDEBUG_FLAG 1
#else
#define NDEBUG_FLAG 0
#endif

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE(x) STRINGIZE(x)

#define LIKELY(x)                                                              \
	(0x1 == __builtin_expect_with_probability((x) ? 1 : 0, 1, 0.9))
#define UNLIKELY(x)                                                            \
	(1 == __builtin_expect_with_probability((x) ? 1 : 0, 0, 0.9))

#define assert(x)                                                              \
	do {                                                                   \
		if (UNLIKELY(!(x))) {                                          \
			if (NDEBUG_FLAG) {                                     \
				__builtin_unreachable();                       \
			} else {                                               \
				(void)bulk_write(                              \
				    NULL, STDERR_FILENO,                       \
				    "\nAssertion failed! Expression \"" #x     \
				    "\" file \"" __FILE__                      \
				    "\" line " STRINGIZE_VALUE(__LINE__) "\n", \
				    priv_strlen(                               \
					"\nAssertion failed! Expression \"" #x \
					"\" file \"" __FILE__                  \
					"\" line " STRINGIZE_VALUE(            \
					    __LINE__) "\n"));                  \
				assert_cleanup();                              \
				sys_exit(EXIT_FAILURE);                        \
			}                                                      \
		}                                                              \
	} while (0 == 1)

static void stream_store_i32(int *dst, int src)
{
	__builtin_ia32_movnti(dst, src);
}

static void stream_store_i64(void *dst, long src)
{
	__builtin_ia32_movnti64(dst, src);
}

/* https://github.com/kraj/musl/blob/kraj/master/arch/x86_64/syscall_arch.h */
static long syscall0(unsigned long n)
{
	long ret;
	asm volatile("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

static long syscall1(unsigned long n, unsigned long a1)
{
	long ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1)
		     : "rcx", "r11", "memory");
	return ret;
}

static long syscall2(unsigned long n, unsigned long a1, unsigned long a2)
{
	long ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1), "S"(a2)
		     : "rcx", "r11", "memory");
	return ret;
}

static long syscall3(unsigned long n, unsigned long a1, unsigned long a2,
		     unsigned long a3)
{
	long ret;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3)
		     : "rcx", "r11", "memory");
	return ret;
}

static long syscall4(unsigned long n, unsigned long a1, unsigned long a2,
		     unsigned long a3, unsigned long a4)
{
	long ret;
	register unsigned long r10 asm("r10") = a4;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10)
		     : "rcx", "r11", "memory");
	return ret;
}

static long syscall5(unsigned long n, unsigned long a1, unsigned long a2,
		     unsigned long a3, unsigned long a4, unsigned long a5)
{
	long ret;
	register unsigned long r10 asm("r10") = a4;
	register unsigned long r8 asm("r8") = a5;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
		     : "rcx", "r11", "memory");
	return ret;
}

static long syscall6(unsigned long n, unsigned long a1, unsigned long a2,
		     unsigned long a3, unsigned long a4, unsigned long a5,
		     unsigned long a6)
{
	long ret;
	register unsigned long r10 asm("r10") = a4;
	register unsigned long r8 asm("r8") = a5;
	register unsigned long r9 asm("r9") = a6;
	asm volatile("syscall"
		     : "=a"(ret)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8),
		       "r"(r9)
		     : "rcx", "r11", "memory");
	return ret;
}

/* io */
#define SYS_WRITE 0x01
#define SYS_READ 0x00
#define SYS_EXIT 0x3c
#define SYS_SYNC 0xa2
#define SYS_OPENAT 0x101
#define SYS_CLOSE 0x03
#define SYS_WRITEV 0x14
#define SYS_READV 0x13
#define SYS_LSEEK 0x08
#define SYS_MSYNC 0x1a
#define SYS_FSYNC 0x4a
#define SYS_FDATASYNC 0x4b
#define SYS_DUP3 0x124
#define SYS_FLOCK 0x49
#define SYS_FCNTL 0x48

/* mem */
#define SYS_MMAP 0x09
#define SYS_MUNMAP 0x0b
#define SYS_MADVISE 0x1c
#define SYS_MLOCK 0x95
#define SYS_MUNLOCK 0x96
#define SYS_MLOCKALL 0x97
#define SYS_MUNLOCKALL 0x98

/* time */
#define SYS_NANOSLEEP 0x23
#define SYS_CLOCK_GETRES 0xe5
#define SYS_GETTIMEOFDAY 0x60
#define SYS_CLOCK_GETTIME 0xe4

/* process, useful */
#define SYS_GETPID 0x27
#define SYS_CLONE 0x38
#define SYS_WAIT4 0x3d
#define SYS_GETPRIORITY 0x8c
#define SYS_SETPRIORITY 0x8d
#define SYS_SETRLIMIT 0xa0
#define SYS_GETRLIMIT 0x61

/* signal */
#define SYS_KILL 0x3e
#define SYS_RT_SIGSUSPEND 0x82
#define SYS_RT_SIGACTION 0x0d

/* network */
#define SYS_SOCKET 0x29
#define SYS_EPOLL_CREATE1 0x123
#define SYS_EPOLL_CTL 0xe9
#define SYS_EPOLL_PWAIT 0x119
#define SYS_BIND 0x31
#define SYS_LISTEN 0x32
#define SYS_CONNECT 0x2a
#define SYS_ACCEPT 0x2b
#define SYS_RECVFROM 0x2d
#define SYS_SENDTO 0x2c
#define SYS_SETSOCKOPT 0x36
#define SYS_GETSOCKOPT 0x37

/* misc */
#define SYS_GETPPID 0x6e
#define SYS_GETUID 0x66
#define SYS_GETEUID 0x6b
#define SYS_GETGROUPS 0x73
#define SYS_GETGID 0x68
#define SYS_GETEGID 0x6c
#define SYS_SETUID 0x69
#define SYS_SETGID 0x6a
#define SYS_SETGROUPS 0x74
#define SYS_PTRACE 0x65
#define SYS_GETSID 0x7c
#define SYS_GETPGID 0x79
#define SYS_SETSID 0x70
#define SYS_SETPGID 0x6d
#define SYS_SYSLOG 0x67
#define SYS_SHUTDOWN 0x30
#define SYS_SOCKETPAIR 0x35
#define SYS_PSELECT6 0x10e
#define SYS_LINKAT 0x109
#define SYS_SYMLINKAT 0x10a
#define SYS_FCHMOD 0x5b
#define SYS_TRUNCATE 0x4c
#define SYS_MKDIRAT 0x102
#define SYS_IOCTL 0x10
#define SYS_RENAMEAT 0x108
#define SYS_EXECVE 0x3b
#define SYS_FTRUNCATE 0x4d
#define SYS_MQ_OPEN 0xf0
#define SYS_MQ_UNLINK 0xf1
#define SYS_MQ_GETSETATTR 0xf5
#define SYS_MQ_NOTIFY 0xf4
#define SYS_MKNODAT 0x103
#define SYS_FSTAT 0x05
#define SYS_GETCWD 0x4f
#define SYS_CHROOT 0xa1
#define SYS_CHDIR 0x50
#define SYS_UNLINKAT 0x107
#define SYS_UMASK 0x5f

#define EOF (-1)

#define EINTR 0x4

static unsigned long priv_strlen(const char *s)
{
	unsigned long result;

	result = 0;
	while (*s != '\0') {
		++result;
		++s;
	}
	return result;
}

static void assert_cleanup()
{
	/* cleanup before exit */
}

static void sys_exit(int status)
{
	(void)syscall1(SYS_EXIT, (long)status);
}

static int sys_write(long *restrict result, int fd, const void *restrict data,
		     unsigned long nbytes);

static int sys_read(long *restrict result, int fd, void *restrict buf,
		    unsigned long nbytes);

static int bulk_write(long *restrict result, int fd, const void *restrict data,
		      unsigned long nbytes)
{
	long res;
	long nbytes_succeeded;
	int err;
	char *curr_data;

	assert(nbytes != 0);
	assert(data != NULL);
	assert(fd >= 0);

	nbytes_succeeded = 0;
	curr_data = (char *)data;

	while (1 == 1) {
		err = sys_write(&res, fd, curr_data, nbytes);
		if (UNLIKELY(err != 0)) {
			if (LIKELY(-err == EINTR)) {
				continue;
			} else {
				break;
			}
		} else {
#if defined(KERNEL_MITIGATION_ERRNO)
			if (UNLIKELY(res < 0 || res > nbytes)) {
				err = -KERNEL_MITIGATION_ERRNO;
				break;
			}
#else
			assert(res >= 0 && res <= nbytes);
#endif
			if (LIKELY(res == nbytes)) {
				break;
			}
			curr_data += res;
			nbytes_succeeded += res;
			nbytes -= res;
		}
	}

	if (UNLIKELY(result != NULL)) {
		*result = nbytes_succeeded;
	}
	return err;
}

static int bulk_read(long *restrict result, int fd, void *restrict buf,
		     unsigned long nbytes)
{
	long res;
	long nbytes_succeeded;
	int err;
	char *curr_buf;

	assert(nbytes != 0);
	assert(buf != NULL);
	assert(fd >= 0);

	nbytes_succeeded = 0;
	curr_buf = (char *)buf;

	while (1 == 1) {
		err = sys_read(&res, fd, curr_buf, nbytes);
		if (UNLIKELY(err != 0)) {
			if (LIKELY(-err == EINTR)) {
				continue;
			} else {
				break;
			}
		} else {
#if defined(KERNEL_MITIGATION_ERRNO)
			if (UNLIKELY(res < 0 || res > nbytes)) {
				err = -KERNEL_MITIGATION_ERRNO;
				break;
			}
#else
			assert(res >= 0 && res <= nbytes);
#endif
			if (LIKELY(res == nbytes)) {
				break;
			}
			curr_buf += res;
			nbytes_succeeded += res;
			nbytes -= res;
		}
	}

	if (UNLIKELY(result != NULL)) {
		*result = nbytes_succeeded;
	}
	return err;
}

static int sys_write(long *restrict result, int fd, const void *restrict data,
		     unsigned long nbytes)
{
	long rax;

	assert(data != NULL);
	assert(fd >= 0);

	rax =
	    syscall3(SYS_WRITE, (unsigned long)fd, (unsigned long)data, nbytes);
	if (UNLIKELY(rax < 0 && rax > -0x1000)) {
		return (int)rax;
	} else if (LIKELY(result != NULL)) {
		*result = rax;
	}
	return 0;
}

static int sys_read(long *restrict result, int fd, void *restrict buf,
		    unsigned long nbytes)
{
	long rax;

	assert(buf != NULL);
	assert(fd >= 0);

	rax = syscall3(SYS_READ, (unsigned long)fd, (unsigned long)buf, nbytes);
	if (UNLIKELY(rax < 0 && rax > -0x1000)) {
		return (int)rax;
	} else if (LIKELY(result != NULL)) {
		*result = rax;
	}
	return 0;
}

static void sys_sync(void)
{
	(void)syscall0(SYS_SYNC);
}

static float sinf(float x)
{
	float result;
	float a;
	int sign;
	unsigned long b;
	unsigned int i;

	assert(x == x);

	a = x;
	b = 1;
	sign = 1;
	result = 0;

	for (i = 0; i < 8; i += 2) {
		result += a / b * sign;
		a *= x * x;
		b *= (i + 2) * (i + 3);
		sign = -sign;
	}

	return result;
}

static void memcpy(void *restrict dst, const void *restrict src,
		   unsigned long len)
{
	char *p_dst;
	char *p_src;

	assert(dst != NULL);
	assert(src != NULL);
	assert(len > 0);

	p_dst = (char *)dst;
	p_src = (char *)src;

	while (len != 0) {
		*p_dst = *p_src;
		++p_dst;
		++p_src;
		--len;
	}
}

static unsigned long strlen(const char *s)
{
	unsigned long result;

	result = 0;
	while (*s != '\0') {
		assert(result != ULONG_MAX);
		++result;
		++s;
	}
	return result;
}

#define STR_ENTER_CHARACTERS "Enter 3 characters:"
#define STR_HERE_ARE_CHARACTERS "Here are the entered characters:"

int main(int argc, char **argv)
{
	int err;
	char buf[64];

#if defined(KERNEL_MITIGATION_ERRNO)
	assert(0 != strlen(STRINGIZE_VALUE(KERNEL_MITIGATION_ERRNO)));
#endif

	assert(sizeof buf >= 3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS));

	memcpy(buf, "\n" STR_HERE_ARE_CHARACTERS " ???\n",
	       3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS));

	err = bulk_write(NULL, STDOUT_FILENO, "\n" STR_ENTER_CHARACTERS "\n",
			 2 + strlen(STR_ENTER_CHARACTERS));
	if (UNLIKELY(err != 0)) {
		(void)bulk_write(NULL, STDERR_FILENO, "\nError\n", 7);
		return EXIT_FAILURE;
	}
	err = bulk_read(NULL, STDIN_FILENO,
			buf + 1 + strlen(STR_HERE_ARE_CHARACTERS) + 1, 3);
	if (UNLIKELY(err != 0)) {
		(void)bulk_write(NULL, STDERR_FILENO, "\nError\n", 7);
		return EXIT_FAILURE;
	}
	err = bulk_write(NULL, STDOUT_FILENO, buf,
			 3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS));
	if (UNLIKELY(err != 0)) {
		(void)bulk_write(NULL, STDERR_FILENO, "\nError\n", 7);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

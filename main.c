/* x86-64 or Aarch64 Linux */

/* compile with -fno-builtin to prevent surprises */

/*
	signed char
	char
	short
	int
	long

	unsigned char
	unsigned short
	unsigned int
	unsigned long

	rationale: macroing keywords if necessary in future
*/

/* sys_calls: check < 0 for errors */

#define asm __asm__

#if defined(EFINE_X86_64)
#endif

#if defined(EFINE_AARCH64)
#endif

#if defined(KERNEL_MITIGATION_ERRNO)
#if KERNEL_MITIGATION_ERRNO == 0
#error "KERNEL_MITIGATION_ERRNO == 0"
#endif
#endif

#if defined(STDIO_DESCRIPTOR_NO_HACKING)
#endif

#define LONG_MAX 0x7fffFFFFffffFFFFl
#define ULONG_MAX 0xffffFFFFffffFFFFul
#define INT_MAX 0x7fFFffFFl
#define UINT_MAX 0xffFFffFFl
#define SHRT_MAX 0x7fffL
#define USHRT_MAX 0xffffL
#define SCHAR_MAX 0x7fL
#define UCHAR_MAX 0xffL

#define FLT_MIN __FLT_MIN__
#define INT_PRECISE_FLT_MAX ((long)(1ul << 24))
#define FLT_MAX (1 / FLT_MIN)

#define DBL_MIN __DBL_MIN__
#define LONG_PRECISE_DBL_MAX ((long)(1ul << 53))
#define DBL_MAX (1 / DBL_MIN)

#define DBL_TO_FLT_MAX 3.402823e+38

#define M_PI 3.14159265358979323846

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

#define OPTIONAL_ALIGNAS(x) _Alignas((x))

#define OFTEN(x) (__builtin_expect_with_probability((x), 1, 0.9))
#define RARELY(x) (__builtin_expect_with_probability((x), 0, 0.9))

#define MORE_LIKELY(x) (__builtin_expect_with_probability((x), 1, 0.6))
#define LESS_LIKELY(x) (__builtin_expect_with_probability((x), 0, 0.6))

#define ASSERT(x)                                                              \
	do {                                                                   \
		if (RARELY(!(x))) {                                            \
			if (NDEBUG_FLAG) {                                     \
				__builtin_unreachable();                       \
			} else {                                               \
				(void)write_no_eintr(                          \
				    STDERR_FILENO,                             \
				    "\nAssertion failed! Expression \"" #x     \
				    "\" file \"" __FILE__                      \
				    "\" line " STRINGIZE_VALUE(__LINE__) "\n", \
				    priv_strlen(                               \
					"\nAssertion failed! Expression \"" #x \
					"\" file \"" __FILE__                  \
					"\" line " STRINGIZE_VALUE(            \
					    __LINE__) "\n"),                   \
				    NULL);                                     \
				assert_cleanup();                              \
				sys_exit(EXIT_FAILURE);                        \
			}                                                      \
		}                                                              \
	} while (0 == 1)

/* p is a non-constant l-value of type ((const) void**) that is evaluated
 * multiple times
 */
#define ASSUME_ALIGNED_DEREF(p, alignment)                                     \
	do {                                                                   \
		if (NDEBUG_FLAG) {                                             \
			*(p) = __builtin_assume_aligned(*(p), (alignment), 0); \
		} else {                                                       \
			if ((unsigned long)*(p) % (alignment) != 0) {          \
				(void)write_no_eintr(                          \
				    STDERR_FILENO,                             \
				    "\nAssertion failed! Address "             \
				    "dereferenced from \"" #p                  \
				    "\" is not multiple of (" #alignment       \
				    ") file \"" __FILE__                       \
				    "\" line " STRINGIZE_VALUE(__LINE__) "\n", \
				    priv_strlen(                               \
					"\nAssertion failed! Address "         \
					"dereferenced from \"" #p              \
					"\" is not multiple of (" #alignment   \
					") file \"" __FILE__                   \
					"\" line " STRINGIZE_VALUE(            \
					    __LINE__) "\n"),                   \
				    NULL);                                     \
				assert_cleanup();                              \
				sys_exit(EXIT_FAILURE);                        \
			}                                                      \
		}                                                              \
	} while (0 == 1)

typedef union unn_syscall_result_ {
	long l;
	unsigned long ul;
	void *p;
} unn_syscall_result;

/* borrowed from musl.
   https://github.com/kraj/musl/blob/kraj/master/arch/ */

#if defined(EFINE_X86_64)

static void syscall0(unsigned long n, unn_syscall_result *res)
{
	asm volatile("syscall" : "=a"(*res) : "a"(n) : "rcx", "r11", "memory");
}

static void syscall1(unsigned long a1, unsigned long n, unn_syscall_result *res)
{
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1)
		     : "rcx", "r11", "memory");
}

static void syscall2(unsigned long a1, unsigned long a2, unsigned long n,
		     unn_syscall_result *res)
{
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1), "S"(a2)
		     : "rcx", "r11", "memory");
}

static void syscall3(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long n, unn_syscall_result *res)
{
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3)
		     : "rcx", "r11", "memory");
}

static void syscall4(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long n, unn_syscall_result *res)
{
	register unsigned long r10 asm("r10") = a4;
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10)
		     : "rcx", "r11", "memory");
}

static void syscall5(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long n,
		     unn_syscall_result *res)
{
	register unsigned long r10 asm("r10") = a4;
	register unsigned long r8 asm("r8") = a5;
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
		     : "rcx", "r11", "memory");
}

static void syscall6(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long a6,
		     unsigned long n, unn_syscall_result *res)
{
	register unsigned long r10 asm("r10") = a4;
	register unsigned long r8 asm("r8") = a5;
	register unsigned long r9 asm("r9") = a6;
	asm volatile("syscall"
		     : "=a"(*res)
		     : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8),
		       "r"(r9)
		     : "rcx", "r11", "memory");
}

/* io */
#define SYS_WRITE 0x01
#define SYS_READ 0x00
#define SYS_OPENAT 0x101
#define SYS_CLOSE 0x03
#define SYS_LSEEK 0x08
#define SYS_SYNC 0xa2
#define SYS_MSYNC 0x1a
#define SYS_FSYNC 0x4a
#define SYS_FDATASYNC 0x4b

/* mem */
#define SYS_MMAP 0x09
#define SYS_MUNMAP 0x0b
#define SYS_MADVISE 0x1c
#define SYS_MLOCKALL 0x97
#define SYS_MUNLOCKALL 0x98

/* time */
#define SYS_NANOSLEEP 0x23
#define SYS_CLOCK_GETTIME 0xe4 /* not on 32 bit */

/* process */
#define SYS_EXIT 0x3c
#define SYS_GETPID 0x27
#define SYS_CLONE3 0x1b3
#define SYS_WAIT4 0x3d

/* signal */
#define SYS_KILL 0x3e
#define SYS_RT_SIGACTION 0x0d

/* NOT implemented yet: */

/* network */
#define SYS_SOCKET 0x29
#define SYS_BIND 0x31
#define SYS_LISTEN 0x32
#define SYS_CONNECT 0x2a
#define SYS_ACCEPT 0x2b
#define SYS_SHUTDOWN 0x30
#define SYS_SETSOCKOPT 0x36
#define SYS_GETSOCKOPT 0x37
#define SYS_EPOLL_CREATE1 0x123
#define SYS_EPOLL_CTL 0xe9
#define SYS_EPOLL_PWAIT 0x119
#define SYS_RECVFROM 0x2d
#define SYS_SENDTO 0x2c

/* more io */
#define SYS_DUP3 0x124
#define SYS_FLOCK 0x49
#define SYS_FCNTL 0x48
#define SYS_WRITEV 0x14
#define SYS_READV 0x13
#define SYS_UNLINKAT 0x107
#define SYS_TRUNCATE 0x4c
#define SYS_FTRUNCATE 0x4d

/* more mem */
#define SYS_MLOCK 0x95
#define SYS_MUNLOCK 0x96

/* more time */
#define SYS_CLOCK_GETRES 0xe5
#define SYS_GETTIMEOFDAY 0x60

/* more process */
#define SYS_GETPRIORITY 0x8c
#define SYS_SETPRIORITY 0x8d
#define SYS_SETRLIMIT 0xa0
#define SYS_GETRLIMIT 0x61

/* more signal */
#define SYS_RT_SIGSUSPEND 0x82

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
#define SYS_SOCKETPAIR 0x35
#define SYS_PSELECT6 0x10e
#define SYS_LINKAT 0x109
#define SYS_SYMLINKAT 0x10a
#define SYS_FCHMOD 0x5b
#define SYS_MKDIRAT 0x102
#define SYS_IOCTL 0x10
#define SYS_RENAMEAT 0x108
#define SYS_EXECVE 0x3b
#define SYS_MQ_OPEN 0xf0
#define SYS_MQ_UNLINK 0xf1
#define SYS_MQ_GETSETATTR 0xf5
#define SYS_MQ_NOTIFY 0xf4
#define SYS_MKNODAT 0x103
#define SYS_FSTAT 0x05
#define SYS_GETCWD 0x4f
#define SYS_CHROOT 0xa1
#define SYS_CHDIR 0x50
#define SYS_UMASK 0x5f
#define SYS_FUTEX 0xca

static void disable_denormals(void)
{
	unsigned int tmp;
	asm volatile("stmxcsr %0" : "=m"(tmp)::"cc", "memory");
	tmp |= 0x8040;
	asm volatile("ldmxcsr %0" : : "m"(tmp) : "cc", "memory");
}

#elif defined(EFINE_AARCH64)

static void syscall0(unsigned long n, unn_syscall_result *res)
{
	register unsigned long x0 asm("x0");
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0" : "=r"(x0) : "r"(x8) : "cc", "memory");
	res->ul = x0;
}

static void syscall1(unsigned long a1, unsigned long n, unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0" : "=r"(x0) : "r"(x8), "0"(x0) : "cc", "memory");
	res->ul = x0;
}

static void syscall2(unsigned long a1, unsigned long a2, unsigned long n,
		     unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x1 asm("x1") = a2;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0"
		     : "=r"(x0)
		     : "r"(x8), "0"(x0), "r"(x1)
		     : "cc", "memory");
	res->ul = x0;
}

static void syscall3(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long n, unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x1 asm("x1") = a2;
	register unsigned long x2 asm("x2") = a3;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0"
		     : "=r"(x0)
		     : "r"(x8), "0"(x0), "r"(x1), "r"(x2)
		     : "cc", "memory");
	res->ul = x0;
}

static void syscall4(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long n, unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x1 asm("x1") = a2;
	register unsigned long x2 asm("x2") = a3;
	register unsigned long x3 asm("x3") = a4;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0"
		     : "=r"(x0)
		     : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3)
		     : "cc", "memory");
	res->ul = x0;
}

static void syscall5(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long n,
		     unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x1 asm("x1") = a2;
	register unsigned long x2 asm("x2") = a3;
	register unsigned long x3 asm("x3") = a4;
	register unsigned long x4 asm("x4") = a5;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0"
		     : "=r"(x0)
		     : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4)
		     : "cc", "memory");
	res->ul = x0;
}

static void syscall6(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long a6,
		     unsigned long n, unn_syscall_result *res)
{
	register unsigned long x0 asm("x0") = a1;
	register unsigned long x1 asm("x1") = a2;
	register unsigned long x2 asm("x2") = a3;
	register unsigned long x3 asm("x3") = a4;
	register unsigned long x4 asm("x4") = a5;
	register unsigned long x5 asm("x5") = a6;
	register unsigned long x8 asm("x8") = n;
	asm volatile("svc 0"
		     : "=r"(x0)
		     : "r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4),
		       "r"(x5)
		     : "cc", "memory");
	res->ul = x0;
}

/* io */
#define SYS_WRITE 0x40
#define SYS_READ 0x3f
#define SYS_OPENAT 0x38
#define SYS_CLOSE 0x39
#define SYS_LSEEK 0x3e
#define SYS_SYNC 0x51
#define SYS_MSYNC 0xe3
#define SYS_FSYNC 0x52
#define SYS_FDATASYNC 0x53

/* mem */
#define SYS_MMAP 0xde
#define SYS_MUNMAP 0xd7
#define SYS_MADVISE 0xe9
#define SYS_MLOCKALL 0xe6
#define SYS_MUNLOCKALL 0xe7

/* time */
#define SYS_NANOSLEEP 0x65
#define SYS_CLOCK_GETTIME 0x71

/* process */
#define SYS_EXIT 0x5d
#define SYS_GETPID 0xac
#define SYS_CLONE3 0x1b3
#define SYS_WAIT4 0x104

/* signal */
#define SYS_KILL 0x81
#define SYS_RT_SIGACTION 0x86

#else

static void syscall0(unsigned long n, unn_syscall_result *res);

static void syscall1(unsigned long a1, unsigned long n,
		     unn_syscall_result *res);

static void syscall2(unsigned long a1, unsigned long a2, unsigned long n,
		     unn_syscall_result *res);

static void syscall3(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long n, unn_syscall_result *res);

static void syscall4(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long n,
		     unn_syscall_result *res);

static void syscall5(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long n,
		     unn_syscall_result *res);

static void syscall6(unsigned long a1, unsigned long a2, unsigned long a3,
		     unsigned long a4, unsigned long a5, unsigned long a6,
		     unsigned long n, unn_syscall_result *res);

/* io */
#define SYS_WRITE 0
#define SYS_READ 0
#define SYS_OPENAT 0
#define SYS_CLOSE 0
#define SYS_LSEEK 0
#define SYS_SYNC 0
#define SYS_MSYNC 0
#define SYS_FSYNC 0
#define SYS_FDATASYNC 0

/* mem */
#define SYS_MMAP 0
#define SYS_MUNMAP 0
#define SYS_MADVISE 0
#define SYS_MLOCKALL 0
#define SYS_MUNLOCKALL 0

/* time */
#define SYS_NANOSLEEP 0
#define SYS_CLOCK_GETTIME 0

/* process */
#define SYS_EXIT 0
#define SYS_GETPID 0
#define SYS_CLONE3 0
#define SYS_WAIT4 0

/* signal */
#define SYS_KILL 0
#define SYS_RT_SIGACTION 0

#endif

#define EOF (-1)

#define AT_FDCWD (-100)

#define O_APPEND 0x400
#define O_CLOEXEC 0x80000
#define O_CREAT 0x40
#define O_DIRECTORY 0x10000
#define O_DSYNC 0x1000
#define O_EXCL 0x80
#define O_NOCTTY 0x100
#define O_NOFOLLOW 0x20000
#define O_NONBLOCK 0x800
#define O_NDELAY O_NONBLOCK
#define O_SYNC 0x101000
#define O_TRUNC 0x200

#define O_RDONLY 0x0
#define O_WRONLY 0x1
#define O_RDWR 0x2

#define S_IRWXU 0x1c0
#define S_IRUSR 0x100
#define S_IWUSR 0x080
#define S_IXUSR 0x040
#define S_IRWXG 0x038
#define S_IRGRP 0x020
#define S_IWGRP 0x010
#define S_IXGRP 0x008
#define S_IRWXO 0x007
#define S_IROTH 0x004
#define S_IWOTH 0x002
#define S_IXOTH 0x001

#define PROT_NONE 0x0
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4

#define MAP_PRIVATE 0x2
#define MAP_ANONYMOUS 0x20
#define MAP_LOCKED 0x2000

#define MADV_NORMAL 0x0
#define MADV_RANDOM 0x1
#define MADV_SEQUENTIAL 0x2
#define MADV_WILLNEED 0x3
#define MADV_DONTNEED 0x4

#define MCL_CURRENT 0x1
#define MCL_FUTURE 0x2

#define MS_SYNC 0x4
#define MS_INVALIDATE 0x2

#define SIGHUP 0x1
#define SIGINT 0x2
#define SIGQUIT 0x3
#define SIGKILL 0x9
#define SIGSEGV 0xb
#define SIGTERM 0xf
#define SIGSTOP 0x13

#define SIG_DFL ((void (*)(int))0x0)
#define SIG_IGN ((void (*)(int))0x1)

#define SA_RESETHAND (0x7fffFFFFl + 1)
#define SA_SIGINFO 0x4

#define WNOHANG 0x1
#define WCONTINUED 0x8
#define WUNTRACED 0x2

#define SEEK_CUR 0x1
#define SEEK_END 0x2
#define SEEK_SET 0x0

#define STDOUT_FILENO 0x1
#define STDERR_FILENO 0x2
#define STDIN_FILENO 0x0

#define EINTR 0x4
#define EINVAL 0x16

static unsigned long priv_strlen(const char *s)
{
	unsigned long result;

	result = 0;
	while (MORE_LIKELY(*s != '\0')) {
		++result;
		++s;
	}
	return result;
}

static void assert_cleanup(void)
{
	/* cleanup before exit */
}

static void sys_exit(int status)
{
	/* too long assembly because of 'int' */
	unn_syscall_result rax;
	syscall1((long)status, SYS_EXIT, &rax);
	(void)rax;
}

static long sys_write(long fd, const void *restrict data, unsigned long nbytes,
		      long *restrict result);

static long write_no_eintr(long fd, const void *restrict data,
			   unsigned long nbytes, long *restrict result)
{
	long res;
	long nbytes_succeeded;
	long err;
	char *curr_data;

	ASSERT(nbytes != 0);
	ASSERT(data != NULL);
	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

	nbytes_succeeded = 0;
	curr_data = (char *)data;

	while (1 == 1) {
		err = sys_write(fd, curr_data, nbytes, &res);

		if (RARELY(err < 0)) {
			if (MORE_LIKELY(-err == EINTR)) {
				continue;
			} else {
				break;
			}
		} else {
			if (OFTEN((unsigned long)res == nbytes)) {
				break;
			}
			curr_data += res;
			nbytes_succeeded += res;
			nbytes -= res;
		}
	}

	if (LESS_LIKELY(result != NULL)) {
		*result = nbytes_succeeded;
	}
	return err;
}

static void memcpy(void *restrict dst, const void *restrict src,
		   unsigned long len)
{
	char *p_dst;
	char *p_src;

	ASSERT(dst != NULL);
	ASSERT(src != NULL);
	ASSERT(len > 0);

	p_dst = (char *)dst;
	p_src = (char *)src;

	while (len != 0) {
		*p_dst = *p_src;
		++p_dst;
		++p_src;
		--len;
	}
}

static long sys_write(long fd, const void *restrict data, unsigned long nbytes,
		      long *restrict result)
{
	unn_syscall_result rax;

	ASSERT(data != NULL);
	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	ASSERT(fd != STDIN_FILENO);
#endif

	syscall3((unsigned long)(long)fd, (unsigned long)data, nbytes,
		 SYS_WRITE, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax < 0 || (unsigned long)rax > nbytes)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);
	ASSERT((unsigned long)rax.l <= nbytes);

	if (MORE_LIKELY(result != NULL)) {
		*result = rax.l;
	}
	return rax.l;
}

static long sys_read(long fd, void *restrict buf, unsigned long nbytes,
		     long *restrict result)
{
	unn_syscall_result rax;

	ASSERT(buf != NULL);
	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	ASSERT(fd != STDOUT_FILENO);
	ASSERT(fd != STDERR_FILENO);
#endif

	syscall3((unsigned long)(long)fd, (unsigned long)buf, nbytes, SYS_READ,
		 &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax < 0 || (unsigned long)rax > nbytes)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);
	ASSERT((unsigned long)rax.l <= nbytes);

	if (OFTEN(result != NULL)) {
		*result = rax.l;
	}
	return rax.l;
}

static long read_no_eintr(long fd, void *restrict buf, unsigned long nbytes,
			  long *restrict result)
{
	long res;
	long nbytes_succeeded;
	long err;
	char *curr_buf;

	ASSERT(nbytes != 0);
	ASSERT(buf != NULL);
	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

	nbytes_succeeded = 0;
	curr_buf = (char *)buf;

	while (1 == 1) {
		err = sys_read(fd, curr_buf, nbytes, &res);
		if (RARELY(err < 0)) {
			if (MORE_LIKELY(-err == EINTR)) {
				continue;
			} else {
				break;
			}
		} else {
			if (OFTEN((unsigned long)res == nbytes)) {
				break;
			}
			curr_buf += res;
			nbytes_succeeded += res;
			nbytes -= res;
		}
	}

	if (MORE_LIKELY(result != NULL)) {
		*result = nbytes_succeeded;
	}
	return err;
}

static void sys_sync(void)
{
	unn_syscall_result rax;
	(void)syscall0(SYS_SYNC, &rax);
	(void)rax;
}

static long sys_lseek(long fd, long offset, long whence, long *result)
{
	unn_syscall_result rax;

	ASSERT(fd >= 0);
	ASSERT(whence >= 0);
	ASSERT(fd <= INT_MAX);
	ASSERT(whence <= INT_MAX);

	syscall3((unsigned long)fd, (unsigned long)offset,
		 (unsigned long)whence, SYS_LSEEK, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax <= -0x1000)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);
	if (OFTEN(result != NULL)) {
		*result = rax.l;
	}
	return rax.l;
}

static long sys_clone3(void *restrict args, unsigned long size,
		       int *restrict result)
{
	unn_syscall_result rax;

	ASSERT(args != NULL);
	if (args != NULL) {
		ASSUME_ALIGNED_DEREF(&args, sizeof(long));
	}

	syscall2((unsigned long)args, size, SYS_CLONE3, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax <= -0x1000 || rax > INT_MAX)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);
	ASSERT(rax.l <= INT_MAX);

	if (OFTEN(result != NULL)) {
		*result = (int)rax.l;
	}
	return rax.l;
}

static long sys_clone3_verbose(unsigned long flags, void *restrict pidfd,
			       void *restrict child_tid,
			       void *restrict parent_tid,
			       unsigned long exit_signal, void *restrict stack,
			       unsigned long stack_size, void *restrict tls,
			       void *restrict set_tid,
			       unsigned long set_tid_size, unsigned long cgroup,
			       unsigned long n_args, int *restrict result)
{
	unsigned long args[11];
	void *tmp;

	ASSERT(n_args <= 11);

	tmp = (void *)args;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	if (set_tid != NULL) {
		ASSUME_ALIGNED_DEREF(&set_tid, 4);
	}
	if (parent_tid != NULL) {
		ASSUME_ALIGNED_DEREF(&parent_tid, 4);
	}
	if (child_tid != NULL) {
		ASSUME_ALIGNED_DEREF(&child_tid, 4);
	}
	if (pidfd != NULL) {
		ASSUME_ALIGNED_DEREF(&pidfd, 4);
	}

	switch (n_args) {
	case 11:
		args[10] = cgroup;
	case 10:
		args[9] = set_tid_size;
	case 9:
		args[8] = (unsigned long)set_tid;
	case 8:
		args[7] = (unsigned long)tls;
	case 7:
		args[6] = stack_size;
	case 6:
		args[5] = (unsigned long)stack;
	case 5:
		args[4] = exit_signal;
	case 4:
		args[3] = (unsigned long)parent_tid;
	case 3:
		args[2] = (unsigned long)parent_tid;
	case 2:
		args[1] = (unsigned long)pidfd;
	case 1:
		args[0] = flags;
	default:
		break;
	}

	return sys_clone3((void *)args, n_args * sizeof(long), result);
}

static long sys_openat_uint(long dirfd, const void *restrict pathname,
			    long flags, unsigned long mode,
			    int *restrict result)
{
	unn_syscall_result rax;
	unsigned long dirfd_ul;

	ASSERT(result != NULL);
	ASSERT(flags >= 0);
	ASSERT(dirfd == AT_FDCWD || dirfd >= 0);
	ASSERT(AT_FDCWD >= -LONG_MAX);
	ASSERT(AT_FDCWD < 0);
	ASSERT(mode <= UINT_MAX);
	ASSERT(flags <= INT_MAX);
	ASSERT(dirfd <= INT_MAX);

	if (dirfd == AT_FDCWD) {
		dirfd_ul = ULONG_MAX - (-AT_FDCWD) + 1;
	} else {
		dirfd_ul = (unsigned long)dirfd;
	}

	syscall4(dirfd_ul, (unsigned long)pathname, (unsigned long)(long)flags,
		 (unsigned long)mode, SYS_OPENAT, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax > INT_MAX || rax <= -0x1000)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	if (LESS_LIKELY(rax == STDOUT_FILENO || rax == STDERR_FILENO ||
			rax == STDIN_FILENO)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
#endif /* KERNEL_MITIGATION_ERRNO */

	ASSERT(rax.l <= INT_MAX);
	ASSERT(rax.l > -0x1000);
#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	ASSERT(rax.l != STDOUT_FILENO);
	ASSERT(rax.l != STDERR_FILENO);
	ASSERT(rax.l != STDIN_FILENO);
#endif

	if (OFTEN(result != NULL)) {
		*result = (int)rax.l;
	}
	return rax.l;
}

static long sys_openat(long dirfd, const void *restrict pathname, long flags,
		       int *restrict result)
{
	unn_syscall_result rax;
	unsigned long dirfd_ul;

	ASSERT(result != NULL);
	ASSERT(flags >= 0);
	ASSERT(dirfd == AT_FDCWD || dirfd >= 0);
	ASSERT(AT_FDCWD >= -LONG_MAX);
	ASSERT(AT_FDCWD < 0);
	ASSERT(dirfd <= INT_MAX);
	ASSERT(flags <= INT_MAX);

	if (dirfd == AT_FDCWD) {
		dirfd_ul = ULONG_MAX - (-AT_FDCWD) + 1;
	} else {
		dirfd_ul = (unsigned long)dirfd;
	}

	syscall3(dirfd_ul, (unsigned long)pathname, (unsigned long)(long)flags,
		 SYS_OPENAT, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax > INT_MAX || rax <= -0x1000)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	if (LESS_LIKELY(rax == STDOUT_FILENO || rax == STDERR_FILENO ||
			rax == STDIN_FILENO)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
#endif /* KERNEL_MITIGATION_ERRNO */

	ASSERT(rax.l <= INT_MAX);
	ASSERT(rax.l > -0x1000);
#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	ASSERT(rax.l != STDOUT_FILENO);
	ASSERT(rax.l != STDERR_FILENO);
	ASSERT(rax.l != STDIN_FILENO);
#endif

	if (OFTEN(result != NULL)) {
		*result = (int)rax.l;
	}
	return rax.l;
}

static long sys_close(long fd)
{
	unn_syscall_result rax;

	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);
#if defined(STDIO_DESCRIPTOR_NO_HACKING)
	ASSERT(fd != STDIN_FILENO);
	ASSERT(fd != STDOUT_FILENO);
	ASSERT(fd != STDERR_FILENO);
#endif

	syscall1((unsigned long)(long)fd, SYS_CLOSE, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_fsync(long fd)
{
	unn_syscall_result rax;

	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

	syscall1((unsigned long)(long)fd, SYS_FSYNC, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_fdatasync(long fd)
{
	unn_syscall_result rax;

	ASSERT(fd >= 0);
	ASSERT(fd <= INT_MAX);

	syscall1((unsigned long)(long)fd, SYS_FDATASYNC, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_mmap(void *restrict addr, unsigned long length, long prot,
		     long flags, long fd, long offset, void **restrict result)
{
	unn_syscall_result rax;
	unsigned long offset_ul;
	unsigned long fd_ul;

	ASSERT(prot >= 0);
	ASSERT(flags >= 0);
	ASSERT(fd >= 0 || fd == -1);
	ASSERT(prot <= INT_MAX);
	ASSERT(flags <= INT_MAX);
	ASSERT(fd <= INT_MAX);

	if (offset < 0) {
		offset -= -LONG_MAX - 1;
		offset_ul = (unsigned long)offset;
		offset_ul -= -LONG_MAX - 1;
	} else {
		offset_ul = (unsigned long)offset;
	}

	if (fd == -1) {
		fd_ul = ULONG_MAX;
	} else {
		fd_ul = (unsigned long)fd;
	}

	/* 'offset' is changed */

	syscall6((unsigned long)addr, length, (unsigned long)(long)prot,
		 (unsigned long)(long)flags, fd_ul, offset_ul, SYS_MMAP, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l < 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);

	if (OFTEN(result != NULL)) {
		*result = rax.p;
	}

	return rax.l;
}

static long sys_munmap(void *addr, unsigned long length)
{
	unn_syscall_result rax;

	syscall2((unsigned long)addr, length, SYS_MUNMAP, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_madvise(void *addr, unsigned long length, long advice)
{
	unn_syscall_result rax;

	ASSERT(addr != NULL);
	ASSERT(length != 0);
	ASSERT(advice >= 0);
	ASSERT(advice <= INT_MAX);

	syscall3((unsigned long)addr, length, (unsigned long)(long)advice,
		 SYS_MADVISE, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_mlockall(long flags)
{
	unn_syscall_result rax;

	ASSERT(flags >= 0);
	ASSERT(0 != (flags & MCL_CURRENT) || 0 != (flags & MCL_FUTURE));
	ASSERT(flags <= INT_MAX);

	syscall1((unsigned long)(long)flags, SYS_MLOCKALL, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_munlockall(void)
{
	unn_syscall_result rax;

	syscall0(SYS_MUNLOCKALL, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_msync(void *addr, unsigned long length, long flags)
{
	unn_syscall_result rax;

	ASSERT(flags >= 0);
	ASSERT(flags <= INT_MAX);

	syscall3((unsigned long)addr, length, (unsigned long)(long)flags,
		 SYS_MSYNC, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_kill(long pid, long sig)
{
	unn_syscall_result rax;
	long pid_l;
	unsigned long pid_ul;

	ASSERT(pid <= INT_MAX);
	ASSERT(sig <= INT_MAX);
	ASSERT(sig >= 0);

	if (pid < 0) {
		pid_l = (long)pid;
		pid_l -= -LONG_MAX - 1;
		pid_ul = (unsigned long)pid_l;
		pid_ul -= -LONG_MAX - 1;
	} else {
		pid_ul = (unsigned long)(long)pid;
	}

	syscall2(pid_ul, (unsigned long)(long)sig, SYS_KILL, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_rt_sigaction(long signum, const void *restrict act,
			     void *restrict oldact, unsigned long sigsetsize)
{
	unn_syscall_result rax;

	ASSERT(signum > 0);
	ASSERT(signum != SIGKILL);
	ASSERT(signum != SIGSTOP);
	ASSERT(signum <= INT_MAX);

	if (act != NULL) {
		ASSUME_ALIGNED_DEREF(&act, sizeof(long));
	}
	if (oldact != NULL) {
		ASSUME_ALIGNED_DEREF(&act, sizeof(long));
	}

	syscall4((unsigned long)(long)signum, (unsigned long)act,
		 (unsigned long)oldact, sigsetsize, SYS_RT_SIGACTION, &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_rt_sigaction_verbose(
    long signum, void (*act_sa_handler)(int), const void *restrict act_sa_mask,
    int act_sa_flags, void (*act_sa_restorer)(void),
    void *restrict oldact_sa_handler, void *restrict oldact_sa_mask,
    int *restrict oldact_sa_flags, void *restrict oldact_sa_restorer,
    unsigned long sigsetsize, void *restrict act_buffer,
    void *restrict oldact_buffer)
{
	long err;

	ASSERT(sizeof(int) <= sizeof(long));
	ASSERT(signum > 0);
	ASSERT(signum != SIGKILL);
	ASSERT(signum != SIGSTOP);
	ASSERT(signum <= INT_MAX);
	ASSERT(sigsetsize % sizeof(long) == 0);

	if (act_buffer != NULL) {
		ASSUME_ALIGNED_DEREF(&act_buffer, sizeof(long));
	}
	if (oldact_buffer != NULL) {
		ASSUME_ALIGNED_DEREF(&oldact_buffer, sizeof(long));
	}

	if (act_buffer != NULL) {
		memcpy((char *)act_buffer, &act_sa_handler,
		       sizeof act_sa_handler);
		if (act_sa_mask != NULL) {
			ASSUME_ALIGNED_DEREF(&act_sa_mask, sizeof(long));
			memcpy((char *)act_buffer + sizeof(long), act_sa_mask,
			       sigsetsize);
		}
		memcpy((char *)act_buffer + sizeof(long) + sigsetsize,
		       &act_sa_flags, sizeof act_sa_flags);
		memcpy((char *)act_buffer + sizeof(long) + sigsetsize +
			   sizeof(long),
		       &act_sa_restorer, sizeof act_sa_restorer);
	}

	err = sys_rt_sigaction(signum, act_buffer, oldact_buffer, sigsetsize);
	if (err < 0) {
		return err;
	}

	if (oldact_buffer != NULL) {
		if (MORE_LIKELY(oldact_sa_handler != NULL)) {
			ASSUME_ALIGNED_DEREF(&oldact_sa_handler,
					     sizeof(void (**)(int)));
			memcpy(oldact_sa_handler, oldact_buffer,
			       sizeof(void (**)(int)));
		}
		if (MORE_LIKELY(oldact_sa_mask != NULL)) {
			ASSUME_ALIGNED_DEREF(&oldact_sa_mask, sizeof(long));
			memcpy(oldact_sa_mask,
			       (char *)oldact_buffer + sizeof(long),
			       sigsetsize);
		}
		if (MORE_LIKELY(oldact_sa_flags != NULL)) {
			*oldact_sa_flags = *(int *)((char *)oldact_buffer +
						    sizeof(long) + sigsetsize);
		}
		if (MORE_LIKELY(oldact_sa_restorer != NULL)) {
			ASSUME_ALIGNED_DEREF(&oldact_sa_restorer,
					     sizeof(void (**)(void)));
			memcpy(oldact_sa_restorer,
			       (char *)oldact_buffer + sizeof(long) +
				   sigsetsize + sizeof(long),
			       sizeof(void (**)(void)));
		}
	}

	return 0;
}

static long sys_nanosleep(const void *restrict duration, void *restrict rem)
{
	unn_syscall_result rax;

	ASSERT(duration != NULL);
	if (duration != NULL) {
		ASSUME_ALIGNED_DEREF(&duration, sizeof(long));
	}
	if (rem != NULL) {
		ASSUME_ALIGNED_DEREF(&rem, sizeof(long));
	}

	syscall2((unsigned long)duration, (unsigned long)rem, SYS_NANOSLEEP,
		 &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l; /* clang-llvm does not realize 'rax' can be returned */
}

static long sys_nanosleep_verbose(long duration_tv_sec, long duration_tv_nsec)
{
	long err;
	long ts[2];
	void *tmp;

	tmp = (void *)ts;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	ASSERT(duration_tv_sec >= 0);
	ASSERT(duration_tv_nsec >= 0);
	ASSERT(duration_tv_nsec < 1000 * 1000 * 1000);

	ts[0] = duration_tv_sec;
	ts[1] = duration_tv_nsec;

	err = sys_nanosleep(ts, NULL);
#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(-err == EINVAL)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
	ASSERT(-err != EINVAL);
	return err;
}

static long sys_nanosleep_verbose_rem(long duration_tv_sec,
				      long duration_tv_nsec,
				      void *restrict rem_tv_sec,
				      void *restrict rem_tv_nsec)
{
	long err;
	long ts[2];
	long rem_ts[2];
	void *tmp;

	tmp = (void *)ts;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));
	tmp = (void *)rem_ts;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	ASSERT(duration_tv_sec >= 0);
	ASSERT(duration_tv_nsec >= 0);
	ASSERT(duration_tv_nsec < 1000 * 1000 * 1000);

	ts[0] = duration_tv_sec;
	ts[1] = duration_tv_nsec;

	err = sys_nanosleep(ts, rem_ts);
#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(-err == EINVAL)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
	ASSERT(-err != EINVAL);

	if (LESS_LIKELY(err < 0 && -err != EINTR)) {
		return err;
	}

	if (rem_tv_sec != NULL) {
		ASSUME_ALIGNED_DEREF(&rem_tv_sec, sizeof(long));
		*(long *)rem_tv_sec = rem_ts[0];
	}
	if (rem_tv_nsec != NULL) {
		ASSUME_ALIGNED_DEREF(&rem_tv_nsec, sizeof(long));
		*(long *)rem_tv_nsec = rem_ts[1];
	}

	return err;
}

/* duration_and_buf size 32 bytes */
/* first sizeof(long) bytes are duration in secs */
/* next sizeof(long) bytes are duration in nsecs */
static long nanosleep_no_eintr_with_buff(void *duration_and_buf)
{
	long err;
	void *xp;
	void *yp;
	void *tmp;

	ASSERT(duration_and_buf != NULL);
	ASSUME_ALIGNED_DEREF(&duration_and_buf, sizeof(long));

	xp = duration_and_buf;
	yp = (char *)duration_and_buf + sizeof(long) * 2;

	ASSERT(*(long *)xp >= 0);
	ASSERT(*((long *)xp + 1) >= 0);
	ASSERT(*((long *)xp + 1) < 1000 * 1000 * 1000);

	while (1 == 1) {
		err = sys_nanosleep(xp, yp);
		if (LESS_LIKELY(-err == EINTR)) {
			tmp = xp;
			xp = yp;
			yp = tmp;
			continue;
		} else {
			break;
		}
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(-err == EINVAL)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
	ASSERT(-err != EINVAL);
	return err;
}

static long nanosleep_no_eintr(long tv_sec, long tv_nsec)
{
	long buf[4];
	void *tmp;

	tmp = (void *)buf;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	ASSERT(tv_sec >= 0);
	ASSERT(tv_nsec >= 0);
	ASSERT(tv_nsec < 1000 * 1000 * 1000);

	buf[0] = tv_sec;
	buf[1] = tv_nsec;
	return nanosleep_no_eintr_with_buff(buf);
}

static long sys_clock_gettime(long clockid, void *tp)
{
	unn_syscall_result rax;

	ASSERT(clockid >= -INT_MAX - 1);
	ASSERT(clockid <= INT_MAX);
	ASSERT(tp != NULL);
	ASSERT(sizeof(long) >= 8);

	if (tp != NULL) {
		ASSUME_ALIGNED_DEREF(&tp, sizeof(long));
	}

	syscall2((unsigned long)clockid, (unsigned long)tp, SYS_CLOCK_GETTIME,
		 &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l != 0)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l == 0);
	return rax.l;
}

static long sys_clock_gettime_verbose(long clockid, long *restrict tp_tv_sec,
				      long *restrict tp_tv_nsec)
{
	long err;
	long tp[2];
	void *tmp;

	ASSERT(clockid >= -INT_MAX - 1);
	ASSERT(clockid <= INT_MAX);

	tmp = (void *)tp;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	err = sys_clock_gettime(clockid, (void *)tp);
	if (err < 0) {
		return err;
	}

	if (tp_tv_sec != NULL) {
		*tp_tv_sec = tp[0];
	}
	if (tp_tv_nsec != NULL) {
		*tp_tv_nsec = tp[1];
	}

	return err;
}

static void sys_getpid(int *result)
{
	unn_syscall_result rax;

	ASSERT(result != NULL);

	syscall0(SYS_GETPID, &rax);

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax.l < 0 || rax.l > INT_MAX)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif

	ASSERT(rax.l >= 0);
	ASSERT(rax.l <= INT_MAX);
	if (OFTEN(result != NULL)) {
		*result = (int)rax.l;
	}
}

static long sys_wait4(long pid, void *restrict wstatus, long options,
		      void *restrict rusage, int *result)
{
	unn_syscall_result rax;

	ASSERT(pid >= -INT_MAX - 1);
	ASSERT(pid <= INT_MAX);
	if (wstatus != NULL) {
		ASSUME_ALIGNED_DEREF(&wstatus, sizeof(long));
	}
	if (rusage != NULL) {
		ASSUME_ALIGNED_DEREF(&rusage, sizeof(long));
	}

	syscall4((unsigned long)pid, (unsigned long)wstatus,
		 (unsigned long)options, (unsigned long)rusage, SYS_WAIT4,
		 &rax);
	if (RARELY(rax.l < 0 && rax.l > -0x1000)) {
		return rax.l;
	}

#if defined(KERNEL_MITIGATION_ERRNO)
	if (RARELY(rax < -0x1000 || rax > INT_MAX)) {
		return -KERNEL_MITIGATION_ERRNO;
	}
#endif
	ASSERT(rax.l >= -0x1000);
	ASSERT(rax.l <= INT_MAX);

	if (result != NULL) {
		*result = (int)rax.l;
	}
	return rax.l;
}

static long sys_wait4_verbose(long pid, void *restrict wstatus, long options,
			      long *restrict ru_utime_tv_sec,
			      long *restrict ru_utime_tv_nsec,
			      long *restrict ru_stime_tv_sec,
			      long *restrict ru_stime_tv_nsec,
			      int *restrict result)
{
	long err;
	long rusage_buf[sizeof(long) * 18];
	void *tmp;
	int res;

	ASSERT(pid >= -INT_MAX - 1);
	ASSERT(pid <= INT_MAX);
	if (wstatus != NULL) {
		ASSUME_ALIGNED_DEREF(&wstatus, sizeof(long));
	}

	tmp = (void *)rusage_buf;
	ASSUME_ALIGNED_DEREF(&tmp, sizeof(long));

	err = sys_wait4(pid, wstatus, options, (void *)rusage_buf, &res);
	if (err < 0) {
		return err;
	}

	if (ru_utime_tv_sec != NULL) {
		*ru_utime_tv_sec = rusage_buf[0];
	}
	if (ru_utime_tv_nsec != NULL) {
		*ru_utime_tv_nsec = rusage_buf[1];
	}
	if (ru_stime_tv_sec != NULL) {
		*ru_stime_tv_sec = rusage_buf[2];
	}
	if (ru_stime_tv_nsec != NULL) {
		*ru_stime_tv_nsec = rusage_buf[3];
	}

	if (result != NULL) {
		*result = res;
	}
	return err;
}

/* [0, pi/2] -> [0, 1) */
static float sinf_base(float x)
{
	float result;
	float a;
	int sign;
	unsigned long b;
	unsigned int i;

	ASSERT(x == x);
	ASSERT(x < FLT_MAX);
	ASSERT(x > -FLT_MAX);
	ASSERT(x >= 0);
	ASSERT(x <= M_PI / 2);

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

	ASSERT(result == result);
	ASSERT(result < FLT_MAX);
	ASSERT(result > -FLT_MAX);
	ASSERT(result >= 0);
	ASSERT(result < 1);
	return result;
}

static float fmodf_nonneg(float x, float y)
{
	ASSERT(x == x);
	ASSERT(x < FLT_MAX);
	ASSERT(x > -FLT_MAX);
	ASSERT(x >= 0);
	ASSERT(y >= 0);
	ASSERT(y != 0);

	while (x >= y) {
		x -= y;
	}

	if (x < 0) {
		x = 0;
	}

	ASSERT(x == x);
	ASSERT(x < FLT_MAX);
	ASSERT(x > -FLT_MAX);
	ASSERT(x >= 0);
	return x;
}

static float sinf(float x)
{
	int sign;

	ASSERT(x == x);
	ASSERT(x < FLT_MAX);
	ASSERT(x > -FLT_MAX);

	sign = 1;

	if (LESS_LIKELY(x < 0)) {
		x = -x;
		sign = -sign;
	}

	ASSERT(x >= 0);

	x = fmodf_nonneg(x, M_PI * 2);

	ASSERT(x >= 0);
	ASSERT(x < M_PI * 2);

	if (x >= M_PI) {
		x -= M_PI;
		if (x < 0) {
			x = 0;
		}
		if (x > M_PI) {
			x = M_PI;
		}
		sign = -sign;
	}

	ASSERT(x >= 0);
	ASSERT(x <= M_PI);

	if (x >= M_PI / 2) {
		x = M_PI - x;
		if (x < 0) {
			x = 0;
		}
		if (x > M_PI / 2) {
			x = M_PI / 2;
		}
	}

	ASSERT(x >= 0);
	ASSERT(x <= M_PI / 2);

	return sinf_base(x) * sign;
}

static unsigned long strlen(const char *s)
{
	unsigned long result;

	result = 0;
	while (OFTEN(*s != '\0')) {
		ASSERT(result != ULONG_MAX);
		++result;
		++s;
	}
	return result;
}

#define STR_ENTER_CHARACTERS "Enter 3 characters:"
#define STR_WAIT "Wait..."
#define STR_HERE_ARE_CHARACTERS "Here are the entered characters:"

int main(int argc, char **argv)
{
	long err;
	char buf[64];

	(void)argc;
	(void)argv;

#if defined(KERNEL_MITIGATION_ERRNO)
	ASSERT(0 != strlen(STRINGIZE_VALUE(KERNEL_MITIGATION_ERRNO)));
#endif

	ASSERT(sizeof buf >= 3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS));

	memcpy(buf, "\n" STR_HERE_ARE_CHARACTERS " ???\n",
	       3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS));

	err = write_no_eintr(STDOUT_FILENO, "\n" STR_ENTER_CHARACTERS "\n",
			     2 + strlen(STR_ENTER_CHARACTERS), NULL);
	if (RARELY(err < 0)) {
		(void)write_no_eintr(STDERR_FILENO, "\nError\n", 7, NULL);
		return EXIT_FAILURE;
	}
	err = read_no_eintr(STDIN_FILENO,
			    buf + 1 + strlen(STR_HERE_ARE_CHARACTERS) + 1, 3,
			    NULL);
	if (RARELY(err < 0)) {
		(void)write_no_eintr(STDERR_FILENO, "\nError\n", 7, NULL);
		return EXIT_FAILURE;
	}
	err = write_no_eintr(STDOUT_FILENO, "\n" STR_WAIT "\n",
			     2 + strlen(STR_WAIT), NULL);
	if (RARELY(err < 0)) {
		(void)write_no_eintr(STDERR_FILENO, "\nError\n", 7, NULL);
		return EXIT_FAILURE;
	}
	err = nanosleep_no_eintr(3, 0);
	if (RARELY(err < 0)) {
		(void)write_no_eintr(STDERR_FILENO, "\nError\n", 7, NULL);
		return EXIT_FAILURE;
	}
	err = write_no_eintr(STDOUT_FILENO, buf,
			     3 + 1 + 2 + strlen(STR_HERE_ARE_CHARACTERS), NULL);
	if (RARELY(err < 0)) {
		(void)write_no_eintr(STDERR_FILENO, "\nError\n", 7, NULL);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(bartlby, 1.1.4, helmut@januschka.com)
AC_PREFIX_DEFAULT(/usr/local/bartlby)



AC_ARG_WITH(mysql-user, AC_HELP_STRING([--with-mysql-user], [Mysql User]), MYSQL_USER="${withval}", MYSQL_USER="root")
AC_SUBST(MYSQL_USER)

AC_ARG_WITH(mysql-password,AC_HELP_STRING([--with-mysql-password], [Mysql password]), MYSQL_PASS="$withval", MYSQL_PASS="xxx")
AC_SUBST(MYSQL_PASS)

AC_ARG_WITH(mysql-host, AC_HELP_STRING([--with-mysql-host], [Mysql host]), MYSQL_HOST="${withval}",MYSQL_HOST="localhost")
AC_SUBST(MYSQL_HOST)

AC_ARG_WITH(mysql-db,AC_HELP_STRING([--with-mysql-db], [Mysql db]), MYSQL_DB="$withval", MYSQL_DB="bartlby")
AC_SUBST(MYSQL_DB)


AC_ARG_WITH(plugin-dir,AC_HELP_STRING([--with-plugin-dir], [Where are the plugins located]), PLUGIN_DIR="${withval}", PLUGIN_DIR="${prefix}/bartlby-plugins/")
AC_SUBST(PLUGIN_DIR)



AC_CHECK_LIB([mysqlclient], [mysql_init],[LD_MYSQL="-lmysqlclient"],[AC_MSG_ERROR([libmysqlclient needed to be found])])
AC_CHECK_LIB([dl], [dlopen],[LD_DL="-ldl"],[AC_MSG_ERROR([libdl needed to be found])])

AC_SUBST(LD_DL)
AC_SUBST(LD_MYSQL)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h malloc.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm gethostbyname gettimeofday inet_ntoa memmove memset setenv socket strdup strstr])


AC_CONFIG_FILES([Makefile.conf bartlby.cfg bartlby.startup])
AC_OUTPUT

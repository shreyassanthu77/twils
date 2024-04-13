MODE = debug
# MODE = release

OS = linux
ARCH = x86_64
ifeq (${OS}, linux)
TARGET = ${ARCH}-linux-musl
else
ifeq (${OS}, windows)
TARGET = ${ARCH}-windows-gnu
endif
endif

CC = zig cc -target ${TARGET} 
SRC = main.c \
			./inc/str/string.c \
			./tailwind/tailwind.c \
			./inc/jsr/jsr.c
INCLUDE = -I./inc -I.
CFLAGS = -static -std=c2x -Wall -Wextra -Werror -Wno-unused-parameter -Wno-format
ifeq (${MODE}, release)
CFLAGS += -O3 -s
endif
OBJDIR = bin/obj
OUTDIR = bin/${MODE}
EXE = 
ifeq (${OS}, windows)
EXE = .exe
endif
BIN = ${OUTDIR}/main${EXE}

WINPTH_CMD = ${CC} -c -s -O2 \
					-I./inc/winpthread/include \
					-Wall -Wextra -Werror -Wno-sign-compare \
					-Wno-unused-parameter -Wno-unused-variable \
					-D__USE_MINGW_ANSI_STDIO=1

WINPTHREAD_SRC = inc/winpthread/src/barrier.c \
							inc/winpthread/src/clock.c \
							inc/winpthread/src/cond.c \
							inc/winpthread/src/libgcc/dll_math.c \
							inc/winpthread/src/misc.c \
							inc/winpthread/src/mutex.c \
							inc/winpthread/src/nanosleep.c \
							inc/winpthread/src/ref.c \
							inc/winpthread/src/rwlock.c \
							inc/winpthread/src/sched.c \
							inc/winpthread/src/sem.c \
							inc/winpthread/src/spinlock.c \
							inc/winpthread/src/thread.c

build-winpthread: ${WINPTHREAD_SRC}
	@mkdir -p ${OBJDIR}
ifeq (${OS}, windows)
	@mkdir -p ${OBJDIR}/winpthread
	@${WINPTH_CMD} ./inc/winpthread/src/barrier.c -c -o ${OBJDIR}/winpthread/barrier.obj
	@${WINPTH_CMD} ./inc/winpthread/src/clock.c -c -o ${OBJDIR}/winpthread/clock.obj
	@${WINPTH_CMD} ./inc/winpthread/src/cond.c -c -o ${OBJDIR}/winpthread/cond.obj
	@${WINPTH_CMD} ./inc/winpthread/src/libgcc/dll_math.c -c -o ${OBJDIR}/winpthread/dll_math.obj
	@${WINPTH_CMD} ./inc/winpthread/src/misc.c -c -o ${OBJDIR}/winpthread/misc.obj
	@${WINPTH_CMD} ./inc/winpthread/src/mutex.c -c -o ${OBJDIR}/winpthread/mutex.obj
	@${WINPTH_CMD} ./inc/winpthread/src/nanosleep.c -c -o ${OBJDIR}/winpthread/nanosleep.obj
	@${WINPTH_CMD} ./inc/winpthread/src/ref.c -c -o ${OBJDIR}/winpthread/ref.obj
	@${WINPTH_CMD} ./inc/winpthread/src/rwlock.c -c -o ${OBJDIR}/winpthread/rwlock.obj
	@${WINPTH_CMD} ./inc/winpthread/src/sched.c -c -o ${OBJDIR}/winpthread/sched.obj
	@${WINPTH_CMD} ./inc/winpthread/src/sem.c -c -o ${OBJDIR}/winpthread/sem.obj
	@${WINPTH_CMD} ./inc/winpthread/src/spinlock.c -c -o ${OBJDIR}/winpthread/spinlock.obj
	@${WINPTH_CMD} ./inc/winpthread/src/thread.c -c -o ${OBJDIR}/winpthread/thread.obj
endif
ifeq (${OS}, windows)
OBJ = ${OBJDIR}/winpthread/barrier \
			${OBJDIR}/winpthread/clock \
			${OBJDIR}/winpthread/cond \
			${OBJDIR}/winpthread/dll_math \
			${OBJDIR}/winpthread/misc \
			${OBJDIR}/winpthread/mutex \
			${OBJDIR}/winpthread/nanosleep \
			${OBJDIR}/winpthread/ref \
			${OBJDIR}/winpthread/rwlock \
			${OBJDIR}/winpthread/sched \
			${OBJDIR}/winpthread/sem \
			${OBJDIR}/winpthread/spinlock \
			${OBJDIR}/winpthread/thread.obj
endif

QJS_CMD = ${CC} -c -s -O3 \
					-Wall -Wextra -Werror \
					-Wno-unused-parameter -Wno-sign-compare \
					-Wno-incompatible-pointer-types -Wno-unused-variable \
					-Wno-unused-but-set-variable \
					-I./inc \
					-DCONFIG_BIGNUM \
					-DCONFIG_VERSION=\"$(shell cat inc/quickjs/VERSION)\" \

ifeq (${OS}, windows)
	QJS_CMD += -I./inc/winpthread/include
endif
ifeq (${MODE}, release)
	QJS_CMD += -DNO_ASSERT
endif

QJS_FILES = inc/quickjs/libregexp.c \
						inc/quickjs/libunicode.c \
						inc/quickjs/libbf.c \
						inc/quickjs/cutils.c \
						inc/quickjs/quickjs.c

build-quickjs: ${QJS_FILES} build-winpthread
	@mkdir -p ${OBJDIR}
ifeq (${OS}, windows)
	@${QJS_CMD} inc/quickjs/libregexp.c  -c -o ${OBJDIR}/libregexp.obj
	@${QJS_CMD} inc/quickjs/libunicode.c -c -o ${OBJDIR}/libunicode.obj
	@${QJS_CMD} inc/quickjs/libbf.c -c -o ${OBJDIR}/libbf.obj
	@${QJS_CMD} inc/quickjs/cutils.c -c -o ${OBJDIR}/cutils.obj
	@${QJS_CMD} inc/quickjs/quickjs.c -c -o ${OBJDIR}/quickjs.obj
else
	@${QJS_CMD} ${QJS_FILES} -o ${OBJDIR}/quickjs-build.o
endif

ifeq (${OS}, windows)
OBJ += ${OBJDIR}/libregexp.obj ${OBJDIR}/libunicode.obj ${OBJDIR}/libbf.obj ${OBJDIR}/cutils.obj ${OBJDIR}/quickjs.obj
else
OBJ += ${OBJDIR}/quickjs-build.o
endif

build: ${SRC} build-quickjs 
	@echo "Building ${BIN}"
	@mkdir -p ${OUTDIR}
	@${CC} ${CFLAGS} ${INCLUDE} ${SRC} ${OBJ} -o ${BIN}

run: build
	@/usr/bin/time -f 'took %es, rss %Mkb max and %tkb avg, I/O %I' ${BIN}


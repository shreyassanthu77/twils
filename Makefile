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
			./tailwind/tailwind.c

INCLUDE = -I./inc -I.
CFLAGS = -static -std=c2x -Wall -Wextra -Werror -Wno-unused-parameter -Wno-format

QJS_CMD = ${CC} -c \
					-Wall -Wextra -Werror \
					-Wno-unused-parameter -Wno-sign-compare \
					-Wno-incompatible-pointer-types -Wno-unused-variable \
					-Wno-unused-but-set-variable \
					-DCONFIG_VERSION=\"$(shell cat inc/quickjs/VERSION)\"

ifeq (${MODE}, release)
	CFLAGS += -O2 -s
	QJS_CMD += -O2 -s
endif

QJS_FILES = inc/quickjs/libregexp.c \
						inc/quickjs/libunicode.c \
						inc/quickjs/libbf.c \
						inc/quickjs/cutils.c \
						inc/quickjs/quickjs.c

OBJDIR = bin/obj
OUTDIR = bin/${MODE}
EXE = 
ifeq (${OS}, windows)
	EXE = .exe
endif
BIN = ${OUTDIR}/main${EXE}

build-quickjs: ${QJS_FILES}
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
OBJ = ${OBJDIR}/libregexp.obj ${OBJDIR}/libunicode.obj ${OBJDIR}/libbf.obj ${OBJDIR}/cutils.obj ${OBJDIR}/quickjs.obj
else
OBJ = ${OBJDIR}/quickjs-build.o
endif

build: ${SRC} build-quickjs
	@echo "Building ${BIN}"
	@mkdir -p ${OUTDIR}
	@${CC} ${CFLAGS} ${SRC} ${INCLUDE} ${OBJ} -o ${BIN}

run: build
	@/usr/bin/time -f 'took %es, rss %Mkb max and %tkb avg, I/O %I' ${BIN}


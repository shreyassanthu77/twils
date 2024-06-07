target = x86_64-linux-musl
export CC = zig cc -target $(target)
export CXX = zig c++ -target $(target)
export CGO_ENABLED = 1

tags = 'osusergo,netgo,static'
extra_ldflags = "-static -O2 -s"
ldflags = '-linkmode external -extldflags $(extra_ldflags)'

cflags = -Wall -Wextra -Werror -std=c2x

mk_bin:
	mkdir -p bin

yacc: mk_bin
	${CC} ${cflags} -o bin/yacc parser/yacc.c

parser: yacc
	./bin/yacc -b parser/css -d parser/css.y
	${CC} ${cflags} -o bin/parser parser/lexer.c parser/css.tab.c parser/test.c parser/ast.c -Iparser
	./bin/parser example.css

build:
	go build -tags $(tags) -ldflags $(ldflags) -o main main.go

run: build
	./main

watch:
	gow -c run -tags $(tags) -ldflags $(ldflags) main.go

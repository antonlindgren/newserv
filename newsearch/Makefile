include ../build.mk

CFLAGS+=$(INCPCRE)
LDFLAGS+=$(LIBPCRE)

.PHONY: all clean distclean
all: newsearch.so

NSCOMMANDS=ns-not.o ns-and.o ns-or.o ns-eq.o ns-match.o ns-hostmask.o ns-realname.o ns-away.o ns-modes.o ns-nick.o ns-ident.o ns-regex.o ns-host.o ns-channel.o ns-lt.o ns-gt.o ns-timestamp.o ns-country.o ns-authname.o ns-ip.o ns-kill.o ns-gline.o ns-exists.o ns-services.o ns-size.o ns-name.o ns-topic.o ns-oppct.o ns-cumodecount.o ns-cumodepct.o ns-hostpct.o ns-authedpct.o ns-length.o ns-kick.o ns-authts.o ns-channels.o ns-server.o ns-authid.o ns-notice.o newsearch_ast.o ns-any.o ns-channeliter.o ns-var.o ns-all.o ns-cumodes.o ns-cidr.o ns-nickiter.o ns-ipv6.o ns-away.o ns-quit.o ns-killed.o ns-renamed.o ns-age.o ns-newnick.o ns-reason.o ns-message.o ns-concat.o

newsearch.so: newsearch.o formats.o y.tab.o lex.yy.o parser.o ${NSCOMMANDS}

y.tab.c y.tab.h: newsearch.y
	$(YACC) -y -d newsearch.y

lex.yy.c: newsearch.l y.tab.h
	$(LEX) newsearch.l

clean:
	rm -f *.o *.so y.tab.c y.tab.h lex.yy.c
	rm -rf .deps

distclean:


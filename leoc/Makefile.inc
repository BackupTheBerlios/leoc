# vim:syn=Makefile:ts=4
# $Id: Makefile.inc,v 1.1 2006/09/20 09:10:24 tikkeri Exp $
%.c: %.y
	$(YACC) $(YFLAGS) -o $@ $<
	@if test -e y.tab.h; then mv y.tab.h \`sed s/\.c/\.h/g | echo $@\`; fi


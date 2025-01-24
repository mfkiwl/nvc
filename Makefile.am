EXTRA_DIST = autogen.sh nvc.1 README.md NEWS.md THANKS.md

ACLOCAL_AMFLAGS = -I m4

man_MANS = nvc.1

AM_CPPFLAGS = -I$(top_srcdir)/thirdparty -I$(top_srcdir)/src
AM_CFLAGS   = -Wall $(EXTRA_CFLAGS) $(LTO_FLAG)
AM_CXXFLAGS = -Wall $(EXTRA_CFLAGS) $(LTO_FLAG)
AM_LDFLAGS  = $(EXTRA_LDFLAGS) $(LLVM_LDFLAGS)

AM_CC       = $(PTHREAD_CC)
AM_CFLAGS   += $(PTHREAD_CFLAGS)
AM_LDFLAGS  += $(PTHREAD_LIBS)

AM_CFLAGS += $(check_CFLAGS) $(libdw_CFLAGS) $(libffi_CFLAGS)

bin_PROGRAMS =
noinst_LIBRARIES =
include_HEADERS =
check_PROGRAMS =
check_LIBRARIES =
noinst_PROGRAMS =
pkgdata_DATA =
pkglibexec_PROGRAMS =
pkglibexec_SCRIPTS =
EXTRA_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =
DISTCLEANFILES =
BOOTSTRAPLIBS =
LIBDIRS =
lib_LIBRARIES =
pkglib_LIBRARIES =

include thirdparty/Makemodule.am
include src/Makemodule.am
include src/rt/Makemodule.am
include src/jit/Makemodule.am
include src/vhpi/Makemodule.am
include src/vlog/Makemodule.am
include test/Makemodule.am
include test/vhpi/Makemodule.am
include lib/Makemodule.am
include lib/std/Makemodule.am
include lib/std.08/Makemodule.am
include lib/std.19/Makemodule.am
include lib/nvc/Makemodule.am
include lib/nvc.08/Makemodule.am
include lib/nvc.19/Makemodule.am
include lib/ieee/Makemodule.am
include lib/ieee.08/Makemodule.am
include lib/ieee.19/Makemodule.am
include lib/vital/Makemodule.am
include lib/synopsys/Makemodule.am
include tools/Makemodule.am
include contrib/Makemodule.am

compile-commands:
	$(MAKE) -C $(top_builddir) clean
	bear -- $(MAKE) -j -C $(top_builddir) $(bin_PROGRAMS) \
	  $(check_PROGRAMS) $(EXTRA_PROGRAMS) $(pkglibexec_PROGRAMS)

release: update-test-dist
	echo $(PACKAGE_VERSION) | grep -E "^[[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+$$"
	$(AM_V_GEN)( \
	  v=$(PACKAGE_VERSION); d=$$(date '+%Y-%m-%d'); \
	  n=$$(date '+%d'); l=$$(date '+%B %Y'); \
	  case $$d in \
	    1?) n=$${n}th ;; \
	    *1) n=$${n}st ;; \
	    *2) n=$${n}nd ;; \
	    *3) n=$${n}rd ;; \
	    *)  n=$${n}th ;; \
	  esac; \
	  sed -i "s/\(Unreleased changes\)/\1\n\n## Version $$v - $$d/" $(top_srcdir)/NEWS.md; \
	  sed -i -e "/define(__version,/cdefine(__version, \`$$v')dnl/" \
	    -e "/define(__release_date,/cdefine(__release_date, \`$$n $$l')dnl/" \
	    -e "/define(__release_date_short,/cdefine(__release_date_short, \`$$d')dnl/" \
	    $(top_srcdir)/www/defs.m4; \
	  $(MAKE) -C $(top_builddir) distcheck; \
	  gpg -b $(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz \
	)

clean-local: clean-test clean-libs

.PHONY: bootstrap cov-reset cov-report clean-libs clean-test compile-commands release

EXTRA_DIST = LICENSE CODING-STYLE Doxyfile autogen.sh
SUBDIRS = src

astyle:
	astyle --options=none --mode=c -A3 -s4 -m0 -p -xd -H -U -k3 -W3 -c -z2 -j -n $(addsuffix /*.h,$(SUBDIRS)) $(addsuffix /*.c,$(SUBDIRS)) | grep --color -E '^Formatted|$$'

# gather code complexity stats
pmccabe:
	@pmccabe -v /dev/null
	@pmccabe $(addsuffix /*.c,$(filter-out tests, $(SUBDIRS))) | sort -n
	@echo
	@pmccabe -v /dev/null
	@pmccabe -Ft $(addsuffix /*.c,$(filter-out tests, $(SUBDIRS))) | sort -n

dist-hook:
	@if test -d "$(srcdir)/.git"; \
	then \
		echo Creating ChangeLog && \
		( cd "$(top_srcdir)" && \
		  echo '# Generated by Makefile. Do not edit.'; echo; \
		  $(top_srcdir)/build-aux/missing --run git log --stat ) > ChangeLog.tmp \
		&& mv -f ChangeLog.tmp $(distdir)/ChangeLog \
		|| ( rm -f ChangeLog.tmp ; \
		     echo Failed to generate ChangeLog >&2 ); \
	else \
		echo A git clone is required to generate a ChangeLog >&2; \
	fi

TARGET_ARCH = $(if $(target_alias),$(target_alias),$(shell uname -m))
install-tgz:
	rm -rf /tmp/$(PACKAGE)-$(VERSION)
	make install prefix=/tmp/$(PACKAGE)-$(VERSION)/usr/local sysconfdir=/tmp/$(PACKAGE)-$(VERSION)/etc
	tar -czf $(PACKAGE)-$(VERSION).bin.$(TARGET_ARCH).tgz -C /tmp/$(PACKAGE)-$(VERSION) . --owner=0 --group=0
	rm -rf /tmp/$(PACKAGE)-$(VERSION)


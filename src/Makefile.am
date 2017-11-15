if DEBUG
  AM_CFLAGS = -g -O3
else
  AM_CFLAGS = -O3
endif

AM_CFLAGS += -I$(top_srcdir)/src
AM_CFLAGS += $(GLIB_CFLAGS)

AM_CFLAGS += -Wall

bin_PROGRAMS = fboverlayset
fboverlayset_SOURCES = main.c fbaccess.c

fboverlayset_LDADD = $(AM_LDFLAGS)
fboverlayset_LDADD += $(GLIB_LIBS)

[SUBSYSTEM::LIBWBCLIENT_OLD]
PUBLIC_DEPENDENCIES = LIBSAMBA-ERRORS LIBEVENTS
PRIVATE_DEPENDENCIES = NDR_WINBIND MESSAGING

LIBWBCLIENT_OLD_OBJ_FILES = $(libclisrcdir)/wbclient/wbclient.o

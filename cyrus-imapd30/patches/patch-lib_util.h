$NetBSD$

--- lib/util.h.orig	2017-04-20 01:21:48.000000000 +0000
+++ lib/util.h
@@ -69,7 +69,7 @@
 #endif /* ENABLE_REGEX */
 
 #ifndef HAVE_GLIBC_COMPAR_FN_T
-typedef int (*__compar_fn_t)(const void *, const void *);
+//typedef int (*__compar_fn_t)(const void *, const void *);
 #endif
 
 #define BIT32_MAX 4294967295U

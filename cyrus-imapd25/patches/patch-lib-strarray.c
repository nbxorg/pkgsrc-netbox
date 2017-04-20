$NetBSD$

--- lib/strarray.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ lib/strarray.c
@@ -368,7 +368,7 @@ EXPORTED char **strarray_takevf(strarray
 
 EXPORTED void strarray_sort(strarray_t *sa, compar_t *cmp)
 {
-    qsort(sa->data, sa->count, sizeof(char *), (__compar_fn_t)cmp);
+    qsort(sa->data, sa->count, sizeof(char *), (int (*)(const void *, const void *))cmp);
 }
 
 

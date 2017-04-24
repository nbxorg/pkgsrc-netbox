$NetBSD$

--- lib/strarray.c.orig	2017-04-23 07:41:20.494335394 +0000
+++ lib/strarray.c
@@ -383,7 +383,7 @@ EXPORTED char **strarray_takevf(strarray
 
 EXPORTED void strarray_sort(strarray_t *sa, compar_t *cmp)
 {
-    qsort(sa->data, sa->count, sizeof(char *), (__compar_fn_t)cmp);
+    qsort(sa->data, sa->count, sizeof(char *), (int (*)(const void *, const void *))cmp);
 }
 
 

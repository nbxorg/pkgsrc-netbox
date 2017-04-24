$NetBSD$

--- lib/arrayu64.c.orig	2017-04-07 05:31:10.000000000 +0000
+++ lib/arrayu64.c
@@ -256,7 +256,7 @@ static int _numeric_sort(const void *a,
 EXPORTED void arrayu64_sort(arrayu64_t *au, compar_t *cmp)
 {
     if (!cmp) cmp = _numeric_sort;
-    qsort(au->data, au->count, sizeof(uint64_t), (__compar_fn_t)cmp);
+    qsort(au->data, au->count, sizeof(uint64_t), (int (*)(const void *, const void *))cmp);
 }
 
 EXPORTED void arrayu64_uniq(arrayu64_t *au)

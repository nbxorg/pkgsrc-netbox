$NetBSD$

--- lib/glob.c.orig	2017-04-07 05:31:10.000000000 +0000
+++ lib/glob.c
@@ -109,7 +109,7 @@ EXPORTED glob *glob_init(const char *str
     buf_appendcstr(&buf, "]|$)");
 
     glob *g = xmalloc(sizeof(glob));
-    regcomp(&g->regex, buf_cstring(&buf), 0);
+    regcomp(&g->regex, buf_cstring(&buf), REG_EXTENDED);
     buf_free(&buf);
 
     return g;

$NetBSD$

--- imap/protocol.h.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/protocol.h
@@ -54,7 +54,7 @@ enum {
 struct stdprot_t;
 struct backend;
 
-#define MAX_CAPA 9
+#define MAX_CAPA 10
 
 enum {
     /* generic capabilities */

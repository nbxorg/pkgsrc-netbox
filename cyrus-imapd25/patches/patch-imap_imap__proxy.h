$NetBSD$

--- imap/imap_proxy.h.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/imap_proxy.h
@@ -62,7 +62,8 @@ enum {
     CAPA_MULTIAPPEND	= (1 << 5),
     CAPA_ACLRIGHTS	= (1 << 6),
     CAPA_LISTEXTENDED	= (1 << 7),
-    CAPA_SASL_IR	= (1 << 8)
+    CAPA_SASL_IR	= (1 << 8),
+    CAPA_METADATA	= (1 << 9)
 };
 
 extern struct protocol_t imap_protocol;

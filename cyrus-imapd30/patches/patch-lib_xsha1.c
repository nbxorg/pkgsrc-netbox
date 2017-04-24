$NetBSD$

--- lib/xsha1.c.orig	2017-04-07 05:31:10.000000000 +0000
+++ lib/xsha1.c
@@ -89,7 +89,7 @@ struct _SHA_CTX {
 /* blk0() and blk() perform the initial expand. */
 /* I got the idea of expanding during the round function from SSLeay */
 
-#ifdef LITTLE_ENDIAN
+#ifndef WORDS_BIGENDIAN
 #define blk0(i) (block->l[i] = (rol(block->l[i],24)&(sha1_quadbyte)0xFF00FF00) \
     |(rol(block->l[i],8)&(sha1_quadbyte)0x00FF00FF))
 #else

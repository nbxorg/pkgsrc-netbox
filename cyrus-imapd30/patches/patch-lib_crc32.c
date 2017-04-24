$NetBSD$

--- lib/crc32.c.orig	2017-04-07 05:31:10.000000000 +0000
+++ lib/crc32.c
@@ -611,7 +611,7 @@ static const uint32_t crc32_lookup[16][2
     }
 };
 
-#ifndef LITTLE_ENDIAN
+#ifdef WORDS_BIGENDIAN
 /* swap endianness */
 static uint32_t swap(uint32_t x)
 {
@@ -634,7 +634,7 @@ static uint32_t crc32_slice8(uint32_t pr
 
     /* process eight bytes at once (Slicing-by-8) */
     while (length >= 8) {
-#ifdef LITTLE_ENDIAN
+#ifndef WORDS_BIGENDIAN
         uint32_t one = *current++ ^ crc;
         uint32_t two = *current++;
         crc = crc32_lookup[0][(two>>24) & 0xFF] ^
@@ -682,7 +682,7 @@ static uint32_t crc32_slice16(uint32_t p
     while (length >= bytes_at_once) {
         size_t unrolling;
         for (unrolling = 0; unrolling < unroll; unrolling++) {
-#ifdef LITTLE_ENDIAN
+#ifndef WORDS_BIGENDIAN
         uint32_t one   = *current++ ^ crc;
         uint32_t two   = *current++;
         uint32_t three = *current++;

$NetBSD$

--- lib/cyrusdb_quotalegacy.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ lib/cyrusdb_quotalegacy.c
@@ -632,7 +632,7 @@ static int foreach(struct dbengine *db,
 
     /* sort the quotaroots (ignoring paths) */
     if (pathbuf.data)
-	qsort(pathbuf.data, pathbuf.count, sizeof(char *), (__compar_fn_t)db->compar);
+	qsort(pathbuf.data, pathbuf.count, sizeof(char *), (int (*)(const void *, const void *))db->compar);
 
     for (i = 0; i < pathbuf.count; i++) {
 	const char *data, *key;

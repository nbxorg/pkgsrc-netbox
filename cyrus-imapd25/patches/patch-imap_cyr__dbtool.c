$NetBSD$

--- imap/cyr_dbtool.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/cyr_dbtool.c
@@ -248,11 +248,14 @@ int main(int argc, char *argv[])
     struct txn *tid = NULL;
     struct txn **tidp = NULL;
 
-    while ((opt = getopt(argc, argv, "C:ntT")) != EOF) {
+    while ((opt = getopt(argc, argv, "C:MntT")) != EOF) {
 	switch (opt) {
 	case 'C': /* alt config file */
 	    alt_config = optarg;
 	    break;
+	case 'M': /* use "improved_mboxlist_sort" */
+	    db_flags |= CYRUSDB_MBOXSORT;
+	    break;
 	case 'n': /* create new */
 	    db_flags |= CYRUSDB_CREATE;
 	    break;

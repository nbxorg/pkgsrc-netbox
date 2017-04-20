$NetBSD$

--- imap/ctl_cyrusdb.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/ctl_cyrusdb.c
@@ -196,7 +196,30 @@ static void recover_reserved(void)
 static const char *dbfname(struct cyrusdb *db)
 {
     static char buf[MAX_MAILBOX_PATH];
-    snprintf(buf, MAX_MAILBOX_PATH, "%s%s", config_dir, db->name);
+    const char *fname = NULL;
+
+    /* find absolute path to db files in configuration */
+    if (!strcmp(db->name, FNAME_MBOXLIST))
+        fname = config_getstring(IMAPOPT_MBOXLIST_DB_PATH);
+    else if (!strcmp(db->name, FNAME_QUOTADB))
+        fname = config_getstring(IMAPOPT_QUOTA_DB_PATH);
+    else if (!strcmp(db->name, FNAME_GLOBALANNOTATIONS))
+        fname = config_getstring(IMAPOPT_ANNOTATION_DB_PATH);
+    else if (!strcmp(db->name, FNAME_DELIVERDB))
+        fname = config_getstring(IMAPOPT_DUPLICATE_DB_PATH);
+    else if (!strcmp(db->name, FNAME_TLSSESSIONS))
+        fname = config_getstring(IMAPOPT_TLSCACHE_DB_PATH);
+    else if (!strcmp(db->name, FNAME_PTSDB))
+        fname = config_getstring(IMAPOPT_PTSCACHE_DB_PATH);
+    else if (!strcmp(db->name, FNAME_STATUSCACHEDB))
+        fname = config_getstring(IMAPOPT_STATUSCACHE_DB_PATH);
+
+    /* use default if no special path was found */
+    if (!fname)
+        snprintf(buf, MAX_MAILBOX_PATH, "%s%s", config_dir, db->name);
+    else
+        snprintf(buf, MAX_MAILBOX_PATH, "%s", fname);
+
     return buf;
 }
 

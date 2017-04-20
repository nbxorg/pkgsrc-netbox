$NetBSD$

--- imap/mbdump.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/mbdump.c
@@ -828,6 +828,7 @@ EXPORTED int undump_mailbox(const char *
     const char *sieve_path = NULL;
     int sieve_usehomedir = config_getswitch(IMAPOPT_SIEVEUSEHOMEDIR);
     const char *userid = NULL;
+    int first_annotation = 1;
     char *annotation = NULL;
     struct buf content = BUF_INITIALIZER;
     char *seen_file = NULL;
@@ -900,8 +901,6 @@ EXPORTED int undump_mailbox(const char *
     mailbox_get_usage(mailbox, old_quota_usage);
 
     astate = annotate_state_new();
-    r = annotate_state_set_mailbox(astate, mailbox);
-    if (r) goto done;
 
     while(1) {
 	char fnamebuf[MAX_MAILBOX_PATH + 1024];
@@ -925,6 +924,16 @@ EXPORTED int undump_mailbox(const char *
 	    int i;
 	    char *tmpuserid;
 
+	    /* is this the first annotation? re-read cyrus.header/index */
+	    if (first_annotation) {
+		first_annotation = 0;
+		mailbox_close(&mailbox);
+		r = mailbox_open_exclusive(mbname, &mailbox);
+		if (r) goto done;
+		r = annotate_state_set_mailbox(astate, mailbox);
+		if (r) goto done;
+	    }
+
 	    for(i=2; file.s[i]; i++) {
 		if(file.s[i] == '/') break;
 	    }

$NetBSD$

--- imap/imapd.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/imapd.c
@@ -8089,7 +8089,7 @@ static int parse_metadata_string_or_list
 
     // Assume by default the arguments are a list of entries,
     // until proven otherwise.
-    *is_list = 1;
+    *is_list = 0;
 
     c = prot_getc(imapd_in);
     if (c == EOF) {
@@ -8136,10 +8136,10 @@ static int parse_metadata_string_or_list
 
 	strarray_append(entries, arg.s);
 
-	// It is only not a list if there are wildcards,
-	// otherwise it's just a list of 1 item
-	if (strchr(arg.s, '*') || strchr(arg.s, '%')) {
-	    *is_list = 0;
+	// It is a list if there are wildcards
+	if (!strchr(arg.s, '*') && !strchr(arg.s, '%')) {
+	    // No wildcards; Not a list
+	    *is_list = 1;
 	}
     }
 
@@ -8804,7 +8804,7 @@ static void cmd_getmetadata(const char *
     if (nlists == 2) {
 	/* no options */
 	mboxes = &lists[0];
-	mbox_is_pattern = !is_list[0];
+	mbox_is_pattern = is_list[0];
     }
     if (nlists == 3) {
 	/* options, either before or after */
@@ -8818,12 +8818,12 @@ static void cmd_getmetadata(const char *
 	    /* (options) (mailboxes) */
 	    options = &lists[0];
 	    mboxes = &lists[1];
-	    mbox_is_pattern = !is_list[1];
+	    mbox_is_pattern = is_list[1];
 	    break;
 	case 2:
 	    /* (mailboxes) (options) */
 	    mboxes = &lists[0];
-	    mbox_is_pattern = !is_list[0];
+	    mbox_is_pattern = is_list[0];
 	    options = &lists[1];
 	    break;
 	case 3:
@@ -8879,8 +8879,8 @@ static void cmd_getmetadata(const char *
 	}
     }
 
-    if (have_private) strarray_append(&newa, "value.priv");
-    if (have_shared) strarray_append(&newa, "value.shared");
+    if (have_private) strarray_append(&newa, "/private");
+    if (have_shared) strarray_append(&newa, "/shared");
 
     astate = annotate_state_new();
     annotate_state_set_auth(astate,
@@ -10032,8 +10032,23 @@ static int backend_version(struct backen
 {
     const char *minor;
 
+    /* IMPORTANT:
+     *
+     * When adding checks for new versions, you must also backport these
+     * checks to previous versions (especially 2.4 and 2.5).
+     *
+     * Otherwise, old versions will be unable to recognise the new version,
+     * assume it is ancient, and downgrade the index to the oldest version
+     * supported (version 6, prior to v2.3).
+     */
+
+    /* It's like looking in the mirror and not suffering from schizophrenia */
+    if (strstr(be->banner, cyrus_version())) {
+	return MAILBOX_MINOR_VERSION;
+    }
+
     /* master branch? */
-    if (strstr(be->banner, "git3.0")) {
+    if (strstr(be->banner, "Cyrus IMAP 3.0")) {
 	return 13;
     }
 

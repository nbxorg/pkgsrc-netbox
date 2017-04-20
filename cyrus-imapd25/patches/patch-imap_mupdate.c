$NetBSD$

--- imap/mupdate.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/mupdate.c
@@ -1606,28 +1606,51 @@ static void cmd_set(struct conn *C,
 
     m = database_lookup(mailbox, NULL);
     if (m && t == SET_RESERVE) {
+	/* Check if we run in a discrete murder topology */
 	if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD) {
-	    /* failed; mailbox already exists */
-	    msg = EXISTS;
-	    goto done;
+	    /* Replicated backends with the same server name issue
+	     * reservations twice. Suppress bailing out on the second one
+	     * (the replica).
+	     */
+	    if (strcmp(m->location, location)) {
+		/* failed; mailbox already exists */
+		msg = EXISTS;
+		goto done;
+	    }
 	}
 	/* otherwise do nothing (local create on master) */
     }
 
     if ((!m || m->t != SET_ACTIVE) && t == SET_DEACTIVATE) {
-	/* failed; mailbox not currently active */
-	msg = NOTACTIVE;
-	goto done;
+	/* Check if we run in a discrete murder topology */
+	if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD) {
+	    /* Replicated backends with the same server name issue
+	     * deactivation twice. Suppress bailing out on the second one
+	     * (the replica).
+	     */
+	    if (strcmp(m->location, location)) {
+		/* failed; mailbox not currently active */
+		msg = NOTACTIVE;
+		goto done;
+	    }
+	}
     } else if (t == SET_DEACTIVATE) {
 	t = SET_RESERVE;
     }
     
     if (t == SET_DELETE) {
 	if (!m) {
+	    /* Check if we run in a discrete murder topology */
 	    if (config_mupdate_config == IMAP_ENUM_MUPDATE_CONFIG_STANDARD) {
-		/* failed; mailbox doesn't exist */
-		msg = DOESNTEXIST;
-		goto done;
+		/* Replicated backends with the same server name issue
+		 * deletion twice. Suppress bailing out on the second one
+		 * (the replica).
+		 */
+		if (strcmp(m->location, location)) {
+		    /* failed; mailbox doesn't exist */
+		    msg = DOESNTEXIST;
+		    goto done;
+		}
 	    }
 	    /* otherwise do nothing (local delete on master) */
 	} else {

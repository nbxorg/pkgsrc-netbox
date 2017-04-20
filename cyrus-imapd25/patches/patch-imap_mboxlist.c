$NetBSD$

--- imap/mboxlist.c.orig	2015-11-30 06:30:33.000000000 +0000
+++ imap/mboxlist.c
@@ -1327,6 +1327,7 @@ EXPORTED int mboxlist_renamemailbox(cons
 			   int local_only, int forceuser, int ignorequota)
 {
     int r;
+    int mupdatecommiterror = 0;
     long myrights;
     int isusermbox = 0; /* Are we renaming someone's inbox */
     int partitionmove = 0;
@@ -1530,6 +1531,7 @@ EXPORTED int mboxlist_renamemailbox(cons
 	    syslog(LOG_ERR,
 		   "MUPDATE: can't commit mailbox entry for '%s'",
 		   newname);
+            mupdatecommiterror = r;
 	}
 	if (mupdate_h) mupdate_disconnect(&mupdate_h);
 	free(loc);
@@ -1540,7 +1542,37 @@ EXPORTED int mboxlist_renamemailbox(cons
 	r = mailbox_commit(newmailbox);
 
     if (r) {
-	/* XXX - rollback DB changes if it was an mupdate failure */
+	/* rollback DB changes if it was an mupdate failure */
+        if (mupdatecommiterror) {
+            r = 0;
+
+            /* recreate an old entry */
+            if (!isusermbox)
+                r = cyrusdb_store(mbdb, oldname, strlen(oldname),
+				  mboxent, strlen(mboxent), &tid);
+
+            /* delete the new entry */
+            if (!r)
+                r = cyrusdb_delete(mbdb, newname, strlen(newname), &tid, 0);
+
+            /* Commit transaction */
+            if (!r)
+                r = cyrusdb_commit(mbdb, tid);
+
+            tid = NULL;
+            if (r) {
+                /* XXX HOWTO repair this mess! */
+                syslog(LOG_ERR, "DBERROR: failed DB rollback on mailboxrename %s %s: %s",
+                       oldname, newname, cyrusdb_strerror(r));
+                syslog(LOG_ERR, "DBERROR: mailboxdb on mupdate and backend ARE NOT CONSISTENT");
+                syslog(LOG_ERR, "DBERROR: mailboxdb on mupdate has entry for %s, mailboxdb on backend has entry for %s and files are on the old position", oldname, newname);
+                r = IMAP_IOERROR;
+            } else {
+                r = mupdatecommiterror;
+            }
+
+        }
+
 	if (newmailbox) mailbox_delete(&newmailbox);
 	if (partitionmove && newpartition)
 	    mailbox_delete_cleanup(newpartition, newname);

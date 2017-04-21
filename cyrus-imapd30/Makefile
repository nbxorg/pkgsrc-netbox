# $NetBSD$

DISTNAME=	cyrus-imapd-3.0.1
CATEGORIES=	devel
MASTER_SITES=	ftp://ftp.cyrusimap.org/cyrus-imapd/

MAINTAINER=	yuka@netbox.org
HOMEPAGE=	ftp://ftp.cyrusimap.org/cyrus-imapd/
COMMENT=	Cyrus IMAP server
LICENSE=	bsd-original # like

#USE_TOOLS += pkg-config libtool autotool automake aclocal bison autoconf gmake perl:run makedepend
USE_TOOLS += pkg-config gmake bison perl:run makedepend
USE_LIBTOOL= yes
USE_PKGLOCALEDIR= yes
GNU_CONFIGURE= yes
GNU_LIBTOOL= yes
PERL5_CONFIGURE= no 

BUILD_DEFS+= VARBASE
USE_LANGUAGES+= c c++ C99
CHECK_PORTABILITY_SKIP= configure.lineno

MAKE_JOBS_SAFE=no
#PKGCONFIG_OVERRIDE+= libcyrus_min.pc.in
#PKGCONFIG_OVERRIDE+= libcyrus_sieve.pc.in
#PKGCONFIG_OVERRIDE+= libcyrus.pc.in

CYRUS_USER?=	cyrus	
CYRUS_GROUP?=	mail

BUILD_DEFS+=	CYRUS_USER CYRUS_GROUP
FILES_SUBST+=	CYRUS_USER=${CYRUS_USER}
FILES_SUBST+=	CYRUS_GROUP=${CYRUS_GROUP}


.include "../../security/cyrus-sasl/buildlink3.mk"
.include "../../security/openssl/buildlink3.mk"
.include "../../textproc/jansson/buildlink3.mk"
.include "../../textproc/icu/buildlink3.mk"
.include "../../devel/flex/buildlink3.mk"
.include "../../lang/perl5/module.mk"

.include "../../mk/bsd.prefs.mk"


CONFIGURE_ARGS+= --with-cyrus-user=${CYRUS_USER}
CONFIGURE_ARGS+= --with-cyrus-group=${CYRUS_USER}
CONFIGURE_ARGS+= --with-sasl=${BUILDLINK_PREFIX.cyrus-sasl}
CONFIGURE_ARGS+= --with-perl=${PERL5:Q}

CONFIGURE_ARGS+= --with-pgsql=${BUILDLINK_PREFIX.postgresql96-client}
.include "../../databases/postgresql96-client/buildlink3.mk"

.include "../../time/libical/buildlink3.mk"
.include "../../textproc/libxml2/buildlink3.mk"
.include "../../www/nghttp2/buildlink3.mk"
CONFIGURE_ARGS+= --enable-http --enable-calalarmd

.include "../../mk/krb5.buildlink3.mk"

CONFIGURE_ARGS+= --enable-autocreate
CONFIGURE_ARGS+= --enable-idled

CONFIGURE_ARGS+= --enable-murder
CONFIGURE_ARGS+= --enable-replication

MAKE_ENV+= PERL=${PERL5:Q}

.include "../../mk/pthread.buildlink3.mk"

.include "../../mk/bsd.pkg.mk"

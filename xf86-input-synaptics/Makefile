# $NetBSD$
#

DISTNAME=        xf86-input-synaptics-1.9.0
CATEGORIES=     x11
#MASTER_SITES=   http://xorg.freedesktop.org/archive/individual/driver/ 
MASTER_SITES=	${MASTER_SITE_XORG:=driver/}
#http://cgit.freedesktop.org/xorg/driver/xf86-input-synaptics/snapshot/
EXTRACT_SUFX=	.tar.bz2

MAINTAINER=     yuka@netbox.org
HOMEPAGE=       http://cgit.freedesktop.org/xorg/driver/xf86-input-synaptics/
COMMENT=        This is the Synaptics TouchPad driver for the modular Xorg server 
LICENSE=        mit

PKG_DESTDIR_SUPPORT=    user-destdir
BUILD_DEPENDS+=         xorg-util-macros>=1.1:../../devel/xorg-util-macros
DEPENDS+=               libXrandr-[0-9]*:../libXrandr
#still not sure DEPENDS+=xf86-input-mouse-[0-9]*:../xf86-input-mouse

GNU_CONFIGURE=          yes
USE_TOOLS+=             pkg-config aclocal automake m4 autom4te autoheader autoconf autoreconf 

CONFIGURE_ENV.SunOS+= DRIVER_MAN_SUFFIX=4

PKGCONFIG_OVERRIDE+=    xorg-synaptics.pc.in

pre-configure:
        cd ${WRKSRC} && ./autogen.sh

#CONFIGURE_ARGS+=--prefix=/usr/pkg --sysconfdir=/etc/X11 --localstatedir=/var

.include "../../x11/randrproto/buildlink3.mk"
.include "../../x11/inputproto/buildlink3.mk"
.include "../../x11/kbproto/buildlink3.mk"
.include "../../x11/xproto/buildlink3.mk"

.include "../../x11/modular-xorg-server/buildlink3.mk"

.include "../../mk/bsd.pkg.mk"

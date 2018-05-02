#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/liujiawei/linux-avn/build/tcc8971/tmp/sysroots/tcc8971-lcn-2.0
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/liujiawei/linux-avn/build/tcc8971/tmp/sysroots/tcc8971-lcn-2.0/usr/lib/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"

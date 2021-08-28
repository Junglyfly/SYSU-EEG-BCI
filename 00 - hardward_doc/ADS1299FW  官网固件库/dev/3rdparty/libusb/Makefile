# ============================================================================
#
# Copyright 2007, 2008, 2009, Fuel7, Inc. All Rights Reserved
# This software is the confidential and proprietary information of Fuel7, Inc.
# ("Confidential Information").  You shall not disclose such Confidential
# Information and shall use it only in accordance with the terms of the
# license agreement you entered into with Fuel7.
#
# FUEL7 MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
# SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
# NON-INFRINGEMENT. FUEL7 SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
# LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR
# ITS DERIVATIVES.
#
# CopyrightVersion 1.0.14
#
# @brief Download, extract, and build libusb
#
# @created 2011/10/14
# 
# @author Bret Brown
#
# ============================================================================

# Location for Makefile.package and Makefile.stages
COMMON_DIR ?= ../fuel7/multi/common

# (optional) for configure --build option
# The build option for configure specifies the building platform
# TODO: Determine based on current system arch
PACKAGE_CONFIGURE_BUILD ?= x86_64-pc-linux-gnu

# The host option for configure specifies the target (runtime) platform
# Set the target package architecture
ifneq ($(CROSS_COMPILE),)
PACKAGE_ARCH ?= $(patsubst %-,%,$(CROSS_COMPILE))
else
PACKAGE_ARCH ?= $(PACKAGE_CONFIGURE_BUILD)
endif

# (optional) Specifies the make target for package dependencies
#PACKAGE_DEPS_TARGET ?= 

# Set the staging directory for all target output
#STAGING_DIR ?= $(abspath ../../..)/staging/$(PACKAGE_ARCH)

# Set the package name as will be downloaded from the specified site
# Example: opkg-0.1.8.tar.gz
PACKAGE_FILE = libusb-1.0.8.tar.bz2

# Set the protocol and mirror location for downloading the PACKAGE file
# Example: http://opkg.googlecode.com/files/opkg-0.1.8.tar.gz
MIRROR_URLS = http://downloads.sourceforge.net/project/libusb/libusb-1.0/libusb-1.0.8/libusb-1.0.8.tar.bz2?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Flibusb%2Ffiles%2Flibusb-1.0%2Flibusb-1.0.8%2F

# (optional) Specifies the set of patch files to apply
PACKAGE_PATCH_FILES =

# (optional) Specifies the configure options to be used
#PACKAGE_CONFIGURE_OPTS = --disable-static

#PACKAGE_CONFIGURE_EXPORTS = ac_cv_file__proc_stat=yes \
#                            ac_cv_file__proc_meminfo=yes \
#                            ac_cv_func_malloc_0_nonnull=yes \
#                            ac_cv_func_realloc_0_nonnull=yes

PACKAGE_FORCE_AUTORECONF_OPTS = -vfs --install

PACKAGE_CONFIGURE_EXPORTS =

# (optional) Specifies the LDFLAGS passed to configure
#LDFLAGS =

# (optional) Set the directory that will contain temporary the installed files
INSTALL_DIR ?= $(abspath .)/install

# (optional) Specifies the location of dependent include and libraries for this package
PACKAGE_DEPS =

include $(COMMON_DIR)/build/Makefile.package

distclean: clean
	$(Verbose-2)rm -rf "$(PACKAGE_OUTPUT_DIR)" "$(INSTALL_DIR)"


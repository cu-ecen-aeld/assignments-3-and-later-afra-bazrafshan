#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u
OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.1.10
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
echo "finder app dir: ${FINDER_APP_DIR}"
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR}

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}

    # TODO: Add your kernel build steps here
make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- mrproper 
make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- defconfig 
make -j4 ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- all
#make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- modules
make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- dtbs
fi

echo "Adding the Image in outdir"

echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs
fi

# TODO: Create necessary base directories
mkdir  -p rootfs
cd rootfs 
mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib urs/sbin
mkdir -p var/log
cd ..

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
git clone git://busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    # TODO:  Configure busybox
    make menuconfig
else
    cd busybox
fi
find . -name "*." -exec dos2unix {} \;
# TODO: Make and install busybox
make distclean
make defconfig
make -j4 ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} all
make CONFIG_PREFIX=${OUTDIR} ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu-
echo "Library dependencies" 

#${CROSS_COMPILE}readelf -a bin/busybox | grep "program interpreter" 
#${CROSS_COMPILE}readelf -a bin/busybox | grep "Shared library"
#result1=$(${CROSS_COMPILE}readelf -a bin/busybox | grep "program interpreter")

#echo "Result of readelf command: ${result1}"
#result2=${CROSS_COMPILE}readelf -a bin/busybox | grep "Shared library"

#echo "Result of readelf command: ${result2}"

# TODO: Add library dependencies to rootfs
export SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)
cd ${OUTDIR}/rootfs
#cp -a $SYSROOT/lib/ld-linux-armhf.so.3 lib
#cp -a $SYSROOT/lib/ld-2.22.so lib
#cp -a $SYSROOT/lib/libc.so.6 lib
#cp -a $SYSROOT/lib/libc-2.22.so lib
#cp -a $SYSROOT/lib/libm.so.6 lib
#cp -a $SYSROOT/lib/libm-2.22.so lib
# TODO: Make device nodes
sudo mknod -m 666 dev/null c 1 3
# TODO: Clean and build the writer utility
rm -f writer.o 
rm -f writer
${CROSS_COMPILE}gcc /${FINDER_APP_DIR}/writer.c -o ${OUTDIR}/rootfs/writer
# TODO: Copy the finder related scripts and executables to the /home directory
cp /${FINDER_APP_DIR}/finder.sh /${FINDER_APP_DIR}/autorun-qemu.sh /${FINDER_APP_DIR}/conf/username.txt /${FINDER_APP_DIR}/conf/assignment.txt /${FINDER_APP_DIR}/finder-test.sh "${OUTDIR}/rootfs/"
# on the target rootfs
cd "$OUTDIR/rootfs"
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
gzip -f ${OUTDIR}/initramfs.cpio
# TODO: Chown the root directory

# TODO: Create initramfs.cpio.gz

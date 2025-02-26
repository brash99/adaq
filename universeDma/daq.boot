#
#  Load and initialize Universe DMA library for 
#  Motorola PPC family MV2300 2400 2700 5100 series boards
#

cd "mizar:/home/abbottd/vxWorks/universeDma"
# Load Universe DMA Library
ld<universeDma.o
#initialize (1 no interrupts) or (0 interrupts enabled)
sysVmeDmaInit(0) 
# Set for 64bit PCI transfers
sysVmeDmaSet(4,1)
# A24 VME Slave (1) or A32 Slave (2)
sysVmeDmaSet(11,2)
# BLK32 (4) or MBLK(64) (5) VME transfers
sysVmeDmaSet(12,4)






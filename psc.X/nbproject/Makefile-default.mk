#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../chkstat.c ../chksum.c ../commands.c ../init.c ../io_config.c ../psc.c ../response.c ../usb_function.c ../Utility.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/chkstat.o ${OBJECTDIR}/_ext/1472/chksum.o ${OBJECTDIR}/_ext/1472/commands.o ${OBJECTDIR}/_ext/1472/init.o ${OBJECTDIR}/_ext/1472/io_config.o ${OBJECTDIR}/_ext/1472/psc.o ${OBJECTDIR}/_ext/1472/response.o ${OBJECTDIR}/_ext/1472/usb_function.o ${OBJECTDIR}/_ext/1472/Utility.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/chkstat.o.d ${OBJECTDIR}/_ext/1472/chksum.o.d ${OBJECTDIR}/_ext/1472/commands.o.d ${OBJECTDIR}/_ext/1472/init.o.d ${OBJECTDIR}/_ext/1472/io_config.o.d ${OBJECTDIR}/_ext/1472/psc.o.d ${OBJECTDIR}/_ext/1472/response.o.d ${OBJECTDIR}/_ext/1472/usb_function.o.d ${OBJECTDIR}/_ext/1472/Utility.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/chkstat.o ${OBJECTDIR}/_ext/1472/chksum.o ${OBJECTDIR}/_ext/1472/commands.o ${OBJECTDIR}/_ext/1472/init.o ${OBJECTDIR}/_ext/1472/io_config.o ${OBJECTDIR}/_ext/1472/psc.o ${OBJECTDIR}/_ext/1472/response.o ${OBJECTDIR}/_ext/1472/usb_function.o ${OBJECTDIR}/_ext/1472/Utility.o

# Source Files
SOURCEFILES=../chkstat.c ../chksum.c ../commands.c ../init.c ../io_config.c ../psc.c ../response.c ../usb_function.c ../Utility.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GB106
MP_LINKER_FILE_OPTION=,-Tp24FJ256GB106.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/chkstat.o: ../chkstat.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o.ok ${OBJECTDIR}/_ext/1472/chkstat.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/chkstat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/chkstat.o.d" -o ${OBJECTDIR}/_ext/1472/chkstat.o ../chkstat.c    
	
${OBJECTDIR}/_ext/1472/chksum.o: ../chksum.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o.ok ${OBJECTDIR}/_ext/1472/chksum.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/chksum.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/chksum.o.d" -o ${OBJECTDIR}/_ext/1472/chksum.o ../chksum.c    
	
${OBJECTDIR}/_ext/1472/commands.o: ../commands.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.ok ${OBJECTDIR}/_ext/1472/commands.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/commands.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/commands.o.d" -o ${OBJECTDIR}/_ext/1472/commands.o ../commands.c    
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.ok ${OBJECTDIR}/_ext/1472/init.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d" -o ${OBJECTDIR}/_ext/1472/init.o ../init.c    
	
${OBJECTDIR}/_ext/1472/io_config.o: ../io_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o.ok ${OBJECTDIR}/_ext/1472/io_config.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/io_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/io_config.o.d" -o ${OBJECTDIR}/_ext/1472/io_config.o ../io_config.c    
	
${OBJECTDIR}/_ext/1472/psc.o: ../psc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o.ok ${OBJECTDIR}/_ext/1472/psc.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/psc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/psc.o.d" -o ${OBJECTDIR}/_ext/1472/psc.o ../psc.c    
	
${OBJECTDIR}/_ext/1472/response.o: ../response.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o.ok ${OBJECTDIR}/_ext/1472/response.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/response.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/response.o.d" -o ${OBJECTDIR}/_ext/1472/response.o ../response.c    
	
${OBJECTDIR}/_ext/1472/usb_function.o: ../usb_function.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o.ok ${OBJECTDIR}/_ext/1472/usb_function.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/usb_function.o.d" -o ${OBJECTDIR}/_ext/1472/usb_function.o ../usb_function.c    
	
${OBJECTDIR}/_ext/1472/Utility.o: ../Utility.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o.ok ${OBJECTDIR}/_ext/1472/Utility.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Utility.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/Utility.o.d" -o ${OBJECTDIR}/_ext/1472/Utility.o ../Utility.c    
	
else
${OBJECTDIR}/_ext/1472/chkstat.o: ../chkstat.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o.ok ${OBJECTDIR}/_ext/1472/chkstat.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/chkstat.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/chkstat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/chkstat.o.d" -o ${OBJECTDIR}/_ext/1472/chkstat.o ../chkstat.c    
	
${OBJECTDIR}/_ext/1472/chksum.o: ../chksum.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o.ok ${OBJECTDIR}/_ext/1472/chksum.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/chksum.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/chksum.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/chksum.o.d" -o ${OBJECTDIR}/_ext/1472/chksum.o ../chksum.c    
	
${OBJECTDIR}/_ext/1472/commands.o: ../commands.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o.ok ${OBJECTDIR}/_ext/1472/commands.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/commands.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/commands.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/commands.o.d" -o ${OBJECTDIR}/_ext/1472/commands.o ../commands.c    
	
${OBJECTDIR}/_ext/1472/init.o: ../init.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o.ok ${OBJECTDIR}/_ext/1472/init.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/init.o.d" -o ${OBJECTDIR}/_ext/1472/init.o ../init.c    
	
${OBJECTDIR}/_ext/1472/io_config.o: ../io_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o.ok ${OBJECTDIR}/_ext/1472/io_config.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/io_config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/io_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/io_config.o.d" -o ${OBJECTDIR}/_ext/1472/io_config.o ../io_config.c    
	
${OBJECTDIR}/_ext/1472/psc.o: ../psc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o.ok ${OBJECTDIR}/_ext/1472/psc.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/psc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/psc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/psc.o.d" -o ${OBJECTDIR}/_ext/1472/psc.o ../psc.c    
	
${OBJECTDIR}/_ext/1472/response.o: ../response.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o.ok ${OBJECTDIR}/_ext/1472/response.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/response.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/response.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/response.o.d" -o ${OBJECTDIR}/_ext/1472/response.o ../response.c    
	
${OBJECTDIR}/_ext/1472/usb_function.o: ../usb_function.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o.ok ${OBJECTDIR}/_ext/1472/usb_function.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/usb_function.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/usb_function.o.d" -o ${OBJECTDIR}/_ext/1472/usb_function.o ../usb_function.c    
	
${OBJECTDIR}/_ext/1472/Utility.o: ../Utility.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o.ok ${OBJECTDIR}/_ext/1472/Utility.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/Utility.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Utility.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I".." -I"." -mlarge-data -msmall-scalar -MMD -MF "${OBJECTDIR}/_ext/1472/Utility.o.d" -o ${OBJECTDIR}/_ext/1472/Utility.o ../Utility.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG  -o ${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,--stack=16,-L"..",-L".",-Map="${DISTDIR}/psc.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
else
${DISTDIR}/psc.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/psc.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,--stack=16,-L"..",-L".",-Map="${DISTDIR}/psc.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex ${DISTDIR}/psc.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

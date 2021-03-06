/* $Id$ */
/** @file
 * IPRT, ISO 9660 File System
 */

/*
 * Copyright (C) 2017 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */

#ifndef ___iprt_formats_iso9660_h
#define ___iprt_formats_iso9660_h

#include <iprt/types.h>
#include <iprt/assert.h>


/** @defgroup grp_rt_formats_iso9660    ISO 9660 structures and definitions
 * @ingroup grp_rt_formats
 * @{
 */


/** The (default) logical sectors size of ISO 9660. */
#define ISO9660_SECTOR_SIZE                 2048

/** Accessor for ISO9660U16 and ISO9660U32 that retrievs the member value for
 *  the host endianess. */
#ifdef RT_BIG_ENDIAN
# define ISO9660_GET_ENDIAN(a_pInt) ((a_pInt)->be)
#else
# define ISO9660_GET_ENDIAN(a_pInt) ((a_pInt)->le)
#endif


/**
 * ISO 9660 16-bit unsigned integer type.
 */
typedef struct ISO9660U16
{
    /** Little endian. */
    uint16_t            le;
    /** Big endian. */
    uint16_t            be;
} ISO9660U16;
/** Pointer to an ISO 9660 16-bit unsigned integer type. */
typedef ISO9660U16 *PISO9660U16;
/** Pointer to a const ISO 9660 16-bit unsigned integer type. */
typedef ISO9660U16 const *PCISO9660U16;

/** ISO 9660 big endian 16-bit unsigned integer. */
typedef uint16_t        ISO9660U16BE;


/**
 * ISO 9660 32-bit unsigned integer type.
 */
typedef struct ISO9660U32
{
    /** Little endian. */
    uint32_t            le;
    /** Big endian. */
    uint32_t            be;
} ISO9660U32;
/** Pointer to an ISO 9660 32-bit unsigned integer type. */
typedef ISO9660U32 *PISO9660U32;
/** Pointer to a const ISO 9660 32-bit unsigned integer type. */
typedef ISO9660U32 const *PCISO9660U32;

/** ISO 9660 little endian 32-bit unsigned integer. */
typedef uint32_t        ISO9660U32LE;
/** ISO 9660 big endian 32-bit unsigned integer. */
typedef uint32_t        ISO9660U32BE;

/**
 * ISO 9660 timestamp (date & time).
 */
typedef struct ISO9660TIMESTAMP
{
    /** 0x00: For digit year (0001-9999). */
    char                achYear[4];
    /** 0x04: Month of the year (01-12). */
    char                achMonth[2];
    /** 0x06: Day of month (01-31). */
    char                achDay[2];
    /** 0x08: Hour of day (00-23). */
    char                achHour[2];
    /** 0x0a: Minute of hour (00-59). */
    char                achMinute[2];
    /** 0x0c: Second of minute (00-59). */
    char                achSecond[2];
    /** 0x0e: Hundreth of second (00-99). */
    char                achCentisecond[2];
    /** 0x10: The UTC (GMT) offset in 15 min units. */
    int8_t              offUtc;
} ISO9660TIMESTAMP;
AssertCompileSize(ISO9660TIMESTAMP, 17);
/** Pointer to an ISO 9660 timestamp. */
typedef ISO9660TIMESTAMP *PISO9660TIMESTAMP;
/** Pointer to a const ISO 9660 timestamp. */
typedef ISO9660TIMESTAMP const *PCISO9660TIMESTAMP;

/**
 * ISO 9660 record timestamp (date & time).
 */
typedef struct ISO9660RECTIMESTAMP
{
    /** 0: Years since 1900. */
    uint8_t             bYear;
    /** 1: Month of year (1-12). */
    uint8_t             bMonth;
    /** 2: Day of month (1-31). */
    uint8_t             bDay;
    /** 3: Hour of day (0-23). */
    uint8_t             bHour;
    /** 4: Minute of hour (0-59). */
    uint8_t             bMinute;
    /** 5: Second of minute (0-59). */
    uint8_t             bSecond;
    /** 6: The UTC (GMT) offset in 15 min units. */
    int8_t              offUtc;
} ISO9660RECTIMESTAMP;
AssertCompileSize(ISO9660RECTIMESTAMP, 7);
/** Pointer to an ISO 9660 record timestamp. */
typedef ISO9660RECTIMESTAMP *PISO9660RECTIMESTAMP;
/** Pointer to a const ISO 9660 record timestamp. */
typedef ISO9660RECTIMESTAMP const *PCISO9660RECTIMESTAMP;


/**
 * ISO 9660 directory record.
 */
#pragma pack(1)
typedef struct ISO9660DIRREC
{
    /** 0x00: Length of this record in bytes. */
    uint8_t             cbDirRec;
    /** 0x01: Extended attribute record length in bytes? */
    uint8_t             cbExtAttr;
    /** 0x02: Location of extent (logical block number).
     * @note Misaligned. */
    ISO9660U32          offExtent;
    /** 0x0a: Size of the data (file section).  Does not include EAs.
     * @note Misaligned. */
    ISO9660U32          cbData;
    /** 0x12: Recording time and date. */
    ISO9660RECTIMESTAMP RecTime;
    /** 0x19: File flags (ISO9660_FILE_FLAGS_XXX). */
    uint8_t             fFileFlags;
    /** 0x1a: File unit size for interlaved mode. */
    uint8_t             bFileUnitSize;
    /** 0x1b: Interlave gap size. */
    uint8_t             bInterleaveGapSize;
    /** 0x1c: Volume sequence number where the extent resides. */
    ISO9660U16          VolumeSeqNo;
    /** 0x20: Length of file identifier field. */
    uint8_t             bFileIdLength;
    /** 0x21: File identifier (d-characters or d1-characters). */
    char                achFileId[1];
    /* There are more fields following:
     *   - one byte optional padding so the following field is at an even boundrary.
     *   - system use field until cbDirRec is reached.
     */
} ISO9660DIRREC;
#pragma pack()
AssertCompileMemberOffset(ISO9660DIRREC, offExtent,     0x02);
AssertCompileMemberOffset(ISO9660DIRREC, cbData,        0x0a);
AssertCompileMemberOffset(ISO9660DIRREC, RecTime,       0x12);
AssertCompileMemberOffset(ISO9660DIRREC, fFileFlags,    0x19);
AssertCompileMemberOffset(ISO9660DIRREC, bFileIdLength, 0x20);
AssertCompileMemberOffset(ISO9660DIRREC, achFileId,     0x21);
/** Pointer to an ISO 9660 directory record. */
typedef ISO9660DIRREC *PISO9660DIRREC;
/** Pointer to a const ISO 9660 directory record. */
typedef ISO9660DIRREC const *PCISO9660DIRREC;

/** @name ISO9660_FILE_FLAGS_XXX
 * @{ */
/** Existence - Hide the file from the user. */
#define ISO9660_FILE_FLAGS_HIDDEN           UINT8_C(0x01)
/** Directory - Indicates a directory as apposed to a regular file (0). */
#define ISO9660_FILE_FLAGS_DIRECTORY        UINT8_C(0x02)
/** Assocated File - Indicates that the file is an associated file. */
#define ISO9660_FILE_FLAGS_ASSOCIATED_FILE  UINT8_C(0x04)
/** Record - Indicates specified file content record format (see EAs). */
#define ISO9660_FILE_FLAGS_RECORD           UINT8_C(0x08)
/** Protection - Indicates owner/group or permission protection in EAs. */
#define ISO9660_FILE_FLAGS_PROTECTION       UINT8_C(0x10)
/** Reserved bit, MBZ. */
#define ISO9660_FILE_FLAGS_RESERVED_5       UINT8_C(0x20)
/** Reserved bit, MBZ. */
#define ISO9660_FILE_FLAGS_RESERVED_6       UINT8_C(0x40)
/** Multi-extend - Indicates that this isn't the final record for the file.
 * @remarks Use for working around 4 GiB file size limitation. */
#define ISO9660_FILE_FLAGS_MULTI_EXTENT     UINT8_C(0x80)
/** @} */


/**
 * ISO 9660 path table record.
 */
#pragma pack(1)
typedef struct ISO9660PATHREC
{
    /** 0x00: Length of this record in bytes. */
    uint8_t             cbDirId;
    /** 0x01: Extended attribute record length in bytes? */
    uint8_t             cbExtAttr;
    /** 0x02: Location of extent (logical block number).
     * @note Endianess depends on table.
     * @note Misaligned. */
    uint32_t            offExtent;
    /** 0x06: Parent directory number.
     * @note Endianess depends on table.  */
    uint16_t            idParentRec;
    /** 0x08: Directory identifier (d-characters or d1-characters). */
    char                achDirId[RT_FLEXIBLE_ARRAY];
    /* There will be a zero padding byte following if the directory identifier length is odd. */
} ISO9660PATHREC;
#pragma pack()
AssertCompileMemberOffset(ISO9660PATHREC, cbExtAttr,   0x01);
AssertCompileMemberOffset(ISO9660PATHREC, offExtent,   0x02);
AssertCompileMemberOffset(ISO9660PATHREC, idParentRec, 0x06);
AssertCompileMemberOffset(ISO9660PATHREC, achDirId,   0x08);


/**
 * ISO 9660 extended attribute record.
 */
typedef struct ISO9660EXATTRREC
{
    /** 0x000: The owener ID. */
    ISO9660U16          idOwner;
    /** 0x004: The owener ID. */
    ISO9660U16          idGroup;
    /** 0x008: File permissions (ISO9660_PERM_XXX). */
    ISO9660U16BE        fPermissions;
    /** 0x00a: File creation timestamp. */
    ISO9660TIMESTAMP    BirthTimestamp;
    /** 0x01b: File modification timestamp. */
    ISO9660TIMESTAMP    ModifyTimestamp;
    /** 0x02c: File expiration timestamp. */
    ISO9660TIMESTAMP    ExpireTimestamp;
    /** 0x03d: File effective timestamp. */
    ISO9660TIMESTAMP    EffectiveTimestamp;
    /** 0x04e: Record format. */
    uint8_t             bRecordFormat;
    /** 0x04f: Record attributes. */
    uint8_t             fRecordAttrib;
    /** 0x050: Record length. */
    ISO9660U16          RecordLength;
    /** 0x054: System identifier (a-characters or a1-characters). */
    char                achSystemId[0x20];
    /** 0x074: System specific bytes. */
    uint8_t             abSystemUse[64];
    /** 0x0b4: Extended attribute record version (ISO9660EXATTRREC_VERSION).   */
    uint8_t             bExtRecVersion;
    /** 0x0b5: Length of escape sequences. */
    uint8_t             cbEscapeSequences;
    /** 0x0b6: Reserved for the future, MBZ. */
    uint8_t             abReserved183[64];
    /** 0x0f6: Length of the application use field. */
    ISO9660U16          cbAppUse;
    /** 0x0fa: Variable sized application use field. */
    uint8_t             abAppUse[RT_FLEXIBLE_ARRAY];
    /* This is followed by escape sequences with length given by cbEscapeSequnces. */
} ISO9660EXATTRREC;
AssertCompileMemberOffset(ISO9660EXATTRREC, EffectiveTimestamp, 0x03d);
AssertCompileMemberOffset(ISO9660EXATTRREC, cbAppUse, 0x0f6);

/** The ISO9660EXATTRREC::bExtRecVersion value.    */
#define ISO9660EXATTRREC_VERSION        UINT8_C(0x01)

/** @name ISO9660_PERM_XXX - ISO9660EXATTRREC::fPermissions
 * @{ */
/** @todo figure out this wird permission stuff...   */
/** @} */


/**
 * ISO 9660 volume descriptor header.
 */
typedef struct ISO9660VOLDESCHDR
{
    /** Descriptor type ISO9660VOLDESC_TYPE_XXX. */
    uint8_t             bDescType;
    /** Standard identifier 'CD001'   */
    uint8_t             achStdId[5];
    /** The descriptor version. */
    uint8_t             bDescVersion;
    /* (This is followed by the descriptor specific data). */
} ISO9660VOLDESCHDR;
AssertCompileSize(ISO9660VOLDESCHDR, 7);
/** Pointer to a volume descriptor header.  */
typedef ISO9660VOLDESCHDR *PISO9660VOLDESCHDR;
/** Pointer to a const volume descriptor header. */
typedef ISO9660VOLDESCHDR const *PCISO9660VOLDESCHDR;

/** @name ISO9660VOLDESC_TYPE_XXX - volume descriptor types
 * @{ */
/** See ISO9660BOOTRECORD. */
#define ISO9660VOLDESC_TYPE_BOOT_RECORD     UINT8_C(0x00)
/** See ISO9660PRIMARYVOLDESC. */
#define ISO9660VOLDESC_TYPE_PRIMARY         UINT8_C(0x01)
/** See ISO9660SUPVOLDESC. */
#define ISO9660VOLDESC_TYPE_SUPPLEMENTARY   UINT8_C(0x02)
/** See ISO9660VOLPARTDESC. */
#define ISO9660VOLDESC_TYPE_PARTITION       UINT8_C(0x03)
/** Terminates the volume descriptor set.  Has no data (zeros), version is 1. */
#define ISO9660VOLDESC_TYPE_TERMINATOR      UINT8_C(0xff)
/** @} */

/** The value of ISO9660VOLDESCHDR::achStdId   */
#define ISO9660VOLDESC_STD_ID               "CD001"
#define ISO9660VOLDESC_STD_ID_0             'C'
#define ISO9660VOLDESC_STD_ID_1             'D'
#define ISO9660VOLDESC_STD_ID_2             '0'
#define ISO9660VOLDESC_STD_ID_3             '0'
#define ISO9660VOLDESC_STD_ID_4             '1'



/**
 * ISO 9660 boot record (volume descriptor).
 */
typedef struct ISO9660BOOTRECORD
{
    /** The volume descriptor header.
     * Type is ISO9660VOLDESC_TYPE_BOOT_RECORD and version
     * ISO9660BOOTRECORD_VERSION. */
    ISO9660VOLDESCHDR   Hdr;
    /** Boot system identifier string (a-characters). */
    char                achBootSystemId[32];
    /** Boot identifier (a-characters). */
    char                achBootId[32];
    /** Boot system specific content. */
    uint8_t             abBootSystemSpecific[1977];
} ISO9660BOOTRECORD;
AssertCompileSize(ISO9660BOOTRECORD, ISO9660_SECTOR_SIZE);
/** Pointer to a ISO 9660 boot record. */
typedef ISO9660BOOTRECORD *PISO9660BOOTRECORD;
/** Pointer to a const ISO 9660 boot record. */
typedef ISO9660BOOTRECORD const *PCISO9660BOOTRECORD;

/** The value of ISO9660BOOTRECORD::Hdr.uDescVersion. */
#define ISO9660BOOTRECORD_VERSION           UINT8_C(1)



/**
 * ISO 9660 primary volume descriptor.
 */
typedef struct ISO9660PRIMARYVOLDESC
{
    /** 0x000: The volume descriptor header.
     * Type is ISO9660VOLDESC_TYPE_PRIMARY and version
     * ISO9660PRIMARYVOLDESC_VERSION. */
    ISO9660VOLDESCHDR   Hdr;
    /** 0x007: Explicit alignment zero padding. */
    uint8_t             bPadding8;
    /** 0x008: System identifier (a-characters). */
    char                achSystemId[32];
    /** 0x028: Volume identifier (d-characters). */
    char                achVolumeId[32];
    /** 0x048: Unused field, zero filled. */
    ISO9660U32          Unused73;
    /** 0x050: Volume space size in logical blocks (cbLogicalBlock). */
    ISO9660U32          VolumeSpaceSize;
    /** 0x058: Unused field(s), zero filled. */
    uint8_t             abUnused89[32];
    /** 0x078: The number of volumes in the volume set. */
    ISO9660U16          cVolumesInSet;
    /** 0x07c: Volume sequence number. */
    ISO9660U16          VolumeSeqNo;
    /** 0x080: Logical block size in bytes. */
    ISO9660U16          cbLogicalBlock;
    /** 0x084: Path table size. */
    ISO9660U32          cbPathTable;
    /** 0x08c: Type L(ittle endian) path table location (block offset). */
    ISO9660U32LE        offTypeLPathTable;
    /** 0x090: Optional type L(ittle endian) path table location (block offset). */
    ISO9660U32LE        offOptionalTypeLPathTable;
    /** 0x094: Type M (big endian) path table location (block offset). */
    ISO9660U32BE        offTypeMPathTable;
    /** 0x098: Optional type M (big endian) path table location (block offset). */
    ISO9660U32BE        offOptionalTypeMPathTable;
    /** 0x09c: Directory entry for the root directory (union). */
    union
    {
        uint8_t         ab[34];
        ISO9660DIRREC   DirRec;
    }                   RootDir;
    /** 0x0be: Volume set identifier (d-characters). */
    char                achVolumeSetId[128];
    /** 0x13e: Publisher identifier (a-characters).  Alternatively, it may refere to
     * a file in the root dir if it starts with 0x5f and restricts itself to 8
     * d-characters. */
    char                achPublisherId[128];
    /** 0x1be: Data preparer identifier (a-characters).
     * Same file reference alternative as previous field. */
    char                achDataPreparerId[128];
    /** 0x23e: Application identifier (a-characters).
     * Same file reference alternative as previous field. */
    char                achApplicationId[128];
    /** 0x2be: Copyright (root) file identifier (d-characters).
     * All spaces if none. */
    char                achCopyrightFileId[37];
    /** 0x2e3: Abstract (root) file identifier (d-characters).
     * All spaces if none. */
    char                achAbstractFileId[37];
    /** 0x308: Bibliographic file identifier (d-characters).
     * All spaces if none. */
    char                achBibliographicFileId[37];
    /** 0x32d: Volume creation date and time. */
    ISO9660TIMESTAMP    BirthTime;
    /** 0x33e: Volume modification date and time. */
    ISO9660TIMESTAMP    ModifyTime;
    /** 0x34f: Volume (data) expiration date and time.
     * If not specified, don't regard data as obsolete. */
    ISO9660TIMESTAMP    ExpireTime;
    /** 0x360: Volume (data) effective date and time.
     * If not specified, info can be used immediately. */
    ISO9660TIMESTAMP    EffectiveTime;
    /** 0x371: File structure version (ISO9660_FILE_STRUCTURE_VERSION). */
    uint8_t             bFileStructureVersion;
    /** 0x372: Reserve for future, MBZ. */
    uint8_t             bReserved883;
    /** 0x373: Reserve for future, MBZ. */
    uint8_t             abAppUse[512];
    /** 0x573: Reserved for future standardization, MBZ. */
    uint8_t             abReserved1396[653];
} ISO9660PRIMARYVOLDESC;
AssertCompileSize(ISO9660PRIMARYVOLDESC, ISO9660_SECTOR_SIZE);
/** Pointer to a ISO 9660 primary volume descriptor. */
typedef ISO9660PRIMARYVOLDESC *PISO9660PRIMARYVOLDESC;
/** Pointer to a const ISO 9660 primary volume descriptor. */
typedef ISO9660PRIMARYVOLDESC const *PCISO9660PRIMARYVOLDESC;

/** The value of ISO9660PRIMARYVOLDESC::Hdr.uDescVersion. */
#define ISO9660PRIMARYVOLDESC_VERSION           UINT8_C(1)
/** The value of ISO9660PRIMARYVOLDESC::bFileStructureVersion and
 *  ISO9660SUPVOLDESC::bFileStructureVersion. */
#define ISO9660_FILE_STRUCTURE_VERSION          UINT8_C(1)



/**
 * ISO 9660 supplementary volume descriptor.
 *
 * This is in the large parts identicial to the primary descriptor, except it
 * have a few more fields where the primary one has reserved spaces.
 */
typedef struct ISO9660SUPVOLDESC
{
    /** 0x000: The volume descriptor header.
     * Type is ISO9660VOLDESC_TYPE_SUPPLEMENTARY and version
     * ISO9660SUPVOLDESC_VERSION. */
    ISO9660VOLDESCHDR   Hdr;
    /** 0x007: Volume flags (ISO9660SUPVOLDESC_VOL_F_XXX).
     * @note This is reserved in the primary volume descriptor. */
    uint8_t             fVolumeFlags;
    /** 0x008: System identifier (a1-characters) of system that can act upon
     * sectors 0 thru 15.
     * @note Purpose differs from primary description. */
    char                achSystemId[32];
    /** 0x028: Volume identifier (d1-characters).
     * @note Character set differs from primary description. */
    char                achVolumeId[32];
    /** 0x048: Unused field, zero filled. */
    ISO9660U32          Unused73;
    /** 0x050: Volume space size in logical blocks (cbLogicalBlock). */
    ISO9660U32          VolumeSpaceSize;
    /** 0x058: Escape sequences.
     * Complicated stuff, see ISO 2022 and ECMA-35.
     * @note This is reserved in the primary volume descriptor. */
    uint8_t             abEscapeSequences[32];
    /** 0x078: The number of volumes in the volume set. */
    ISO9660U16          cVolumesInSet;
    /** 0x07c: Volume sequence number. */
    ISO9660U16          VolumeSeqNo;
    /** 0x080: Logical block size in bytes. */
    ISO9660U16          cbLogicalBlock;
    /** 0x084: Path table size. */
    ISO9660U32          cbPathTable;
    /** 0x08c: Type L(ittle endian) path table location (block offset). */
    ISO9660U32LE        offTypeLPathTable;
    /** 0x090: Optional type L(ittle endian) path table location (block offset). */
    ISO9660U32LE        offOptionalTypeLPathTable;
    /** 0x094: Type M (big endian) path table location (block offset). */
    ISO9660U32BE        offTypeMPathTable;
    /** 0x098: Optional type M (big endian) path table location (block offset). */
    ISO9660U32BE        offOptionalTypeMPathTable;
    /** 0x09c: Directory entry for the root directory (union). */
    union
    {
        uint8_t         ab[34];
        ISO9660DIRREC   DirRec;
    }                   RootDir;
    /** 0x0be: Volume set identifier (d1-characters).
     * @note Character set differs from primary description. */
    char                achVolumeSetId[128];
    /** 0x13e: Publisher identifier (a1-characters).  Alternatively, it may refere
     * to a file in the root dir if it starts with 0x5f and restricts itself to 8
     * d1-characters.
     * @note Character set differs from primary description. */
    char                achPublisherId[128];
    /** 0x1be: Data preparer identifier (a1-characters).
     * Same file reference alternative as previous field.
     * @note Character set differs from primary description. */
    char                achDataPreparerId[128];
    /** 0x23e: Application identifier (a1-characters).
     * Same file reference alternative as previous field.
     * @note Character set differs from primary description. */
    char                achApplicationId[128];
    /** 0x2be: Copyright (root) file identifier (d1-characters).
     * All spaces if none.
     * @note Character set differs from primary description. */
    char                achCopyrightFileId[37];
    /** 0x2e3: Abstract (root) file identifier (d1-characters).
     * All spaces if none.
     * @note Character set differs from primary description. */
    char                achAbstractFileId[37];
    /** 0x308: Bibliographic file identifier (d1-characters).
     * All spaces if none.
     * @note Character set differs from primary description. */
    char                achBibliographicFileId[37];
    /** 0x32d: Volume creation date and time. */
    ISO9660TIMESTAMP    BirthTime;
    /** 0x33e: Volume modification date and time. */
    ISO9660TIMESTAMP    ModifyTime;
    /** 0x34f: Volume (data) expiration date and time.
     * If not specified, don't regard data as obsolete. */
    ISO9660TIMESTAMP    ExpireTime;
    /** 0x360: Volume (data) effective date and time.
     * If not specified, info can be used immediately. */
    ISO9660TIMESTAMP    EffectiveTime;
    /** 0x371: File structure version (ISO9660_FILE_STRUCTURE_VERSION). */
    uint8_t             bFileStructureVersion;
    /** 0x372: Reserve for future, MBZ. */
    uint8_t             bReserved883;
    /** 0x373: Reserve for future, MBZ. */
    uint8_t             abAppUse[512];
    /** 0x573: Reserved for future standardization, MBZ. */
    uint8_t             abReserved1396[653];
} ISO9660SUPVOLDESC;
AssertCompileSize(ISO9660SUPVOLDESC, ISO9660_SECTOR_SIZE);
/** Pointer to a ISO 9660 supplementary volume descriptor. */
typedef ISO9660SUPVOLDESC *PISO9660SUPVOLDESC;
/** Pointer to a const ISO 9660 supplementary volume descriptor. */
typedef ISO9660SUPVOLDESC const *PCISO9660SUPVOLDESC;
/** The value of ISO9660SUPVOLDESC::Hdr.uDescVersion. */
#define ISO9660SUPVOLDESC_VERSION               UINT8_C(1)

/** @name ISO9660SUPVOLDESC_VOL_F_XXX - ISO9660SUPVOLDESC::fVolumeFlags
 * @{ */
#define ISO9660SUPVOLDESC_VOL_F_ESC_ONLY_REG    UINT8_C(0x00)
#define ISO9660SUPVOLDESC_VOL_F_ESC_NOT_REG     UINT8_C(0x01)
/** @} */



/**
 * ISO 9660 volume partition descriptor.
 */
typedef struct ISO9660VOLPARTDESC
{
    /** 0x000: The volume descriptor header.
     * Type is ISO9660VOLDESC_TYPE_PARTITION and version
     * ISO9660VOLPARTDESC_VERSION. */
    ISO9660VOLDESCHDR   Hdr;
    /** 0x007: Alignment padding. */
    uint8_t             bPadding8;
    /** 0x008: System identifier (a-characters). */
    char                achSystemId[32];
    /** 0x028: Volume partition identifier (d-characters). */
    char                achVolumePartitionId[32];
    /** 0x048: The location of the partition (logical block number). */
    ISO9660U32          offVolumePartition;
    /** 0x050: The partition size in logical blocks (cbLogicalBlock). */
    ISO9660U32          VolumePartitionSize;
    /** 0x058: System specific data. */
    uint8_t             achSystemUse[1960];
} ISO9660VOLPARTDESC;
AssertCompileSize(ISO9660VOLPARTDESC, ISO9660_SECTOR_SIZE);
/** Pointer to an ISO 9660 volume partition description. */
typedef ISO9660VOLPARTDESC *PISO9660VOLPARTDESC;
/** Pointer to a const ISO 9660 volume partition description. */
typedef ISO9660VOLPARTDESC const *PCISO9660VOLPARTDESC;
/** The value of ISO9660VOLPARTDESC::Hdr.uDescVersion. */
#define ISO9660VOLPARTDESC_VERSION               UINT8_C(1)



/** @name Joliet escape sequence identifiers.
 *
 * These bytes appears in the supplementary volume descriptor field
 * abEscapeSequences.  The ISO9660SUPVOLDESC_VOL_F_ESC_NOT_REG flags will not
 * be set.
 *
 * @{ */
#define ISO9660_JOLIET_ESC_SEQ_0            UINT8_C(0x25)   /**< First escape sequence byte.*/
#define ISO9660_JOLIET_ESC_SEQ_1            UINT8_C(0x2f)   /**< Second escape sequence byte.*/
#define ISO9660_JOLIET_ESC_SEQ_2_LEVEL_1    UINT8_C(0x40)   /**< Third escape sequence byte: level 1 */
#define ISO9660_JOLIET_ESC_SEQ_2_LEVEL_2    UINT8_C(0x43)   /**< Third escape sequence byte: level 2 */
#define ISO9660_JOLIET_ESC_SEQ_2_LEVEL_3    UINT8_C(0x45)   /**< Third escape sequence byte: level 3 */
/** @} */


/** @} */

#endif


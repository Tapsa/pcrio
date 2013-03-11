/*
    Copyright (c) 2012, Armin Preiml

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted (subject to the limitations in the
    disclaimer below) provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the
      distribution.

    * Neither the name of the author nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
    GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
    HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
    IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PCRDEF_H
#define PCRDEF_H

#define IMAGE_OPTIONAL_HDR32_MAGIC 0x010b
#define IMAGE_OPTIONAL_HDR64_MAGIC 0x020b

#define PE_SIGNATURE "PE\0\0"

#define DATA_DIRECTORY_COUNT 16

enum Data_directory_id
{
  DATA_DIRECTORY_ID_RESOURCE = 2
};

// IDs of the Type nodes after the root node in the rsrc section
enum Resource_type
{
  RESOURCE_TYPE_UNKNOWN = 0, // temporary type for root node, or unsupported
  RESOURCE_TYPE_STRINGS = 6,
  RESOURCE_TYPE_VERSION = 16
};

class Image_dos_header
{  // DOS .EXE header
public:
  uint16_t e_magic;         // Magic number
  uint16_t e_cblp;          // Bytes on last page of file
  uint16_t e_cp;            // Pages in file
  uint16_t e_crlc;          // Relocations
  uint16_t e_cparhdr;       // Size of header in paragraphs
  uint16_t e_minalloc;      // Minimum extra paragraphs needed
  uint16_t e_maxalloc;      // Maximum extra paragraphs needed
  uint16_t e_ss;            // Initial (relative) SS value
  uint16_t e_sp;            // Initial SP value
  uint16_t e_csum;          // Checksum
  uint16_t e_ip;            // Initial IP value
  uint16_t e_cs;            // Initial (relative) CS value
  uint16_t e_lfarlc;        // File address of relocation table
  uint16_t e_ovno;          // Overlay number
  uint16_t e_res[4];        // Reserved words
  uint16_t e_oemid;         // OEM identifier (for e_oeminfo)
  uint16_t e_oeminfo;       // OEM information; e_oemid specific
  uint16_t e_res2[10];      // Reserved words
  uint32_t e_lfanew;        // File address of new exe header
};

class Image_file_header
{
public:
  uint16_t machine;
  uint16_t number_of_sections;
  uint32_t time_stamp;
  uint32_t symbol_table_ptr;
  uint32_t symbol_count;
  uint16_t size_of_optional_header;
  uint16_t charactersitics;
};

class Image_data_directory
{
public:
  uint32_t rva;
  uint32_t size;
};

class Image_optional_header32
{
public:
  uint16_t magic;
  unsigned char major_linker_version;
  unsigned char minor_linker_version;
  uint32_t size_of_code;
  uint32_t size_of_initialized_data;
  uint32_t size_of_uninitialized_data;
  uint32_t address_of_entry_point;
  uint32_t base_of_code;
  uint32_t base_of_data;
  uint32_t image_base;
  uint32_t section_alignment;
  uint32_t file_alignment;
  uint16_t major_operating_system_version;
  uint16_t minor_operating_system_version;
  uint16_t major_image_version;
  uint16_t minor_image_version;
  uint16_t major_subsystem_version;
  uint16_t minor_subsystem_version;
  uint32_t win32_version_value;
  uint32_t size_of_image;
  uint32_t size_of_headers;
  uint32_t check_sum;
  uint16_t subsystem;
  uint16_t dll_characteristics;
  uint32_t size_of_stack_reserve;
  uint32_t size_of_stack_commit;
  uint32_t size_of_heap_reserve;
  uint32_t size_of_heap_commit;
  uint32_t loader_flags;
  uint32_t number_of_rva_and_sizes;
  Image_data_directory data_directory[DATA_DIRECTORY_COUNT];
};

class Image_section_header
{
public:
  char name[8]; // needs extra treatment for names > 8 characters (see doc)
  uint32_t virtual_size;
  uint32_t virtual_adress;
  uint32_t size_of_raw_data;
  uint32_t pointer_to_raw_data;
  uint32_t pointer_to_relocations;
  uint32_t pointer_to_linenumbers;
  uint16_t number_of_relocations;
  uint16_t number_of_linenumbers;
  uint32_t characteristics;
};

class Resource_data_entry
{
public:
  uint32_t data_rva; // Only important on read and will be overwritten on write
  uint32_t size;
  uint32_t codepage;
  uint32_t reserved; // must be 0

};

class Resource_data
{
public:
  enum Resource_type type; // if RESOURCE_TYPE_STRINGS: strings are set
                      // else data is filled

  char *raw_data;

  uint16_t number_of_strings;
  char **strings;

  Resource_data_entry data_entry;
};

class Resource_directory_entry
{
public:
  uint32_t id; //or name rva
  uint32_t rva; // if high bit: subdirectory_entry rva else: data_entry rva
};

class Resource_directory_table
{
public:
  uint32_t characteristics;
  uint32_t time_stamp;
  uint16_t major_version;
  uint16_t minor_version;
  uint16_t number_of_name_entries;
  uint16_t number_of_id_entries;
};

// tree in memory:
class Resource_tree_node
{
public:
  // if there is leaf data, directory table values are set to 0 and must not
  // be changed
  Resource_directory_table directory_table;

  // Do not touch! Only important on read and will automatically be set on
  // write.
  Resource_directory_entry directory_entry;

  // contains
  // either ( if name == NULL)
  uint32_t id;
  // or
  char *name;

  // contains
  // either subnodes
  Resource_tree_node **name_entries;
  Resource_tree_node **id_entries;

  // or leaf data
  Resource_data *resource_data;

};

class Pcr_language
{
public:
  uint32_t id;
  uint32_t codepage;
};

class Language_info
{
public:
  Pcr_language lang;
  uint32_t item_count;
};

class Language_info_array
{
public:
  Language_info *array;
  uint32_t count;
};

class Rsrc_string_ptr
{
public:
  char **sptr;

  uint32_t id;
  uint32_t language_id;
  uint32_t codepage;
};

/**
 * This class stores nodes containing the resource data. Windows uses three
 * levels to store the data.
 *
 * 1. Type      // Type of the data. E.g. Strings, Version info (see enum Resource_type)
 * 2. Name
 * 3. Language  // id of the language
 *
 */
class Resource_section_data
{
public:
  Resource_tree_node *root_node;

  Language_info_array Language_info;
  Pcr_language *default_language;

  // simple string cache, will improve access time if for e.g. 1. get_strlen, 2. get_string
  Rsrc_string_ptr rsrc_string_cache;
};

class Pcr_file
{
public:
  Image_dos_header dos_header;

  char *rm_stub; // MS-DOS Real-Mode Stub Program

  char signature[4];

  Image_file_header image_file_header;
  Image_optional_header32 *image_optional_header32;
  Image_section_header *section_table;

  Resource_section_data *rsrc_section_data;

  // other section data
  char **section_data;
};

#endif // PCRDEF_H

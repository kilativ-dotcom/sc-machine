/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _sc_types_h_
#  define _sc_types_h_

#  include "sc_defines.h"

#  ifndef null_ptr
#    define null_ptr ((void *)0)
#  endif

#  define sc_unused(object) (void)object

// base types
typedef signed char sc_int8;
typedef unsigned char sc_uint8;
typedef signed short sc_int16;
typedef unsigned short sc_uint16;
typedef signed int sc_int32;
typedef unsigned int sc_uint32;
typedef long long sc_int64;
typedef unsigned long long sc_uint64;
typedef float sc_float;

typedef unsigned long sc_ulong;
typedef unsigned int sc_uint;
typedef int sc_int;

typedef sc_uint32 sc_uint;
typedef char sc_char;
typedef unsigned char sc_uchar;

typedef void * sc_pointer;

#  define sc_min(a, b) ((a) < (b) ? (a) : (b))
#  define sc_max(a, b) ((a) > (b) ? (a) : (b))
#  define sc_boundary(x, a, b) sc_max(a, sc_min(x, b))

// booleans
#  define SC_FALSE 0
#  define SC_TRUE 1

typedef sc_uint8 sc_bool;

// type limits
#  define SC_MININT8 ((sc_int8)0x80)
#  define SC_MAXINT8 ((sc_int8)0x7f)
#  define SC_MAXUINT8 ((sc_uint8)0xff)

#  define SC_MININT16 ((sc_int16)0x8000)
#  define SC_MAXINT16 ((sc_int16)0x7fff)
#  define SC_MAXUINT16 ((sc_uint16)0xffff)

#  define SC_MININT32 ((sc_int32)0x80000000)
#  define SC_MAXINT32 ((sc_int32)0x7fffffff)
#  define SC_MAXUINT32 ((sc_uint32)0xffffffff)

#  define SC_ADDR_SEG_MAX SC_MAXUINT16
#  define SC_ADDR_OFFSET_MAX SC_MAXUINT16

#  define SC_SEGMENT_ELEMENTS_COUNT SC_MAXUINT16  // number of elements in segment
#  define SC_SEGMENT_MAX SC_MAXUINT16             // max number of segments

// Types for segment and offset
typedef sc_uint16 sc_addr_seg;
typedef sc_uint16 sc_addr_offset;

typedef sc_uint64 sc_addr_hash;

//! Structure to store sc-element address
struct _sc_addr
{
  sc_addr_seg seg;
  sc_addr_offset offset;
};

//! Make sc-addr empty
#  define SC_ADDR_MAKE_EMPTY(addr) \
    (addr).seg = 0; \
    (addr).offset = 0

#  define SC_ADDR_EMPTY \
    (sc_addr) \
    { \
      0, 0 \
    }

//! Check if specified sc-addr is empty
#  define SC_ADDR_IS_EMPTY(addr) (((addr).seg == 0) && ((addr).offset == 0))
#  define SC_ADDR_IS_NOT_EMPTY(addr) (!SC_ADDR_IS_EMPTY(addr))
//! Check if two sc-addr's are equivalent
#  define SC_ADDR_IS_EQUAL(addr, addr2) (((addr).seg == (addr2).seg) && ((addr).offset == (addr2).offset))
#  define SC_ADDR_IS_NOT_EQUAL(addr, addr2) (!SC_ADDR_IS_EQUAL(addr, addr2))

/*! Next defines help to pack local part of sc-addr (segment and offset) into int value
 * and get them back from int
 */
#  define SC_ADDR_LOCAL_TO_INT(addr) (sc_uint32)(((addr).seg << 16) | ((addr).offset & 0xffff))
#  define SC_ADDR_LOCAL_OFFSET_FROM_INT(v) (sc_uint16)((v)&0x0000ffff)
#  define SC_ADDR_LOCAL_SEG_FROM_INT(v) SC_ADDR_LOCAL_OFFSET_FROM_INT(v >> 16)
#  define SC_ADDR_LOCAL_FROM_INT(hash, addr) \
    addr.seg = SC_ADDR_LOCAL_SEG_FROM_INT(hash); \
    addr.offset = SC_ADDR_LOCAL_OFFSET_FROM_INT(hash)

typedef sc_uint16 sc_type;

// sc-element types
#  define sc_type_node (sc_type)0x1
#  define sc_type_link (sc_type)0x2
#  define sc_type_edge_common (sc_type)0x4
#  define sc_type_arc_common (sc_type)0x8
#  define sc_type_arc_access (sc_type)0x10

// sc-element constant
#  define sc_type_const (sc_type)0x20
#  define sc_type_var (sc_type)0x40

// sc-element positivity
#  define sc_type_arc_pos (sc_type)0x80
#  define sc_type_arc_neg (sc_type)0x100
#  define sc_type_arc_fuz (sc_type)0x200

// sc-element permanently
#  define sc_type_arc_temp (sc_type)0x400
#  define sc_type_arc_perm (sc_type)0x800

// struct node types
#  define sc_type_node_tuple (sc_type)(0x80)
#  define sc_type_node_struct (sc_type)(0x100)
#  define sc_type_node_role (sc_type)(0x200)
#  define sc_type_node_norole (sc_type)(0x400)
#  define sc_type_node_class (sc_type)(0x800)
#  define sc_type_node_abstract (sc_type)(0x1000)
#  define sc_type_node_material (sc_type)(0x2000)

#  define sc_type_arc_pos_const (sc_type)(sc_type_arc_access | sc_type_const | sc_type_arc_pos)
#  define sc_type_arc_neg_const (sc_type)(sc_type_arc_access | sc_type_const | sc_type_arc_neg)
#  define sc_type_arc_pos_const_perm (sc_type)(sc_type_arc_access | sc_type_const | sc_type_arc_pos | sc_type_arc_perm)
#  define sc_type_arc_pos_const_temp (sc_type)(sc_type_arc_access | sc_type_const | sc_type_arc_pos | sc_type_arc_temp)
#  define sc_type_arc_neg_const_temp (sc_type)(sc_type_arc_access | sc_type_const | sc_type_arc_neg | sc_type_arc_temp)
#  define sc_type_arc_pos_var_perm (sc_type)(sc_type_arc_access | sc_type_var | sc_type_arc_pos | sc_type_arc_perm)

// type mask
#  define sc_type_element_mask \
    (sc_type)(sc_type_node | sc_type_link | sc_type_edge_common | sc_type_arc_common | sc_type_arc_access)
#  define sc_type_constancy_mask (sc_type)(sc_type_const | sc_type_var)
#  define sc_type_positivity_mask (sc_type)(sc_type_arc_pos | sc_type_arc_neg | sc_type_arc_fuz)
#  define sc_type_permanency_mask (sc_type)(sc_type_arc_perm | sc_type_arc_temp)
#  define sc_type_node_mask \
    (sc_type)( \
        sc_type_node_tuple | sc_type_node_struct | sc_type_node_role | sc_type_node_norole | sc_type_node_class \
        | sc_type_node_abstract | sc_type_node_material)
#  define sc_type_arc_mask (sc_type)(sc_type_arc_access | sc_type_arc_common | sc_type_edge_common)

#  define sc_type_has_subtype(_type, _subtype) ((_type & _subtype) == _subtype)
#  define sc_type_has_not_subtype(_type, _subtype) !sc_type_has_subtype(_type, _subtype)
#  define sc_type_has_subtype_in_mask(_type, _mask) ((_type & _mask) != 0)
#  define sc_type_has_not_subtype_in_mask(_type, _mask) !sc_type_has_subtype_in_mask(_type, _mask)

#  define sc_type_is_structure_and_arc(_node_type, _arc_type) \
    (sc_type_has_subtype(_node_type, sc_type_node_struct) \
     && (sc_type_has_subtype_in_mask(_arc_type, sc_type_arc_pos_const)))

typedef sc_uint16 sc_states;

#  define SC_STATE_REQUEST_DELETION 0x1
#  define SC_STATE_ELEMENT_EXIST 0x2

#  define SC_ACCESS_LVL_MAX_VALUE 15
#  define SC_ACCESS_LVL_MIN_VALUE 0

#  define sc_access_lvl_make(read, write) (sc_uint8)((write) | ((read) << 4))

#  define sc_access_lvl_make_max sc_access_lvl_make(SC_ACCESS_LVL_MAX_VALUE, SC_ACCESS_LVL_MAX_VALUE)
#  define sc_access_lvl_make_min sc_access_lvl_make(SC_ACCESS_LVL_MIN_VALUE, SC_ACCESS_LVL_MIN_VALUE)

// results
enum _sc_result
{
  // SC_RESULT_ERROR should be 0 anytime
  SC_RESULT_ERROR = 0,             // unknown error
                                   // SC_RESULT_OK should be 1 anytime
  SC_RESULT_OK = 1,                // no any error
  SC_RESULT_NO,                    // no any result
  SC_RESULT_ERROR_INVALID_PARAMS,  // invalid function parameters error
  SC_RESULT_ERROR_INVALID_TYPE,    // invalid type error
  SC_RESULT_ERROR_INVALID_STATE,   // invalid state of processed object
  SC_RESULT_ERROR_NOT_FOUND,       // item not found
  SC_RESULT_ERROR_FULL_MEMORY,
  SC_RESULT_ERROR_FILE_MEMORY_IO,
  SC_RESULT_ERROR_ADDR_IS_NOT_VALID,
  SC_RESULT_ERROR_ELEMENT_IS_NOT_NODE,
  SC_RESULT_ERROR_ELEMENT_IS_NOT_LINK,
  SC_RESULT_ERROR_ELEMENT_IS_NOT_CONNECTOR,
  SC_RESULT_ERROR_STREAM_IO,
  SC_RESULT_ERROR_INVALID_SYSTEM_IDENTIFIER,
  SC_RESULT_ERROR_DUPLICATED_SYSTEM_IDENTIFIER,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_IS_NOT_AUTHENTICATED,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_READ_PERMISSIONS,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_WRITE_PERMISSIONS,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_ERASE_PERMISSIONS,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_PERMISSIONS_TO_READ_PERMISSIONS,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_PERMISSIONS_TO_WRITE_PERMISSIONS,
  SC_RESULT_ERROR_SC_MEMORY_CONTEXT_HAS_NO_PERMISSIONS_TO_ERASE_PERMISSIONS,
  SC_RESULT_UNKNOWN,  // result unknown

  // add atomic types before
  SC_RESULT_COUNT,  // number of result types
};

// events
enum _sc_event_type
{
  SC_EVENT_UNKNOWN = -1,
  SC_EVENT_ADD_OUTPUT_ARC = 0,
  SC_EVENT_ADD_INPUT_ARC = 1,
  SC_EVENT_REMOVE_OUTPUT_ARC = 2,
  SC_EVENT_REMOVE_INPUT_ARC = 3,
  SC_EVENT_REMOVE_ELEMENT = 4,
  SC_EVENT_CONTENT_CHANGED = 5
};

// structure to store statistics info
struct _sc_stat
{
  sc_uint64 node_count;  // amount of all sc-nodes stored in memory
  sc_uint64 arc_count;   // amount of all sc-arcs stored in memory
  sc_uint64 link_count;  // amount of all sc-links stored in memory
};

#endif

typedef struct _sc_arc sc_arc;
typedef struct _sc_arc_info sc_arc_info;
typedef sc_uint16 sc_permissions;
typedef struct _sc_element_flags sc_element_flags;
typedef struct _sc_memory_context sc_memory_context;
typedef struct _sc_element sc_element;
typedef struct _sc_segment sc_segment;
typedef struct _sc_addr sc_addr;
typedef struct _sc_iterator_param sc_iterator_param;
typedef struct _sc_iterator_result sc_iterator_result;
typedef struct _sc_iterator3 sc_iterator3;
typedef struct _sc_iterator5 sc_iterator5;
typedef struct _sc_event sc_event;
typedef enum _sc_result sc_result;
typedef enum _sc_event_type sc_event_type;
typedef struct _sc_stat sc_stat;

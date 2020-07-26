/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software and related documentation ("Software") are intellectual
* property owned by TARA Systems and are copyright of TARA Systems.
* Any modification, copying, reproduction or redistribution of the Software in
* whole or in part by any means not in accordance with the End-User License
* Agreement for Embedded Wizard is expressly prohibited. The removal of this
* preamble is expressly prohibited.
* 
********************************************************************************
*
* DESCRIPTION:
*   This is the main header file of the Embedded Wizard Runtime Environment 
*   EWRTE. This environment consists of functions, type definitions and 'C' 
*   macros used inside the automatic generated 'C' code. It allows the user
*   to compile and run the automatic generated 'C' code.
*
*******************************************************************************/

#ifndef EWRTE_H
#define EWRTE_H


/* Include platform specific configuration file */
#include <ewconfig.h>


#ifdef __cplusplus
  extern "C"
  {
#endif

/* The current version of the Runtime Environment. */
#define EW_RTE_VERSION 0x0009001E


/* Assigning zero (0) to the EW_PRINT_MEMORY_USAGE macro should turn it off
   as if the macro had not been defined. Otherwise if the macro is defined
   without any value, the option is enabled per default. */
#if defined EW_PRINT_MEMORY_USAGE && (( 1 - EW_PRINT_MEMORY_USAGE - 1 ) == 0 )
  #undef EW_PRINT_MEMORY_USAGE
#endif

/* Assigning zero (0) to the EW_PRINT_GFX_TASKS macro should turn it off as
   if the macro had not been defined. Otherwise if the macro is defined without
   any value, the option is enabled per default. */
#if defined EW_PRINT_GFX_TASKS && (( 1 - EW_PRINT_GFX_TASKS - 1 ) == 0 )
  #undef EW_PRINT_GFX_TASKS
#endif


/* For compatibility with older version: re-building the Graphics engine with
   the EW_PRINT_GFX_TASKS macro defined should activate the mode
   EW_SUPPORT_GFX_TASK_TRACING */
#if defined EW_PRINT_GFX_TASKS && !defined EW_SUPPORT_GFX_TASK_TRACING
  #define EW_SUPPORT_GFX_TASK_TRACING
#endif


/* Assigning zero (0) to the EW_PRINT_PERF_COUNTERS macro should turn it off as
   if the macro had not been defined. Otherwise if the macro is defined without
   any value, the option is enabled per default. */
#if defined EW_PRINT_PERF_COUNTERS && (( 1 - EW_PRINT_PERF_COUNTERS - 1 ) == 0 )
  #undef EW_PRINT_PERF_COUNTERS
#endif

/* Assigning zero (0) to the EW_RUN_BENCHMARK_SHORT macro should turn it off as
   if the macro had not been defined. Otherwise if the macro is defined without
   any value, the option is enabled per default. */
#if defined EW_RUN_BENCHMARK_SHORT && (( 1 - EW_RUN_BENCHMARK_SHORT - 1 ) == 0 )
  #undef EW_RUN_BENCHMARK_SHORT
#endif

/* Assigning zero (0) to the EW_RUN_BENCHMARK_FULL macro should turn it off as
   if the macro had not been defined. Otherwise if the macro is defined without
   any value, the option is enabled per default. */
#if defined EW_RUN_BENCHMARK_FULL && (( 1 - EW_RUN_BENCHMARK_FULL - 1 ) == 0 )
  #undef EW_RUN_BENCHMARK_FULL
#endif

/* Assigning zero (0) to the EW_RUN_VERIFICATION macro should turn it off as
   if the macro had not been defined. Otherwise if the macro is defined without
   any value, the option is enabled per default. */
#if defined EW_RUN_VERIFICATION && (( 1 - EW_RUN_VERIFICATION - 1 ) == 0 )
  #undef EW_RUN_VERIFICATION
#endif


/* Following EW_ENABLE_PROFILER macro exists for compatibility only */
#if defined EW_ENABLE_PROFILER && !defined EW_PRINT_MEMORY_USAGE
  #define EW_PRINT_MEMORY_USAGE
#endif


/* The Runtime Environment and the Graphics Engine allow creation of 64-Bit
   applications - this however only for the LP64 model (used by UNIX systems or 
   Apple iOS / macOS). Other models are currently not supported. */
#if defined __ILP64__ || defined __LLP64__ || defined _WIN64 ||                \
    defined _ILP64 || defined _LLP64
  #error "The 64-bit model of your compiler is not supported. Use LP64 model."
#endif


/******************************************************************************
* CONFIG MACRO:
*   EW_MAX_STRING_CACHE_SIZE
*
* DESCRIPTION:
*   The following macro exists for configuration purpose of the size of the
*   string cache. As soon as cache has reached the specified size, the cache is
*   cleaned. The macro should be configured in the ewconfig.h file or in the
*   Make file.
*
******************************************************************************/

/* If not explicitly specified, assume following default value for the maximum
   string constant cache size (in bytes) */
#ifndef EW_MAX_STRING_CACHE_SIZE
  #define EW_MAX_STRING_CACHE_SIZE  0x00008000
#endif

/* Validate the maximum string cache size */
#if (( EW_MAX_STRING_CACHE_SIZE < 0x00000000 ) ||                             \
     ( EW_MAX_STRING_CACHE_SIZE > 0x00100000 ))
  #error "The maximum string constant cache size out of range."
#endif


/******************************************************************************
* CONFIG VARIABLE:
*   EwMaxStringCacheSize
*
* DESCRIPTION:
*   The following variable exists for configuration purpose of the size of the
*   string cache. As soon as cache has reached the specified size, the cache is
*   cleaned. The variable should be modified just before the application is
*   initialized.
*
******************************************************************************/
extern int EwMaxStringCacheSize; /* = EW_MAX_STRING_CACHE_SIZE macro */


/*******************************************************************************
* MACRO:
*   EW_WARNING
*   EW_ERROR
*   EW_PANIC
*
* DESCRIPTION:
*   The following macros serve for error handling in the Runtime Environment
*   and the Graphics Engine. The macros print out error messages with the file
*   name and the line number, where the problem has occurred. The last macro
*   EW_PANIC also causes the application to terminate immediately.
*
*   All macros support arguments, which can be referred from the message by
*   using the standard printf() escape sequence.
*   
* ARGUMENTS:
*   aMsg - Zero-terminated string containing the message and optional printf()
*     escape sequences for the parameters aArg1, aArg2 and aArg3.
*   aArg1,
*   aArg2,
*   aArg3 - Optional arguments for the message to print out.
*
* RETURN VALUE:
*   None. EW_PANIC never returns!
*
*******************************************************************************/
#define EW_WARNING( aMsg )                                                     \
  EwPrint( "[WARNING in %s:%d] ", __FILE__, __LINE__ ),                        \
  EwPrint( aMsg ),                                                             \
  EwPrint( "\n" )

#define EW_WARNING_1( aMsg, aArg1 )                                            \
  EwPrint( "[WARNING in %s:%d] ", __FILE__, __LINE__ ),                        \
  EwPrint( aMsg, aArg1 ),                                                      \
  EwPrint( "\n" )

#define EW_WARNING_2( aMsg, aArg1, aArg2 )                                     \
  EwPrint( "[WARNING in %s:%d] ", __FILE__, __LINE__ ),                        \
  EwPrint( aMsg, aArg1, aArg2 ),                                               \
  EwPrint( "\n" )

#define EW_WARNING_3( aMsg, aArg1, aArg2, aArg3 )                              \
  EwPrint( "[WARNING in %s:%d] ", __FILE__, __LINE__ ),                        \
  EwPrint( aMsg, aArg1, aArg2, aArg3 ),                                        \
  EwPrint( "\n" )

#define EW_ERROR( aMsg )                                                       \
  EwPrint( "[ERROR in %s:%d] ", __FILE__, __LINE__ ),                          \
  EwPrint( aMsg ),                                                             \
  EwPrint( "\n" )

#define EW_ERROR_1( aMsg, aArg1 )                                              \
  EwPrint( "[ERROR in %s:%d] ", __FILE__, __LINE__ ),                          \
  EwPrint( aMsg, aArg1 ),                                                      \
  EwPrint( "\n" )

#define EW_ERROR_2( aMsg, aArg1, aArg2 )                                       \
  EwPrint( "[ERROR in %s:%d] ", __FILE__, __LINE__ ),                          \
  EwPrint( aMsg, aArg1, aArg2 ),                                               \
  EwPrint( "\n" )

#define EW_ERROR_3( aMsg, aArg1, aArg2, aArg3 )                                \
  EwPrint( "[ERROR in %s:%d] ", __FILE__, __LINE__ ),                          \
  EwPrint( aMsg, aArg1, aArg2, aArg3 ),                                        \
  EwPrint( "\n" )

#define EW_PANIC( aMsg )                                                       \
  EwPrint( "[FATAL ERROR in %s:%d] ", __FILE__, __LINE__ ),                    \
  EwPrint( aMsg ),                                                             \
  EwPrint( "\n" ),                                                             \
  EwPanic()

#define EW_PANIC_1( aMsg, aArg1 )                                              \
  EwPrint( "[FATAL ERROR in %s:%d] ", __FILE__, __LINE__ ),                    \
  EwPrint( aMsg, aArg1 ),                                                      \
  EwPrint( "\n" ),                                                             \
  EwPanic()

#define EW_PANIC_2( aMsg, aArg1, aArg2 )                                       \
  EwPrint( "[FATAL ERROR in %s:%d] ", __FILE__, __LINE__ ),                    \
  EwPrint( aMsg, aArg1, aArg2 ),                                               \
  EwPrint( "\n" ),                                                             \
  EwPanic()

#define EW_PANIC_3( aMsg, aArg1, aArg2, aArg3 )                                \
  EwPrint( "[FATAL ERROR in %s:%d] ", __FILE__, __LINE__ ),                    \
  EwPrint( aMsg, aArg1, aArg2, aArg3 ),                                        \
  EwPrint( "\n" ),                                                             \
  EwPanic()


/*******************************************************************************
* MACRO:
*   EW_SHIFT
*   EW_ROTATE
*
* DESCRIPTION:
*   The both EW_SHIFT and EW_ROTATE macros provide a kind of universal bit shift
*   and rotation operations. The macros are intended to calculate with constant
*   number of sift/rotation steps. In this case the 'C' preprocessor can apply
*   the best possible mode to perform the operation.
*   
* ARGUMENTS:
*   aValue - Value to shift or rotate.
*   aSteps - Number of bits to shift/rotate. If aSteps < 0, the value is shifed
*     or rotated to the left. If aSteps > 0, the value is rotated to the right.
*
* RETURN VALUE:
*   Returns the shifted or rotated value.
*
*******************************************************************************/
#define EW_SHIFT( aValue, aBits )                                              \
  (                                                                            \
    (( aBits ) < 0 )? (( aValue ) << -( aBits )) :                             \
                      (( aValue ) >>  ( aBits ))                               \
  )

#define EW_ROTATE( aValue, aBits )                                             \
  (                                                                            \
    ( aBits )? (                                                               \
                 (( aValue ) >> ( aBits )) |                                   \
                 (( aValue ) << (( 32 - ( aBits )) % 32 ))                     \
               ) :                                                             \
               ( aValue )                                                      \
  )


/*******************************************************************************
* TYPE:
*   XObject
*   XClass
*   XHandle
*
* DESCRIPTION:
*   The type XObject defines the fundamental set of data fields necessary to
*   keep an object alive and to manage it. All Embedded Wizard objects use
*   XObject internally. XObject can be compared to the root of all Embedded 
*   Wizard objects.
*
*   The second type XClass does represent a Chora class definition. It contains
*   a pointer to the data structure, where the class and its VMT is described.
*
*  The third type XHandle represents an abstract pointer or handle within the 
*  target system.
*
* ELEMENTS:
*   _VMT   - Pointer to a virtual method table (VMT) associated to the object.
*     The _VMT contains a list of methods supported by the object and an 
*     additional description of the class the objects belongs to: 
*     class name, objects size in bytes and a reference to the next base class 
*     in the inheritance tree.
*   _Mark - _Mark is used internally by the Garbage Collector. During the mark
*     phase, the Garbage Collector collects all objects within a global mark
*     list. Thereupon all objects not belonging to this list all considered as
*     garbage.
*   _Link  - Pointer to the higher object in which this object is aggregated in
*     (embedded). Aggregated objects will be stored (embedded) in the memory 
*     block of the higher object together with its other data fields. _Link is 
*     important to recognize relationships between aggregated objects and will 
*     be used by the Garbage Collector. If this object has not been aggregated
*     in an other object _Link is NULL.
*   _GCT   - Pointer to a table associated to the object and used during the
*     mark phase of the Garbage Collection. The table is really a VMT. See the
*     definition of the structure _vmt_XObject and its field _GCInfo for more
*     details.
*
*******************************************************************************/
struct _vmt_XObject;
struct _obj_XObject
{
  struct
  {
    const struct _vmt_XObject* VMT;
    struct _obj_XObject*       Mark;
    struct _obj_XObject*       Link;
    const struct _vmt_XObject* GCT;
  } _;
};
typedef struct _obj_XObject* XObject;
typedef const void*          XClass;
typedef unsigned long        XHandle;


/*******************************************************************************
* TYPE:
*   _vmt_XObject
*
* DESCRIPTION:
*   The type _vmt_XObject defines the fundamental structure of a virtual method 
*   table VMT and will be used to describe a class. All Embedded Wizard objects
*   uses _vmt_XObject internally. _vmt_XObject can be compared to the VMT of 
*   the root class of all Embedded Wizard classes.
*
* ELEMENTS:
*   _MagicNo - Number identifying the kind of the VMT: class or class variant.
*   _Name  - The name of the class as a zero terminated string (for debugging 
*     purpose and test only).
*   _Ancestor - Reference to the next base class (the super class) of this class
*     in the inheritance tree. If there is no further super classes, _Ancestor 
*     will point to the global __vmt_XObject data structure. This global data 
*     structure is the root of all Embedded Wizard objects.
*   _SubVariants - Pointer to the null terminated list of variants derived from
*     this class. During the creation of an object, this list is evaluated in
*     order to find and use the appropriate variant. 
*   _Size  - The size of an object instance in bytes. _Size will be used by the
*     EwNewObject() function to determine the number of bytes to reserve for 
*     a new object instance of this class.
*   _GCInfo - An array with offsets to allow the Garbage Collector to access
*     and evaluate data fields containing references to other objects. For
*     optimization purpose the data fields are grouped together starting with
*     all members containing ordinary pointers to other objects. Then follow
*     members where slots are stored. Then follow all data members containing
*     property references followed then by embedded objects. Finally members
*     containing references to strings are found. The array thus consist of 6
*     entries:
*       #0 : Offset to the first variable/array/property containing a pointer
*            to another object.
*       #1 : Offset to the first variable/array/property containing a slot.
*       #2 : Offset to the first variable/array/property containing a property
*            reference.
*       #3 : Offset to first embedded object.
*       #4 : Offset to first string.
*       #5 : Offset to the first data field not containing any references to
*            other objects nor strings.
*   _Init - Pointer to the _Init() method. The _Init() method will be invoked
*     to initialize a new object instance. (see below for details).
*   _ReInit - Pointer to the _ReInit() method. The _ReInit() method will be
*     invoked to re-initialize an object instance after the language has been
*     changed. (see below for details).
*   _Done - Pointer to the _Done() method. The _Done() method will be invoked
*     to deinitialize an object if the object is not used anymore. (see below 
*     for details).
*
*******************************************************************************/
struct _XClassVariant;
struct _vmt_XObject
{
  unsigned int                 _MagicNo;
  const char*                  _Name;
  const struct _vmt_XObject*   _Ancestor;
  const struct _XClassVariant* _SubVariants;
  int                          _Size;
  int                          _GCInfo[6];
  void                       (*_Init  )( XObject _this, XObject aLink, XHandle aArg );
  void                       (*_ReInit)( XObject _this );
  void                       (*_Done  )( XObject _this );
};
extern const struct _vmt_XObject __vmt_XObject;


/*******************************************************************************
* TYPE:
*   XClassVariant
*
* DESCRIPTION:
*   The structure XClassVariant is used to describe a multivariant class. A list
*   of such structures store links to derived sub variants of the origin class.
*   The end of the list should be signed with a zero entry.
*
* ELEMENTS:
*   Style   - The style, the class variant 'Variant' is used for. Style == 0 
*    does terminate the list. Style == 0 also signs a static class variant. A
*    static class variant overrides the origin class permanently.
*   Variant - The pointer to the affected class variant.
*
*******************************************************************************/
struct _XClassVariant
{
  int                        Style;
  const struct _vmt_XObject* Variant;
};
typedef struct _XClassVariant XClassVariant;


/*******************************************************************************
* MACRO:
*   EW_DECLARE_CLASS
*
* DESCRIPTION:
*   The macro EW_DECLARE_CLASS allows a forward declaration of the class aClass
*   so the name of this class can be used in the definition of data fields and 
*   methods in an other class.
*
* ARGUMENTS:
*   aClass - The name of the class to be declared.
*
*******************************************************************************/
#define EW_DECLARE_CLASS( aClass )                                             \
  struct _obj_##aClass;                                                        \
  typedef struct _obj_##aClass* aClass;


/*******************************************************************************
* MACRO:
*   EW_DEFINE_FIELDS
*   EW_END_OF_FIELDS
*   EW_VARIABLE
*   EW_PROPERTY
*   EW_OBJECT
*   EW_ARRAY
*   EW_VTHISPTR
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of an Embedded Wizard object. The usage of these macros makes 
*   the generatated 'C' code more independent from the used 'C' compiler and 
*   makes it readable for the human.
*
*   The macro EW_DEFINE_FIELDS starts the definition of data fields contained
*   in objects of the class aClass. The data fields defined in the base class
*   aSuperClass will be taken over automatically and stored in the field _Super.
*
*   EW_DEFINE_FIELDS defines an object template only. Using this template it is 
*   possible to access data fields of the object similary to access a member of 
*   a 'C' structure. The names and types of the data fields will be specified 
*   by the macros EW_VARIABLE, EW_PROPERTY, EW_OBJECT and EW_ARRAY.
*
*   The macro EW_VTHISPTR is used by multivariant classes only, to define the
*   pointer to the used object variant.
*
*   See the example below:
*
*     EW_DEFINE_FIELDS( MyClass, XObject )
*       EW_VARIABLE( Next, MyClass )
*       EW_VARIABLE( Prev, MyClass )
*       EW_PROPERTY( Visible, int )
*       EW_ARRAY   ( Matrix, char, [8][6])
*     EW_END_OF_FIELDS( MyClass )
*
*   At the end of the data fields definition the macro EW_END_OF_FIELDS has to 
*   occure.
*
* ARGUMENTS:
*   aClass       - The name of the class the data fields will be defined for.
*   aSuperClass  - The name of the next base class (the super class) of this 
*     class in the inheritance tree. If there is no further super classes, 
*     aSuperClass will get the name XObject (the root class of all Embedded 
*     Wizard objects).
*   aName        - The name of the data field (the name of a variable, property,
*     array or an aggregated (embedded) object).
*   aType        - The type of the data field (int, long, float,..., etc.)
*   aObjectClass - The class name of an aggregated object.
*   aArraySize   - The number of entries to stora in an array.
*
*******************************************************************************/
#define EW_DEFINE_FIELDS( aClass, aSuperClass )                                \
  struct _vmt_##aClass;                                                        \
  struct _obj_##aClass                                                         \
  {                                                                            \
    union                                                                      \
    {                                                                          \
      const struct _vmt_##aClass* VMT;                                         \
      struct _obj_XObject         XObject;                                     \
      struct _obj_##aSuperClass   Super;                                       \
    } _;

#define EW_END_OF_FIELDS( aClass )                                             \
  };

#define EW_VARIABLE( aName, aType )                                            \
  aType aName;

#define EW_PROPERTY( aName, aType )                                            \
  aType aName;

#define EW_OBJECT( aName, aObjectClass )                                       \
  struct _obj_##aObjectClass aName;

#define EW_ARRAY( aName, aType, aArraySize )                                   \
  aType aName aArraySize;

#define EW_VTHISPTR()                                                          \
  XObject _vthis;

#define _Super   _.Super
#define _VMT     _.VMT
#define _XObject _.XObject
#define _Link    _.Link
#define _Mark    _.Mark
#define _GCT     _XObject._.GCT


/*******************************************************************************
* MACRO:
*   EW_DEFINE_METHODS
*   EW_END_OF_METHODS
*   EW_METHOD
*   EW_USE_METHOD
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a virtual method table (VMT). The usage of these macros makes 
*   the generatated 'C' code more independent from the used 'C' compiler and 
*   makes it readable for the human.
*
*   The macro EW_DEFINE_METHODS starts the list of method names supported by
*   the class aClass. Each entry of the method list should be defined using 
*   the macro EW_METHOD. Because of the macro EW_DEFINE_METHODS takes care 
*   about the definition of the three fundamental methods _Init(), _Done() and
*   _ReInit() it is no necessary to define them explicitly.
*
*   EW_DEFINE_METHODS defines a template only. Using this template it is 
*   possible to invoke methods similary to calling functions via a function 
*   pointers in 'C'. The names of the methods will be specified by the macro
*   EW_METHOD. See the example below:
*
*     EW_DEFINE_METHODS( MyClass, XObject )
*       EW_METHOD( IsVisible,   int  )( void )
*       EW_METHOD( Draw,        void )( int aX, int aY )
*       EW_METHOD( HandleEvent, void )( CoreEvent aEvent )
*     EW_END_OF_METHODS( MyClass )
*
*   At the end of the methods definition the macro EW_END_OF_METHODS has to 
*   occure.
*
* ARGUMENTS:
*   aClass      - The name of the class the method list will be defined for.
*   aSuperClass - The name of the next base class (the super class) of this 
*     class in the inheritance tree. If there is no further super classes, 
*     aSuperClass will get the name XObject (the root class of all Embedded 
*     Wizard objects).
*   aName       - The name of the method to be defined in EW_METHOD macro.
*   aType       - The return type of the method.
*
*******************************************************************************/
#define EW_DEFINE_METHODS( aClass, aSuperClass )                               \
  void aClass##__Init  ( aClass _this, XObject aLink, XHandle aArg );          \
  void aClass##__ReInit( aClass _this );                                       \
  void aClass##__Done  ( aClass _this );                                       \
  struct _vmt_##aClass                                                         \
  {                                                                            \
    unsigned int                     _MagicNo;                                 \
    const char*                      _Name;                                    \
    const struct _vmt_##aSuperClass* _Ancestor;                                \
    const struct _XClassVariant*     _SubVariants;                             \
    int                              _Size;                                    \
    int                              _GCInfo[6];                               \
    void (*_Init  )( aClass _this, XObject aLink, XHandle aArg );              \
    void (*_ReInit)( aClass _this );                                           \
    void (*_Done  )( aClass _this )

#define EW_END_OF_METHODS( aClass )                                            \
    ;                                                                          \
    int _Reserved;                                                             \
  };                                                                           \
  extern const struct _vmt_##aClass __vmt_##aClass;                            \
  extern const int                  __vthis_##aClass;

#define EW_METHOD( aName, aType )                                              \
    ;                                                                          \
    aType (*aName)

#define EW_USE_METHOD( aName )                                                 \
    (void*) aName,


/*******************************************************************************
* MACRO:
*   EW_DEFINE_CLASS
*   EW_END_OF_CLASS
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a virtual method table (VMT). The usage of these macros makes 
*   the generatated 'C' code more independent from the used 'C' compiler and 
*   makes it readable for the human.
*
*   By using these macros the global VMT associated to the class aClass will be
*   initialized. Every class will be described by its own global VMT. This VMT 
*   contains a set of pointers to methods supported by the class and a pointer 
*   to the VMT of its next base class in the inheritance tree. Using the 
*   pointers it is possible to invoke the methods similary to calling functions
*   in 'C'. See the example below:
*
*     EW_DEFINE_CLASS( MyDerivedClass, MyClass )
*       MyClass_IsVisible,
*       MyClass_Draw,
*       MyDerivedClass_HandleEvent,
*     EW_END_OF_CLASS( MyDerivedClass )
*
*   The macro EW_DEFINE_CLASS initializes a data structure only. The necessary 
*   structure template has to be defined previously using the macros 
*   EW_DEFINE_METHODS, EW_END_OF_METHODS, etc. 
*
* ARGUMENTS:
*   aClass      - The name of the class the VMT will be defined for.
*   aSuperClass - The name of the next base class (the super class) of this 
*     class in the inheritance tree. If there is no further super classes, 
*     aSuperClass will get the name XObject (the root class of all Embedded 
*     Wizard objects).
*   aGCInfo0 ..
*   aGCInfo5    - Names of data members enclosing fields where references to
*     other objects are stored. This information is used by Garbage Collector
*     to evaluate dependencies between objects.
*   aName       - The name of the class in the Chora notation with :: signs.
*
*******************************************************************************/
#define EW_DEFINE_CLASS( aClass, aSuperClass, aGCInfo0, aGCInfo1, aGCInfo2,    \
                         aGCInfo3, aGCInfo4, aGCInfo5, aName )                 \
  const int    __vthis_##aClass = sizeof( struct _obj_##aSuperClass );         \
  const struct _vmt_##aClass __vmt_##aClass =                                  \
  {                                                                            \
    0x434C4153,                                                                \
    aName,                                                                     \
    &__vmt_##aSuperClass,                                                      \
    _vof_##aClass,                                                             \
    sizeof( struct _obj_##aClass ),                                            \
    { (int)&((aClass)0)->aGCInfo0, (int)&((aClass)0)->aGCInfo1,                \
      (int)&((aClass)0)->aGCInfo2, (int)&((aClass)0)->aGCInfo3,                \
      (int)&((aClass)0)->aGCInfo4, (int)&((aClass)0)->aGCInfo5 },              \
    aClass##__Init,                                                            \
    aClass##__ReInit,                                                          \
    aClass##__Done,

#define EW_END_OF_CLASS( aClass )                                              \
    0                                                                          \
  };

#define _None  _.VMT


/*******************************************************************************
* MACRO:
*   EW_DEFINE_VFIELDS
*   EW_END_OF_VFIELDS
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of an Embedded Wizard object variant. The usage of these macros 
*   makes the generatated 'C' code more independent from the used 'C' compiler 
*   and makes it readable for the human.
*
*   The macro EW_DEFINE_VFIELDS starts the definition of data fields contained
*   in instances of the class variant aClass. The data fields defined in the 
*   base class aSuperClass will be taken over automatically and stored in the 
*   field _Super.
*
*   EW_DEFINE_VFIELDS defines an object template only. Using this template it is
*   possible to access data fields of the object similary to access a member of 
*   a 'C' structure. The names and types of the data fields will be specified 
*   by the macros EW_VARIABLE, EW_PROPERTY, EW_OBJECT and EW_ARRAY (See above).
*
*   At the end of the data fields definition the macro EW_END_OF_VFIELDS has to 
*   occure.
*
* ARGUMENTS:
*   aClass       - The name of the class variant, the data fields will be 
*    defined for.
*   aSuperClass  - The name of the next base class variant (the super class) of
*     this class variant in the inheritance tree. If there is no further super 
*     classes, aSuperClass will get the name XObject (the root class of all
*     objects).
*   aOriginClass - The name of the origin class. This is the class, the variant
*     is derived from (not the immediate base class).
*
*******************************************************************************/
#define EW_DEFINE_VFIELDS( aClass, aSuperClass, aOriginClass )                 \
  struct _vmt_##aClass;                                                        \
  struct _obj_##aClass                                                         \
  {                                                                            \
    union                                                                      \
    {                                                                          \
      const struct _vmt_##aClass* VMT;                                         \
      struct _obj_XObject         XObject;                                     \
      struct _obj_##aSuperClass   Super;                                       \
    } _;                                                                       \

#define EW_END_OF_VFIELDS( aClass )                                            \
  };                                                                           \
  typedef struct _obj_##aClass* aClass;


/*******************************************************************************
* MACRO:
*   EW_DEFINE_VMETHODS
*   EW_END_OF_VMETHODS
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a virtual method table (VMT). The usage of these macros makes 
*   the generatated 'C' code more independent from the used 'C' compiler and 
*   makes it readable for the human.
*
*   The macro EW_DEFINE_VMETHODS starts the list of method names supported by
*   the class variant aClass. Each entry of the method list should be defined
*   using the macro EW_METHOD. Because of the macro EW_DEFINE_VMETHODS takes 
*   care about the definition of the three fundamental methods _Init(), _Done()
*   and _ReInit() it is no necessary to define them explicitly.
*
*   EW_DEFINE_VMETHODS defines a template only. Using this template it is 
*   possible to invoke methods similary to calling functions via a function 
*   pointers in 'C'. The names of the methods will be specified by the macro
*   EW_METHOD (See above).
*
*   At the end of the methods definition the macro EW_END_OF_VMETHODS has to 
*   occure.
*
* ARGUMENTS:
*   aClass       - The name of the class variants the method list will be 
*     defined for.
*   aSuperClass  - The name of the next base class variant (the super class) of
*     this class variant in the inheritance tree. If there is no further super 
*     classes, aSuperClass will get the name XObject (the root class of all
*     object variants).
*   aOriginClass - The name of the origin class. This is the class, the variant
*     is derived from (not the immediate base class).
*
*******************************************************************************/
#define EW_DEFINE_VMETHODS( aClass, aSuperClass, aOriginClass )                \
  void aClass##__Init  ( aOriginClass _this, XObject aLink, XHandle aArg );    \
  void aClass##__ReInit( aOriginClass _this );                                 \
  void aClass##__Done  ( aOriginClass _this );                                 \
  struct _vmt_##aClass                                                         \
  {                                                                            \
    unsigned int                      _MagicNo;                                \
    const char*                       _Name;                                   \
    const struct _vmt_##aSuperClass*  _Ancestor;                               \
    const XClassVariant*              _SubVariants;                            \
    int                               _Size;                                   \
    int                               _GCInfo[6];                              \
    void (*_Init  )( aOriginClass _this, XObject aLink, XHandle aArg );        \
    void (*_ReInit)( aOriginClass _this );                                     \
    void (*_Done  )( aOriginClass _this )

#define EW_END_OF_VMETHODS( aClass )                                           \
    ;                                                                          \
    int _Reserved;                                                             \
  };                                                                           \
  extern const struct _vmt_##aClass __vmt_##aClass;


/*******************************************************************************
* MACRO:
*   EW_DEFINE_VCLASS
*   EW_END_OF_VCLASS
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a virtual method table (VMT). The usage of these macros makes 
*   the generatated 'C' code more independent from the used 'C' compiler and 
*   makes it readable for the human.
*
*   By using these macros the global VMT associated to the class variant aClass
*   will be initialized. Every class variant will be described by its own global
*   VMT. This VMT contains a set of pointers to methods supported by the class 
*   variant and a pointer to the VMT of its origin class. Using the pointers it
*   is possible to invoke the methods similary to calling functions in 'C'.
*
*   The macro EW_DEFINE_VCLASS initializes a data structure only. The necessary 
*   structure template has to be defined previously using the macros 
*   EW_DEFINE_VMETHODS, EW_END_OF_VMETHODS, etc. 
*
* ARGUMENTS:
*   aClass       - The name of the class variant, the VMT will be defined for.
*   aSuperClass  - The name of the next base class variant (the super class) of
*     this class variant in the inheritance tree. If there is no further super 
*     classes, aSuperClass will get the name XObject (the root class of all
*     object variants).
*   aOriginClass - The name of the origin class. This is the class, the variant
*     is derived from (not the immediate base class).
*   aGCInfo0 ..
*   aGCInfo5     - Names of data members enclosing fields where references to
*     other objects are stored. This information is used by Garbage Collector
*     to evaluate dependencies between objects.
*   aName        - The name of the class variant in the Chora notation with :: 
*     signs.
*
*******************************************************************************/
#define EW_DEFINE_VCLASS( aClass, aSuperClass, aOriginClass, aGCInfo0,         \
                          aGCInfo1, aGCInfo2, aGCInfo3, aGCInfo4, aGCInfo5,    \
                          aName )                                              \
  const struct _vmt_##aClass __vmt_##aClass =                                  \
  {                                                                            \
    0x56434C41,                                                                \
    aName,                                                                     \
    &__vmt_##aSuperClass,                                                      \
    _vof_##aClass,                                                             \
    sizeof( struct _obj_##aClass ),                                            \
    { (int)&((aClass)0)->aGCInfo0, (int)&((aClass)0)->aGCInfo1,                \
      (int)&((aClass)0)->aGCInfo2, (int)&((aClass)0)->aGCInfo3,                \
      (int)&((aClass)0)->aGCInfo4, (int)&((aClass)0)->aGCInfo5 },              \
    aClass##__Init,                                                            \
    aClass##__ReInit,                                                          \
    aClass##__Done,

#define EW_END_OF_VCLASS( aClass )                                             \
    0                                                                          \
  };


/*******************************************************************************
* MACRO:
*   EW_DEFINE_CLASS_VARIANTS
*   EW_END_OF_CLASS_VARIANTS
*   EW_CLASS_VARIANT
*   EW_STATIC_CLASS_VARIANT
*
* DESCRIPTION:
*   These macros initialize a global list of variants, which are derived from a
*   class. When during the runtime an object of this class is created, Embedded
*   Wizard RTE evaluates the associated list in order to find the class variant,
*   which fits to the currently active styles. Then an object variant of this
*   class variant is created and attached to the origin object. In this manner
*   the origin object can be extended by an additional behaviour, without any
*   modifications of its origin class. The so called 'styles set' can be read 
*   and changed by calling the function EwGetStyles() and EwSetStyles().
*
*   The macro EW_DEFINE_CLASS_VARIANTS starts the list of class variants derived
*   from the class aClass. The entries of the list are definied by the macros
*   EW_CLASS_VARIANT and EW_STATIC_CLASS_VARIANT. The last of these both macros
*   specifies a static class variant. In this case the origin class aClass is
*   overriden by the static class variant permanently.
*
*   See the example below:
*
*     EW_DEFINE_CLASS_VARIANTS( MyUnitMyClass )
*       EW_CLASS_VARIANT( Style1, TheNameOfTheVariant1 )
*       EW_CLASS_VARIANT( Style2, TheNameOfTheVariant2 )
*       EW_STATIC_CLASS_VARIANT( TheNameOfTheVariant3 )
*     EW_END_OF_CLASS_VARIANTS( MyUnitMyConst )
*
* ARGUMENTS:
*   aClass    - The name of the class itself.
*   aStyle    - The name of the style, for which the class variant should be
*     used during the runtime.
*   aVariant  - The name of the class variant to use, when the specified
*     aStyle is currently activated in the styles set.
*
*******************************************************************************/
#define EW_DEFINE_CLASS_VARIANTS( aClass )                                     \
  static const XClassVariant _vof_##aClass[] =                                 \
  {

#define EW_END_OF_CLASS_VARIANTS( aClass )                                     \
    { 0, 0 }                                                                   \
  };

#define EW_CLASS_VARIANT( aStyle, aVariant )                                   \
  { aStyle, (const struct _vmt_XObject*)(const void*)&__vmt_##aVariant }

#define EW_STATIC_CLASS_VARIANT( aVariant )                                    \
  { 0, (const struct _vmt_XObject*)(const void*)&__vmt_##aVariant }


/*******************************************************************************
* MACRO:
*   EW_DEFINE_DISPATCHER
*   EW_END_OF_DISPATCHER
*
* DESCRIPTION:
*   These macros define so called variant dispatcher table. This table is used 
*   as a switch between the methods implemented in a class and overriden in the
*   class variants. The origin implementation of the affected methods uses this
*   definition in order to pass method invokations to the appropriate variants.
*   Usually, this table contains methods overriden in derived class variants
*   only. 
*
* ARGUMENTS:
*   aClass       - The name of the class, which has been overriden by variants.
*   aSuperClass  - The name of the next base class (the super class) of this
*     class in the inheritance tree. If there is no further super classes, 
*     aSuperClass will get the name XObject (the root class of all objects).
*
*******************************************************************************/
#define EW_DEFINE_DISPATCHER( aClass, aSuperClass )                            \
  struct _dmt_##aClass                                                         \
  {                                                                            \
    unsigned int                      _MagicNo;                                \
    const char*                       _Name;                                   \
    const struct _vmt_##aClass*       _Ancestor;                               \
    const XClassVariant*              _SubVariants;                            \
    int                               _Size;                                   \
    int                               _GCInfo[6];                              \
    void (*_Init  )( aClass _this, XObject aLink, XHandle aArg );              \
    void (*_ReInit)( aClass _this );                                           \
    void (*_Done  )( aClass _this )

#define EW_END_OF_DISPATCHER( aClass )                                         \
    ;                                                                          \
    int _Reserved;                                                             \
  };                                                                           \


/*******************************************************************************
* MACRO:
*   EW_CLASS
*   EW_VCLASS
*   EW_CLASS_GCT
*   EW_VCLASS_GCT
*   Super1
*   Super2
*   Super3
*   ...
*   Super24
*
* DESCRIPTION:
*   The following macros are useful to access internal data structures of an 
*   object. The macros hides the complexity of some 'C' expressions so the code
*   generation can be simplified and the generated 'C' code is less dependent
*   on the used 'C' compiler.
*
*   The macro EW_CLASS returns a pointer to the class description of the
*   requiered class aClass. The class description will be stored in the virtual
*   method table (VMT), so EW_CLASS has to return a pointer to the requiered 
*   global VMT data structure. In the same manner the macro EW_VCLASS returns
*   the pointer to the global VMT data structure of a class variant.
*
*   The macros EW_CLASS_GCT and EW_VCLASS_GCT return similarly to EW_CLASS and
*   EW_VCLASS a pointer to the VMT of the respective class or class variant.
*   They are used exclusively to obtain the right version of VMT to be used
*   during Garbage Collection mark phase.
*
*   The macros Super1 .. Super24 simplify the access to inherited data fields
*   of an object. Inherited data fields can be accessed in the context of the
*   inherited class only. For this reason every object stores inherited data 
*   fields in its _Super structure. The macro Super1 is used to access data 
*   fields of the direct next base class. Super2 allows to access data fields 
*   of the base class the direct next base class inherites from, and so far. 
*   For example:
*
*     obj->Super3.Width = 0;
*
*   will be evaluated to:
*
*     obj->_Super._Super._Super.Width = 0;
*
* ARGUMENTS:
*   aObject - Pointer to the object to get the base version or the root class
*     version.
*   aClass  - The name of the class to return the class description (pointer 
*     to the global VMT of aClass).
*
*******************************************************************************/
#define EW_CLASS( aClass )      (&__vmt_##aClass)
#define EW_VCLASS( aClass )     (&__vmt_##aClass)
#define EW_CLASS_GCT( aClass )  ((const struct _vmt_XObject*)EW_CLASS( aClass ))
#define EW_VCLASS_GCT( aClass ) ((const struct _vmt_XObject*)EW_VCLASS( aClass ))
#define Super1                  _Super
#define Super2                  Super1._Super
#define Super3                  Super2._Super
#define Super4                  Super3._Super
#define Super5                  Super4._Super
#define Super6                  Super5._Super
#define Super7                  Super6._Super
#define Super8                  Super7._Super
#define Super9                  Super8._Super
#define Super10                 Super9._Super
#define Super11                 Super10._Super
#define Super12                 Super11._Super
#define Super13                 Super12._Super
#define Super14                 Super13._Super
#define Super15                 Super14._Super
#define Super16                 Super15._Super
#define Super17                 Super16._Super
#define Super18                 Super17._Super
#define Super19                 Super18._Super
#define Super20                 Super19._Super
#define Super21                 Super20._Super
#define Super22                 Super21._Super
#define Super23                 Super22._Super
#define Super24                 Super23._Super


/*******************************************************************************
* MACRO:
*   EW_UNUSED_ARG
*
* DESCRIPTION:
*   The macro EW_UNUSED_ARG() encapsulates a dummy expression to avoid compiler
*   warning due to unused function parameters. The implementation of this macro
*   may be adapted to the apropriate 'C' compiler.
*   
* ARGUMENTS:
*   aArg - The name of the affected parameter.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#define EW_UNUSED_ARG( aArg )                                                    \
  (void)( aArg )


/*******************************************************************************
* METHOD:
*   XObject._Init
*
* DESCRIPTION:
*   The method _Init() will be invoked by to initialize the newly created object
*   instance this. The invoking of the method gets the object a chance to 
*   initialize all user defined data fields and to prepare internal structures 
*   necessary to keep the object alive.
*
*   Because of the inheritance of classes it is necessary to invoke all _Init()
*   methods of all inherited classes to complete the initialization. For this
*   reason the _Init() method has to take care to invoke the inherited _Init() 
*   method before it can begin with the initialization.
*
*   If the object this does aggregate other object instances, _Init() has to
*   take care to initialize these aggregate objects properly before _Init()
*   returns to the caller. In the same way _Init() has to reserve memory the 
*   object needs temporary (for example to store a string).
*   
* ARGUMENTS:
*   _this - Pointer to the object needs the initialization.
*   aLink - Pointer to an object which aggregates (embed) 'this' object. 
*     If 'this' object is not aggregated: aLink == NULL.
*   aArg  - An optional 32 bit argument. The _Init() method passes aArg to the
*     constructor of the class and allows the programmer to access any data in
*     the body of the constructor.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void XObject__Init
( 
  XObject                       _this, 
  XObject                       aLink,
  XHandle                       aArg
);


/*******************************************************************************
* METHOD:
*   XObject._ReInit
*
* DESCRIPTION:
*   The method _ReInit() will be invoked to re-initialize the object instance
*   this. The invoking of the method gets the object a chance to re-evaluate
*   all multilingual initialization expressions after the language has been
*   changed.
*
*   Because of the inheritance of classes it is necessary to invoke all 
*   _ReInit() methods of all inherited classes to complete the re-initialization
*   For this reason the _ReInit() method has to take care to invoke the 
*   inherited _ReInit() method before it can begin with the re-initialization.
*
*   If the object this does aggregate other object instances, _ReInit() has to
*   take care to re-initialize these aggregated objects properly before 
*   _ReInit() returns to the caller.
*   
* ARGUMENTS:
*   _this - Pointer to the object needs the re-initialization.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void XObject__ReInit
( 
  XObject                       _this
);


/*******************************************************************************
* METHOD:
*   XObject._Done
*
* DESCRIPTION:
*   The method _Done() will be invoked by the Garbage Collector to give the 
*   object 'this' a chance to deinitialize itself before the Garbage Collector 
*   frees the memory occuped by the object.
*
*   If the object 'this' does aggregate other object instances, _Done() has to
*   take care to deinitialize these aggregate objects properly before _Done()
*   returns to the caller. In the same way _Done() has to free all the memory 
*   the object has reserved temporary (for example to store a buffer).
*
*   Because of the inheritance of classes it is necessary to invoke all _Done()
*   methods of all inherited classes to complete the deinitialization. For this
*   reason the _Done() method has to take care to invoke the inherited _Done()
*   method.
*
*   The Garbage Collector guarantees to deinitialize only objects not in use 
*   anymore (garbage).
*
* ARGUMENTS:
*   _this - Pointer to the object to deinitialize.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void XObject__Done
( 
  XObject                       _this 
);


/*******************************************************************************
* FUNCTION:
*   EwConfigRuntimeEnvironment
*
* DESCRIPTION:
*   The pseudo function EwConfigRuntimeEnvironment() setups configuration 
*   variables of the Runtime Environment according to macros found in the file
*   'ewconfig.h' file or in the Make file.
*
*   This function has to be executed before using the functions from the Runtime
*   Environment.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#define EwConfigRuntimeEnvironment()                                           \
  do                                                                           \
  {                                                                            \
    EwMaxStringCacheSize = EW_MAX_STRING_CACHE_SIZE;                           \
  }                                                                            \
  while ( 0 )


/*******************************************************************************
* FUNCTION:
*   EwSetStackBaseAddress
*
* DESCRIPTION:
*   The function EwSetStackBaseAddress() remembers the given value as start
*   address of the CPU stack used by Embedded Wizard application. Knowing this
*   the Garbage Collector can be started while the application is executed and
*   can evaluate stack contents in order to mark objects/strings stored on it
*   actually.
*
* ARGUMENTS:
*   aStackBaseAddress - Base address of the CPU stack used by Embedded Wizard
*     application.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetStackBaseAddress
(
  void*                        aStackBaseAddress
);


/*******************************************************************************
* FUNCTION:
*   EwNewObject
*
* DESCRIPTION:
*   The function EwNewObject() implements a feature similar to the 'new' 
*   operator known from the programming language C++ and JAVA. EwNewObject() 
*   determines the size of an object of the given class aClass, reserves the 
*   memory for the object and initializes the object properly.
*
*   The memory reservation will be done by the function EwAlloc(). If there is 
*   no memory available to create a new object of the desired aClass 
*   EwNewObject() will cause an error by calling the EwPanic() function.
*
*   After the memory could be reserved EwNewObject() invokes the _Init() 
*   method of the new object, so the object will get a chance to initialize 
*   itself. 
*
*   Note, that if there is no relationships between the newly created object 
*   and other objects stored in the root set, the object will be disposed by 
*   the next garbage colection. See EwReclaimMemory() for more details about 
*   the Garbage Collector and the root set.
*
* ARGUMENTS:
*   aClass - The name of the desired class to create a new object instance.
*   aArg   - An optional 32 bit argument. The EwNewObject() method passes aArg 
*     to the constructor of the class and allows the programmer to access any 
*     data in the body of the constructor.
*
* RETURN VALUE:
*   EwNewObject() returns a pointer to the newly created object. 
*
*******************************************************************************/
XObject EwNewObjectIndirect
( 
  const void*                  aClass,
  XHandle                      aArg
);


#define EwNewObject( aClass, aArg )                                            \
  ((aClass)EwNewObjectIndirect( EW_CLASS( aClass ), aArg ))


/*******************************************************************************
* FUNCTION:
*   EwAttachObjectVariant
*
* DESCRIPTION:
*   The function EwAttachObjectVariant() creates an object variant and attaches
*   it to the origin object aThis. At first the function verifies, whether the
*   class of the origin object has been overriden by a class variant. Depending
*   on the found class variant, a new object variant is created and initialized.
*   At the end, the pointer to the created object variant is assigned to the 
*   '_vthis' field of the origin object -> the object variant is attached to it.
*
*   For this purpose the function evaluates the list of variants defined by the
*   macros EW_DEFINE_CLASS_VARIANTS (see above). If the affected class has not 
*   been overriden by a class variant, or no variants in the list do correspond
*   to the currently active styles - the function returns to the caller and no
*   object variant is created.
*
*   The memory reservation will be done by the function EwAlloc(). If there is 
*   no memory available to create a new object variant, EwAttachObjectVariant()
*   will cause an error by calling the EwPanic() function.
*
*   After the memory could be reserved, the _Init() method of the new object
*   variant is called in order to give the object a chance to initialize itself
*   properly. The both argument aLink and aArg are then passed to the _Init()
*   method.
*
*   If the origin object is disposed, the attached object variants are also 
*   destroyed by calling the function EwDetachObjectVariant().
*
* ARGUMENTS:
*   aThis - Pointer to the object to attach an object variant.
*   aLink - Pointer to an object which aggregates (embed) aThis object. If 
*     aThis object is not aggregated: aLink == NULL.
*   aArg  - An optional 32 bit argument passed from the _Init() method of aThis
*     object.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwAttachObjectVariant
(
  XObject           aThis,
  XObject           aLink,
  XHandle           aArg
);


/*******************************************************************************
* FUNCTION:
*   EwDetachObjectVariant
*
* DESCRIPTION:
*   The function EwDetachObjectVariant() detaches and destroys the object 
*   variant, which was previously attached to the origin object aThis by the 
*   function EwAttachObjectVariant().
*
*   The origin object aThis stores its object variant in the field '_vthis'. The
*   attached object variant is deinitialized by calling its _Done() Methode. 
*   After this, the memory occupied by the object variant is freed by EwFree().
*
* ARGUMENTS:
*   aThis - Pointer to the object to detach its object variant.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDetachObjectVariant
( 
  XObject           aThis
);


/*******************************************************************************
* FUNCTION:
*   EwCastObject
*
* DESCRIPTION:
*   The function EwCastObject() implements the feature knows from the 
*   programming language C++ and called 'dynamic_cast<>()'. EwCastObject() 
*   verifies whether it is possible to cast the given object aObject to class 
*   aClass and returns the unchanged pointer aObject if the cast is allowed. 
*   In the case the cast is not legal EwCastObject() returns NULL.
*
*   The legal cast assumes that the class of the given object is derived from 
*   aClass or it is aClass itself. To determine whether one class is derived
*   from another EwCastObject() evaluates the inheritance tree starting at the 
*   class of aObject. During the evaluation EwCastObject() will compare each 
*   found base class with aClass until the classes are the same or no other 
*   base classes exist.
*
* ARGUMENTS:
*   aObject - Pointer to the object which should be casted to aClass.
*   aClass  - The name of the desired class the object should be cast to.
*
* RETURN VALUE:
*   EwCastObject() returns aObject if there is a possibility to cast aObject to
*   aClass. If not, or if aObject is already NULL EwCastObject() returns NULL.
*
*******************************************************************************/
void* EwCastObject
( 
  register XObject             aObject, 
  register const void*         aClass
);

#define EwCastObject( aObject, aClass )                                        \
  ((aClass)EwCastObject((XObject)( aObject ), EW_CLASS( aClass )))


/*******************************************************************************
* FUNCTION:
*   EwCastClass
*
* DESCRIPTION:
*   The function EwCastClass() works similar to the function EwCastObject()
*   with the difference, that EwCastClass() is always applied to Chora classes
*   whereas EwCastObject() to instances of the classes. 
*
*   EwCastClass() verifies whether it is possible to cast the given class aClass
*   to the class aDesiredClass and returns the unchanged pointer aClass if the
*   cast is allowed. In the case the cast is not legal EwCastClass() returns 
*   NULL.
*
*   The legal cast assumes that the class aClass is derived from aDesiredClass
*   or it is aDesiredClass itself. To determine whether one class is derived
*   from another EwCastClass() evaluates the inheritance tree starting at the 
*   class aClass. During the evaluation EwCastClass() will compare each found 
*   base class with aDesiredClass until the classes are the same or the root of
*   the inheritance tree has been reached.
*
* ARGUMENTS:
*   aClass        - Pointer to the class which should be casted to 
*     aDesiredClass.
*   aDesiredClass - The name of the desired class the object should be cast to.
*
* RETURN VALUE:
*   EwCastClass() returns aClass if there is a possibility to cast aClass to
*   aDesiredClass. If not, or if aClass is already NULL EwCastClass() returns
*   NULL.
*
*******************************************************************************/
XClass EwCastClass
( 
  register XClass              aClass, 
  register const void*         aDesiredClass
);

#define EwCastClass( aClass, aDesiredClass )                                   \
  EwCastClass( aClass, EW_CLASS( aDesiredClass ))


/*******************************************************************************
* FUNCTION:
*   EwClassOf
*
* DESCRIPTION:
*   The function EwClassOf() determinates the class of an object given in the
*   argument aObject. The function returns a pointer to a global data structure
*   with the description of the class. If aObject == NULL, the function returns
*   NULL.
*
* ARGUMENTS:
*   aObject - Pointer to the object to determinate the class.
*
* RETURN VALUE:
*   EwClassOf() returns a pointer to the class structure or NULL, if aObject 
*   is NULL.
*
*******************************************************************************/
XClass EwClassOf
( 
  register XObject             aObject
);


/*******************************************************************************
* FUNCTION:
*   EwLockObject
*
* DESCRIPTION:
*   The function EwLockObject() adds the given object aObject to the global 
*   root set. Objects which are stored in the root set will still keep alive 
*   together with other objects depending from them.
*
*   Objects which are not stored in the root set and which are not in a 
*   relationships to an other object stored in the root set are garbage and will
*   be disposed by the next garbage collection.
*
*   The size of the root set is limited to 16 entries. A try to lock 17 objects
*   will cause EwPanic() call.
*
*   If the passed argument aObject is NULL, EwLockObject() returns immediatelly.
*
* ARGUMENTS:
*   aObject - Pointer to the object which should be added to the root set.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwLockObject
( 
  register XObject             aObject
);

#define EwLockObject( aObject )                                                \
  EwLockObject((XObject)( aObject ))


/*******************************************************************************
* FUNCTION:
*   EwUnlockObject
*
* DESCRIPTION:
*   The function EwUnlockObject() removes the object aObject from the root set.
*   The object should be added previously to the root set by calling the 
*   function EwLockObject(). If it's not, EwUnlockObject() will cause EwPanic()
*   call.
*
*   After the object has been removed from the root set and if there is no other
*   relationships between the object and other objects still stored in the root 
*   set, the object will be disposed by the next garbage collection.
*   
*   If the passed argument aObject is NULL, EwUnlockObject() returns 
*   immediatelly.
*
* ARGUMENTS:
*   aObject - Pointer to the object which should be removed from the root set.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwUnlockObject
( 
  register XObject             aObject
);

#define EwUnlockObject( aObject )                                              \
  EwUnlockObject((XObject)( aObject ))


/*******************************************************************************
* FUNCTION:
*   EwReclaimMemory
*
* DESCRIPTION:
*   The function EwReclaimMemory() invokes the Garbage Collector in order to 
*   dispose memory occuped by objects which are not used any more. The Garbage 
*   Collector implements the two phases algorithmus 'Mark and Sweep'. 
*
*   In the first phase EwReclaimMemory() distinguishes between live objects and 
*   the garbage. This is done by traversing the graph of object relationships. 
*   All reached objects will be marked. The traversing starts at the root set.
*   The user can add and remove objects to or from the root set by calling the
*   function EwLockObject() and EwUnlockObject(). As long as an object still
*   keeps in the root set, the Garbage Collector can not dispose the objects 
*   and all other objects standing in the relationship to the object.
*
*   In the second phase EwReclaimMemory() deinitializes and frees objects which
*   have not been marked during the first phase. The _Done() method of every 
*   affected object will be called, so the object will get a chance to shut down
*   properly. In the second step the EwFree() function will be called, so the 
*   memory occuped by the object will be released.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The function returns != 0 if at least one Chora object or string has been 
*   released.
*
*******************************************************************************/
int EwReclaimMemory
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwImmediateReclaimMemory
*
* DESCRIPTION:
*   The function EwImmediateReclaimMemory() implements a second version of the
*   above EwReclaimMemory() function. While EwReclaimMemory() function can be
*   used when the GUI application is not executing any code only, this second
*   version may be called even during the execution of the GUI application.
*
*   This is so-called immediate grabage collection.
*
*   In order to not oversee any Chora objects nor strings stored actually in
*   local variables of the just interrupted GUI application, the function
*   EwImmediateReclaimMemory() evaluates the contents of the CPU stack as well
*   as the values of CPU register. As such the function needs to know the base
*   address of the CPU stack. This has to be provided at the start of the GUI 
*   application via an invocation of the function EwSetStackBaseAddress().
*
* ARGUMENTS:
*   aErrorCode - Error code identifying the orginal memory alloc operation 
*     which is failed causing the application to start the immediate garbage
*     collection. The number is used only to print a debug message.
*
* RETURN VALUE:
*   The function returns != 0 if at least one Chora object or string has been 
*   released.
*
*******************************************************************************/
int EwImmediateReclaimMemory
(
  int                          aErrorCode
);


/*******************************************************************************
* FUNCTION:
*   EwTestImmediateReclaimMemory
*
* DESCRIPTION:
*   The function EwTestImmediateReclaimMemory() implements a simple routine to
*   verify the function of immediate garbage collection. The implementation
*   creates few string objects and stores some of them in local variables. Then
*   the garbage collection is started via EwImmediateReclaimMemory(). After the
*   operation the function verifies whether the objects referenced by local
*   variable still exist and the object not referenced anymore are released.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If the test is working the function returns 1. If some of needed objects
*   have been unexpectedly released, the function return 0 (bad error). If
*   some unused objects have not been released the function returns 2 (this
*   may occur).
*
*******************************************************************************/
int EwTestImmediateReclaimMemory
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwReconstructObjects
*
* DESCRIPTION:
*   The function EwReconstructObjects() forces all currently existing objects
*   to execute their _ReInit() methods. The re-construction allows the objects
*   to update themselfe after the language has been changed.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwReconstructObjects
( 
  void
);


/*******************************************************************************
* FUNCTION:
*   EwAlloc
*
* DESCRIPTION:
*   The function EwAlloc() will be called by the EWRTE each time a new memory 
*   block should be reserved. For example, the function EwNewObject() calls 
*   EwAlloc() to reserve memory for new objects.
*
*   EwAlloc() has to reserve a new memory block with the given size aSize. The
*   function has to return a pointer to the memory block or NULL if there is no
*   memory available.
*
* ARGUMENTS:
*   aSize - Desired size of the new memory block in bytes.
*
* RETURN VALUE:
*   EwAlloc() has to return a pointer to the reserved memory block or NULL if 
*   there is no memory available.
*
*******************************************************************************/
void* EwAlloc
(
  int                          aSize
);


/*******************************************************************************
* FUNCTION:
*   EwFree
*
* DESCRIPTION:
*   The function EwFree() should be implemented by the programer. The function 
*   will be called by the EWRTE each time a memory block should be released. 
*   The function EwFree() will be called from the Garbage Collector if garbage 
*   has been found.
*
* ARGUMENTS:
*   aMemory - Address of the memory block to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFree
(
  void*                        aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwIsMemory
*
* DESCRIPTION:
*   The function EwIsMemory() should be implemented together with the both
*   above functions EwAlloc() and EwFree(). EwIsMemory() will be called by
*   the EWRTE in order to test whether the given pointer does address within 
*   the memory area used by the heap (used for EwAlloc() operations).
*
* ARGUMENTS:
*   aPtr - Address to test.
*
* RETURN VALUE:
*   EwIsMemory() has to return != 0 if the given pointer aPtr addresses within 
*   the memory area used by the heap manager. Otherwise 0 should be returned.
*
*******************************************************************************/
int EwIsMemory
(
  void*                        aPtr
);


/******************************************************************************
* TYPE: 
*   XInt8,  XInt16, XInt16, XUInt8,  XUInt16, XUInt32, XBool, XEnum, XSet,
*   XFloat, XChar,  XColor, XLangId, XStylesSet
*
* DESCRIPTION:
*   The following set of type definitions is necessary to cover the simplest
*   instant datatypes implemented and used in the Embedded Wizards programming
*   language 'Chora'. Chora defines following simple instant datatypes: 
*
*     int8, int16, int32, uint8, uint16, uint32,
*     bool, enum,  set,   float, char,   language
*
* DEFINITIONS:
*  XInt8      - 8 bit signed integer. Stores values in range -128 .. +127.
*  XInt16     - 16 bit signed integer. Stores values in range -32768 .. +32767.
*  XInt32     - 32 bit signed integer. Stores values in range -2^31.. 
*    (+2^31 - 1).
*  XUInt8     - 8 bit unsigned integer. Stores values in range 0 .. +255.
*  XUInt16    - 16 bit unsigned integer. Stores values in range 0 .. +65535.
*  XUInt32    - 32 bit unsigned integer. Stores values in range 0 .. 
*    (+2^32 - 1).
*  XBool      - 8 bit Boolean. Stores 0 (zero) if the boolean value is 'false'
*    or not 0 if the value is 'true'.
*  XEnum      - 32 bit enumeration. An enumeration can store an 'one of...' 
*    value.
*  XSet       - 32 bit set. A set can store a combination of up to 32 values.
*    An empty set (without any values) is possible.
*  XFloat     - Floating point value necessary for the floating point 
*    arithmetic.
*  XChar      - 16 bit wide character. This is the standard character used in 
*    Chora. A character can store an UNICODE sign.
*  XLangId    - ID of the language variant.
*  XStylesSet - A set of style variants as a 32 bit pattern.
*
******************************************************************************/
#if defined __LP64__ || defined _LP64
  typedef signed char    XInt8;
  typedef signed short   XInt16;
  typedef signed int     XInt32;
  typedef unsigned char  XUInt8;
  typedef unsigned short XUInt16;
  typedef unsigned int   XUInt32;
  typedef char           XBool;
  typedef unsigned int   XEnum;
  typedef unsigned int   XSet;
  typedef float          XFloat;
  typedef unsigned short XChar;
  typedef int            XLangId;
  typedef unsigned int   XStylesSet;
#else
  typedef signed char    XInt8;
  typedef signed short   XInt16;
  typedef signed long    XInt32;
  typedef unsigned char  XUInt8;
  typedef unsigned short XUInt16;
  typedef unsigned long  XUInt32;
  typedef char           XBool;
  typedef unsigned long  XEnum;
  typedef unsigned long  XSet;
  typedef float          XFloat;
  typedef unsigned short XChar;
  typedef int            XLangId;
  typedef unsigned long  XStylesSet;
#endif


/******************************************************************************
* TYPE: 
*   XPoint
*
* DESCRIPTION:
*   The type definition XPoint is necessary to cover the instant datatype 
*   'point' implemented in the Embedded Wizards programming language 'Chora'. 
*   XPoint can store a position of a point in a 2D coordinate system.
*   The position of the point can be negative.
*
* ELEMENTS:
*   X - Specifies the x-coordinate of the point.
*   Y - Specifies the y-coordinate of the point. 
*
******************************************************************************/
typedef struct
{
  XInt32            X;
  XInt32            Y;
} XPoint;


/******************************************************************************
* TYPE: 
*   XRect
*
* DESCRIPTION:
*   The type definition XRect is necessary to cover the instant datatype 
*   'rect' implemented in the Embedded Wizards programming language 'Chora'. 
*   XRect can store a position and the expansion of an rectangle in a 2D
*   coordinate system.
*   The position and the size of the point can be negative.
*
* ELEMENTS:
*   Point1- Specifies the x, y coordinates of the upper-left corner of the
*     rectangle. 
*   Point2- Specifies the x, y coordinates of the bottom-right corner of the
*     rectangle. 
*
******************************************************************************/
typedef struct
{
  XPoint            Point1;
  XPoint            Point2;
} XRect;


/******************************************************************************
* TYPE: 
*   XString
*
* DESCRIPTION:
*   The type definition XString represents a 'string' instant datatype used in
*   the Embedded Wizard owns programming language Chora. A string can store
*   up to 65535 wide characters, so UNICODE strings are possible. The end of
*   each string will be terminated with a zero (\x0000) sign, similary to the
*   ANSI 'C' strings.
*
******************************************************************************/
typedef XChar* XString;


/******************************************************************************
* TYPE: 
*   XStringRes
*
* DESCRIPTION:
*   The type definition XStringRes is used to describe a single 'string'
*   constant as it stored in the code memory. Because of 'string' constants 
*   are stored in blocks, XStringRes contains a reference to the code block
*   where the data are stored and an offset to the first character of the
*   affected constant within the block.
*
* ELEMENTS:
*   Block  - The address of the block where the data of the string is stored.
*     Depending on the configuration, the data can be compressed. If not, the
*     Runtime Environment is able to use the string constants directly.
*     Please note, a single block can contain multiple 'string' constants.
*   Offset - The offset to the first character of the 'string' constant 
*     relative to the begin of the block - This offset is valid after the block
*     is decompressed only.
*
******************************************************************************/
typedef struct
{
  const void*       Block;
  XUInt16           Offset;
} XStringRes;


/******************************************************************************
* TYPE: 
*   XColor
*
* DESCRIPTION:
*   The type definition XColor is necessary to cover the instant datatype 
*   'color' implemented in the Embedded Wizards programming language 'Chora'. 
*   XColor can store a 32 bit RGBA color value.
*
* ELEMENTS:
*   Red   - The Red component of the color.
*   Green - The Green component of the color.
*   Blue  - The Blue component of the color.
*   Alpha - The Alpha component of the color.
*
******************************************************************************/
typedef struct
{
  XUInt8            Red;
  XUInt8            Green;
  XUInt8            Blue;
  XUInt8            Alpha;
} XColor;


/******************************************************************************
* TYPE: 
*   XVariantOfInt8,   XVariantOfInt16,  XVariantOfInt16,  XVariantOfUInt8, 
*   XVariantOfUInt16, XVariantOfUInt32, XVariantOfBool,   XVariantOfEnum, 
*   XVariantOfSet,    XVariantOfFloat,  XVariantOfChar,   XVariantOfColor,
*   XVariantOfPoint,  XVariantOfRect,   XVariantOfString, XVariantOfLanguage,
*   XVariantOfStylesSet
*
* DESCRIPTION:
*   The XVariantOfXXX type definitions support the multilanguage constants.
*   Multilanguage constants allow to store 2 or more variants of a constant.
*   For example a multilanguage string constant can contain strings in English,
*   German, Greek, etc. language.
*   The evaluation of a multilanguage constant causes a search for the variant
*   which fits to the currently selected language. Each variant consists of a 
*   LangId and the original constant value. The variants will be stored in
*   arrays. The last entry in such array should belong to the default language 
*   (LangId == 0) unless this is variant derived from other constant, which
*   already contains the default value.
*
* ELEMENTS:
*   LangId - The unique number of the language the variant belongs to. The
*     default language uses 0 (zero) LangId == 0.
*   Value  - The language dependent value of the constant.
*
******************************************************************************/
typedef struct
{
  int               LangId;
  XInt8             Value;
} XVariantOfInt8;

typedef struct
{
  int               LangId;
  XInt16            Value;
} XVariantOfInt16;

typedef struct
{
  int               LangId;
  XInt32            Value;
} XVariantOfInt32;

typedef struct
{
  int               LangId;
  XUInt8            Value;
} XVariantOfUInt8;

typedef struct
{
  int               LangId;
  XUInt16           Value;
} XVariantOfUInt16;

typedef struct
{
  int               LangId;
  XUInt32           Value;
} XVariantOfUInt32;

typedef struct
{
  int               LangId;
  XBool             Value;
} XVariantOfBool;

typedef struct
{
  int               LangId;
  XEnum             Value;
} XVariantOfEnum;

typedef struct
{
  int               LangId;
  XSet              Value;
} XVariantOfSet;

typedef struct
{
  int               LangId;
  XFloat            Value;
} XVariantOfFloat;

typedef struct
{
  int               LangId;
  XChar             Value;
} XVariantOfChar;

typedef struct
{
  int               LangId;
  XColor            Value;
} XVariantOfColor;

typedef struct
{
  int               LangId;
  XPoint            Value;
} XVariantOfPoint; 

typedef struct
{
  int               LangId;
  XRect             Value;
} XVariantOfRect;

typedef struct
{
  int               LangId;
  XStringRes        Value;
} XVariantOfString;

typedef struct
{
  int               LangId;
  XLangId           Value;
} XVariantOfLangId;

typedef struct
{
  int               LangId;
  XStylesSet        Value;
} XVariantOfStylesSet;


/******************************************************************************
* TYPE: 
*   XResource
*
* DESCRIPTION:
*   The XResource type definition supports the multilingual resources.
*   Multilingual resources allow to store several language dependent variants 
*   of a single resource. The evaluation of a multilingual resource causes a 
*   search for the language variant which fits to the currently selected 
*   language. The variants will be stored in arrays. The last entry in such 
*   array should belong to the default language (LangId == 0) unless the
*   resource is derived from an other resource, which already defines the
*   default variant.
*
* ELEMENTS:
*   LangId   - The unique number of the language the resource belongs to. The
*     default language uses 0 (zero) LangId == 0.
*   Resource - The pointer to the ROM code where the resource ist stored in.
*
******************************************************************************/
typedef struct
{
  int               LangId;
  const void*       Resource;
} XResource;


/*******************************************************************************
* TYPE:
*   XVariant
*   XSubVariant
*
* DESCRIPTION:
*   The structures XVariant and XSubVariant are used to describe a multilingual 
*   and multivariant constant or resource definition. These structures do store 
*   links to the content of the definition and to the sub variants, which are
*   derived from it.
*
*   The first structure XVariant serves as the descriptor of a such multilingual
*   multivariant definition. The second structure XSubVariant is used to define 
*   a link to one of the derived sub variants.
*
* ELEMENTS:
*   Content        - The pointer to the multilingual content of the definition.
*     If this pointer is null, the definition does not have its own content. In
*     this case, the entire content is inherited from its base variant. The 
*     meaning of the 'Content' depends on the kind of the definition.
*   NoOfLanguages  - How many language variants does the definition consists of?
*   SubVariants    - The pointer to the table, with links to derived variants.
*     If this pointer is null, the definition does not own any derived variants.
*     The table is zero terminated.
*   Style          - The style, the sub variant 'Variant' is used for. 
*   Variant        - The pointer to the sub variant derived from this variant.
*
*******************************************************************************/
struct _XSubVariant;
typedef struct
{
  const void*                  Content;
  int                          NoOfLanguages;
  const struct _XSubVariant*   SubVariants;
} XVariant;

typedef struct _XSubVariant
{
  int                          Style;
  const XVariant*              Variant;
} XSubVariant;


/*******************************************************************************
* MACRO:
*   EW_DEFINE_CONST_WITH_VARIANTS
*   EW_END_OF_CONST_WITH_VARIANTS
*   EW_DEFINE_CONST_VARIANTS_ONLY
*   EW_END_OF_CONST_VARIANTS_ONLY
*   EW_CONST_WITHOUT_VARIANTS
*   EW_CONST_VARIANT
*   EW_STATIC_CONST_VARIANT
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a constant. The usage of these macros makes the generatated
*   'C' code more independent from the used 'C' compiler and makes it readable
*   for the human. These macros are needed for multilingual / multivariant
*   constants only.
*
*   These macros initialize a global list of variants, which are derived from
*   a constant. When the constant is accessed during the runtime, the list is
*   evaluated in order to find the variant, which fits to the currently active
*   styles. The so called 'styles set' can be read and changed by calling the
*   function EwGetStyles() and EwSetStyles(). See the example below:
*
*     EW_DEFINE_CONST_WITH_VARIANTS( MyUnitMyConst )
*       EW_CONST_VARIANT( Style1, TheNameOfTheVariant1 )
*       EW_CONST_VARIANT( Style2, TheNameOfTheVariant2 )
*     EW_END_OF_CONST_WITH_VARIANTS( MyUnitMyConst )
*
*   The CONST_WITH_VARIANTS version of the macros is used to define a constant,
*   which has its own content and a set of derived variants.
*
*   The CONST_VARIANTS_ONLY version provides the definition of a constant, which
*   derives all values from its base variant. In this case, the constant itself
*   does not occupy any space in the ROM. The macros define the list with 
*   derived variants only.
*
*   The CONST_WITHOUT_VARIANTS version of these macros defines a constant with
*   its own content, but without any derived variants.
*
* ARGUMENTS:
*   aConstant - The name of the constant itself.
*   aStyle    - The name of the style, for which the constant variant should be
*     used during the runtime.
*   aVariant  - The name of the constant variant to use, when the specified
*     aStyle is currently activated in the styles set.
*
*******************************************************************************/
#define EW_DECLARE_CONST( aConstant )                                          \
  extern const XVariant aConstant;

#define EW_DEFINE_CONST_WITH_VARIANTS( aConstant )                             \
  static const XSubVariant _v_##aConstant[] =                                  \
  {

#define EW_END_OF_CONST_WITH_VARIANTS( aConstant )                             \
    { 0, 0 }                                                                   \
  };                                                                           \
  const XVariant aConstant =                                                   \
  {                                                                            \
    _##aConstant,                                                              \
    sizeof( _##aConstant ) / sizeof( *_##aConstant ),                          \
    _v_##aConstant                                                             \
  };

#define EW_DEFINE_CONST_VARIANTS_ONLY( aConstant )                             \
  static const XSubVariant _v_##aConstant[] =                                  \
  {

#define EW_END_OF_CONST_VARIANTS_ONLY( aConstant )                             \
    { 0, 0 }                                                                   \
  };                                                                           \
  const XVariant aConstant =                                                   \
  {                                                                            \
    0,                                                                         \
    0,                                                                         \
    _v_##aConstant                                                             \
  };

#define EW_CONST_WITHOUT_VARIANTS( aConstant )                                 \
  const XVariant aConstant =                                                   \
  {                                                                            \
    _##aConstant,                                                              \
    sizeof( _##aConstant ) / sizeof( *_##aConstant ),                          \
    0                                                                          \
  };

#define EW_CONST_VARIANT( aStyle, aVariant )                                   \
  { aStyle, &aVariant }

#define EW_STATIC_CONST_VARIANT( aVariant )                                    \
  { 0, &aVariant }


/*******************************************************************************
* MACRO:
*   EW_DEFINE_RES_WITH_VARIANTS
*   EW_END_OF_RES_WITH_VARIANTS
*   EW_DEFINE_RES_VARIANTS_ONLY
*   EW_END_OF_RES_VARIANTS_ONLY
*   EW_RES_WITHOUT_VARIANTS
*   EW_RES_VARIANT
*   EW_STATIC_RES_VARIANT
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of a resources. The usage of these macros makes the generatated
*   'C' code more independent from the used 'C' compiler and makes it readable
*   for the human. These macros are needed for multilingual / multivariant
*   resources only.
*
*   These macros initialize a global list of variants, which are derived from
*   a resource. When the resource is accessed during the runtime, the list is
*   evaluated in order to find the variant, which fits to the currently active
*   styles. The so called 'styles set' can be read and changed by calling the
*   function EwGetStyles() and EwSetStyles(). See the example below:
*
*     EW_DEFINE_RES_WITH_VARIANTS( MyUnitMyRes )
*       EW_RES_VARIANT( Style1, TheNameOfTheVariant1 )
*       EW_RES_VARIANT( Style2, TheNameOfTheVariant2 )
*     EW_END_OF_RES_WITH_VARIANTS( MyUnitMyRes )
*
*   The RES_WITH_VARIANTS version of the macros is used to define a resource,
*   which has its own content and a set of derived variants.
*
*   The RES_VARIANTS_ONLY version provides the definition of a resource, which
*   derives all values from its base variant. In this case, the resource itself
*   does not occupy any space in the ROM. The macros define the list with 
*   derived variants only.
*
*   The RES_WITHOUT_VARIANTS version of these macros defines a resource with
*   its own content, but without any derived variants.
*
* ARGUMENTS:
*   aResource - The name of the resource itself.
*   aStyle    - The name of the style, for which the resource variant should be
*     used during the runtime.
*   aVariant  - The name of the resource variant to use, when the specified
*     aStyle is currently activated in the styles set.
*
*******************************************************************************/
#define EW_DEFINE_RES_WITH_VARIANTS( aResource )                               \
  static const XSubVariant _v_##aResource[] =                                  \
  {

#define EW_END_OF_RES_WITH_VARIANTS( aResource )                               \
    { 0, 0 }                                                                   \
  };                                                                           \
  const XVariant aResource =                                                   \
  {                                                                            \
    _##aResource,                                                              \
    sizeof( _##aResource ) / sizeof( *_##aResource ),                          \
    _v_##aResource                                                             \
  };

#define EW_DEFINE_RES_VARIANTS_ONLY( aResource )                               \
  static const XSubVariant _v_##aResource[] =                                  \
  {

#define EW_END_OF_RES_VARIANTS_ONLY( aResource )                               \
    { 0, 0 }                                                                   \
  };                                                                           \
  const XVariant aResource =                                                   \
  {                                                                            \
    0,                                                                         \
    0,                                                                         \
    _v_##aResource                                                             \
  };

#define EW_RES_WITHOUT_VARIANTS( aResource )                                   \
  const XVariant aResource =                                                   \
  {                                                                            \
    _##aResource,                                                              \
    sizeof( _##aResource ) / sizeof( *_##aResource ),                          \
    0                                                                          \
  };

#define EW_RES_VARIANT( aStyle, aVariant )                                     \
  { aStyle, &aVariant }

#define EW_STATIC_RES_VARIANT( aVariant )                                      \
  { 0, &aVariant }


/******************************************************************************
* TYPE: 
*   XRef
*
* DESCRIPTION:
*   The XRef type definition is necessary to store a reference to a property.
*
* ELEMENTS:
*   Object - Pointer to an object which contains the referenced property.
*   OnGet  - Pointer to the properties own OnGet() method.
*   OnSet  - Pointer to the properties own OnSet() method.
*
******************************************************************************/
typedef void (*XRefGetSetProc)( void );
typedef struct
{
  void*             Object;
  XRefGetSetProc    OnGet;
  XRefGetSetProc    OnSet;
} XRef;


/* EwNullRef stores a constant 'null' reference, so it is possible to use it
   immediatelly in assignments or comparations */
extern const XRef EwNullRef;


/******************************************************************************
* TYPE: 
*   XSlotProc
*
* DESCRIPTION:
*   The XSlotProc type defines the prototype for all slot methods.
*
* ARGUMENTS:
*   aObject - Pointer to the object which contains the slot.
*   aSender - Pointer to the signal sender.
*
* RETURN VALUE:
*   None
*
******************************************************************************/
typedef void (*XSlotProc)
(
  void*             aObject,
  XObject           aSender
);


/******************************************************************************
* TYPE: 
*   XSlot
*
* DESCRIPTION:
*   The XSlot type definition is necessary to store a slot.
*
* ELEMENTS:
*   Object   - Pointer to an object which contains the slot.
*   SlotProc - Pointer to the slots own method.
*
******************************************************************************/
typedef struct
{
  void*             Object;
  XSlotProc         SlotProc;
} XSlot;


/* EwNullSlot stores a constant 'null' slot, so it is possible to use it
   immediatelly in assignments or comparations */
extern const XSlot EwNullSlot;


/******************************************************************************
* TYPE: 
*   XAutoObject
*
* DESCRIPTION:
*   The XAutoObject type definition describes an auto object within the ROM
*   code.
*
* ELEMENTS:
*   Init - Pointer to a function containing the initialization code of the
*     object.
*
******************************************************************************/
typedef void (*XAutoObjectInitProc)
(
  XObject _this
);

typedef struct
{
  XAutoObjectInitProc Init;
} XAutoObject;


/*******************************************************************************
* MACRO:
*   EW_DECLARE_AUTOOBJECT
*   EW_DEFINE_AUTOOBJECT
*   EW_DEFINE_AUTOOBJECT_VARIANTS
*   EW_END_OF_AUTOOBJECT_VARIANTS
*   EW_AUTOOBJECT_VARIANT
*   EW_STATIC_AUTOOBJECT_VARIANT
*
* DESCRIPTION:
*   These macros have been defined to hide the complexity of the internal
*   structure of an auto object. The usage of these macros makes the generatated
*   'C' code more independent from the used 'C' compiler and makes it readable
*   for the human. These macros are needed for auto objects only.
*
*   The both macros EW_DECLARE_AUTOOBJECT and EW_DEFINE_AUTOOBJECT are used n 
*   order to declare and implement an auto object. The remaining macros do
*   initialize a global list of variants, which are derived from the affected
*   auto object. When the auto object is accessed during the runtime, the list 
*   is evaluated in order to find the variant, which fits to the active style.
*   The so called 'styles set' can be read and changed by calling the function 
*   EwGetStyles() and EwSetStyles(). See the example below:
*
*     EW_DEFINE_AUTOOBJECT_VARIANTS( MyUnitMyAutoObject )
*       EW_AUTOOBJECT_VARIANT( Style1, TheNameOfTheVariant1 )
*       EW_AUTOOBJECT_VARIANT( Style2, TheNameOfTheVariant2 )
*       EW_STATIC_AUTOOBJECT_VARIANT( TheNameOfTheVariant2 )
*     EW_END_OF_AUTOOBJECT_VARIANTS( MyUnitMyAutoObject )
*
*   The entries of the list are definied by the macros EW_AUTOOBJECT_VARIANT 
*   and EW_STATIC_AUTOOBJECT_VARIANT. The last of these both macros specifies 
*   a static, style independent auto object sub-variant.
*
* ARGUMENTS:
*   aAutoObject - The name of the auto object itself.
*   aStyle      - The name of the style, for which the auto object variant 
*     should be used during the runtime.
*   aVariant    - The name of the auto object variant to use, when aStyle is
*     currently activated in the styles set.
*
*******************************************************************************/
#define EW_DECLARE_AUTOOBJECT( aAutoObject, aClass )                           \
  extern const XVariant aAutoObject;                                           \
                                                                               \
  void aAutoObject##__Init( aClass _this );

#define EW_DEFINE_AUTOOBJECT( aAutoObject, aClass )                            \
  static const XAutoObject _##aAutoObject =                                    \
  {                                                                            \
    (XAutoObjectInitProc)aAutoObject##__Init                                   \
  };

#define EW_DEFINE_AUTOOBJECT_VARIANTS( aAutoObject )                           \
  static const XSubVariant _v_##aAutoObject[] =                                \
  {

#define EW_END_OF_AUTOOBJECT_VARIANTS( aAutoObject )                           \
    { 0, 0 }                                                                   \
  };                                                                           \
                                                                               \
  const XVariant aAutoObject =                                                 \
  {                                                                            \
    &_##aAutoObject,                                                           \
    1,                                                                         \
    _v_##aAutoObject                                                           \
  };

#define EW_AUTOOBJECT_VARIANT( aStyle, aVariant )                              \
  { aStyle, &aVariant }

#define EW_STATIC_AUTOOBJECT_VARIANT( aVariant )                               \
  { 0, &aVariant }


/*******************************************************************************
* FUNCTION:
*   EwMathSin
*
* DESCRIPTION:
*   The function EwMathSin() implements the Chora math_sin() function. 
*
* ARGUMENTS:
*   aAngle - An angle in degree.
*
* RETURN VALUE:
*   Returns the determinated sine value.
*
*******************************************************************************/
XFloat EwMathSin
(
  XFloat            aAngle
);


/*******************************************************************************
* FUNCTION:
*   EwMathCos
*
* DESCRIPTION:
*   The function EwMathCos() implements the Chora math_cos() function. 
*
* ARGUMENTS:
*   aAngle - An angle in degree.
*
* RETURN VALUE:
*   Returns the determinated cosine value.
*
*******************************************************************************/
XFloat EwMathCos
(
  XFloat            aAngle
);


/*******************************************************************************
* FUNCTION:
*   EwMathArcSin
*
* DESCRIPTION:
*   The function EwMathArcSin() implements the Chora math_asin() function. 
*
* ARGUMENTS:
*   aValue - Value to calculate the arc sine angle. Only values in the range
*     -1.0 .. +1.0 are valid.
*
* RETURN VALUE:
*   Returns the determinated angle in the range -90 .. +90 degress.
*
*******************************************************************************/
XFloat EwMathArcSin
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwMathArcCos
*
* DESCRIPTION:
*   The function EwMathArcCos() implements the Chora math_acos() function. 
*
* ARGUMENTS:
*   aValue - Value to calculate the arc cosine. Only values in the range
*     -1.0 .. +1.0 are valid.
*
* RETURN VALUE:
*   Returns the determinated angle in the range 0 .. +180 degress.
*
*******************************************************************************/
XFloat EwMathArcCos
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwMathSqrt
*
* DESCRIPTION:
*   The function EwMathSqrt() implements the Chora math_sqrt() function. It
*   calculates the square root value. 
*
* ARGUMENTS:
*   aValue - The value to calculate square root.
*
* RETURN VALUE:
*   Returns the determinated square root value.
*
*******************************************************************************/
XFloat EwMathSqrt
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwMathPow
*
* DESCRIPTION:
*   The function EwMathPow() implements the Chora math_pow() function. It
*   calculates the power a ^ b function.
*
* ARGUMENTS:
*   aA - The value to calculate the pow()
*   aB - The value to calculate the pow()
*
* RETURN VALUE:
*   Returns the determinated power value.
*
*******************************************************************************/
XFloat EwMathPow
(
  XFloat            aA,
  XFloat            aB
);


/*******************************************************************************
* FUNCTION:
*   EwMathRandInt32
*
* DESCRIPTION:
*   The function EwMathRandInt32() implements the Chora math_rand() function
*   for int32 operands. It calculates the random value in the range aValue1 .. 
*   aValue2.
*
* ARGUMENTS:
*   aValue1,
*   aValue2  - Range for the value.
*
* RETURN VALUE:
*   Returns the determinated random value.
*
*******************************************************************************/
XInt32 EwMathRandInt32
(
  XInt32            aValue1,
  XInt32            aValue2
);


/*******************************************************************************
* FUNCTION:
*   EwMathRandFloat
*
* DESCRIPTION:
*   The function EwMathRandFloat() implements the Chora math_rand() function
*   for float operands. It calculates the random value in the range aValue1 .. 
*   aValue2.
*
* ARGUMENTS:
*   aValue1,
*   aValue2  - Range for the value.
*
* RETURN VALUE:
*   Returns the determinated random value.
*
*******************************************************************************/
XFloat EwMathRandFloat
(
  XFloat            aValue1,
  XFloat            aValue2
);


/*******************************************************************************
* FUNCTION:
*   EwMathRound
*
* DESCRIPTION:
*   The function EwMathRound() implements the Chora math_round() function. 
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the correctly rounded value.
*
*******************************************************************************/
XFloat EwMathRound
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwMathFloor
*
* DESCRIPTION:
*   The function EwMathFloor() implements the Chora math_floor() function. 
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the floor rounded value.
*
*******************************************************************************/
XFloat EwMathFloor
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwMathCeil
*
* DESCRIPTION:
*   The function EwMathCeil() implements the Chora math_ceil() function. 
*
* ARGUMENTS:
*   aValue - A value to round.
*
* RETURN VALUE:
*   Returns the ceil rounded value.
*
*******************************************************************************/
XFloat EwMathCeil
(
  XFloat            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwSetContains
*
* DESCRIPTION:
*   The function EwSetContains() implements the Chora instant method 
*   'set.contains()'.
*
* ARGUMENTS:
*   aSet1 - The first set to verify its content.
*   aSet2 - The second set.
*
* RETURN VALUE:
*   The function returns != 0, if the second set aSet2 is contained in the 
*   given aSet1.
*
*******************************************************************************/
XBool EwSetContains
(
  XSet              aSet1,
  XSet              aSet2
);


/*******************************************************************************
* FUNCTION:
*   EwNewColor
*
* DESCRIPTION:
*   The function EwNewColor() initializes a new color with the given values
*   returns the initialized color. 
*
*   EwNewColor() implements the Chora instant constructor: 
*   'color(aRed,aGreen,aBlue)' and 'color(aRed,aGreen,aBlue,aAlpha)'.
*
* ARGUMENTS:
*   aRed   - Red component of the color.
*   aGreen - Green component of the color.
*   aBlue  - Blue component of the color.
*   aAlpha - Alpha component of the color.
*
* RETURN VALUE:
*   Returns the initialized color.
*
*******************************************************************************/
XColor EwNewColor
( 
  XUInt8            aRed, 
  XUInt8            aGreen, 
  XUInt8            aBlue, 
  XUInt8            aAlpha 
);


/*******************************************************************************
* FUNCTION:
*   EwMinusColor
*
* DESCRIPTION:
*   The function EwMinusColor() subtracts two colors aColor1 - aColor2. The 
*   substraction will be done for each color component separately. In the case
*   the substraction results in a value < 0, EwMinusColor() adjusts the value
*   automatically to 0 (zero).
*
*   EwMinusColor() implements the Chora instant operator: 'color - color'
*
* ARGUMENTS:
*   aColor1 - The color to substract.
*   aColor2 - The color to be substracted.
*
* RETURN VALUE:
*   Returns a new color containing the substraction of aColor1 and aColor2.
*
*******************************************************************************/
XColor EwMinusColor
( 
  XColor            aColor1, 
  XColor            aColor2 
);


/*******************************************************************************
* FUNCTION:
*   EwPlusColor
*
* DESCRIPTION:
*   The function EwPlusColor() adds two colors aColor1 + aColor2. The addition
*   will be done for each color component separately. In the case the addition
*   results in a value > 255, EwPlusColor() adjusts the value automatically 
*   to 255.
*
*   EwPlusColor() implements the Chora instant operator: 'color + color'
*
* ARGUMENTS:
*   aColor1 - The first color.
*   aColor2 - The second color.
*
* RETURN VALUE:
*   Returns a new color containing the addition of aColor1 and aColor2.
*
*******************************************************************************/
XColor EwPlusColor
( 
  XColor            aColor1, 
  XColor            aColor2
);


/*******************************************************************************
* FUNCTION:
*   EwBlendColor
*
* DESCRIPTION:
*   The function EwBlendColor() implements an alphablending algorithm to blend
*   one color aColor1 with the second color aColor2. The Alpha component of
*   aColor1 decides about the intensity of the alpha blending. If the Alpha
*   component of aColor1 is 0 (zero), the function returns an unchanged value
*   aColor2. If the Alpha component of aColor1 == 255, the function returns the
*   value aColor1. In all other cases, the function returns a mix-color of the
*   both colors.
*
*   EwBlendColor() implements the Chora instant operator: 'color * color'
*
* ARGUMENTS:
*   aColor1 - The first color.
*   aColor2 - The second color. 
*
* RETURN VALUE:
*   Returns a mix-color of the both colors.
*
*******************************************************************************/
XColor EwBlendColor
( 
  XColor            aColor1,
  XColor            aColor2 
);


/*******************************************************************************
* FUNCTION:
*   EwBlendColorInt
*
* DESCRIPTION:
*   The function EwBlendColorInt() implements an alphablending algorithm to 
*   blend the color aColor with the value aAlpha. This function applies the
*   alpha blending algorithm on all 4 color components of the given color.
*
*   EwBlendColorInt() implements the Chora instant operator: 'color * int32'
*   and 'int32 * color'.
*
* ARGUMENTS:
*   aColor - The color to alphablend with aAlpha.
*   aAlpha - The blending factor in range from 0 .. 255.
*
* RETURN VALUE:
*   Returns a mix-color of the color aColor and the value aAlpha.
*
*******************************************************************************/
XColor EwBlendColorInt
( 
  XColor            aColor, 
  XInt32            aAlpha 
);


/*******************************************************************************
* FUNCTION:
*   EwBlendColorUInt
*
* DESCRIPTION:
*   The function EwBlendColorInt() implements an alphablending algorithm to 
*   blend the color aColor with the value aAlpha. This function applies the
*   alpha blending algorithm on all 4 color components of the given color.
*
*   EwBlendColorUInt() implements the Chora instant operator: 'color * uint32'
*   and 'uint32 * color'.
*
* ARGUMENTS:
*   aColor - The color to alphablend with aAlpha.
*   aAlpha - The blending factor in range from 0 .. 255.
*
* RETURN VALUE:
*   Returns a mix-color of the color aColor and the value aAlpha.
*
*******************************************************************************/
XColor EwBlendColorUInt
( 
  XColor            aColor, 
  XUInt32           aAlpha 
);


/*******************************************************************************
* FUNCTION:
*   EwSetColorRed
*
* DESCRIPTION:
*   The function EwSetColorRed() changes the Red component of the given color 
*   to a new value aRed.
*
*   EwSetColorRed() implements the write access to the Chora instant property 
*   'color.red'.
*
* ARGUMENTS:
*   aColor - The color to change the Red component.
*   aRed   - The new value for the Red component.
*
* RETURN VALUE:
*   Returns a new color with the changed Red component.
*
*******************************************************************************/
XColor EwSetColorRed
( 
  XColor            aColor, 
  XUInt8            aRed 
);


/*******************************************************************************
* FUNCTION:
*   EwSetColorGreen
*
* DESCRIPTION:
*   The function EwSetColorGreen() changes the Green component of the given 
*   color to a new value aGreen.
*
*   EwSetColorGreen() implements the write access to the Chora instant property 
*   'color.green'.
*
* ARGUMENTS:
*   aColor - The color to change the Green component.
*   aGreen - The new value for the Green component.
*
* RETURN VALUE:
*   Returns a new color with the changed Green component.
*
*******************************************************************************/
XColor EwSetColorGreen
( 
  XColor            aColor, 
  XUInt8            aGreen 
);


/*******************************************************************************
* FUNCTION:
*   EwSetColorBlue 
*
* DESCRIPTION:
*   The function EwSetColorBlue() changes the Blue component of the given color 
*   to a new value aBlue.
*
*   EwSetColorBlue() implements the write access to the Chora instant property 
*   'color.blue'.
*
* ARGUMENTS:
*   aColor - The color to change the Blue component.
*   aBlue  - The new value for the Blue component.
*
* RETURN VALUE:
*   Returns a new color with the changed Blue component.
*
*******************************************************************************/
XColor EwSetColorBlue
( 
  XColor            aColor, 
  XUInt8            aBlue 
);


/*******************************************************************************
* FUNCTION:
*   EwSetColorAlpha
*
* DESCRIPTION:
*   The function EwSetColorAlpha() changes the Alpha component of the given 
*   color to a new value aAlpha.
*
*   EwSetColorAlpha() implements the write access to the Chora instant property 
*   'color.alpha'.
*
* ARGUMENTS:
*   aColor - The color to change the Alpha component.
*   aAlpha - The new value for the Alpha component.
*
* RETURN VALUE:
*   Returns a new color with the changed Alpha component.
*
*******************************************************************************/
XColor EwSetColorAlpha
( 
  XColor            aColor, 
  XUInt8            aAlpha 
);


/*******************************************************************************
* FUNCTION:
*   EwCompColor
*
* DESCRIPTION:
*   The function EwCompColor() compares the both given colors aColor1 and 
*   aColor2 and returns a value == 0 if the colors are identical. Otherwise
*   the function returns a value != 0.
*
*   EwCompColor() implements the Chora instant operators: 'color == color' and
*   'color != color'.
*
* ARGUMENTS:
*   aColor1 - The first color.
*   aColor2 - The second color.
*
* RETURN VALUE:
*   Returns zero if the both colors are equal.
*
*******************************************************************************/
int EwCompColor
( 
  XColor            aColor1, 
  XColor            aColor2 
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfColor
*
* DESCRIPTION:
*   The function EwGetVariantOfColor() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple color values.
*
* RETURN VALUE:
*   Returns the determinated color value.
*
*******************************************************************************/
XColor EwGetVariantOfColor
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwNewPoint
*
* DESCRIPTION:
*   The function EwNewPoint() initializes a new point with the given aX and aY 
*   values and returns the initialized point.
*
*   EwNewPoint() implements the Chora instant constructor: 'point(aX,aY)'.
*
* ARGUMENTS:
*   aX - The X-coordinate of the point.
*   aY - The Y-coordinate of the point.
*
* RETURN VALUE:
*   Returns the initialized point.
*
*******************************************************************************/
XPoint EwNewPoint
( 
  XInt32            aX, 
  XInt32            aY 
);


/*******************************************************************************
* FUNCTION:
*   EwCompPoint
*
* DESCRIPTION:
*   The function EwCompPoint() compares the both given points aPoint1 and 
*   aPoint2 and returns a value == 0 if the points are identical. Otherwise
*   the function returns a value != 0.
*
*   EwCompPoint() implements the Chora instant operators: 'point == point' and
*   'point != point'.
*
* ARGUMENTS:
*   aPoint1 - The first point.
*   aPoint2 - The second point.
*
* RETURN VALUE:
*   Returns zero if the both points are equal.
*
*******************************************************************************/
int EwCompPoint
( 
  XPoint            aPoint1, 
  XPoint            aPoint2 
);


/*******************************************************************************
* FUNCTION:
*   EwMovePointNeg
*
* DESCRIPTION:
*   The function EwMovePointNeg() adjusts the position of the point aPoint by 
*   the specified amount aOffset in the negative direction.
*
*   EwMovePointNeg() implements the Chora instant operator: 'point - point'.
*
* ARGUMENTS:
*   aPoint  - The point to be moved.
*   aOffset - The Offset that specifies the amount of vertical and horizontal 
*     space to move aPoint.
*
* RETURN VALUE:
*   Returns a new point with the changed position.
*
*******************************************************************************/
XPoint EwMovePointNeg
( 
  XPoint            aPoint, 
  XPoint            aOffset 
);


/*******************************************************************************
* FUNCTION:
*   EwMovePointPos
*
* DESCRIPTION:
*   The function EwMovePointPos() adjusts the position of the point aPoint by 
*   the specified amount aOffset in the negative direction.
*
*   EwMovePointPos() implements the Chora instant operator: 'point + point'.
*
* ARGUMENTS:
*   aPoint  - The point to be moved.
*   aOffset - The offset that specifies the amount of vertical and horizontal 
*     space to move aPoint.
*
* RETURN VALUE:
*   Returns a new point with the changed position.
*
*******************************************************************************/
XPoint EwMovePointPos
( 
  XPoint            aPoint, 
  XPoint            aOffset 
);


/*******************************************************************************
* FUNCTION:
*   EwNegPoint
*
* DESCRIPTION:
*   The function EwNegPoint() changes the sign of the point from '+' to '-',
*   or from '-' to '+'. The function negates the point position.
*
* ARGUMENTS:
*   aPoint  - The point to negate.
*
* RETURN VALUE:
*   Returns a new point with the changed position.
*
*******************************************************************************/
XPoint EwNegPoint
( 
  XPoint            aPoint
);


/*******************************************************************************
* FUNCTION:
*   EwSetPointX
*
* DESCRIPTION:
*   The function EwSetPointX() changes the X-coordinate of the given point to 
*   a new value aX.
*
*   EwSetPointX() implements the write access to the Chora instant property 
*   'point.x'.
*
* ARGUMENTS:
*   aPoint - The point to change the X-coordinate.
*   aX     - The new value for the X-coordinate.
*
* RETURN VALUE:
*   Returns a new point with the changed position.
*
*******************************************************************************/
XPoint EwSetPointX
( 
  XPoint            aPoint, 
  XInt32            aX 
);


/*******************************************************************************
* FUNCTION:
*   EwSetPointY
*
* DESCRIPTION:
*   The function EwSetPointY() changes the Y-coordinate of the given point to 
*   a new value aY.
*
*   EwSetPointY() implements the write access to the Chora instant property 
*   'point.y'.
*
* ARGUMENTS:
*   aPoint - The point to change the Y-coordinate.
*   aY     - The new value for the Y-coordinate.
*
* RETURN VALUE:
*   Returns a new point with the changed position.
*
*******************************************************************************/
XPoint EwSetPointY
( 
  XPoint            aPoint, 
  XInt32            aY 
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfPoint
*
* DESCRIPTION:
*   The function EwGetVariantOfPoint() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple point values.
*
* RETURN VALUE:
*   Returns the determinated point value.
*
*******************************************************************************/
XPoint EwGetVariantOfPoint
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwNewRect
*
* DESCRIPTION:
*   The function EwNewRect() initializes a new rectangle with the given aX1, 
*   aY1, aX2, aY2 values and returns the initialized rect.
*
*   EwNewRect() implements the Chora instant constructor: 
*   'rect(aX1,aY1,aX2,aY2)'.
*
* ARGUMENTS:
*   aX1 - x-coordinate of the first (top-left) point of the rectangle.
*   aY1 - y-coordinate of the first (top-left) point of the rectangle.
*   aX2 - x-coordinate of the second (bottom-right) point of the rectangle.
*   aY2 - y-coordinate of the second (bottom-right) point of the rectangle.
*
* RETURN VALUE:
*   Returns the initialized rectangle.
*
*******************************************************************************/
XRect EwNewRect
( 
  XInt32            aX1, 
  XInt32            aY1, 
  XInt32            aX2, 
  XInt32            aY2 
);


/*******************************************************************************
* FUNCTION:
*   EwNewRect2Point
*
* DESCRIPTION:
*   The function EwNewRect2Point() initializes a new rectangle with the given 
*   points aPoint1 and aPoint2 and returns the initialized rect.
*
*   EwNewRect2Point() implements the Chora instant constructor: 
*   'rect(aPoint1,aPoint2)'.
*
* ARGUMENTS:
*   aPoint1 - Coordinates of the first (top-left) point of the rectangle.
*   aPoint2 - Coordinates of the second (bottom-right) point of the rectangle.
*
* RETURN VALUE:
*   Returns the initialized rectangle.
*
*******************************************************************************/
XRect EwNewRect2Point
( 
  XPoint            aPoint1, 
  XPoint            aPoint2 
);


/*******************************************************************************
* FUNCTION:
*   EwCompRect
*
* DESCRIPTION:
*   The function EwCompRect() compares the both given rectangles aRect1 and 
*   aRect2 and returns a value == 0 if the rectangles are identical. Otherwise 
*   the function returns a value != 0.
*
*   EwCompRect() implements the Chora instant operators: 'rect == rect' and
*   'rect != rect'.
*
* ARGUMENTS:
*   aRect1 - The first rectangle.
*   aRect2 - The second rectangle.
*
* RETURN VALUE:
*   Returns zero if the both rectangles are equal or both rectangles are empty.
*
*******************************************************************************/
int EwCompRect
( 
  XRect             aRect1, 
  XRect             aRect2 
);


/*******************************************************************************
* FUNCTION:
*   EwIsPointInRect
*
* DESCRIPTION:
*   The function EwIsPointInRect() determines whether the specified point lies 
*   within a rectangle. A point is within a rectangle if it lies on the left or
*   top side or is within all four sides. A point on the right or bottom side 
*   is outside a rectangle.
*
*   EwIsPointInRect() implements the Chora instant operator: 'rect == point'.
*
* ARGUMENTS:
*   aRect  - The rectangle to be tested.
*   aPoint - The coordinates of a point.
*
* RETURN VALUE:
*   Returns a value != 0 if the point aPoint lies within the rectangle aRect.
*   Otherwiese the function returns 0.
*
*******************************************************************************/
XBool EwIsPointInRect
( 
  XRect             aRect, 
  XPoint            aPoint 
);


/*******************************************************************************
* FUNCTION:
*   EwIsRectEmpty
*
* DESCRIPTION:
*   The function EwIsRectEmpty() determines whether the specified rectangle is
*   empty. An empty rectangle has negative or zero width or height.
*
*   EwIsRectEmpty() implements the Chora instant property: 'rect.isempty'.
*
* ARGUMENTS:
*   aRect  - The rectangle to be tested.
*
* RETURN VALUE:
*   Returns a value != 0 if the rectangle is empty.
*
*******************************************************************************/
XBool EwIsRectEmpty
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwInflateRect
*
* DESCRIPTION:
*   The function EwInflateRect() inflates the rectangle aRect by moving its 
*   sides away from its center. To do this, EwInflateRect() subtracts units 
*   from the top-left point and adds units to the bottom-right point of aRect.
*
*   EwInflateRect() implements the Chora instant operator: 'rect * point'.
*
* ARGUMENTS:
*   aRect  - The rectangle to be inflated.
*   aDelta - The number of units to inflate aRect.
*
* RETURN VALUE:
*   Returns a new rectangle with changed position and size.
*
*******************************************************************************/
XRect EwInflateRect
( 
  XRect             aRect, 
  XPoint            aDelta 
);


/*******************************************************************************
* FUNCTION:
*   EwIntersectRect
*
* DESCRIPTION:
*   The function EwIntersectRect() retrieves a rectangle that represents the 
*   intersecting area of two specified rectangles aRect1 and aRect2.
*
*   EwIntersectRect() implements the Chora instant operator: 'rect & rect'.
*
* ARGUMENTS:
*   aRect1 - The first rectangle to be intersect with aRect2.
*   aRect2 - The second rectangle to be intersect with aRect1.
*
* RETURN VALUE:
*   Returns a new rectangle that represents the intersecting area of aRect1 
*   and aRect2. If the intersecting area is empty, the function returns an  
*   empty rectangle.
*
*******************************************************************************/
XRect EwIntersectRect
( 
  XRect             aRect1, 
  XRect             aRect2 
);


/*******************************************************************************
* FUNCTION:
*   EwUnionRect
*
* DESCRIPTION:
*   The function EwUnionRect() retrieves a rectangle that represents the 
*   smallest rectangle that contains two specified rectangular areas aRect1 and 
*   aRect2.
*
*   If one of the passed rectangles aRect1 or aRect2 is empty, no union is build
*   and the function returns the other rectangle immediately.
*
*   EwUnionRect() implements the Chora instant operator: 'rect | rect'.
*
* ARGUMENTS:
*   aRect1 - The first rectangle in the union.
*   aRect2 - The second rectangle in the union.
*
* RETURN VALUE:
*   Returns a new rectangle that represents the union of the two specified 
*   areas aRect1 and aRect2. If the union area is empty, the function returns  
*   an empty rectangle.
*
*******************************************************************************/
XRect EwUnionRect
( 
  XRect             aRect1, 
  XRect             aRect2 
);


/*******************************************************************************
* FUNCTION:
*   EwIntersectRect2
*
* DESCRIPTION:
*   The function EwIntersectRect2() retrieves a rectangle that represents the 
*   intersecting area of two specified rectangles aRect1 and aRect2.
*
*   Unlike the function EwIntersectRect(), empty rectangles are ignored. If one
*   of the passed rectangles aRect1 or aRect2 is empty, no intersection is build
*   and the function returns the other rectangle immediately.
*
*   EwIntersectRect2() implements the Chora instant operator: 'rect && rect'.
*
* ARGUMENTS:
*   aRect1 - The first rectangle to be intersect with aRect2.
*   aRect2 - The second rectangle to be intersect with aRect1.
*
* RETURN VALUE:
*   Returns a new rectangle that represents the intersecting area of aRect1 
*   and aRect2. If the intersecting area is empty, the function returns an  
*   empty rectangle.
*
*******************************************************************************/
XRect EwIntersectRect2
( 
  XRect             aRect1, 
  XRect             aRect2 
);


/*******************************************************************************
* FUNCTION:
*   EwMoveRectNeg
*
* DESCRIPTION:
*   The function EwMoveRectNeg() adjusts the position of the rectangle aRect by
*   the specified amount aOffset in the negative direction.
*
*   EwMoveRectNeg() implements the Chora instant operator: 'rect - point'.
*
* ARGUMENTS:
*   aRect   - The rectangle to be moved.
*   aOffset - The Offset that specifies the amount of vertical and horizontal 
*     space to move aRect.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed position.
*
*******************************************************************************/
XRect EwMoveRectNeg
( 
  XRect             aRect, 
  XPoint            aOffset 
);


/*******************************************************************************
* FUNCTION:
*   EwMoveRectPos
*
* DESCRIPTION:
*   The function EwMoveRectPos() adjusts the position of the rectangle aRect by
*   the specified amount aOffset in the positive direction.
*
*   EwMoveRectPos() implements the Chora instant operator: 'rect + point'.
*
* ARGUMENTS:
*   aRect   - The rectangle to be moved.
*   aOffset - The Offset that specifies the amount of vertical and horizontal 
*     space to move aRect.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed position.
*
*******************************************************************************/
XRect EwMoveRectPos
( 
  XRect             aRect, 
  XPoint            aOffset 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectH
*
* DESCRIPTION:
*   The function EwGetRectH() returns the height of the rectangle. The function
*   calculates the height of the rectangle by subtracting the coordinate of the
*   top border from the bottom border.
*
*   EwGetRectH() implements the read access to the Chora instant property: 
*   'rect.h'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the height.
*
* RETURN VALUE:
*   Returns the height of the rectangle. The value can be negative.
*
*******************************************************************************/
XInt32 EwGetRectH
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectW
*
* DESCRIPTION:
*   The function EwGetRectW() returns the width of the rectangle. The function
*   calculates the width of the rectangle by subtracting the coordinate of the
*   left border from the right border.
*
*   EwGetRectW() implements the read access to the Chora instant property: 
*   'rect.w'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the width.
*
* RETURN VALUE:
*   Returns the width of the rectangle. The value can be negative.
*
*******************************************************************************/
XInt32 EwGetRectW
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectArea
*
* DESCRIPTION:
*   The function EwGetRectArea() calculates the area of the rectangle. The 
*   multiplies the width with the height of the rectangle.
*
*   EwGetRectArea() implements the read access to the Chora instant property: 
*   'rect.area'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the area.
*
* RETURN VALUE:
*   Returns the area of the rectangle. The value can be negative.
*
*******************************************************************************/
XInt32 EwGetRectArea
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectSize
*
* DESCRIPTION:
*   The function EwGetRectSize() calculates the size of the rectangle.
*
*   EwGetRectSize() implements the read access to the Chora instant property: 
*   'rect.size'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the size.
*
* RETURN VALUE:
*   Returns the size of the rectangle.
*
*******************************************************************************/
XPoint EwGetRectSize
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectORect
*
* DESCRIPTION:
*   The function EwGetRectORect() calculates rectangle with the same size as
*   the given rectangle, but with the fixed origin point <0,0>.
*
*   EwGetRectORect() implements the read access to the Chora instant property: 
*   'rect.orect'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the zero fixed rectangle.
*
* RETURN VALUE:
*   Returns new rectangle.
*
*******************************************************************************/
XRect EwGetRectORect
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwGetRectCenter
*
* DESCRIPTION:
*   The function EwGetRectCenter() calculates the coordinates of the point in
*   the center of the given rectangle.
*
*   EwGetRectCenter() implements the read access to the Chora instant property: 
*   'rect.center'.
*
* ARGUMENTS:
*   aRect - The rectangle to get the center of them.
*
* RETURN VALUE:
*   Returns coordinates of the rectangle's center.
*
*******************************************************************************/
XPoint EwGetRectCenter
( 
  XRect             aRect 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectSize
*
* DESCRIPTION:
*   The function EwSetRectSize() changes the size of the given rectangle to a 
*   new value aSize. The function changes the value of Point2.
*
*   EwSetRectSize() implements the write access to the Chora instant property 
*   'rect.size'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the size.
*   aSize - The new size of the rectangle.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed size.
*
*******************************************************************************/
XRect EwSetRectSize
( 
  XRect             aRect,
  XPoint            aSize
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectOrigin
*
* DESCRIPTION:
*   The function EwSetRectOrigin() changes the origin of the given rectangle to
*   a new value aOrigin. The function changes the value of Point1 and Point2.
*
*   EwSetRectOrigin() implements the write access to the Chora instant property 
*   'rect.origin'.
*
* ARGUMENTS:
*   aRect   - The rectangle to change the origin.
*   aOrigin - The new origin of the rectangle.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed origin.
*
*******************************************************************************/
XRect EwSetRectOrigin
( 
  XRect             aRect,
  XPoint            aOrigin
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectX1
*
* DESCRIPTION:
*   The function EwSetRectX1() changes the X-coordinate of the first (top-left)
*   point of the given rectangle to a new value aX1.
*
*   EwSetRectX1() implements the write access to the Chora instant property 
*   'rect.x1'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the X1-coordinate.
*   aX1   - The new value for the X1-coordinate.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed coordinate.
*
*******************************************************************************/
XRect EwSetRectX1
( 
  XRect             aRect, 
  XInt32            aX1 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectY1
*
* DESCRIPTION:
*   The function EwSetRectY1() changes the Y-coordinate of the first (top-left)
*   point of the given rectangle to a new value aY1.
*
*   EwSetRectY1() implements the write access to the Chora instant property 
*   'rect.y1'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the Y1-coordinate.
*   aY1   - The new value for the Y1-coordinate.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed coordinate.
*
*******************************************************************************/
XRect EwSetRectY1
( 
  XRect             aRect, 
  XInt32            aY1 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectX2
*
* DESCRIPTION:
*   The function EwSetRectX2() changes the X-coordinate of the second (bottom-
*   right) point of the given rectangle to a new value aX2.
*
*   EwSetRectX2() implements the write access to the Chora instant property 
*   'rect.x2'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the X2-coordinate.
*   aX2   - The new value for the X2-coordinate.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed coordinate.
*
*******************************************************************************/
XRect EwSetRectX2
( 
  XRect             aRect, 
  XInt32            aX2 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectY2
*
* DESCRIPTION:
*   The function EwSetRectY2() changes the Y-coordinate of the second (bottom-
*   right) point of the given rectangle to a new value aY2.
*
*   EwSetRectY2() implements the write access to the Chora instant property 
*   'rect.y2'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the Y2-coordinate.
*   aY2   - The new value for the Y2-coordinate.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed coordinate.
*
*******************************************************************************/
XRect EwSetRectY2
( 
  XRect             aRect, 
  XInt32            aY2 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectW
*
* DESCRIPTION:
*   The function EwSetRectW() changes the width of the given rectangle to a new
*   value aW.
*
*   EwSetRectW() implements the write access to the Chora instant property 
*   'rect.w'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the width.
*   aW    - The new value for the rectangles width.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed width.
*
*******************************************************************************/
XRect EwSetRectW
( 
  XRect             aRect, 
  XInt32            aW 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectH
*
* DESCRIPTION:
*   The function EwSetRectH() changes the height of the given rectangle to a 
*   new value aH.
*
*   EwSetRectH() implements the write access to the Chora instant property 
*   'rect.h'.
*
* ARGUMENTS:
*   aRect - The rectangle to change the height.
*   aH    - The new value for the rectangles height.
*
* RETURN VALUE:
*   Returns a new rectangle with the changed height.
*
*******************************************************************************/
XRect EwSetRectH
( 
  XRect             aRect, 
  XInt32            aH 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectPoint1
*
* DESCRIPTION:
*   The function EwSetPoint1() changes the coordinates of the first (top-left)
*   point of the rectangle to a new value aPoint1.
*
*   EwSetRectPoint1() implements the write access to the Chora instant property 
*   'rect.point1'.
*
* ARGUMENTS:
*   aRect   - The rectangle to be changed.
*   aPoint1 - The new coordinates for the first (top-left) point.
*
* RETURN VALUE:
*   Returns a new rectangle with changed coordinates.
*
*******************************************************************************/
XRect EwSetRectPoint1
( 
  XRect             aRect, 
  XPoint            aPoint1 
);


/*******************************************************************************
* FUNCTION:
*   EwSetRectPoint2
*
* DESCRIPTION:
*   The function EwSetPoint2() changes the coordinates of the second (bottom-
*   right) point of the rectangle to a new value aPoint2.
*
*   EwSetRectPoint2() implements the write access to the Chora instant property 
*   'rect.point2'.
*
* ARGUMENTS:
*   aRect   - The rectangle to be changed.
*   aPoint2 - The new coordinates for the second (bottom-right) point.
*
* RETURN VALUE:
*   Returns a new rectangle with changed coordinates.
*
*******************************************************************************/
XRect EwSetRectPoint2
( 
  XRect             aRect, 
  XPoint            aPoint2 
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfRect
*
* DESCRIPTION:
*   The function EwGetVariantOfRect() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple rect values.
*
* RETURN VALUE:
*   Returns the determinated rect value.
*
*******************************************************************************/
XRect EwGetVariantOfRect
( 
  const XVariant* aConstant
);



/*******************************************************************************
* FUNCTION:
*   EwNewRef
*
* DESCRIPTION:
*   The function EwNewRef() initializes a new XRef structure with the given
*   arguments aObject, aOnGetProc, aOnSetProc and returns the structure.
*
* ARGUMENTS:
*   aObject    - Pointer to an object which contains the referenced property.
*   aOnGetProc - Pointer to the properties own OnGet() method.
*   aOnSetProc - Pointer to the properties own OnSet() method.
*
* RETURN VALUE:
*   Returns an initialized XRef structure, describing a reference to a 
*   property.
*
*******************************************************************************/
XRef EwNewRef
( 
  void*             aObject, 
  XRefGetSetProc    aOnGetProc, 
  XRefGetSetProc    aOnSetProc 
);

#define EwNewRef( aObject, aOnGetProc, aOnSetProc )                            \
  EwNewRef( aObject, (XRefGetSetProc)aOnGetProc, (XRefGetSetProc)aOnSetProc )


/*******************************************************************************
* FUNCTION:
*   EwCompRef
*
* DESCRIPTION:
*   The function EwCompRef() compares the given references aRef1 and aRef2 
*   and returns a value == 0 if the references are identical. Otherwise the 
*   function returns a value != 0.
*
*   EwCompRef() implements the Chora instant operators: 'ref == ref' and
*   'ref != ref'.
*
* ARGUMENTS:
*   aRef1 - The first reference.
*   aRef2 - The second reference.
*
* RETURN VALUE:
*   Returns zero if both references are equal.
*
*******************************************************************************/
int EwCompRef
( 
  XRef              aRef1, 
  XRef              aRef2 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetInt8
*
* DESCRIPTION:
*   The function EwOnGetInt8() will be called in order to read an int8 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an int8 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XInt8 EwOnGetInt8
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetInt16
*
* DESCRIPTION:
*   The function EwOnGetInt16() will be called in order to read an int16
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an int16 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XInt16 EwOnGetInt16
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetInt32
*
* DESCRIPTION:
*   The function EwOnGetInt32() will be called in order to read an int32 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an int32 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XInt32 EwOnGetInt32
( 
  XRef              aRef 
);



/*******************************************************************************
* FUNCTION:
*   EwOnGetUInt8
*
* DESCRIPTION:
*   The function EwOnGetUInt8() will be called in order to read an uint8 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an uint8 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XUInt8 EwOnGetUInt8
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetUInt16
*
* DESCRIPTION:
*   The function EwOnGetUInt16() will be called in order to read an uint16 
*   property referenced by the aRef argument. The access to the property will 
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an uint16 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XUInt16 EwOnGetUInt16
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetUInt32
*
* DESCRIPTION:
*   The function EwOnGetUInt32() will be called in order to read an uint32 
*   property referenced by the aRef argument. The access to the property will 
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an uint32 property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XUInt32 EwOnGetUInt32
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetBool
*
* DESCRIPTION:
*   The function EwOnGetBool() will be called in order to read a bool property 
*   referenced by the aRef argument. The access to the property will be done 
*   by calling the properties own OnGet() method. The value returned from the 
*   OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a bool property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XBool EwOnGetBool
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetFloat
*
* DESCRIPTION:
*   The function EwOnGetFloat() will be called in order to read a float 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a float property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XFloat EwOnGetFloat
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetChar
*
* DESCRIPTION:
*   The function EwOnGetChar() will be called in order to read a char 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a char property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XChar EwOnGetChar
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetString
*
* DESCRIPTION:
*   The function EwOnGetString() will be called in order to read a string 
*   property referenced by the aRef argument. The access to the property will 
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a string property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XString EwOnGetString
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetPoint
*
* DESCRIPTION:
*   The function EwOnGetPoint() will be called in order to read a point 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a point property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XPoint EwOnGetPoint
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetRect
*
* DESCRIPTION:
*   The function EwOnGetRect() will be called in order to read a rect 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a rect property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XRect EwOnGetRect
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetColor
*
* DESCRIPTION:
*   The function EwOnGetColor() will be called in order to read a color 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a color property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XColor EwOnGetColor
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetEnum
*
* DESCRIPTION:
*   The function EwOnGetEnum() will be called in order to read an enum 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to an enum property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XEnum EwOnGetEnum
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetSet
*
* DESCRIPTION:
*   The function EwOnGetSet() will be called in order to read a set 
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a set property.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XSet EwOnGetSet
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetObject
*
* DESCRIPTION:
*   The function EwOnGetObject() will be called in order to read a object 
*   property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnGet() method. The value 
*   returned from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property conating a pointer to an object.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XObject EwOnGetObject
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetSlot
*
* DESCRIPTION:
*   The function EwOnGetSlot() will be called in order to read a slot property 
*   referenced by the aRef argument. The access to the property will be done by
*   calling the properties own OnGet() method. The value returned from the 
*   OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property containing the slot.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XSlot EwOnGetSlot
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetLangId
*
* DESCRIPTION:
*   The function EwOnGetLangId() will be called in order to read a language 
*   property referenced by the aRef argument. The access to the property will 
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property containing the language.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XLangId EwOnGetLangId
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetStylesSet
*
* DESCRIPTION:
*   The function EwOnGetStylesSet() will be called in order to read a styles
*   property referenced by the aRef argument. The access to the property will 
*   be done by calling the properties own OnGet() method. The value returned 
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property containing the styles set.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XStylesSet EwOnGetStylesSet
( 
  XRef              aRef 
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetClass
*
* DESCRIPTION:
*   The function EwOnGetClass() will be called in order to read a class property
*   referenced by the aRef argument. The access to the property will be done by
*   calling the properties own OnGet() method. The value returned from the 
*   OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property containing the class.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XClass EwOnGetClass
(
  XRef aRef
);


/*******************************************************************************
* FUNCTION:
*   EwOnGetHandle
*
* DESCRIPTION:
*   The function EwOnGetHandle() will be called in order to read a handle
*   property referenced by the aRef argument. The access to the property will
*   be done by calling the properties own OnGet() method. The value returned
*   from the OnGet() method will be passed back to the caller.
*
* ARGUMENTS:
*   aRef - Reference to a property containing the handle.
*
* RETURN VALUE:
*   Returns the value of the property referenced by aRef.
*
*******************************************************************************/
XHandle EwOnGetHandle
(
  XRef aRef
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetInt8
*
* DESCRIPTION:
*   The function EwOnSetInt8() will be called in order to assign a value to an
*   int8 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an int8 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetInt8
( 
  XRef              aRef, 
  XInt8             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetInt16
*
* DESCRIPTION:
*   The function EwOnSetInt16() will be called in order to assign a value to an
*   int16 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an int16 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetInt16
( 
  XRef              aRef, 
  XInt16            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetInt32
*
* DESCRIPTION:
*   The function EwOnSetInt32() will be called in order to assign a value to an
*   int32 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an int32 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetInt32
( 
  XRef              aRef, 
  XInt32            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetUInt8
*
* DESCRIPTION:
*   The function EwOnSetUInt8() will be called in order to assign a value to an
*   uint8 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an uint8 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetUInt8
( 
  XRef              aRef, 
  XUInt8            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetUInt16
*
* DESCRIPTION:
*   The function EwOnSetUInt16() will be called in order to assign a value to an
*   uint16 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an uint16 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetUInt16
( 
  XRef              aRef, 
  XUInt16           aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetUInt32
*
* DESCRIPTION:
*   The function EwOnSetUInt32() will be called in order to assign a value to an
*   uint32 property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an uint32 property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetUInt32
( 
  XRef              aRef, 
  XUInt32           aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetBool
*
* DESCRIPTION:
*   The function EwOnSetBool() will be called in order to assign a value to a
*   bool property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a bool property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetBool
( 
  XRef              aRef, 
  XBool             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetFloat
*
* DESCRIPTION:
*   The function EwOnSetFloat() will be called in order to assign a value to a
*   float property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a float property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetFloat
( 
  XRef              aRef, 
  XFloat            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetChar
*
* DESCRIPTION:
*   The function EwOnSetChar() will be called in order to assign a value to a
*   char property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a char property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetChar
( 
  XRef              aRef, 
  XChar             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetString
*
* DESCRIPTION:
*   The function EwOnSetString() will be called in order to assign a value to 
*   a string property referenced by the aRef argument. The access to the 
*   property will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a string property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetString
( 
  XRef              aRef, 
  XString           aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetPoint
*
* DESCRIPTION:
*   The function EwOnSetPoint() will be called in order to assign a value to a
*   point property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a point property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetPoint
( 
  XRef              aRef, 
  XPoint            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetRect
*
* DESCRIPTION:
*   The function EwOnSetRect() will be called in order to assign a value to a
*   rect property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a rect property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetRect
( 
  XRef              aRef, 
  XRect             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetColor
*
* DESCRIPTION:
*   The function EwOnSetColor() will be called in order to assign a value to a
*   color property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a color property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetColor
( 
  XRef              aRef, 
  XColor            aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetEnum
*
* DESCRIPTION:
*   The function EwOnSetEnum() will be called in order to assign a value to an
*   enum property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to an enum property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetEnum
( 
  XRef              aRef, 
  XEnum             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetSet
*
* DESCRIPTION:
*   The function EwOnSetSet() will be called in order to assign a value to a
*   set property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a set property.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetSet
( 
  XRef              aRef, 
  XSet              aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetObject
*
* DESCRIPTION:
*   The function EwOnSetObject() will be called in order to assign a value to 
*   an object property referenced by the aRef argument. The access to the 
*   property will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing a pointer to an object.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetObject
( 
  XRef              aRef, 
  XObject           aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetSlot
*
* DESCRIPTION:
*   The function EwOnSetSlot() will be called in order to assign a value to a 
*   slot property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing the slot.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetSlot
( 
  XRef              aRef, 
  XSlot             aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetLangId
*
* DESCRIPTION:
*   The function EwOnSetLangId() will be called in order to assign a value to a 
*   language property referenced by the aRef argument. The access to the 
*   property will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing the language.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetLangId
( 
  XRef              aRef, 
  XLangId           aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetStylesSet
*
* DESCRIPTION:
*   The function EwOnSetStylesSet() will be called in order to assign a value 
*   to a styles property referenced by the aRef argument. The access to the 
*   property will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing the styles set.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetStylesSet
( 
  XRef              aRef, 
  XStylesSet        aValue 
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetClass
*
* DESCRIPTION:
*   The function EwOnSetClass() will be called in order to assign a value to a 
*   class property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing the class.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetClass
(
  XRef              aRef,
  XClass            aValue
);


/*******************************************************************************
* FUNCTION:
*   EwOnSetHandle
*
* DESCRIPTION:
*   The function EwOnSetHandle() will be called in order to assign a value to a 
*   handle property referenced by the aRef argument. The access to the property 
*   will be done by calling the properties own OnSet() method.
*
* ARGUMENTS:
*   aRef   - Reference to a property containing the handle.
*   aValue - The value to be assigned to the property.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwOnSetHandle
(
  XRef              aRef,
  XHandle           aValue
);


/*******************************************************************************
* FUNCTION:
*   EwNewSlot
*
* DESCRIPTION:
*   The function EwNewSlot() initializes a new XSlot structure with the given
*   arguments aObject, aSlotProc and returns the structure.
*
* ARGUMENTS:
*   aObject   - Pointer to an object which contains the slot.
*   aSlotProc - Pointer to the slots own method.
*
* RETURN VALUE:
*   Returns an initialized XSlot structure, describing a reference to a slot.
*
*******************************************************************************/
XSlot EwNewSlot
( 
  void*             aObject, 
  XSlotProc         aSlotProc 
);

#define EwNewSlot( aObject, aSlotProc )                                        \
  EwNewSlot( aObject, (XSlotProc)aSlotProc )


/*******************************************************************************
* FUNCTION:
*   EwCompSlot
*
* DESCRIPTION:
*   The function EwCompSlot() compares the given slots aSlot1 and aSlot2 and 
*   returns a value == 0 if the slots are identical. Otherwise the function 
*   returns a value != 0.
*
*   EwCompSlot() implements the Chora instant operators: 'slot == slot' and
*   'slot != slot'.
*
* ARGUMENTS:
*   aSlot1 - The first slot.
*   aSlot2 - The second slot.
*
* RETURN VALUE:
*   Returns zero if both slots are equal.
*
*******************************************************************************/
int EwCompSlot
( 
  XSlot             aSlot1, 
  XSlot             aSlot2 
);


/*******************************************************************************
* FUNCTION:
*   EwSignal
*
* DESCRIPTION:
*   The function EwSignal() will be called in order to deliver a signal to a
*   slot aSlot. In response to the delivery the function EwSignal() calls slots
*   own method SlotProc.
*
* ARGUMENTS:
*   aSlot   - Slot to send a signal.
*   aSender - Who is the sender of the signal?
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSignal
( 
  XSlot             aSlot, 
  XObject           aSender 
);


/*******************************************************************************
* FUNCTION:
*   EwPostSignal
*
* DESCRIPTION:
*   The function EwPostSignal() will be called in order to store a signal for a
*   delayed delivery. The affected slot aSlot will receive the signal as soon as
*   possible, but always before the screen update is performed.
*
* ARGUMENTS:
*   aSlot   - Slot to receive a signal.
*   aSender - Who is the sender of the signal?
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPostSignal
(
  XSlot             aSlot,
  XObject           aSender 
);


/*******************************************************************************
* FUNCTION:
*   EwIdleSignal
*
* DESCRIPTION:
*   The function EwIdleSignal() will be called in order to store a signal for a
*   delayed delivery. The affected slot aSlot will receive the signal as soon as
*   possible, but always after the screen update is performed.
*
* ARGUMENTS:
*   aSlot   - Slot to receive a signal.
*   aSender - Who is the sender of the signal?
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwIdleSignal
(
  XSlot             aSlot,
  XObject           aSender 
);


/*******************************************************************************
* FUNCTION:
*   EwProcessSignals
*
* DESCRIPTION:
*   The function EwProcessSignals() will be called in order to deliver the 
*   signals, which are pending at the moment. The function returns a value, 
*   which indicates whether signals have been delivered or not. If no signals
*   were pending, the function returns 0. In the case, at least one signal has
*   been delivered, != 0 is returned.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If no signals were delivered, the function returns 0. Otherwise != 0 is
*   returned.
*
*******************************************************************************/
int EwProcessSignals
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwAnyPendingSignals
*
* DESCRIPTION:
*   The function EwAnyPendingSignals() returns a value != 0, if there are any
*   signals pending for the delivery.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The function returns a value != 0, if there are signals pending for the
*   delivery.
*
*******************************************************************************/
int EwAnyPendingSignals
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwAttachObjObserver
*
* DESCRIPTION:
*   The function EwAttachObjObserver() implements the statement 'attachobserver'
*   of the Chora language. The function registers a slot method aSlot with the 
*   object aObject. As soon the object has been triggered by the function
*   EwNotifyObjObservers(), all registered slot methods will receive a signal
*   automatically.
*
*   In order to deregister an observer, the function EwDetachObjObserver() 
*   should be used. Please note, that the Garbage Collector will automatically
*   detach an observer from the object, if the affected object or the observer
*   itself is about to be disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be attached as an observer to the
*     object aObject. If aSlot is NULL, the function returns immediately.
*   aObject - The destination object for the registration. If this parameter
*     is NULL, the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers of one and the same object (aObject). This is a
*     kind of subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be registered.
*   If an identic registration already exists, the function simply returns 0.
*
*******************************************************************************/
int EwAttachObjObserver
( 
  XSlot             aSlot,
  XObject           aObject,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwAttachRefObserver
*
* DESCRIPTION:
*   The function EwAttachRefObserver() implements the statement 'attachobserver'
*   of the Chora language. The function registers a slot method aSlot with the 
*   property referred by aRef. As soon the property has been triggered by the 
*   function EwNotifyRefObservers(), all registered slot methods will receive a
*   signal automatically.
*
*   In order to deregister an observer, the function EwDetachRefObserver() 
*   should be used. Please note, that the Garbage Collector will automatically
*   detach an observer from the property, if the object, the property belongs
*   to or the observer itself is about to be disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be attached as an observer to the
*     property referred by aRef. If aSlot is NULL, the function returns 
*     immediately.
*   aRef    - The reference to the destination property for the registration.
*     If this parameter is NULL, the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers of one and the same property referred by aRef. This
*     is a kind of subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be registered.
*   If an identic registration already exists, the function simply returns 0.
*
*******************************************************************************/
int EwAttachRefObserver
( 
  XSlot             aSlot,
  XRef              aRef,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwAttachObserver
*
* DESCRIPTION:
*   The function EwAttachObserver() implements the statement 'attachobserver'
*   of the Chora language. The function registers a slot method aSlot with the
*   global notifier. As soon the the global notifier has been triggered by the
*   function EwNotifyObservers(), all registered slot methods will receive a 
*   signal automatically.
*
*   In order to deregister an observer, the function EwDetachObserver() should 
*   be used. Please note, that the Garbage Collector will automatically detach 
*   an observer from the notifier, if the affected observer is about to be 
*   disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be attached as an observer to the
*     global notifier. If aSlot is NULL, the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers. This is a kind of subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be registered.
*   If an identic registration already exists, the function simply returns 0.
*
*******************************************************************************/
int EwAttachObserver
( 
  XSlot             aSlot,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwDetachObjObserver
*
* DESCRIPTION:
*   The function EwDetachObjObserver() implements the statement 'detachobserver'
*   of the Chora language. The function deregisters a slot method aSlot from the 
*   object aObject. This function is the counterpart of EwAttachObjObserver().
*
*   Please note, that the Garbage Collector will automatically detach an 
*   observer from the object, if the affected object or the observer itself is 
*   about to be disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be detached from the object aObject.
*     If aSlot is NULL, the function returns immediately.
*   aObject - The affected object for the deregistration. If aObject is NULL, 
*     the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers of one and the same object (aObject). This is a
*     kind of subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be deregistered.
*   If no corresponding registration exists, the function simply returns 0.
*
*******************************************************************************/
int EwDetachObjObserver
( 
  XSlot             aSlot,
  XObject           aObject,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwDetachRefObserver
*
* DESCRIPTION:
*   The function EwDetachRefObserver() implements the statement 'detachobserver'
*   of the Chora language. The function deregisters a slot method aSlot from the 
*   property referred by aRef. This function is the counterpart of the function
*   EwAttachRefObserver().
*
*   Please note, that the Garbage Collector will automatically detach an 
*   observer from the property, if the object, the property belongs to or the 
*   observer itself is about to be disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be detached from the property referred
*     by aRef. If aSlot is NULL, the function returns immediately.
*   aRef    - The reference to the affected property for the deregistration.
*     If aRef is NULL, the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers of one and the same property. This is a kind of 
*     subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be deregistered.
*   If no corresponding registration exists, the function simply returns 0.
*
*******************************************************************************/
int EwDetachRefObserver
( 
  XSlot             aSlot,
  XRef              aRef,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwDetachObserver
*
* DESCRIPTION:
*   The function EwDetachObserver() implements the statement 'detachobserver'
*   of the Chora language. The function deregisters a slot method aSlot from the 
*   global notifier. This function is the counterpart of EwAttachObserver().
*
*   Please note, that the Garbage Collector will automatically detach an 
*   observer from the notifier, if the affected observer itself is about to be 
*   disposed.
*
* ARGUMENTS:
*   aSlot   - The slot method, which will be detached from the object aObject.
*     If aSlot is NULL, the function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers. This is a kind of subdivision.
*
* RETURN VALUE:
*   The function returns a value != 0, if the observer could be deregistered.
*   If no corresponding registration exists, the function simply returns 0.
*
*******************************************************************************/
int EwDetachObserver
( 
  XSlot             aSlot,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwNotifyObjObservers
*
* DESCRIPTION:
*   The function EwNotifyObjObservers() implements the Chora statement 
*   'notifyobservers'. The function posts signals for each observer, which 
*   has been previously registered with the given object aObject.
*
*   In order to register an observer the function EwAttachObjObserver() should
*   be used.
*
* ARGUMENTS:
*   aObject - The object to trigger the observers. If aObject is NULL, the 
*     function returns immediately.
*   aId     - This is an additional ID, which distinguish between different
*     groups of observers of one and the same object (aObject). This is a
*     kind of subdivision.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwNotifyObjObservers
( 
  XObject           aObject,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwNotifyRefObservers
*
* DESCRIPTION:
*   The function EwNotifyRefObservers() implements the Chora statement 
*   'notifyobservers'. The function posts signals for each observer, which 
*   has been previously registered with the property referred by aRef.
*
*   In order to register an observer the function EwAttachRefObserver() should
*   be used.
*
* ARGUMENTS:
*   aRef - The reference to the property to trigger the observers. If Ref is 
*     NULL, the function returns immediately.
*   aId  - This is an additional ID, which distinguish between different
*     groups of observers of one and the same property. This is a kind of 
*     subdivision.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwNotifyRefObservers
( 
  XRef              aRef,
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwNotifyObservers
*
* DESCRIPTION:
*   The function EwNotifyObservers() implements the statement 'notifyobservers'
*   of the Chora language. The function posts signals for each observer, which 
*   has been previously registered with the global notifier.
*
*   In order to register an observer the function EwAttachObserver() should be 
*   used.
*
* ARGUMENTS:
*   aId     - This is an ID, which distinguish between different groups of 
*     observers. This is a kind of subdivision.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwNotifyObservers
( 
  XUInt32           aId
);


/*******************************************************************************
* FUNCTION:
*   EwGetAutoObject
*
* DESCRIPTION:
*   The function EwGetAutoObject() will be called from the automatic generated
*   'C' code in order to access an auto object identified by aAutoObject. The
*   function verifies whether the required object is already instantiated and 
*   if it's true, the existing instance is returned immediately. If no instance
*   of the desired auto object could be found, EwGetAutoObject() prepares a new
*   object of the class aClass and initializes it properly. The created object
*   will remains in memory as long as the object is in use. Objects, which are
*   not in use any more are automatically reclaimed by the Garbage Collector.
*
* ARGUMENTS:
*   aAutoObject - Pointer to the description of the auto object in the ROM code.
*   aClass      - The class of the auto object.
*   aFile       - aFile stores the module name, where the auto object has been
*     created. aFile is for debugging purpose only.
*   aLine       - aLine stores the line number, at this the auto object has 
*     been created. aLine is for debugging purpose only.
*
* RETURN VALUE:
*   Returns a pointer to the auto object.
*
*******************************************************************************/
XObject EwGetAutoObject
( 
  const XVariant* aAutoObject,
  const void*     aClass
);

#define EwGetAutoObject( aAutoObject, aClass )                                 \
  ((aClass)EwGetAutoObject( aAutoObject, EW_CLASS( aClass )))



/*******************************************************************************
* FUNCTION:
*   EwRegisterResource
*
* DESCRIPTION:
*   The function EwRegisterResource() registers a resource object aObject in the
*   global MapList, so the resource stored in the object can be shared between
*   different callers. EwRegisterResource() creates a new entry in the MapList
*   to store the pointer to the object and the pointer to the resource in the 
*   ROM code.
*
*   If the MapList is full, the function throws an error.
*
* ARGUMENTS:
*   aObject   - Pointer to the object where the resource has been loaded.
*   aResource - Pointer to the resource in the ROM.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwRegisterResource
( 
  XObject           aObject, 
  const void*       aResource 
);


/*******************************************************************************
* FUNCTION:
*   EwDeregisterResource
*
* DESCRIPTION:
*   The function EwDeregisterResource() removes the resource object aObject 
*   from the global MapList. After the object has been removed, the resource
*   stored in the object can not be shared anymore.
*
* ARGUMENTS:
*   aObject - Pointer to the resource object to remove from the MapList.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDeregisterResource
( 
  XObject           aObject 
);


/*******************************************************************************
* FUNCTION:
*   EwLoadResource
*
* DESCRIPTION:
*   The function EwLoadResource() will be called from the automatic generated
*   'C' code in order to load a resource. EwLoadResource() verifies whether the
*   required resource is already loaded and if it's true, the function returns
*   the pointer to the object where the resource is stored in.
*
*   If the requested resource is not loaded yet, EwLoadResource() creates a new
*   object of the class aClass and invokes it in order to load the resource.
*
* ARGUMENTS:
*   aResource - Pointer to the resource in the ROM code.
*   aClass    - The class of the resource.
*   aFile     - aFile stores the module name, where the resource object has 
*     been created. aFile is for debugging purpose only.
*   aLine     - aLine stores the line number, at this the resource object has 
*     been created. aLine is for debugging purpose only.
*
* RETURN VALUE:
*   Returns a pointer to the resource object wich stores the required resource.
*
*******************************************************************************/
XObject EwLoadResource
( 
  const XVariant* aResource, 
  const void*     aClass 
);

#define EwLoadResource( aResource, aClass )                                    \
  ((aClass)EwLoadResource( aResource, EW_CLASS( aClass )))


/*******************************************************************************
* FUNCTION:
*   EwGetLanguage
*
* DESCRIPTION:
*   The function EwGetLanguage() returns the language ID corresponding to the 
*   currently selected language. The function returns 0 if the 'default' 
*   language is selected.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the ID of the selected language.
*
*******************************************************************************/
XLangId EwGetLanguage
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwSetLanguage
*
* DESCRIPTION:
*   The function EwSetLanguage() changes the selected language ID to aLangId.
*   To select the default language pass 0 (zero) in the argument to this 
*   function.
*
*   After the language has been selected, the Resource Manager tries to find
*   resource which fit the selected language.
*
* ARGUMENTS:
*   aLangId - The ID of the desired language.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetLanguage
( 
  XLangId           aLangId 
);


/*******************************************************************************
* FUNCTION:
*   EwGetStyles
*
* DESCRIPTION:
*   The function EwGetStyles() returns the set corresponding to the currently 
*   active styles. The function returns 0 if no styles are active.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the set containing the active styles.
*
*******************************************************************************/
XStylesSet EwGetStyles
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwSetStyles
*
* DESCRIPTION:
*   The function EwSetStyles() changes the set of active styles to the new value
*   aStyles.
*
* ARGUMENTS:
*   aStylesSet - The new styles set.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSetStyles
( 
  XStylesSet        aStylesSet
);


/*******************************************************************************
* FUNCTION:
*   EwStylesContains
*
* DESCRIPTION:
*   The function EwStylesContains() implements the Chora instant method 
*   'styles.contains()'.
*
* ARGUMENTS:
*   aStylesSet1 - The first style to verify its content.
*   aStylesSet2 - The second style.
*
* RETURN VALUE:
*   The function returns != 0, if the second style aStylesSet2 is contained
*   in the given aStylesSet1.
*
*******************************************************************************/
int EwStylesContains
( 
  XStylesSet        aStylesSet1,
  XStylesSet        aStylesSet2
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOf
*
* DESCRIPTION:
*   The function EwGetVariantOf() will be called to determinate a multilingual/
*   multivariant value of the constant or resource aVariant, depending on the 
*   currently selected language and the styles set.
*
*   If the currently selected language could not be found, the function returns
*   the value corresponding to the default language (LangId == 0). In case of a
*   multivariant definition, the function evaluates the variants in order to 
*   find one, which fits the styles currently active in the styles set.
*
* ARGUMENTS:
*   aVariant - A pointer to the affected multilingual/multivariant definition.
*   aSize    - The size to a single entry in the language table of the affected
*     definition.
*
* RETURN VALUE:
*   Returns the pointer to the found value of the definition.
*
*******************************************************************************/
const void* EwGetVariantOf
( 
  register const XVariant* aVariant,
  int                      aSize
);

#define EwGetVariantOf( aVariant, aKind )                                      \
  ((const aKind*)EwGetVariantOf( aVariant, sizeof( aKind )))


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfLangId
*
* DESCRIPTION:
*   The function EwGetVariantOfLangId() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple language id
*     values.
*
* RETURN VALUE:
*   Returns the determinated language id value.
*
*******************************************************************************/
XLangId EwGetVariantOfLangId
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfStylesSet
*
* DESCRIPTION:
*   The function EwGetVariantOfStylesSet() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple styles set values.
*
* RETURN VALUE:
*   Returns the determinated styles set value.
*
*******************************************************************************/
XStylesSet EwGetVariantOfStylesSet
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfInt8
*
* DESCRIPTION:
*   The function EwGetVariantOfInt8() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple int8 values.
*
* RETURN VALUE:
*   Returns the determinated int8 value.
*
*******************************************************************************/
XInt8 EwGetVariantOfInt8
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfInt16
*
* DESCRIPTION:
*   The function EwGetVariantOfInt16() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple int16 values.
*
* RETURN VALUE:
*   Returns the determinated int16 value.
*
*******************************************************************************/
XInt16 EwGetVariantOfInt16
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfInt32
*
* DESCRIPTION:
*   The function EwGetVariantOfInt32() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple int32 values.
*
* RETURN VALUE:
*   Returns the determinated int32 value.
*
*******************************************************************************/
XInt32 EwGetVariantOfInt32
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfUInt8
*
* DESCRIPTION:
*   The function EwGetVariantOfUInt8() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple uint8 values.
*
* RETURN VALUE:
*   Returns the determinated uint8 value.
*
*******************************************************************************/
XUInt8 EwGetVariantOfUInt8
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfUInt16
*
* DESCRIPTION:
*   The function EwGetVariantOfUInt16() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple uint16 values.
*
* RETURN VALUE:
*   Returns the determinated uint16 value.
*
*******************************************************************************/
XUInt16 EwGetVariantOfUInt16
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfUInt32
*
* DESCRIPTION:
*   The function EwGetVariantOfUInt32() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple uint32 values.
*
* RETURN VALUE:
*   Returns the determinated uint32 value.
*
*******************************************************************************/
XUInt32 EwGetVariantOfUInt32
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfBool
*
* DESCRIPTION:
*   The function EwGetVariantOfBool() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple bool values.
*
* RETURN VALUE:
*   Returns the determinated bool value.
*
*******************************************************************************/
XBool EwGetVariantOfBool
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfFloat
*
* DESCRIPTION:
*   The function EwGetVariantOfFloat() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple float values.
*
* RETURN VALUE:
*   Returns the determinated float value.
*
*******************************************************************************/
XFloat EwGetVariantOfFloat
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfEnum
*
* DESCRIPTION:
*   The function EwGetVariantOfEnum() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple enum values.
*
* RETURN VALUE:
*   Returns the determinated enum value.
*
*******************************************************************************/
XEnum EwGetVariantOfEnum
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfSet
*
* DESCRIPTION:
*   The function EwGetVariantOfSet() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple set values.
*
* RETURN VALUE:
*   Returns the determinated set value.
*
*******************************************************************************/
XSet EwGetVariantOfSet
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwNewString
*
* DESCRIPTION:
*   The function EwNewString() creates a new copy of the string aString. The
*   function reserves memory for the string and copies it there.
*
*   EwNewString() implements the Chora instant constructor: 'string(aString)'.
*
* ARGUMENTS:
*   aString - 
*
* RETURN VALUE:
*   
*
*******************************************************************************/
XString EwNewString
( 
  const XChar*      aString 
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringAnsi
*
* DESCRIPTION:
*   The function EwNewStringAnsi() creates a wide-character copy of the given
*   ANSI string. The function reserves memory for the new string and copies all
*   characters from the aAnsi string into this reserved memory area. During the
*   operation the function extends all ANSI characters to 16 bit wide-char
*   properly.
*
* ARGUMENTS:
*   aAnsi - Pointer to the 8 bit ANSI string.
*
* RETURN VALUE:
*   Returns a pointer to the wide-character copy of the given ANSI string.
*
*******************************************************************************/
XString EwNewStringAnsi
( 
  register const char* aAnsi 
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringUtf8
*
* DESCRIPTION:
*   The function EwNewStringUtf8() creates a wide-character copy of the given
*   UTF-8 string. The function reserves memory for the new string and decodes
*   the input characters from the aUtf8 string into this reserved memory area.
*   During the operation the function decodes all UTF-8 characters to 16 bit 
*   wide-char properly.
*
* ARGUMENTS:
*   aUtf8  - Pointer to the 8 bit UTF-8 string.
*   aCount - Number of characters (bytes) within the aUtf8 string without any
*     zero terminator signs.
*
* RETURN VALUE:
*   Returns a pointer to the wide-character copy of the decoded UTF-8 string.
*
*******************************************************************************/
XString EwNewStringUtf8
(
  const unsigned char* aUtf8,
  int                  aCount
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringUInt
*
* DESCRIPTION:
*   The function EwNewStringUInt() converts the given unsigned number aValue in
*   a string and returns the result. The function adds leading zeros '0' until 
*   the resulted string has reached the length given in the aCount argument.
*
*   EwNewStringUInt() implements the Chora instant constructor: 
*   'string(aValue,aNoOfDigits)'.
*
* ARGUMENTS:
*   aValue - Unsigned 32 bit value to be converted to string.
*   aCount - Desired length of the resulted string.
*   aRadix - Controls the format of the resulting string. This parameter can
*     assume values 2, 8, 10 or 16. Accordingly the number is converted in a
*     binary, octal, decimal or hexadecimal notation.
*
* RETURN VALUE:
*   Returns a string containing the given number aValue converted in notation
*   according to the parameter aRadix.
*
*******************************************************************************/
XString EwNewStringUInt
( 
  XUInt32           aValue, 
  XInt32            aCount,
  XInt32            aRadix
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringInt
*
* DESCRIPTION:
*   The function EwNewStringInt() converts the given signed number aValue in a 
*   string and returns the result. The function adds leading zeros '0' until the
*   resulted string has reached the length given in the aCount argument. If the
*   number is negative, the function adds '-' minus sign.
*
*   EwNewStringInt() implements the Chora instant constructor: 
*   'string(aValue,aNoOfDigits,aRadix)'.
*
* ARGUMENTS:
*   aValue - Signed 32 bit value to be converted to string.
*   aCount - Desired length of the resulted string.
*   aRadix - Controls the format of the resulting string. This parameter can
*     assume values 2, 8, 10 or 16. Accordingly the number is converted in a
*     binary, octal, decimal or hexadecimal notation.
*
* RETURN VALUE:
*   Returns a string containing the given number aValue converted in notation
*   according to the parameter aRadix.
*
*******************************************************************************/
XString EwNewStringInt
( 
  XInt32            aValue, 
  XInt32            aCount,
  XInt32            aRadix
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringFloat
*
* DESCRIPTION:
*   The function EwNewStringFloat() converts the given floating point number 
*   aValue in a string and returns the result. 
*
*   EwNewStringFloat() implements the Chora instant constructor: 
*   'string(aValue,aNoOfDigits,aPrecision)'.
*
* ARGUMENTS:
*   aValue     - The floating point value to be converted in a string.
*   aCount     - Minimum number of characters to store in the resulted string 
*     output.
*   aPrecision - The aPrecision argument specifies the exact number of digits
*     after the decimal point. If this parameter is < 0, the function will
*     automatically estimate the number of digits after the decimal point.
*
* RETURN VALUE:
*   Returns a string containing the given floating point number aValue.
*
*******************************************************************************/
XString EwNewStringFloat
( 
  XFloat            aValue, 
  XInt32            aCount, 
  XInt32            aPrecision 
);


/*******************************************************************************
* FUNCTION:
*   EwNewStringChar
*
* DESCRIPTION:
*   The function EwNewStringChar() creates a string with the given length aCount 
*   and fills the whole string with the character aChar.
*
*   EwNewStringChar() implements the Chora instant constructor: 
*   'string(aChar,aNoOfDigits)'.
*
* ARGUMENTS:
*   aChar  - Character to fill the string.
*   aCount - Desired length of the resulted string.
*
* RETURN VALUE:
*   Returns a string containing aCount characters aChar.
*
*******************************************************************************/
XString EwNewStringChar
( 
  XChar             aChar, 
  XInt32            aCount 
);


/*******************************************************************************
* FUNCTION:
*   EwCompString
*
* DESCRIPTION:
*   The function EwCompString() compares aString1 and aString2 lexicographically
*   and returns a value indicating their relationship. 
*
*   EwCompString() implements the Chora instant operators: 'string == string'
*   'string != string', 'string < string', ...
*
* ARGUMENTS:
*   aString1 - The first string to be compared.
*   aString2 - The second string to be compared.
*
* RETURN VALUE:
*   If aString1 is identical to aString2 the function returns 0 (zero).
*   If aString1 is less than aString2 the function returns -1.
*   If aString1 is greater than aString2 the function returns +1.
*
*******************************************************************************/
int EwCompString
( 
  XString           aString1, 
  XString           aString2 
);


/*******************************************************************************
* FUNCTION:
*   EwLoadString
*
* DESCRIPTION:
*   The function EwLoadString() returns the pointer to the 'string' constant
*   described by aStringConst. Depending on the configuration of the system, the
*   'string' may need to be decompressed before it can be used. In such case all
*   decompressed strings are managed within a common string cache. If possible,
*   the function will reuse such already existing strings.
*
*   The function returns the pointer to the first character of the decompressed
*   'string' constant. If 'string' constants are not compressed, the function
*   returns the pointer to the ROM area where the 'string' is stored.
*
* ARGUMENTS:
*   aStringConst - Pointer to the descriptor of the constant.
*
* RETURN VALUE:
*   Returns a pointer to the string constant.
*
*******************************************************************************/
XString EwLoadString
( 
  const XStringRes* aStringConst
);


/*******************************************************************************
* FUNCTION:
*   EwShareString
*
* DESCRIPTION:
*   The function EwShareString() verifies whether the string passed in the
*   parameter aString is a private copy of the string and changes it to a
*   shared copy. Private string copies are created when a string is modified
*   by the string index operator []. In such case, the operator creates a copy
*   of the original string. Later the index operator can operate on the string
*   without the necessity to copy it again and again. As soon as the string is
*   assigned to a new variable, etc. the optimization will not work anymore.
*   To avoid any incoherence in the string contents, assigning private strings
*   to a variable changes their state to shared. Next time when such shared
*   string is modified with the index operator, a new copy of the string is
*   created. THis is a kind of optimization to perfmorm an efficient copy-on-
*   write string behavior.
*
* ARGUMENTS:
*   aString - Pointer to the string to be changed from private to shared.
*
* RETURN VALUE:
*   Returns the string passed in aString.
*
*******************************************************************************/
XString EwShareString
(
  XString           aString
);


/* Following functions are obsolete with Embedded Wizard 9.30. To avoid compiler
   errors with customer own implementation empty defines of them are provided. */
#define EwRetainString( aDest, aString ) ( *(aDest) = EwShareString( aString ))
#define EwReleaseString( aString )  
#define EwReleaseStrings( aStrings )


/*******************************************************************************
* FUNCTION:
*   EwConcatString
*
* DESCRIPTION:
*   The function EwConcatString() creates a new string large enought to store a
*   copy of aString1 and aString2. Then the function copies the content of 
*   aString1 and appends the content of the second string aStrings2.
*
*   EwConcatString() implements the Chora instant operator: 'string + string'.
*
* ARGUMENTS:
*   aString1 - The first string.
*   aString2 - The second string.
*
* RETURN VALUE:
*   Returns a string containing a copy of aString1 followed by a copy of 
*   aString2.
*
*******************************************************************************/
XString EwConcatString
( 
  XString           aString1, 
  XString           aString2 
);


/*******************************************************************************
* FUNCTION:
*   EwConcatStringChar
*
* DESCRIPTION:
*   The function EwConcatStringChar() appends the character aChar at the end of
*   the string aString and returns the result.
*
*   EwConcatStringChar() implements the Chora instant operator: 
*   'string + char'.
*
* ARGUMENTS:
*   aString - The string.
*   aChar   - The character to be appended to the string.
*
* RETURN VALUE:
*   Returns a string containing a copy of aString followed by aChar.
*
*******************************************************************************/
XString EwConcatStringChar
( 
  XString           aString, 
  XChar             aChar 
);


/*******************************************************************************
* FUNCTION:
*   EwConcatCharString
*
* DESCRIPTION:
*   The function EwConcatCharString() appends aString to the character aChar 
*   and returns the result.
*
*   EwConcatCharString() implements the Chora instant operator: 
*   'char + string'.
*
* ARGUMENTS:
*   aChar   - The character.
*   aString - The string to be apended.
*
* RETURN VALUE:
*   Returns a string containing aChar followed by a copy of aString.
*
*******************************************************************************/
XString EwConcatCharString
( 
  XChar             aChar, 
  XString           aString 
);


/*******************************************************************************
* FUNCTION:
*   EwGetStringChar
*
* DESCRIPTION:
*   The function EwGetStringChar() determinates the character stored at position 
*   aIndex in the string aString. The function returns the zero character if the
*   specified aIndex addresses a character lying beyond the string boundary.
*
*   EwGetStringChar() implements the read access to Chora instant operator: 
*   'string[aIndex]'
*
* ARGUMENTS:
*   aString - The string to get the character.
*   aIndex  - The position of the desired character in the string.
*
* RETURN VALUE:
*   Returns the character stored at position aIndex within the string aString.
*
*******************************************************************************/
XChar EwGetStringChar
( 
  register XString  aString, 
  XInt32            aIndex 
);


/*******************************************************************************
* FUNCTION:
*   EwSetStringChar
*
* DESCRIPTION:
*   The function EwSetStringChar() changes the character stored at position 
*   aIndex in the string passed in the parameter aString to a new value aChar.
*   The function throws an error if the desired character position aIndex is
*   not valid.
*
*   If the originally passed string is a marked internally as being a private
*   string copy, the operation modifies the string in-place. In turn, if the
*   string is marked as being shared (e.g. stored in multiple variables), the
*   operation creates first a copy of the original string and uses it to 
*   change the character.
*
*   EwSetStringChar() implements the write access to Chora instant operator: 
*   'string[aIndex]'
*
* ARGUMENTS:
*   aString - The string to set the character.
*   aIndex  - The position of the desired character in the string.
*
* RETURN VALUE:
*   Returns a string with the changed character. This can be the either the
*   string passed originally in the parameter aString if the string is marked
*   internally as being a private string copy or it can be a copy of the string
*   if it is marked as being shared between several variables. Therefore you
*   should always use the returned value.
*
*******************************************************************************/
XString EwSetStringChar
( 
  register XString  aString, 
  XInt32            aIndex,
  XChar             aChar 
);


/*******************************************************************************
* FUNCTION:
*   EwGetStringLength
*
* DESCRIPTION:
*   The function EwGetStringLength() returns the length of the string aString 
*   in characters. If the string is empty, the function returns 0.
*
*   EwGetStringLength() implements the Chora instant property: 'string.length'
*
* ARGUMENTS:
*   aString - The string to determinate the length of.
*
* RETURN VALUE:
*   Returns the length of the string in characters or 0 if the string is empty.
*
*******************************************************************************/
XInt32 EwGetStringLength
( 
  XString           aString 
);


/*******************************************************************************
* FUNCTION:
*   EwGetStringUpper
*
* DESCRIPTION:
*   The function EwGetStringUpper() converts the string aString to uppercase
*   and returns an altered copy of the string.
*
*   EwGetStringUpper() implements the Chora instant property: 'string.upper'
*
* ARGUMENTS:
*   aString - String to capitalize.
*
* RETURN VALUE:
*   Returns uppercase copy of the string.
*
*******************************************************************************/
XString EwGetStringUpper
( 
  XString           aString 
);


/*******************************************************************************
* FUNCTION:
*   EwGetStringLower
*
* DESCRIPTION:
*   The function EwGetStringLower() converts the string aString to lowercase
*   and returns an altered copy of the string.
*
*   EwGetStringLower() implements the Chora instant property: 'string.lower'
*
* ARGUMENTS:
*   aString - String to convert to lowercase.
*
* RETURN VALUE:
*   Returns lowercase copy of the string.
*
*******************************************************************************/
XString EwGetStringLower
( 
  XString           aString 
);


/*******************************************************************************
* FUNCTION:
*   EwStringLeft
*
* DESCRIPTION:
*   The function EwStringLeft() extracts the first (that is, leftmost) aCount 
*   characters from the string aString and returns a copy of the extracted 
*   substring. If aCount exceeds the string length, then the entire string is 
*   extracted.
*
*   EwStringLeft() implements the Chora instant method: 'string.left(aCount)'
*
* ARGUMENTS:
*   aString - The string to extract a substring.
*   aCount  - The maximum number of characters to extract from aString.
*
* RETURN VALUE:
*   Returns the extracted substring or if aCount == 0 the function returns an
*   empty string.
*
*******************************************************************************/
XString EwStringLeft
( 
  XString           aString, 
  XInt32            aCount 
);


/*******************************************************************************
* FUNCTION:
*   EwStringRight
*
* DESCRIPTION:
*   The function EwStringRight() extracts the last (that is, rightmost) aCount 
*   characters from the string aString and returns a copy of the extracted 
*   substring. If aCount exceeds the string length, then the entire string is 
*   extracted. 
*
*   EwStringRight() implements the Chora instant method: 'string.right(aCount)'
*
* ARGUMENTS:
*   aString - The string to extract a substring.
*   aCount  - The maximum number of characters to extract from aString.
*
* RETURN VALUE:
*   Returns the extracted substring or if aCount == 0 the function returns an
*   empty string.
*
*******************************************************************************/
XString EwStringRight
( 
  XString           aString, 
  XInt32            aCount 
);


/*******************************************************************************
* FUNCTION:
*   EwStringMiddle
*
* DESCRIPTION:
*   The function EwStringMiddle() extracts a substring of length aCount 
*   characters from the string aString, starting at position aIndex. The 
*   function returns a copy of the extracted substring. 
*
*   EwStringMiddle() implements the Chora instant method: 
*   'string.middle(aIndex,aCount)'
*
* ARGUMENTS:
*   aString - The string to extract a substring.
*   aIndex  - The index of the first character in the string where the extracted
*     substring begins.
*   aCount  - The maximum number of characters to extract from aString.
*
* RETURN VALUE:
*   Returns the extracted substring or if aCount == 0 the function returns an
*   empty string.
*
*******************************************************************************/
XString EwStringMiddle
( 
  XString           aString, 
  XInt32            aIndex, 
  XInt32            aCount 
);


/*******************************************************************************
* FUNCTION:
*   EwStringInsert
*
* DESCRIPTION:
*   The function EwStringInsert() inserts the substring aString2 at the given 
*   position aIndex within the string aString1. If aIndex is <= 0 zero, the 
*   insertion will occur before the entire string. If aIndex is > than the 
*   length of the string, the function will concatenate aString1 and aString2
*   together.
*
*   EwStringInsert() implements the Chora instant method: 
*   'string.insert(aString,aIndex)'
*
* ARGUMENTS:
*   aString1 - The string to insert a substring.
*   aString2 - The substring to be inserted.
*   aIndex   - The position in aString1 where the substring should be inserted.
*
* RETURN VALUE:
*   Returns a new string containing aString1 and aString2.
*
*******************************************************************************/
XString EwStringInsert
( 
  XString           aString1, 
  XString           aString2, 
  XInt32            aIndex 
);


/*******************************************************************************
* FUNCTION:
*   EwStringRemove
*
* DESCRIPTION:
*   The function EwStringRemove() removes up to aCount characters from the 
*   string aString starting with the character at position aIndex. 
*
*   EwStringRemove() implements the Chora instant method: 
*   'string.remove(aIndex,aCount)'
*
* ARGUMENTS:
*   aString - The string to remove characters.
*   aIndex  - The index of the first character to remove.
*   aCount  - The number of characters to be removed.
*
* RETURN VALUE:
*   Returns a new string containing altered aString.
*
*******************************************************************************/
XString EwStringRemove
( 
  XString           aString, 
  XInt32            aIndex, 
  XInt32            aCount 
);


/*******************************************************************************
* FUNCTION:
*   EwStringFindChar
*
* DESCRIPTION:
*   The function EwStringFindChar() searches the string aString for the first 
*   occurence of a character aChar. EwStringFindChar() starts the search 
*   operation at the position aStartIndex in the string. The function returns 
*   the index of the first character in aString1 that matches the requested 
*   character.
*
*   EwStringFindChar() implements the Chora instant method: 
*   'string.find(aChar,aStartIndex)'
*
* ARGUMENTS:
*   aString     - The string to serach with.
*   aChar       - A character to search for.
*   aStartIndex - The position of the character to begin the search.
*
* RETURN VALUE:
*   Returns the position in the string aString where the character aChar has 
*   been found or -1 if aChar has not been found.
*
*******************************************************************************/
XInt32 EwStringFindChar
( 
  XString           aString, 
  XChar             aChar, 
  XInt32            aStartIndex 
);


/*******************************************************************************
* FUNCTION:
*   EwStringFind
*
* DESCRIPTION:
*   The function EwStringFind() searches the string aString for the first 
*   occurence of a substring aString2. EwStringFind() starts the search 
*   operation at the position aStartIndex in the string. The function returns 
*   the index of the first character in aString1 that matches the requested 
*   substring.
*
*   EwStringFind() implements the Chora instant method: 
*   'string.find(aString,aStartIndex)'
*
* ARGUMENTS:
*   aString1    - The string to serach with.
*   aString2    - A substring to search for.
*   aStartIndex - The position of the character to begin the search.
*
* RETURN VALUE:
*   Returns the position in the string aString1 where the substring aString2 
*   has been found or -1 if aString2 has not been found.
*
*******************************************************************************/
XInt32 EwStringFind
( 
  XString           aString1, 
  XString           aString2, 
  XInt32            aStartIndex 
);


/*******************************************************************************
* FUNCTION:
*   EwStringParseInt32
*
* DESCRIPTION:
*   The function EwStringParseInt32() parses the string interpreting its content
*   as an integral number. The number can be prefixed by an optional +/- sign.
*   Whitespace signs lying at the begin of the string are skipped over.
*
*   The function parses the numbers according to the notation resulting from the
*   parameter aRadix. In this manner strings with binary, octal, decimal or even
*   hexadecimal notation can be read.
*
*   Generally the function tries to read as many signs as possible. If the end of
*   the string or an unexpected sign is found, the function stops and returns the
*   already read number as signed integer value. If the string is invalid (it
*   doesn't contain any expected digit or letter), the value passed in the
*   parameter aDefault is returned instead.
*
*   EwStringParseInt32() implements the Chora instant method: 
*   'string.parse_int32(aDefault,aRadix)'
*
* ARGUMENTS:
*   aString  - The string to parse its content.
*   aDefault - Value to return if the string doesn't contain a valid number.
*   aRadix   - Determines the format of the number in the string. This parameter
*     can assume values 2, 8, 10 or 16. Accordingly the string content is assumed
*     as being a number with binary, octal, decimal or hexadecimal notation.
*
* RETURN VALUE:
*   Returns the parsed number value as signed integer or the value from the
*   parameter aDefault if the string doesn't contain a valid number.
*
*******************************************************************************/
XInt32 EwStringParseInt32
( 
  XString           aString,
  XInt32            aDefault, 
  XInt32            aRadix
);


/*******************************************************************************
* FUNCTION:
*   EwStringParseUInt32
*
* DESCRIPTION:
*   The function EwStringParseUInt32() parses the string interpreting its content
*   as an integral number. The number can be prefixed by an optional +/- sign.
*   Whitespace signs lying at the begin of the string are skipped over.
*
*   The function parses the numbers according to the notation resulting from the
*   parameter aRadix. In this manner strings with binary, octal, decimal or even
*   hexadecimal notation can be read.
*
*   Generally the function tries to read as many signs as possible. If the end of
*   the string or an unexpected sign is found, the function stops and returns the
*   already read number as unsigned integer value. If the string is invalid (it
*   doesn't contain any expected digit or letter), the value passed in the 
*   parameter aDefault is returned instead.
*
*   EwStringParseUInt32() implements the Chora instant method: 
*   'string.parse_uint32(aDefault,aRadix)'
*
* ARGUMENTS:
*   aString  - The string to parse its content.
*   aDefault - Value to return if the string doesn't contain a valid number.
*   aRadix   - Determines the format of the number in the string. This parameter
*     can assume values 2, 8, 10 or 16. Accordingly the string content is assumed
*     as being a number with binary, octal, decimal or hexadecimal notation.
*
* RETURN VALUE:
*   Returns the parsed number value as unsigned integer or the value from the
*   parameter aDefault if the string doesn't contain a valid number.
*
*******************************************************************************/
XUInt32 EwStringParseUInt32
( 
  XString           aString,
  XUInt32           aDefault, 
  XInt32            aRadix
);


/*******************************************************************************
* FUNCTION:
*   EwStringParseFloat
*
* DESCRIPTION:
*   The function EwStringParseFloat() parses the string interpreting its content
*   as a floating point number. The number can be prefixed by an optional +/- 
*   sign. Whitespace signs lying at the begin of the string are skipped over. 
*
*   In the floating point notation the number can consist of an integer and/or
*   a fractional portion. The fractional portion starts with a '.'. Optionally
*   the number can be followed by an exponent portion. The exponent portion
*   starts with the e or E sign followed by an optional +/- sign and an integral
*   number. The following example shows the complete number with all portions:
*
*   "123.456e-07"
*
*   Generally the function tries to read as many signs as possible. If the end
*   of the string or an unexpected sign is found, the function stops and returns
*   the already read number as floating point value. If the string is invalid
*   (it doesn't contain any expected digit), the value passed in the parameter
*   aDefault is returned instead.
*
*   EwStringParseFloat() implements the Chora instant method: 
*   'string.parse_float(aDefault)'
*
* ARGUMENTS:
*   aString  - The string to parse its content.
*   aDefault - Value to return if the string doesn't contain a valid number.
*
* RETURN VALUE:
*   Returns the parsed number value as floating point or the value from the
*   parameter aDefault if the string doesn't contain a valid number.
*
*******************************************************************************/
XFloat EwStringParseFloat
( 
  XString           aString,
  XFloat            aDefault
);


/*******************************************************************************
* FUNCTION:
*   EwStringToAnsi
*
* DESCRIPTION:
*   The function EwStringToAnsi() converts up to aCount characters from the 
*   given 16 bit wide char string aString to an 8 bit ANSI string and stores it
*   in the memory area aDest. The resulted string is terminated by the zero
*   0x00.
*
*   Character codes greater than 255 are replaced by the aDefChar value. This
*   is necessary, because the character codes in the source string can became
*   values in range 0 .. 65535. The destination string can accomodate codes
*   in the range 0 .. 255 only.
*
*   The function returns the number of converted characters incl. the last zero
*   terminator 0x00.
*
* ARGUMENTS:
*   aString  - The source 16 bit wide char string to convert.
*   aDest    - Pointer to the destination memory, where the converted 8 bit 
*     ANSI string should be stored in. This memory area has to be at least
*     aCount bytes large.
*   aCount   - The number of characters incl. the zero terminator to convert.
*   aDefChar - Default character to replace all characters, which are > 255.
*
* RETURN VALUE:
*   Returns the number of bytes written into the aDest memory area.
*
*******************************************************************************/
XInt32 EwStringToAnsi
( 
  XString           aString, 
  char*             aDest,
  XInt32            aCount,
  char              aDefChar
);


/*******************************************************************************
* FUNCTION:
*   EwGetUtf8StringLength
*
* DESCRIPTION:
*   The function EwGetUtf8StringLength() analyzes the characters in the given
*   16 bit wide char string aString and estimates the length in bytes for a
*   corresponding UTF8 string. This value can be used to reserve memory before
*   using the function EwStringToUtf8().
*
* ARGUMENTS:
*   aString  - The source 16 bit wide char string to convert.
*
* RETURN VALUE:
*   Returns the number of bytes to store the corresponding string in UTF8 
*   format without the zero terminator sign.
*
*******************************************************************************/
XInt32 EwGetUtf8StringLength
(
  XString           aString
);


/*******************************************************************************
* FUNCTION:
*   EwStringToUtf8
*
* DESCRIPTION:
*   The function EwStringToUtf8() converts the characters from the given 16 bit
*   wide char string aString to an 8 bit UTF8 string and stores it in the memory
*   area aDest. The resulted string is terminated by the zero 0x00.
*
*   The function returns the number of bytes written in aDest incl. the last 
*   zero terminator 0x00.
*
* ARGUMENTS:
*   aString  - The source 16 bit wide char string to convert.
*   aDest    - Pointer to the destination memory, where the converted 8 bit 
*     UTF8 string should be stored in. This memory area has to be at least
*     aCount bytes large.
*   aCount   - The number of bytes within aDest available for the operation.
*
* RETURN VALUE:
*   Returns the number of bytes written into the aDest memory area incl. the 
*   zero terminator sign.
*
*******************************************************************************/
XInt32 EwStringToUtf8
( 
  XString           aString, 
  unsigned char*    aDest,
  XInt32            aCount
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfString
*
* DESCRIPTION:
*   The function EwGetVariantOfString() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple string values.
*
* RETURN VALUE:
*   Returns the determinated string value.
*
*******************************************************************************/
XString EwGetVariantOfString
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetVariantOfChar
*
* DESCRIPTION:
*   The function EwGetVariantOfChar() will be called to determinate a value
*   of a multilingual/multivariant constant depending on the currently selected
*   language and the styles set.
*
*   If the currently selected language could not be found in the multilingual
*   constant, the function returns the value corresponding to the default 
*   language (LangId == 0). In case of a multivariant constant, the function
*   evaluates the variants in order to find one, which fits the styles currently
*   active in the styles set.
*
* ARGUMENTS:
*   aVariants - A pointer to the constant containing multiple char values.
*
* RETURN VALUE:
*   Returns the determinated char value.
*
*******************************************************************************/
XChar EwGetVariantOfChar
( 
  const XVariant* aConstant
);


/*******************************************************************************
* FUNCTION:
*   EwGetCharUpper
*
* DESCRIPTION:
*   The function EwGetCharUpper() converts the character aChar to uppercase.
*
*   EwGetCharUpper() implements the Chora instant property: 'char.upper'
*
* ARGUMENTS:
*   aChar - Character to convert to uppercase.
*
* RETURN VALUE:
*   Returns uppercase copy of the character.
*
*******************************************************************************/
XChar EwGetCharUpper
( 
  XChar             aChar 
);


/*******************************************************************************
* FUNCTION:
*   EwGetCharLower
*
* DESCRIPTION:
*   The function EwGetCharLower() converts the character aChar to lowercase.
*
*   EwGetCharLower() implements the Chora instant property: 'char.lower'
*
* ARGUMENTS:
*   aChar - Character to convert to lowercase.
*
* RETURN VALUE:
*   Returns lowercase copy of the character.
*
*******************************************************************************/
XChar EwGetCharLower
( 
  XChar             aChar 
);


/*******************************************************************************
* TYPE:
*   XTicks
*
* DESCRIPTION:
*   XTicks is used to store the timer ticks as a 64 bit number. The usage of
*   64 bit arithmetic allows very, very long timer expirations.
*   
* ELEMENTS:
*   Lo - Lower 32 bit of the number.
*   Hi - Upper 32 bit of the number.
*
*******************************************************************************/
typedef struct
{
  XUInt32         Lo;
  XUInt32         Hi;
} XTicks;


/*******************************************************************************
* TYPE:
*   XTimerProc
*
* DESCRIPTION:
*   XTimerProc is a prototype for an application routine to execute when the
*   timer expires.
*   
* ARGUMENTS:
*   aArg - An optional argument to pass to the timer procedure.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
typedef void (*XTimerProc)
( 
  XHandle           aArg 
);


/*******************************************************************************
* TYPE:
*   XTimer
*
* DESCRIPTION:
*   The type XTimer defines a structure to store a timer. XTimer structures will 
*   be stored within a global timers list.
*   
* ELEMENTS:
*   Next         - Points to the next timer within the global timers list.
*   Proc         - Pointer to an user defined routine to execute when the timer
*     expires. These routines will be called in context of the function
*     EwProcessTimers() only. It avoids any race conditions.
*   Arg          - An optional argument to pass to the timer procedure Proc.
*   Enabled      - If not zero, the timer is running.
*   Ticks        - 64 Bit delay to the next timer expiriation in milliseconds.
*     EwProcessTimers() compares this value with the internal clock and
*     decides whether the timer is expired or not.
*   InitialTime  - The number of milliseconds for the first time expiration.
*   RepeatTime   - The number of milliseconds between two timer expirations.
*
*******************************************************************************/
typedef struct _XTimer
{
  struct _XTimer* Next;
  XTimerProc      Proc;
  XHandle         Arg;
  XBool           Enabled;
  XTicks          Ticks;
  XInt32          InitialTime;
  XInt32          RepeatTime;
} XTimer;


/*******************************************************************************
* FUNCTION:
*   EwCreateTimer
*
* DESCRIPTION:
*   The function EwCreateTimer() initializes an new timer and inserts it into 
*   the global timer list Timers.
*   
* ARGUMENTS:
*   aProc - Entry point of an user defined function. The function will be each
*     time the timer is expired. The function will be called in context of the
*     function EwProcessTimers() only.
*   aArg  - Optional 32 bit argument to pass in the timer procedure aProc.
*
* RETURN VALUE:
*   Returns a pointer to a new timer.
*
*******************************************************************************/
XTimer* EwCreateTimer
( 
  XTimerProc        aProc, 
  XHandle           aArg 
);


/*******************************************************************************
* FUNCTION:
*   EwDestroyTimer
*
* DESCRIPTION:
*   The function EwDestroyTimer() removes the timer from the timers list and
*   frees the memory reserved for the timer.
*   
* ARGUMENTS:
*   aTimer - Pointer to the timer to destroy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDestroyTimer
( 
  XTimer*           aTimer 
);


/*******************************************************************************
* FUNCTION:
*   EwStartTimer
*
* DESCRIPTION:
*   The function EwStartTimer() starts the timer aTimer with the given number
*   of milliseconds aDelay. If the timer is already started, the function
*   reinitializes the timer with a new expiration time.
*   
* ARGUMENTS:
*   aTimer       - Pointer to the timer to start.
*   aInitialTime - The number of milliseconds for the first time expiration.
*   aRepeatTime  - The number of milliseconds between two timer expirations.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStartTimer
( 
  XTimer*           aTimer, 
  XInt32            aInitialTime, 
  XInt32            aRepeatTime
);


/*******************************************************************************
* FUNCTION:
*   EwResetTimer
*
* DESCRIPTION:
*   The function EwResetTimer() stops the given timer aTimer.
*   
* ARGUMENTS:
*   aTimer - Pointer to the timer to be stopped.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwResetTimer
( 
  XTimer*           aTimer 
);


/*******************************************************************************
* FUNCTION:
*   EwProcessTimers
*
* DESCRIPTION:
*   The function EwProcessTimers() should be called in order to process all
*   pending timer events. EwProcessTimers() traverses the global timers list
*   and invokes all timer procedures assigned to timers which are expired.
*   After all pending timer events are evaluated, EwProcessTimers() clears
*   all pending flags.
*
*   The function returns a value != 0, if at least one timer has expired.
*   
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If at least one timer is expired, the function returns a value != 0. 
*   If no timers are expired, 0 is returned.
*
*******************************************************************************/
int EwProcessTimers
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwNextTimerExpiration
*
* DESCRIPTION:
*   The function EwNextTimerExpiration() evaluates the list of pending timers
*   in order to find out the timer which should expire next time. The function
*   returns the number of miliseconds till the expiration of the found timer.
*
*   Please note: Because of the multitasking environment, the returned delay
*   value can become very inprecise! For this reason you should never use the 
*   determined number of miliseconds as a parameter for a 'sleep()' or similar 
*   function. This can cause the timers to expire very unprecise.
*   
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the delay in milliseconds for the next timer expiration.
*
*******************************************************************************/
int EwNextTimerExpiration
( 
  void
);


/*******************************************************************************
* FUNCTION:
*   EwDecompress
*
* DESCRIPTION:
*   The function EwDecompress() implements a simple algorithm used for the 
*   decompression of resources, constants, etc. The function decompresses the 
*   entire compressed data block aData and stores the result in aDest memory.
*
*   When decompressing bitmap contents, the arguments aWidth and aPitch provide
*   the decompressor with the information about the memory layout of the bitmap.
*   This is also useful, when decompressing non continuous memory contents.
*
* ARGUMENTS:
*   aData   - Pointer to compressed data.
*   aDest   - Pointer to the memory where the decompressed data should be stored
*     in.
*   aWidth  - In case of bitmap decompression, the width of a single pixel row
*     in bytes. If the destination is a continuous memory area, aWidth should be
*     0.
*   aPitch  - In case of bitmap decompression, the distance between two pixel
*     rows. If the destination is a continuous memory area, aPitch should be 0.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDecompress
( 
  const unsigned int*  aData,
  unsigned char*       aDest,
  int                  aWidth,
  int                  aPitch
);


/*******************************************************************************
* FUNCTION:
*   EwAdaptByteOrder2
*
* DESCRIPTION:
*   The function EwAdaptByteOrder2() provides a re-order function to convert
*   an array of short words from the default little-endian format to the target
*   CPU dependent endian format.
*
*   If the endian format of the target CPU is already little, no modification
*   will take place here and the function returns immediatelly.
*
* ARGUMENTS:
*   aData  - Pointer to the array containing short words.
*   aCount - Number of words within the array to convert.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwAdaptByteOrder2
(
  unsigned short*   aData,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwAdaptByteOrder4
*
* DESCRIPTION:
*   The function EwAdaptByteOrder4() provides a re-order function to convert
*   an array of long words from the default little-endian format to the target
*   CPU dependent endian format.
*
*   If the endian format of the target CPU is already little, no modification
*   will take place here and the function returns immediatelly.
*
* ARGUMENTS:
*   aData  - Pointer to the array containing long words.
*   aCount - Number of words within the array to convert.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwAdaptByteOrder4
(
  unsigned int*     aData,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwFormatUIntToAnsiString
*
* DESCRIPTION:
*   The function EwFormatUIntToAnsiString() converts the given unsigned number
*   aValue in an Ansi string and stores the results in the buffer aBuf. The
*   function adds leading zeros '0' until the resulted string has reached the
*   length given in the aCount argument. If the paramneter aSign is != 0, an
*   additional '+' sign will prefix the formatted number.
*
* ARGUMENTS:
*   aBuf   - Buffer to store the resulting string.
*   aValue - Unsigned 32 bit value to be converted to string.
*   aCount - Desired length of the resulted string.
*   aRadix - Controls the format of the resulting string. This parameter can
*     assume values 2, 8, 10 or 16. Accordingly the number is converted in a
*     binary, octal, decimal or hexadecimal notation.
*   aUpper - If != 0, use upper case signs for hex digits.
*   aSign  - If != 0, instructs the function to prefix the number with the 
*     '+' sign.
*
* RETURN VALUE:
*   Returns the length of the resulting string. The string within aBuf is not
*   zero terminated.
*
*******************************************************************************/
int EwFormatUIntToAnsiString
( 
  char*             aBuf,
  unsigned long     aValue, 
  int               aCount,
  int               aRadix,
  int               aUpper,
  int               aSign
);


/*******************************************************************************
* FUNCTION:
*   EwFormatIntToAnsiString
*
* DESCRIPTION:
*   The function EwFormatIntToAnsiString() converts the given signed number 
*   aValue in an Ansi string and stores the results in the buffer aBuf. The
*   function adds leading zeros '0' until the resulted string has reached the
*   length given in the aCount argument. If the number is negative or aSign is
*   != 0, the function prefix the formatted number with the '+' or '-' minus
*   sign.
*
* ARGUMENTS:
*   aValue - Signed 32 bit value to be converted to string.
*   aCount - Desired length of the resulted string.
*   aRadix - Controls the format of the resulting string. This parameter can
*     assume values 2, 8, 10 or 16. Accordingly the number is converted in a
*     binary, octal, decimal or hexadecimal notation.
*   aUpper - If != 0, use upper case signs for hex digits.
*   aSign  - If != 0, instructs the function to prefix the number with the 
*     '+' sign even if the number is positive.
*
* RETURN VALUE:
*   Returns the length of the resulting string. The string within aBuf is not
*   zero terminated.
*
*******************************************************************************/
int EwFormatIntToAnsiString
( 
  char*             aBuf,
  long              aValue, 
  int               aCount,
  int               aRadix,
  int               aUpper,
  int               aSign
);


/*******************************************************************************
* FUNCTION:
*   EwFormatFloatToAnsiString
*
* DESCRIPTION:
*   The function EwFormatFloatToAnsiString() converts the given floating point
*   number aValue in an Ansi string and stores the results in the buffer aBuf.
*   The function adds leading zeros '0' until the resulted string has reached
*   the length given in the aCount argument. If the number is negative or aSign
*   is != 0, the function prefixes the formatted number with the '+' or '-' 
*   minus sign.
*
* ARGUMENTS:
*   aValue     - The floating point value to be converted in a string.
*   aCount     - Minimum number of characters to store in the resulted string 
*     output.
*   aPrecision - The aPrecision argument specifies the exact number of digits
*     after the decimal point. If this parameter is < 0, the value specifies
*     the maximum number of digits. Any final '0' (zero) signs are removed
*     in this case.
*
* RETURN VALUE:
*   Returns the length of the resulting string. The string within aBuf is not
*   zero terminated. If aValue does not contain a valid number, the function
*   returns 0 without storing any data in aBuf.
*
*******************************************************************************/
int EwFormatFloatToAnsiString
( 
  char*             aBuf,
  float             aValue, 
  int               aCount, 
  int               aPrecision,
  int               aSign
);


/*******************************************************************************
* FUNCTION:
*   EwTrace
*
* DESCRIPTION:
*   The function EwTrace() works similary to the printf() function. EwTrace()
*   expects a format string and one or more additional arguments. The format
*   string contains escape sequences, one for each argument. These sequences
*   describe how the additional arguments should be interpreted and printed out 
*   on the debug console. 
*
*   Every escape sequence starts with a '%' percent sign followed by a single
*   sign as a command.
*
*   EwTrace() will be called from the automatic generated 'C' code in response
*   to Choras 'trace' statement.
*
* ARGUMENTS:
*   aFormat - Contains a zero-terminated string with escape sequences. The 
*     following escape sequences are supported:
*       %i - print a signed integer value (int8, int16, int32)
*       %u - print an unsigned integer value (uint8, uint16, uint32)
*       %b - print a boolean value (bool)
*       %c - print a character (char)
*       %s - print a string value (string)
*       %f - print a floating point value (float)
*       %o - print a color value (color)
*       %l - print a language id
*       %g - print a styles set value (styles)
*       %e - print an enumeration value (enum)
*       %t - print a set value (set)
*       %p - print a point value (point)
*       %r - print a rectangle value (rect)
*       %^ - print a value of a reference to a property (ref to)
*       %* - print a pointer to an object and the class of the object
*       %$ - print a class
*       %0 - print 'null'
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwTrace
( 
  const char*       aFormat, 
  ... 
);


/*******************************************************************************
* FUNCTION:
*   EwThrow
*
* DESCRIPTION:
*   The function EwThrow() implements the Chora 'throw' statement. The function
*   will be called by the Run Time Environment if a Chora exception has been
*   thrown.
*
*   Internally, the function prints the message and calls EwPanic() in order
*   to stop or restart the system.
*
* ARGUMENTS:
*   aMessage - Contains a failure description.
*
* RETURN VALUE:
*   EwThrow() never returns.
*
*******************************************************************************/
void EwThrow
( 
  const XChar*      aMessage
);


/*******************************************************************************
* FUNCTION:
*   EwError
*   EwErrorS
*   EwErrorPD
*   EwErrorDD
*
* DESCRIPTION:
*   These functions are intended to format and report fatal runtime error
*   messages. Depending on the version of the used function, the reported
*   message will additionally include following information:
*
*     ErrorS()  - ANSI C string enclosed between two ""
*     ErrorPD() - pointer in hex-notation and signed decimal number
*     ErrorDD() - two signed decimal numbers
*
*   The functions report the message by calling the function EwPrint().
*
* ARGUMENTS:
*   aCode           - Number identifying the error.
*   aString         - Pointer to an ANSI C, zero terminated string.
*   aPointer        - Pointer to display in hex notation.
*   aSignedDecimal,
*   aSignedDecimal1,
*   aSignedDecimal2 - Value to display in decimal notation.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwError
(
  int               aCode
);

void EwErrorS
(
  int               aCode,
  const char*       aString
);

void EwErrorPD
(
  int               aCode,
  const void*       aPointer,
  int               aSignedDecimal
);

void EwErrorDD
(
  int               aCode,
  int               aSignedDecimal1,
  int               aSignedDecimal2
);


/*******************************************************************************
* FUNCTION:
*   EwPrint
*
* DESCRIPTION:
*   The function EwPrint() prints formatted output to the debug console. This
*   function works similary to the ANSI 'C' printf() function. However, only
*   following escape sequences are supported:
*
*     %d, %u, %p, %f, %s, %X, %x, %c
*
*   The escape sequences %d, %u, %X, %x can additionally be prefixed by a sign
*   and number specifying the desired length of the resulting string:
*
*     %[+][0][len]d
*     %[+][0][len]u
*     %[+][0][len]X
*     %[+][0][len]x
*
*   The escape sequence %f can additionally be prefixed by a sign, the number
*   specifying the desired length of the resulting string and a second number
*   for the count of digits following the decomal sign:
*
*     %[+][0][len][.prec]f
*
*   For the escape sequence %s it is possible to specify the desired length to
*   fill with space signs as well as the max. width when to truncate the 
*   string. If the sequence starts with '-' sign, the text is left aligned:
*
*     %[len][.max-len]s
*     %-[len][.max-len]s
*
*   Instead of specifying [len], [prec] and [max-len] as literals within the
*   string, it is possible the use the '*' (asterix) sign as instruction to
*   get the corresponding value from the argzments following aFormat. For
*   example:
*
*    %+0*.*f
*
*   Besides the above escape sequences known from printf(), following two are
*   also supported:
*
*     %S - prints the content of a 16-bit zero terminated string.
*     %C - prints a 16-bit character.
*
* ARGUMENTS:
*   aFormat - Zero-terminated string containing the message and the above
*     explained escape sequences.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPrint
( 
  const char*       aFormat, 
  ... 
);


/*******************************************************************************
* FUNCTION:
*   EwPrintProfilerStatistic
*
* DESCRIPTION:
*   The function EwPrintProfilerStatistic() evaluates the list of existing
*   objects and prints out a statistic information with the amount of memory 
*   allocated by objects, strings and resources. Additionally, the current
*   total memory allocation and the maximum peak value is reported.
*   If the argument aDetailed is set to non-zero, a detailed object report is 
*   printed with all existing objects and their classes.
*
* ARGUMENTS:
*   aDetailed - Flag to switch on the detailed report.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwPrintProfilerStatistic
( 
  int              aDetailed
);


/******************************************************************************
* TYPE: 
*   XPerfCounter
*
* DESCRIPTION:
*   The type definition XPerfCounter represents an instance of the performance
*   counter. These counters exist for performance investigation and debugging
*   purpose only. In the real release application no performance counters are
*   used.
*
*   To create an instance of the performance counter the macro EW_PERF_COUNTER()
*   is available.
*
* ELEMENTS:
*   Next            - Pointer to the next performance counter within the global
*     table of performance counters.
*   Name            - The identification name of the counter. This name will
*     appear in the outputs of the function EwPrintPerfCounters().
*   Group           - The identification name of the group this counter belongs
*     to. This name will appear in the outputs of the function 
*     EwPrintPerfCounters() and is used additionally to perform time 
*     calculations for the entire group of counters this counter belongs to.
*   Recursions      - Counts the recursive EwStartPerfCounter() invocations.
*   Calls           - Counts each EwStartPerfCounter() invocation.
*   StartTime1      - Absolute time value at the beginning of the time measuring
*     expressed in seconds.
*   StartTime2      - Absolute time value at the beginning of the time measuring
*     expressed in microseconds.
*   TotalTime       - Total time collected by the performance counter expressed
*     in microseconds.
*   MinTime         - The minimum time value measured by the performance counter
*     expressed in microseconds.
*   MaxTime         - The maximum time value measured by the performance counter
*     expressed in microseconds.
*   GroupFirst      - Pointer to the first counter of the corresponding group.
*   GroupRecursions - Recursions counter for the group.
*   GroupCalls      - Calls ounter for the group.
*   GroupTotalTime  - Total time collected for the group expressed in 
*     microseconds.
*
******************************************************************************/
typedef struct _XPerfCounter
{
  struct _XPerfCounter* Next;
  const char*           Name;
  const char*           Group;
  unsigned long         Recursions;
  unsigned long         Calls;
  unsigned long         StartTime1;
  unsigned long         StartTime2;
  unsigned long         TotalTime;
  unsigned long         MinTime;
  unsigned long         MaxTime;
  struct _XPerfCounter* GroupFirst;
  unsigned long         GroupRecursions;
  unsigned long         GroupCalls;
  unsigned long         GroupTotalTime;
} XPerfCounter;


/*******************************************************************************
* MACRO:
*   EW_PERF_COUNTER
*   EW_INSTRUMENT_FUNC
*   EW_INSTRUMENT_VOID_FUNC
*
* DESCRIPTION:
*   The following macros serve for the performance investigation and debugging
*   only. EW_PERF_COUNTER defines a new instance of a performance counter. This
*   counter can then be used to measure performance. See following functions:
*   EwAddPerfCounter(), EwStartPerfCounter() and EwStopPerfCounter().
*
*   EW_INSTRUMENT_FUNC and EW_INSTRUMENT_VOID_FUNC implement wrapper functions
*   with their aim the performance of a function can be evaluated.
*
* ARGUMENTS:
*   aName       - Unique name of the performance counter.
*   aGroup      - The group name, the counter belongs to.
*   aFuncName   - Origin name of the function to wrap. 
*   aReturntype - The return type of the function to wrap.
*   aArgsDecl   - The declarations list of function parameters.
*   aArgs       - The list of parameters in the function invocation.
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  #define EW_PERF_COUNTER( aName, aGroup )                                     \
    XPerfCounter _PC_##aName =                                                 \
    {                                                                          \
      0,                                                                       \
      #aName,                                                                  \
      #aGroup,                                                                 \
      0,                                                                       \
      0,                                                                       \
      0,                                                                       \
      0,                                                                       \
      0,                                                                       \
      0xFFFFFFFF,                                                              \
      0,                                                                       \
      0,                                                                       \
      0,                                                                       \
      0,                                                                       \
      0                                                                        \
    };

  #define EW_INSTRUMENT_FUNC( aFuncName, aReturnType, aArgsDecl, aArgs )       \
    aReturnType aFuncName aArgsDecl                                            \
    {                                                                          \
      aReturnType result;                                                      \
      EwStartPerfCounter( aFuncName );                                         \
      result = aFuncName##_ aArgs ;                                            \
      EwStopPerfCounter ( aFuncName );                                         \
      return result;                                                           \
    }

  #define EW_INSTRUMENT_VOID_FUNC( aFuncName, aArgsDecl, aArgs )               \
    void aFuncName aArgsDecl                                                   \
    {                                                                          \
      EwStartPerfCounter( aFuncName );                                         \
      aFuncName##_ aArgs ;                                                     \
      EwStopPerfCounter ( aFuncName );                                         \
    }
#else
  #define EW_PERF_COUNTER( aName, aGroup )
  #define EW_INSTRUMENT_FUNC( aFuncName, aReturnType, aArgsDecl, aArgs )
  #define EW_INSTRUMENT_VOID_FUNC( aFuncName, aArgsDecl, aArgs )
#endif


/*******************************************************************************
* FUNCTION:
*   EwAddPerfCounter
*
* DESCRIPTION:
*   The function EwAddPerfCounter() adds the given performance counter to the
*   global list of counters. In this manner the values collected by the counter
*   can be printed out in the EwPrintPerfCounters() function.
*
*   Once added there is no way to remove the counter from the list. This feature
*   exists for performance investigation and debugging purpose only. In the real
*   release application no performance counters should be used.
*
* ARGUMENTS:
*   aPerfCounter - Pointer to the structure describing the performance counter
*     to add.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwAddPerfCounter
  (
    XPerfCounter*    aPerfCounter
  );

  #define EwAddPerfCounter( aPerfCounter )                                     \
    EwAddPerfCounter( &_PC_##aPerfCounter )
#else
  #define EwAddPerfCounter( aPerfCounter )
#endif


/*******************************************************************************
* FUNCTION:
*   EwStartPerfCounter
*
* DESCRIPTION:
*   The function EwStartPerfCounter() starts the time measuring for the given
*   performance counter. Each call of this function is counted by the counter.
*
*   The time measuring runs till the function EwStopPerfCounter() is called for
*   this performance counter. In case of recursive calls with the same counter
*   the measuring starts with the first EwStartPerfCounter() and ends with the
*   last EwStopPerfCounter().
*
* ARGUMENTS:
*   aPerfCounter - Pointer to the structure describing the performance counter
*     to start the time measuring.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwStartPerfCounter
  (
    XPerfCounter*    aPerfCounter
  );
  #define EwStartPerfCounter( aPerfCounter )                                   \
    EwStartPerfCounter( &_PC_##aPerfCounter )
#else
  #define EwStartPerfCounter( aPerfCounter )
#endif


/*******************************************************************************
* FUNCTION:
*   EwStopPerfCounter
*
* DESCRIPTION:
*   The function EwStopPerfCounter() stops the time measuring for the given
*   performance counter. From the measured time values the function stores the
*   minimum and the maximum as well as the total time the performance counter 
*   has collected.
*
*   The time measuring starts with the function call EwStopPerfCounter(). In
*   case of recursive calls with the same counter the measuring stops with the
*   last EwStopPerfCounter() invocation.
*
* ARGUMENTS:
*   aPerfCounter - Pointer to the structure describing the performance counter
*     to stop the time measuring.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwStopPerfCounter
  (
    XPerfCounter*    aPerfCounter
  );
  #define EwStopPerfCounter( aPerfCounter )                                    \
    EwStopPerfCounter( &_PC_##aPerfCounter )
#else
  #define EwStopPerfCounter( aPerfCounter )
#endif


/*******************************************************************************
* FUNCTION:
*   EwResetPerfCounters
*
* DESCRIPTION:
*   The function EwResetPerfCounters() resets all performance counters so new
*   performance test can run. This function is usually called immediately before
*   a performance test begins.
*
*   After this function call EwStartPerfCounter() and EwStopPerfCounter() are
*   used to measure and collect time values. The collected values can then be
*   printed to the console by using the function EwPrintPerfCounters().
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwResetPerfCounters
  (
    void
  );
#else
  #define EwResetPerfCounters()
#endif


/*******************************************************************************
* FUNCTION:
*   EwPrintPerfCounters
*
* DESCRIPTION:
*   The function EwPrintPerfCounters() prints the time values measured and
*   collected in all performance counters.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwPrintPerfCounters
  (
    void
  );
#else
  #define EwPrintPerfCounters()
#endif


/*******************************************************************************
* FUNCTION:
*   EwConsoleOutput
*
* DESCRIPTION:
*   The function EwConsoleOutput() prints the message to the debug console. 
*   This function should be implemented by the programer in order to output 
*   trace messages on the debug console, etc. 
*
* ARGUMENTS:
*   aMessage - Zero-terminated string containing the message to output.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwConsoleOutput
( 
  const char*       aMessage
);


/*******************************************************************************
* FUNCTION:
*   EwPanic
*
* DESCRIPTION:
*   The function EwPanic() will be called by the Run Time Environment if a fatal
*   system failure is detected.
*
*   This function can be adapted to decide what to do when a system failure 
*   is occured. Generally EwPanic() should log the system failure and restart 
*   the system.
*
*   For debugging purpose EwPanic() can run into a for-ever-loop or it can 
*   cause a monitor breakpoint, etc.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   EwPanic() will never return.
*
*******************************************************************************/
void EwPanic
( 
  void
);


/*******************************************************************************
* FUNCTION:
*   EwSaveRegister
*
* DESCRIPTION:
*   The function EwSaveRegister() has the job to copy all general purpose CPU
*   register to the memory area specified in the parameter aMemory.
*
* ARGUMENTS:
*   aBuffer - Pointer to a memory area where to save the register contents.
*     The capacity of the memory area is limited to 32 registers. This means
*     on a 32-bit CPU it is 128 bytes large. On a 64-bit CPU it is 256 bytes
*     large.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwSaveRegister
( 
  void*             aBuffer
);


/*******************************************************************************
* FUNCTION:
*   EwGetTicks
*
* DESCRIPTION:
*   The function EwGetTicks() should return the number of milliseconds that have
*   been elapsed since the system was started. It is limited to the resolution 
*   of the system timer. The ticks are used as base for EmWi timers.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Number of milliseconds since the system was started. 
*
*******************************************************************************/
unsigned long EwGetTicks
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwGetTime
*
* DESCRIPTION:
*   The function EwGetTime() should return the current time as the number of
*   seconds elapsed since midnight 1.1.1970. If possible the real time clock 
*   should be queried in order to determinate the current time. This function 
*   is used by the Mosaic class Core::Time and should be adapted to the real
*   time clock or to the system timer.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns the number of seconds elapsed since midnight 1.1.1970. 
*
*******************************************************************************/
unsigned long EwGetTime
( 
  void 
);


/*******************************************************************************
* FUNCTION:
*   EwGetPerfCounter
*
* DESCRIPTION:
*   The function EwGetPerfCounter() should return the current time as number of
*   seconds and microseconds. The reference time point is nonrelevant.
*
* ARGUMENTS:
*   aSeconds         - Pointer to a variable where the current time is stored.
*   aMicroseconds    - Pointer to a variable where the current time is stored.
*   aCPUMicroseconds - Pointer to a variable where the real CPU usage is stored.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
#ifdef EW_PRINT_PERF_COUNTERS
  void EwGetPerfCounter
  ( 
    unsigned long*   aSeconds,
    unsigned long*   aMicroseconds,
    unsigned long*   aCPUMicroseconds
  );
#else
  #define EwGetPerfCounter( aSeconds, aMicroseconds, aCPUMicroseconds )
#endif


/*******************************************************************************
* FUNCTION:
*   EwZero
*
* DESCRIPTION:
*   The function EwZero() provides a plattform independent version of the 'C'
*   standard library function memset( aDstPtr, 0, aCount ).
*
*   This function fills the given memory area with the constant value zero.
*
* ARGUMENTS:
*   aDstPtr - Pointer to memory area to fill with zero.
*   aCount  - Number of bytes to fill with zero.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwZero
( 
  void*             aDstPtr,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwFill
*
* DESCRIPTION:
*   The function EwFill() provides a plattform independent version of the 'C'
*   standard library function memset( aDstPtr, aValue, aCount ).
*
*   This function fills the given memory area with the constant value aValue.
*
* ARGUMENTS:
*   aDstPtr - Pointer to memory area to fill with the value.
*   aValue  - Byte value to fill the memory.
*   aCount  - Number of bytes to fill with the value.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFill
( 
  void*             aDstPtr,
  unsigned char     aValue,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwMove
*
* DESCRIPTION:
*   The function EwMove() provides a plattform independent version of the 'C'
*   standard library function memove( aDstPtr, aSrcPtr, aCount ).
*
*   This function copies the given source memory area into the destination area.
*
* ARGUMENTS:
*   aDstPtr - Pointer to the destination memory area where to write the data.
*   aSrcPtr - Pointer to the source memory to read.
*   aCount  - Number of bytes to copy from aSrcPtr to aDstPtr.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwMove
( 
  void*             aDstPtr,
  const void*       aSrcPtr,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwCopy
*
* DESCRIPTION:
*   The function EwCopy() provides a plattform independent version of the 'C'
*   standard library function memcpy( aDstPtr, aSrcPtr, aCount ).
*
*   This function copies the given source memory area into the destination area.
*
* ARGUMENTS:
*   aDstPtr - Pointer to the destination memory area where to write the data.
*   aSrcPtr - Pointer to the source memory to read.
*   aCount  - Number of bytes to copy from aSrcPtr to aDstPtr.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwCopy
( 
  void*             aDstPtr,
  const void*       aSrcPtr,
  int               aCount
);


/*******************************************************************************
* FUNCTION:
*   EwStrCpy
*
* DESCRIPTION:
*   The function EwStrCpy() provides a plattform independent version of the 'C'
*   standard library function strcpy( aDst, aSrc ).
*
*   This function copies a zero terminated ASCII string from the source to the
*   destination memory.
*
* ARGUMENTS:
*   aDst - Pointer to the destination memory area where to write the string.
*   aSrc - Pointer to the source memory with the string to copy.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStrCpy
( 
  char*             aDst,
  const char*       aSrc
);


/*******************************************************************************
* FUNCTION:
*   EwStrCat
*
* DESCRIPTION:
*   The function EwStrCat() provides a plattform independent version of the 'C'
*   standard library function strcat( aDst, aSrc ).
*
*   This function appends the zero terminated ASCII string from the source at
*   the end of the destination string.
*
* ARGUMENTS:
*   aDst - Pointer to the destination memory area where to write the string.
*   aSrc - Pointer to the source memory with the string to append.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwStrCat
( 
  char*             aDst,
  const char*       aSrc
);


/*******************************************************************************
* FUNCTION:
*   EwStrCmp
*
* DESCRIPTION:
*   The function EwStrCmp() provides a plattform independent version of the 'C'
*   standard library function strcmp( aStr1, aStr2 ).
*
*   This function compares the both strings and returns:
*     value <  0 if aStr1 is less than aStr2
*     value == 0 if aStr1 is equal aStr2
*     value >  0 if aStr1 is greater than aStr2
*
* ARGUMENTS:
*   aStr1,
*   aStr2 - Pointers to the both strings to compare.
*
* RETURN VALUE:
*   Returns a value indicating the lexicographic relation of the both strings.
*
*******************************************************************************/
int EwStrCmp
( 
  const char*       aStr1,
  const char*       aStr2
);


/*******************************************************************************
* FUNCTION:
*   EwStrLen
*
* DESCRIPTION:
*   The function EwStrLen() provides a plattform independent version of the 'C'
*   standard library function strlen( aStr ).
*
*   This function returns the length of the string in bytes.
*
* ARGUMENTS:
*   aStr - Pointer to the string to obtain its length.
*
* RETURN VALUE:
*   Returns a length of the string.
*
*******************************************************************************/
int EwStrLen
( 
  const char*       aStr
);


/*******************************************************************************
* FUNCTION:
*   EwInitHeap
*
* DESCRIPTION:
*   The function EwInitHeap() initializes the heap manager. Thereupon memory
*   pools can be added by using the function EwAddHeapMemoryPool(). If there
*   was heap manager initialized previously, the associated information is
*   discarded with this invocation.
*
* ARGUMENTS:
*   aLargeObjectSize - Determines the size of objects considered as large and
*     long-lasting. Such objects are allocated from the end of the list of free
*     memory blocks. All other objects are allocated from the begin of the list.
*     This reduces the probability for heap fragmentation. If this parameter is
*     <= 0 a default value for the threshold setting is assumed.
*
* RETURN VALUE:
*   Returns != 0 if successful.
*
*******************************************************************************/
int EwInitHeap
(
  int                          aLargeObjectSize
);


/*******************************************************************************
* FUNCTION:
*   EwDoneHeap
*
* DESCRIPTION:
*   The function EwDoneHeap() deinitializes the heap manager.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDoneHeap
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwAddHeapMemoryPool
*
* DESCRIPTION:
*   The function EwAddHeapMemoryPool() adds to the heap manager a new memory 
*   area. From now the memory is used by subsequent EwAllocHeapBlock() function
*   invocations. Once added the memory area can't be removed again without 
*   reinitialization of the entire heap manager (see EwInitHeap()).
*
* ARGUMENTS:
*   aAddress - The start address of the memory area.
*   aSize    - The size of the memory area in bytes.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwAddHeapMemoryPool
(
  void*                        aAddress,
  long                         aSize
);


/*******************************************************************************
* FUNCTION:
*   EwGetHeapInfo
*
* DESCRIPTION:
*   The function EwGetHeapInfo() returns few information reflecting the actuall
*   state of the heap manager. As such it is useful for debugging purpose and
*   tests. The functions copies the values into variables referred by the
*   function parameters. If a parameter is 0 (zero), the value is ignored.
*
* ARGUMENTS:
*   aNoOfMemoryPools   - Receives the number of managed memory pools. Memory
*     pools are added to the heap by using the function EwAddHeapMemoryPool().
*   aTotalSize         - Receives the size in bytes of the entire memory (used
*     and free).
*   aFreeSize          - Receives the size in bytes of the free memory.
*   aNoOfUsedBlocks    - Receives the number of allocated memory blocks.
*   aNoOfFreeBlocks    - Receives the number of free blocks.
*   aSmallestFreeBlock - Receives the size in bytes of the smallest free block.
*   aLargestFreeBlock  - Receives the size in bytes of the largest free block.
*   aSmallAllocCounter - Receives the total number of performed alloc operations
*     with small blocks.
*   aLargeAllocCounter - Receives the total number of performed alloc operations
*     with large blocks.
*   aFreeCounter       - Receives the total number of performed free operations.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwGetHeapInfo
(
  long*                        aNoOfMemoryPools,
  long*                        aTotalSize,
  long*                        aFreeSize,
  long*                        aNoOfUsedBlocks,
  long*                        aNoOfFreeBlocks,
  long*                        aSmallestFreeBlock,
  long*                        aLargestFreeBlock,
  unsigned long*               aSmallAllocCounter,
  unsigned long*               aLargeAllocCounter,
  unsigned long*               aFreeCounter
);


/*******************************************************************************
* FUNCTION:
*   EwDumpHeap
*
* DESCRIPTION:
*   The function EwDumpHeap() exists for debugging purpose. It evaluates the
*   contents of all memory pools managed actually be the heap manager and
*   prints the associated information as well as the existing blocks.
*
* ARGUMENTS:
*   aDetailed - If != 0, the functions prints a list of all memory blocks.
*     If == 0, only heap overview is printed.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwDumpHeap
(
  int                          aDetailed
);


/*******************************************************************************
* FUNCTION:
*   EwVerifyHeap
*
* DESCRIPTION:
*   The function EwVerifyHeap() exists for debugging purpose. It evaluates the
*   contents of all memory pools managed actually be the heap manager with the
*   aim to verify whether the heap is coherent. In case the verification were
*   successful, the function returns != 0. Otherwise an adequate messages is
*   reported and the function returns 0.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Returns != 0 if the heap is coherent. In case of an error, the value 0 is
*   returned.
*
*******************************************************************************/
int EwVerifyHeap
(
  void
);


/*******************************************************************************
* FUNCTION:
*   EwAllocHeapBlock
*
* DESCRIPTION:
*   The function EwAllocHeapBlock() tries to allocate memory block with the 
*   given number of bytes. Once not needed anymore, the memory should be 
*   released by using the function EwFreeHeapBlock().
*
* ARGUMENTS:
*   aSize - Size of the memory to allocate in bytes.
*
* RETURN VALUE:
*   Returns a pointer to the allocated memory or 0 (zero) if there is not 
*   enough free memory on the heap.
*
*******************************************************************************/
void* EwAllocHeapBlock
(
  int                          aSize
);


/*******************************************************************************
* FUNCTION:
*   EwFreeHeapBlock
*
* DESCRIPTION:
*   The function EwFreeHeapBlock() releases the memory allocated by a preceding 
*   call to the method EwAllocHeapBlock().
*
* ARGUMENTS:
*   aMemory - Pointer to the memory block to release.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwFreeHeapBlock
(
  void*                        aMemory
);


/*******************************************************************************
* FUNCTION:
*   EwIsHeapPtr
*
* DESCRIPTION:
*   The function EwIsHeapPtr() verifies whether the given pointer aPtr refers a
*   memory area controlled by the heap manager (see EwAllocHeapBlock(), 
*   EwFreeHeapBlock()).
*
* ARGUMENTS:
*   aPtr - Address to test.
*
* RETURN VALUE:
*   EwIsHeapPtr() returns != 0 if the given pointer aPtr refers a memory area 
*   used by the heap manager. Otherwise 0 is returned.
*
*******************************************************************************/
int EwIsHeapPtr
(
  void*                        aMemory
);


#ifdef __cplusplus
  }
#endif

#endif /* EWRTE_H */

/* pba, msy */

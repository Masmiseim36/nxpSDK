/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EMBEDDED_RPC__CGENERATOR_H_
#define _EMBEDDED_RPC__CGENERATOR_H_

#include "Generator.h"
#include "cpptempl.h"
#include <set>

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpcgen {
/*!
 * @brief Code generator for C.
 */
class CGenerator : public Generator
{
public:
    /*!
     * @brief This function is constructor of CGenerator class.
     *
     * Interface definition contains all information about parsed files and builtin types.
     *
     * @param[in] def Pointer to interface definition.
     */
    CGenerator(InterfaceDefinition *def)
    : Generator(def)
    , m_generateClientOutput(true)
    , m_generateServerOutput(true)
    {
    }

    /*!
     * @brief This function is destructor of CGenerator class.
     *
     * This function close opened files.
     */
    virtual ~CGenerator() {}

    /*!
     * @brief This function generate output code for output files.
     *
     * This code call all necessary functions for prepare output code and parse it into output files.
     */
    virtual void generate();

protected:
    cpptempl::data_map m_templateData; /*!< Data prepared for templates files. */

    // TODO: not sure how to get rid of these members yet. Talk to Chris about it.
    bool m_generateClientOutput; /*!< For generating only client output. */
    bool m_generateServerOutput; /*!< For generating only server output. */

    std::vector<ListType *> m_listBinaryTypes; /*!<
                                                 * Contains binary types transformed to list<uint8>.
                                                 * More ListType are present when @length annotation is used for binary type.
                                                 * If binary without @length is used then it is placed on first place in this vector.
                                                 */

    std::vector<StructType *> m_structListTypes; /*!<
                                                  * Contains list types transformed to struct{list<>}.
                                                  * To distinguish between user defined struct{list<>} and transformed list<> to struct{list<>}.
                                                  */

    /*!
     * @brief This function prepare helpful functions located in template files.
     *
     * These function may be used while parsing templates
     */
    void parseSubtemplates();

    /*!
     * @brief This function generate output files.
     *
     * This function call functions for generating client/server header/source files.
     *
     * @param[in] fileNameExtension Extension for file name (for example for case that each interface will be generated
     * in its
     * set of output files).
     */
    void generateOutputFiles(const std::string &fileNameExtension);

    /*!
     * @brief This function generate output common header file.
     *
     * @param[in] fileName Name for output common header file.
     */
    void generateCommonHeaderFile(std::string fileName);

    /*!
     * @brief This function generate output client source file.
     *
     * @param[in] fileName Name for output client source file.
     */
    void generateClientSourceFile(std::string fileName);

    /*!
     * @brief This function generate output server header file.
     *
     * @param[in] fileName Name for output server header file.
     */
    void generateServerHeaderFile(std::string fileName);

    /*!
     * @brief This function generate output server source file.
     *
     * @param[in] fileName Name for output server source file.
     */
    void generateServerSourceFile(std::string fileName);

    /*!
     * @brief This function transforms binary data type to list or structure.
     *
     * @return Pointer to List or Struct wrapper of list type.
     */
    void transformBinaryDataType();

    /*!
     * @brief This function transforms data types to structures.
     *
     * This function is searching for lists and binary data types and converts them into structures.
     */
    void transformDataTypes();

    /*!
     * @brief This function change list and binary data type to structure.
     *
     * This function return given data type or new structure data type.
     * If given data type contains list or binary data type, then this data type will be changed to the structure.
     * New created structure is placed before top data type.
     *
     * @param[in] topDataType Function member DataType. If it is list or binary it will be changed to the structure.
     * @param[in] direction Direction of function member.
     * @param[in] dataType Top data type.
     *
     * @return Pointer to given or new DataType.
     */
    DataType *transformDataType(DataType *dataType, _param_direction direction);

    /*!
     * @brief This function transform binary data type to list and set annotation to it.
     *
     * @param[in] structMember Structure member, Function parameter or Union member.
     */
    void setBinaryList(StructMember *structMember);

    /*!
     * @brief This function return interface function template data.
     *
     * This function return interface function template data with all data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] fn From this are set interface function template data.
     * @param[in] fnIndex Function index.
     *
     * @return Contains interface function data.
     */
    cpptempl::data_map getFunctionTemplateData(Function *fn, int fnIndex);

    /*!
     * @brief This function will get interface comments and convert to language specific ones
     *
     * @param[in] iface Pointer to interface.
     * @param[inout] ifaceInfo Data map, which contains information about interface and interface members.
     */
    void getInterfaceComments(Interface *iface, cpptempl::data_map &ifaceInfo);

    /*!
     * @brief This function sets const template data.
     *
     * This function sets const template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeConstTemplateData();

    // Functions that populate type-specific template data

    /*!
     * @brief This function sets structures template data.
     *
     * This function sets structures template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeStructsTemplateData();

    /*!
     * @brief This function return template data for given structure.
     *
     * This function return structure's template data with all data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] structType Given structure type.
     *
     * @return Contain structure template data.
     */
    cpptempl::data_map getStructTemplateData(StructType *structType);

    /*!
     * @brief This function sets struct member information to struct data map variable.
     *
     * This function sets struct member information to struct data map variable, which
     * are necessary for generating output code for output files. Also it will set information,
     * if enum temporary variable is need.
     *
     * @param[in] structType Struct, which contains struct members.
     * @param[in,out] structInfo Data map, which contains information about struct and struct members.
     */
    void setStructMembersTemplateData(StructType *structType, cpptempl::data_map &structInfo);

    /*!
     * @brief This function sets enum template data.
     *
     * This function sets enum template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeEnumsTemplateData();

    /*!
     * @brief This function return template data for given enum.
     *
     * This function return enum's template data with all data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] enumType Given enum type.
     *
     * @return Contain enum template data.
     */
    cpptempl::data_map getEnumTemplateData(EnumType *enumType);

    /*!
     * @brief This function return enum members template data.
     *
     * This function return enum members template data with all data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] enumType Pointer to enum.
     *
     * @return Contains enum members data.
     */
    cpptempl::data_list getEnumMembersTemplateData(EnumType *enumType);

    /*!
     * @brief This function sets alias (type definition) template data.
     *
     * This function sets alias template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeAliasesTemplateData();

    /*!
     * @brief This function returns alias type of given DataType.
     *
     * @return AliasType or nullptr, when alias was not find.
     */
    AliasType *getAliasType(DataType *dataType);

    /*!
     * @brief This function returns alias name of given DataType.
     *
     * @return Alias name or empty string, when alias was not find.
     */
    std::string getAliasName(DataType *dataType);

    /*!
     * @brief This function return necessary template data for data type.
     *
     * This function return data type template data with all necessary data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] t From this are set data type template data.
     * @param[in] isFunction If set to true return parameters will have
     *          comma at start of sequence.
     *
     * @return Contains data type template data.
     */
    cpptempl::data_map getTypeInfo(DataType *t, bool isFunction);

    /*!
     * @brief This function returns error return value for given function.
     *
     * @param[in] fn Function from which is returned error return value.
     *
     * @return Return string representation of error return value.
     */
    std::string getErrorReturnValue(Function *fn);

    /*!
     * @brief This function return interface function prototype.
     *
     * @param[in] fn Function for prototyping.
     *
     * @return String prototype representation for given function.
     */
    std::string getFunctionPrototype(Function *fn);

    /*!
     * @brief This function return interface function representation called by server side.
     *
     * @param[in] fn Function for interface function representation.
     *
     * @return String representation for given function.
     */
    std::string getFunctionServerCall(Function *fn);

    /*!
     * @brief This function return name with guard.
     *
     * @param[in] filename File name for guard name.
     *
     * @return Guard name.
     */
    std::string generateIncludeGuardName(const std::string &filename);

    /*!
     * @brief This function return string representation for given BuiltinType.
     *
     * @param[in] t Builtin type.
     *
     * return std::string String representation for given builtin type.
     */
    std::string getBuiltinTypename(const BuiltinType *t);

    /*!
     * @brief This function set to variable returnName type or type with variable name.
     *
     * This function set to variable returnName "type" if given returnName is "",
     * or return "type with variable name", when given returnName is set to
     * variable name.
     *
     * @param[in] t data type.
     * @param[in] name Return type name.
     *
     * @return Return data type name with given name.
     *
     * @exception internal_error Thrown, when unknown data type is called.
     */
    std::string getTypenameName(DataType *t, const std::string &name);

    /*!
     * @brief This function return necessary template data for decode and encode data type.
     *
     * This function prepare data for decode or encode functions in c_coders template file.
     *
     * @param[in] name Variable name.
     * @param[in] t Variable data type.
     * @param[in] structType Structure holdings structure members.
     * @param[in] inDataContainer Is inside data container (struct, list, array).
     * @param[in] structMember Null for return.
     * @param[out] containsEnum Return true, when data type contains enum type.
     * @param[in] isFunctionParam Tru for function param else false (structure member).
     *
     * @return Template data for decode or encode data type.
     */
    cpptempl::data_map getEncodeDecodeCall(const std::string &name,
                                           DataType *t,
                                           StructType *structType,
                                           bool inDataContainer,
                                           StructMember *structMember,
                                           bool &containsEnum,
                                           bool isFunctionParam);

    /*!
     * @brief This function add to template data, which kind of BuiltinType is data type.
     *
     * @param[in] t Data type.
     * @param[out] templateData Template data.
     * @param[in] structType Structure holdings structure members.
     * @param[in] structMember Null for return.
     * @param[in] isFunctionParam Tru for function param else false (structure member).
     */
    void getEncodeDecodeBuiltin(BuiltinType *t,
                                cpptempl::data_map &templateData,
                                StructType *structType,
                                StructMember *structMember,
                                bool isFunctionParam);

    /*!
     * @brief This function encapsulate gave name, if it start with pointer.
     *
     * It is necessary for out and inout parameters, because int *a[] is not same as int (*a)[].
     *
     * @param[inout] name Variable name.
     */
    void giveBracesToArrays(std::string &name);

    /*!
     * @brief This function return "*", if it is need.
     *
     * It is need by rules of passing data types for each direction type.
     *
     * @param[in] structMember Contains direction type and data type.
     *
     * @return Pointer or empty.
     */
    std::string getExtraDirectionPointer(StructMember *structMember);

    /*!
     * @brief This function return "*", if it is need.
     *
     * It is need by rules of taking data types from return type.
     *
     * @param[in] dataType Data type.
     *
     * @return Pointer or empty.
     */
    std::string getExtraPointerInReturn(DataType *dataType);

    /*!
     * @brief This function call first erpc_alloc on server side for parameters if it is need (except out param).
     *
     * It is need by rules of passing data types for each direction type.
     *
     * @param[in] name Parameter name.
     * @param[in] structMember Contains direction type and data type.
     *
     * @return Erpc_alloc function or empty.
     */
    std::string firstAllocOnServerWhenIsNeed(std::string name, StructMember *structMember);

    /*!
     * @brief This function call first erpc_alloc on server side for parameters if it is need (only out param).
     *
     * @param[in] name Parameter name.
     * @param[in] structMember Contains direction type and data type.
     *
     * @return Erpc_alloc function or empty.
     */
    std::string firtAllocOutParamOnServerWhenIsNeed(std::string name, StructMember *structMember);

    /*!
     * @brief This function call first erpc_alloc function on client side for parameters if it is need.
     *
     * It is need by rules of passing data types for each direction type.
     *
     * @param[in] name Parameter name.
     * @param[in] structMember Contains direction type and data type.
     *
     * @return Erpc_alloc function or empty.
     */
    std::string firstAllocOnClientWhenIsNeed(std::string name, StructMember *structMember);

    /*!
     * @brief This function call first erpc_alloc on client side return statement if it is need.
     *
     * It is need by rules of taking data types from return type.
     *
     * @param[in] name Parameter name.
     * @param[in] dataType Contains direction type and data type.
     *
     * @return Erpc_alloc function or empty.
     */
    std::string firstAllocOnReturnWhenIsNeed(std::string name, DataType *dataType);

    /*!
     * @brief This function return call for alloc space based on given data type.
     *
     * @param[in] pointer True, if inside alloc (sizeof()) need pointer to data type, else false.
     * @param[in] name Name, for which is allocating space.
     * @param[in] symbol Given symbol type contains annotations and data type.
     *
     * @return Return erpc_alloc call or empty string.
     */
    std::string allocateCall(bool pointer, std::string &name, Symbol *symbol);

    /*!
     * @brief This function will add to given lists given map based on given by symbol direction.
     *
     * Based on directions of structures or functions parameters, given map will be added to given lists.
     *
     * @param[in] symbolType Contains structure or function parameter.
     * @param[in,out] to1Core List structures map, which are use for core 1 direction.
     * @param[in,out] to2Core List structures map, which are use for core 2 direction.
     * @param[in] dataMap Map with information about structure or function parameter.
     */
    void setToCore(Symbol *symbolType,
                   cpptempl::data_list &to1Core,
                   cpptempl::data_list &to2Core,
                   cpptempl::data_map &dataMap);

    /*!
     * @brief This function return information from eRPC file about maximum size of element.
     *
     * For parameters like strings/lists can be set max_size annotations. This size represent
     * pre-allocated size on client side.
     *
     * @param[in] structMember StructMember contains annotation with length.
     *
     * @return String representation of length variable.
     *
     * @throw Throw semantic error if annotation doesn't exist.
     */
    std::string getMaxLength(StructMember *structMember);

    /*!
     * @brief This function returns true, when given data type need be freed.
     *
     * @param[in] dataType Given data type.
     *
     * @retval true When given data type need be freed.
     * @retval false When given data type don't need be freed (like int, enum).
     */
    bool isNeedCallFree(DataType *dataType);

    /*!
     * @ This function set variables for calling first freeing function on server side for parameters, when it is need.
     *
     * It free what was allocated by this function: std::string firstAllocOnServerWhenIsNeed(std::string name,
     * StructMember *structMember);
     *
     * @param[in] symbol StructMember when function parameter or DataType when return type.
     * @param[in] info DataMap which contains information about data type for output.
     */
    void setCallingFreeFunctions(Symbol *symbol, cpptempl::data_map &info);

    /*!
     * @brief This function return space if given string is not empty.
     *
     * @param[in] param Given string.
     *
     * @return Space or empty.
     */
    std::string returnSpaceWhenNotEmpty(const std::string &param);

    /*!
     * @brief This function will return number, which represent type of direction for structures or functions
     * parameters.
     *
     * @param[in] symbolType Contains structure or function parameter.
     *
     * @retval "1" - for "in" type.
     *         "2" - for "out" type.
     *         "3" - for "inout" type.
     */
    uint8_t getDirection(Symbol *symbolType);

    /*!
     * @brief This function check, if data type contains string data type.
     *
     * @param[in] dataType Given data type.
     *
     * @retval True if data type contains string, else false.
     *
     * @see bool CGenerator::containsList(DataType * dataType)
     * @see bool CGenerator::containsBinary(DataType * dataType)
     */
    bool containsString(DataType *dataType);

    /*!
     * @brief This function check, if data type contains list data type.
     *
     * @param[in] dataType Given data type.
     *
     * @retval True if data type contains list, else false.
     *
     * @see bool CGenerator::containsBinary(DataType * dataType)
     * @see bool CGenerator::containsString(DataType * dataType)
     */
    bool containsList(DataType *dataType);

    /*!
     * @brief This function returns true when structure is used as a wrapper for binary type.
     *
     * Binary type which is not using length annotation is in CGenerator presented as struct{ list<uint8> }.
     *
     * @param[in] structType Potential structure wrapper.
     *
     * @retval true When structure is used as a wrapper for binary type.
     * @retval false When structure is not used as a wrapper for binary type.
     */
    bool isBinaryStruct(StructType *structType);

    /*!
     * @brief This function returns true when list was created for replacing binary type.
     *
     * Binary type which is using length annotation is in CGenerator presented as list<uint8>.
     *
     * @param[in] structType Potential structure wrapper.
     *
     * @retval true When structure is used as a wrapper for binary type.
     * @retval false When structure is not used as a wrapper for binary type.
     */
    bool isBinaryList(ListType *listType);

    /*!
     * @brief This function returns true when structure is used as a wrapper for list type.
     *
     * @param[in] structType Potential structure wrapper.
     *
     * @retval true When structure is used as a wrapper for list type.
     * @retval false When structure is not used as a wrapper for list type.
     */
    bool isListStruct(StructType *structType);

    /*!
     * @brief This function returns true when "retain" annotation was set for function parameter.
     *
     * This annotation have effect for server side of generated files. Allocated space will be not freed by server shim
     * code.
     *
     * @param[in] structMember Function parameter.
     *
     * @retval true When "retain" annotation wasn't set to the function parameter.
     * @retval false When "retain" annotation was set to the function parameter.
     */
    bool generateServerFunctionParamFreeFunctions(StructMember *structMember);
};
} // namespace erpcgen

#endif // _EMBEDDED_RPC__CGENERATOR_H_

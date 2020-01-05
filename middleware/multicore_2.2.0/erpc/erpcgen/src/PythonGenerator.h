/*
 * Copyright (c) 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#ifndef _EMBEDDED_RPC__PYTHONGENERATOR_H_
#define _EMBEDDED_RPC__PYTHONGENERATOR_H_

#include "Generator.h"
#include "cpptempl.h"
#include <set>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpcgen {

/*!
 * @brief Code generator for Python.
 */
class PythonGenerator : public Generator
{
public:
    /*!
     * @brief This function is constructor of PythonGenerator class.
     *
     * @param[in] prog Pointer to program with generating options.
     */
    PythonGenerator(InterfaceDefinition *def);

    /*!
     * @brief This function is destructor of PythonGenerator class.
     *
     * This function close opened files.
     */
    virtual ~PythonGenerator() {}

    /*!
     * @brief This function generate output code for output files.
     *
     * This code call all necessary functions for prepare output code and parse it into output files.
     */
    virtual void generate();

protected:
    cpptempl::data_map m_templateData; //!< Data prepared for templates files.
    std::string m_suffixStrip;         //!< String to remove from suffixes of names.
    size_t m_suffixStripSize;          //!< Length of the suffix filter string.

    /*!
     * @brief This function prepare helpful functions located in template files.
     *
     * These function may be used while parsing templates
     */
    void parseSubtemplates();

    /*!
     * @brief This function generate output files.
     *
     * This function call functions for generating client/server source files.
     *
     * @param[in] fileName Name for output files.
     */
    void generateOutputFiles(const std::string &fileName);

    /*!
     * @brief This function generate output common module.
     *
     * @param[in] fileName Name for output files.
     */
    void generateCommonFile(std::string fileName);

    /*!
     * @brief This function generate output package init file.
     *
     * @param[in] fileName Name for output files.
     */
    void generateInitFile(std::string fileName);

    /*!
     * @brief This function generate output client file.
     *
     * @param[in] fileName Name for output files.
     */
    void generateClientFile(std::string fileName);

    /*!
     * @brief This function generate output server file.
     *
     * @param[in] fileName Name for output files.
     */
    void generateServerFile(std::string fileName);

    /*!
     * @brief This function generate output interface file.
     *
     * @param[in] fileName Name for output files.
     */
    void generateInterfaceFile(std::string fileName);

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
     * @brief This function return interface function prototype.
     *
     * @param[in] fn Function for prototyping.
     *
     * @return String prototype representation for given function.
     */
    std::string getFunctionPrototype(Function *fn);

    /*!
     * @brief This function sets const template data.
     *
     * This function sets const template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeConstTemplateData();

    /*!
     * @brief This function sets enum template data.
     *
     * This function sets enum template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeEnumsTemplateData();

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
     * @brief Fill in template data for type aliases.
     */
    void makeAliasesTemplateData();

    /*!
     * @brief This function sets structures template data.
     *
     * This function sets structures template data with all data, which
     * are necessary for generating output code for output files.
     */
    void makeStructsTemplateData();

    /*!
     * @brief This function sets struct member information to struct data map variable.
     *
     * This function sets struct member information to struct data map variable, which
     * are necessary for generating output code for output files.
     *
     * @param[in] structType Struct, which contains struct members.
     * @param[in,out] structInfo Data map, which contains information about struct and struct members.
     */
    void setStructMembersTemplateData(StructType *structType, cpptempl::data_map &structInfo);

    /*!
     * @brief Fill in template data for a single struct member.
     */
    void setOneStructMemberTemplateData(StructMember *member, cpptempl::data_map &member_info);

    /*!
     * @brief This function return necessary template data for data type.
     *
     * This function return data type template data with all necessary data, which
     * are necessary for generating output code for output files.
     *
     * @param[in] t Data type. From this are set data type template data.
     *
     * @return Contains data type template data.
     */
    cpptempl::data_map getTypeInfo(DataType *t);

    /*!
     * @brief This function return string representation for given BuiltinType.
     *
     * @param[in] t Builtin type.
     *
     * @return String representation for given builtin type.
     */
    std::string getBuiltinTypename(const BuiltinType *t);

    /*!
     * @brief Filter symbol names.
     */
    std::string filterName(const std::string &name);

    //! @brief Possible Doxygen comment styles.
    enum comment_type
    {
        kMultilineComment, //!< Leading multi-line comment
        kInlineComment,    //!< Trailing inline comment.
    };

    /*!
     * @brief Converts a C++-style Doxygen comment into a Python Doxygen comment.
     *
     * @param[inout] comment The C++ comment.
     * @param[in] commentType Inline or multiline comment.
     *
     * @return Python form of the provided comment.
     */
    std::string convertComment(const std::string &comment, comment_type commentType);

    /*!
     * @brief Strip leading and trailing whitespace.
     */
    std::string stripWhitespace(const std::string &s);
};

} // namespace erpcgen

#endif // _EMBEDDED_RPC__PYTHONGENERATOR_H_

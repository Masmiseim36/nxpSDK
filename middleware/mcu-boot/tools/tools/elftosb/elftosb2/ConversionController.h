/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_ConversionController_h_)
#define _ConversionController_h_

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "smart_ptr.h"
#include "ElftosbLexer.h"
#include "ElftosbAST.h"
#include "EvalContext.h"
#include "Value.h"
#include "SourceFile.h"
#include "Operation.h"
#include "DataSource.h"
#include "DataTarget.h"
#include "OutputSection.h"
#include "BootImage.h"
#include "OptionDictionary.h"
#include "BootImageGenerator.h"
#include "Keyblob.h"

namespace elftosb
{
/*!
 * \brief Manages the entire elftosb file conversion process.
 *
 * Instances of this class are intended to be used only once. There is no
 * way to reset an instance once it has started or completed a conversion.
 * Thus the run() method is not reentrant. State information is stored in
 * the object during the conversion process.
 *
 * Two things need to be done before the conversion can be started. The
 * command file path has to be set with the setCommandFilePath() method,
 * and the paths of any externally provided (i.e., from the command line)
 * files need to be added with addExternalFilePath(). Once these tasks
 * are completed, the run() method can be called to parse and execute the
 * command file. After run() returns, pass an instance of
 * BootImageGenerator to the generateOutput() method in order to get
 * an instance of BootImage that can be written to the output file.
 */
class ConversionController : public OptionDictionary, public EvalContext::SourceFileManager
{
public:
    //! \brief Default constructor.
    ConversionController();

    //! \brief Destructor.
    virtual ~ConversionController();

    //! \name Paths
    //@{
    //! \brief Specify the command file that controls the conversion process.
    void setCommandFilePath(const std::string &path);

    //! \brief Specify the path of a file provided by the user from outside the command file.
    void addExternalFilePath(const std::string &path);
    //@}

    //! \name Conversion
    //@{
    //! \brief Process input files.
    void run();

    //! \brief Uses a BootImageGenerator object to create the final output boot image.
    BootImage *generateOutput(BootImageGenerator *generator);
    //@}

    //! \name SourceFileManager interface
    //@{
    //! \brief Returns true if a source file with the name \a name exists.
    virtual bool hasSourceFile(const std::string &name);

    //! \brief Gets the requested source file.
    virtual SourceFile *getSourceFile(const std::string &name);

    //! \brief Returns the default source file, or NULL if none is set.
    virtual SourceFile *getDefaultSourceFile();
    //@}

    //! \brief Returns a reference to the context used for expression evaluation.
    inline EvalContext &getEvalContext() { return m_context; }
protected:
    //! \name AST processing
    //@{
    void parseCommandFile();
    void processOptions(ListASTNode *options);
    void processConstants(ListASTNode *constants);
    void processSources(ListASTNode *sources);
    void processKeyblob(ExprASTNode *idExpr, ListASTNode *entires);
    void processSections(ListASTNode *sections);
    OutputSection *convertDataSection(DataSectionContentsASTNode *dataSection,
                                      uint32_t sectionID,
                                      OptionDictionary *optionsDict);
    //@}

    //! \name Statement conversion
    //@{
    OperationSequence *convertStatementList(ListASTNode *statements);
    OperationSequence *convertOneStatement(StatementASTNode *statement);
    OperationSequence *convertLoadStatement(LoadStatementASTNode *statement);
    OperationSequence *convertCallStatement(CallStatementASTNode *statement);
    OperationSequence *convertFromStatement(FromStatementASTNode *statement);
    OperationSequence *convertModeStatement(ModeStatementASTNode *statement);
    OperationSequence *convertResetStatement(ResetStatementASTNode *statement);
    OperationSequence *convertIfStatement(IfStatementASTNode *statement);
    OperationSequence *convertEraseStatement(EraseStatementASTNode *statement);
    OperationSequence *convertMemEnableStatement(MemEnableStatementASTNode *statement);
    OperationSequence *convertKeywrapStatement(KeywrapStatementASTNode *statement);
    OperationSequence *convertEncryptStatement(EncryptStatementASTNode *statement);
    void handleMessageStatement(MessageStatementASTNode *statement);
    //@}

    //! \name Utilities
    //@{
    Value *convertAssignmentNodeToValue(ASTNode *node, std::string &ident);
    SourceFile *getSourceFromName(std::string *sourceName, int line);
    DataSource *createSourceFromNode(ASTNode *dataNode);
    DataTarget *createTargetFromNode(ASTNode *targetNode);
    std::string *substituteVariables(const std::string *message);
    DataSource *createIVTDataSource(IVTConstASTNode *ivtNode);
    //@}

    //! \name Debugging
    //@{
    void testLexer(ElftosbLexer &lexer);
    void printIntConstExpr(const std::string &ident, IntConstExprASTNode *expr);
    uint32_t getMemoryId(ASTNode *statement, std::string &memoryName);
    //@}

protected:
    typedef std::map<std::string, SourceFile *> source_map_t; //!< Map from source name to object.
    typedef std::vector<std::string> path_vector_t;           //!< List of file paths.
    typedef std::vector<OutputSection *> section_vector_t;    //!< List of output sections.
    typedef std::vector<std::string> source_name_vector_t;    //!< List of source names.
    typedef std::vector<Keyblob *> keyblob_vector_t;          //!< List of keyblobs.

    smart_ptr<std::string> m_commandFilePath; //!< Path to command file.
    smart_ptr<CommandFileASTNode> m_ast;      //!< Root of the abstract syntax tree.
    EvalContext m_context;                    //!< Evaluation context for expressions.
    source_map_t m_sources;                   //!< Map of source names to file objects.
    path_vector_t m_externPaths;              //!< Paths provided on the command line by the user.
    SourceFile *m_defaultSource;              //!< Source to use when one isn't provided.
    section_vector_t m_outputSections;        //!< List of output sections the user wants.
    source_name_vector_t m_failedSources;     //!< List of sources that failed to open successfully.
    keyblob_vector_t m_keyblobs;              //!< List of keyblobs the user defines.
    Keyblob *m_keywrapKeyblob;                //!< Keyblob used for keywrapping
    Keyblob *m_encryptKeyblob;                //!< Keyblob used for OTFAD encryption
};

//! \brief Whether to support HAB keywords during parsing.
//!
//! This is a standalone global solely so that the bison-generated parser code can get to it
//! as simply as possible.
extern bool g_enableHABSupport;

}; // namespace elftosb

#endif // _ConversionController_h_

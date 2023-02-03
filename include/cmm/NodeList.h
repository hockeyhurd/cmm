/**
 * A list of includes for all nodes in this compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#pragma once

#ifndef CMM_NODE_LIST_H
#define CMM_NODE_LIST_H

#include <cmm/ArgNode.h>
#include <cmm/BinOpNode.h>
#include <cmm/BlockNode.h>
#include <cmm/CastNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/DerefNode.h>
#include <cmm/EnumDefinitionStatementNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/FieldAccessNode.h>
#include <cmm/FunctionCallNode.h>
#include <cmm/FunctionDeclarationStatementNode.h>
#include <cmm/FunctionDefinitionStatementNode.h>
#include <cmm/IfElseStatementNode.h>
#include <cmm/LitteralNode.h>
#include <cmm/Node.h>
#include <cmm/ParameterNode.h>
#include <cmm/ParenExpressionNode.h>
#include <cmm/ReturnStatementNode.h>
#include <cmm/StatementNode.h>
#include <cmm/StructDefinitionStatementNode.h>
#include <cmm/StructFwdDeclarationStatementNode.h>
#include <cmm/TranslationUnitNode.h>
#include <cmm/TypeNode.h>
#include <cmm/UnaryOpNode.h>
#include <cmm/VariableDeclarationStatementNode.h>
#include <cmm/VariableNode.h>
#include <cmm/WhileStatementNode.h>

#endif //!CMM_NODE_LIST_H


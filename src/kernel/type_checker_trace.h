/*
Copyright (c) 2013 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "kernel/trace.h"
#include "kernel/expr.h"
#include "kernel/context.h"

namespace lean {
/**
   \brief Trace produced by the type checker when the application \c m_app
   is an application <tt>(f ...)</tt>, the type \c T of \c f contains metavariables, and
   it is not clear whether it is a Pi or not. In this case, the type checker adds
   the constraint

       <tt>ctx |- T == Pi (x : ?A), ?B x</tt>

   where, \c ?A and \c ?B are fresh metavariables.
   This trace cell is used to justify/trace the new constraint.
*/
class function_expected_trace_cell : public trace_cell {
    context m_ctx;
    expr    m_app;
public:
    function_expected_trace_cell(context const & c, expr const & app):m_ctx(c), m_app(app) {}
    virtual ~function_expected_trace_cell();
    virtual format pp(formatter const & fmt, options const & opts) const;
    virtual void get_children(buffer<trace> &) const;
    virtual expr const & get_main_expr() const;
    context const & get_context() const { return m_ctx; }
    expr const & get_app() const { return m_app; }
};

/**
   \brief Trace produced by the type checker for application argument type matching.
   When checking an application <tt>(f a_1 ... a_i ...)</tt>, the type
   \c T_inferred of \c a_i must be convertible to the expected type \c T_expected.  If
   \c T_expected or \c T_inferred contain metavariables, and it is not clear whether
   \c T_inferred is convertible to \c T_expected, then the type checker adds the constraint

      <tt>ctx |- T_inferred << T_expected</tt>

   This trace cell is used to justify this constraint.
*/
class app_type_match_trace_cell : public trace_cell {
    context  m_ctx;
    expr     m_app;
    unsigned m_i;
public:
    app_type_match_trace_cell(context const & c, expr const & a, unsigned i):m_ctx(c), m_app(a), m_i(i) {}
    virtual ~app_type_match_trace_cell();
    virtual format pp(formatter const & fmt, options const & opts) const;
    virtual void get_children(buffer<trace> &) const;
    virtual expr const & get_main_expr() const;
    context const & get_context() const { return m_ctx; }
    expr const & get_app() const { return m_app; }
    unsigned get_arg_pos() const { return m_i; }
};

/**
   \brief Trace produced by the type checker when the type \c T of \c type must be of the form
   <tt>Type l</tt>, and \c T constains metavariables, and it is not of this form.
   The type checker adds the following constraint

      <tt>ctx |- T << Type U</tt>

   This trace cell is used to justify these constraints.
*/
class type_expected_trace_cell : public trace_cell {
    context m_ctx;
    expr    m_type;
public:
    type_expected_trace_cell(context const & c, expr const & t):m_ctx(c), m_type(t) {}
    virtual ~type_expected_trace_cell();
    virtual format pp(formatter const & fmt, options const & opts) const;
    virtual void get_children(buffer<trace> &) const;
    virtual expr const & get_main_expr() const;
    context const & get_context() const { return m_ctx; }
    expr const & get_type() const { return m_type; }
};

/**
   \brief If \c type is of the form <tt>Pi x : A, B</tt>, where \c A has type \c T1 and \c B has type \c T2,
   and \c T1 or \c T2 have metavariables, then the type checker adds the following constraint

     <tt>ctx |- max(T1, T2) == ?T

   where ?T is a new metavariable, and this trace cell is used to justify the new constraint.
*/
class max_type_trace_cell : public type_expected_trace_cell {
public:
    max_type_trace_cell(context const & c, expr const & t):type_expected_trace_cell(c, t) {}
};

/**
   \brief Trace produced by the type checker when checking whether the type \c T_inferred of \c value
   is convertible to the expected type \c T_expected. If \c T_expected or \c T_inferred contain
   metavariables, and it is not clear whether \c T_inferred is convertible to \c T_expected,
   then the type checker adds the constraint

      <tt>ctx |- T_inferred << T_expected</tt>

   This trace cell is used to justify this constraint.
*/
class def_type_match_trace_cell : public trace_cell {
    context m_ctx;
    name    m_name;
    expr    m_value;
public:
    def_type_match_trace_cell(context const & c, name const & n, expr const & v):m_ctx(c), m_name(n), m_value(v) {}
    virtual ~def_type_match_trace_cell();
    virtual format pp(formatter const & fmt, options const & opts) const;
    virtual void get_children(buffer<trace> &) const;
    virtual expr const & get_main_expr() const;
    context const & get_context() const { return m_ctx; }
    name const & get_name() const { return m_name; }
    expr const & get_value() const { return m_value; }
};

inline trace mk_function_expected_trace(context const & ctx, expr const & app) { return trace(new function_expected_trace_cell(ctx, app)); }
inline trace mk_app_type_match_trace(context const & ctx, expr const & app, unsigned i) { return trace(new app_type_match_trace_cell(ctx, app, i)); }
inline trace mk_type_expected_trace(context const & ctx, expr const & type) { return trace(new type_expected_trace_cell(ctx, type)); }
inline trace mk_max_type_trace(context const & ctx, expr const & type) { return trace(new max_type_trace_cell(ctx, type)); }
inline trace mk_def_type_match_trace(context const & ctx, name const & n, expr const & v) { return trace(new def_type_match_trace_cell(ctx, n, v)); }
}

/* sqlite__meta.c
 *
 * Loads objects in database.
 * This file contains generated code. Do not modify!
 */

#include "sqlite__type.h"

/* Variable definitions */
cx_object _o;
cx_package sqlite_o;
cx_class sqlite_server_o;
cx_method sqlite_server_construct_o;
cx_method sqlite_server_destruct_o;
cx_member sqlite_server_events_o;
cx_observer sqlite_server_onDeclare_o;
cx_observer sqlite_server_onDefine_o;
cx_method sqlite_server_post_o;
cx_member sqlite_server_thread_o;

/* Load objects in database */
int sqlite_load(void) {
    cx_object _a_; /* Used for resolving anonymous objects */
    _a_ = NULL;

    /* Declare ::cortex */
    _o = cx_declare(root_o, "cortex", cx_type(cx_object_o));
    if (!_o) {
        cx_error("sqlite_load: failed to declare object '::cortex'.");
        goto error;
    }

    /* Define ::cortex */
    if (!cx_checkState(_o, CX_DEFINED)) {
        if (cx_define(_o)) {
            cx_error("sqlite_load: failed to define object '::cortex'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite */
    sqlite_o = cx_declare(_o, "sqlite", cx_type(cx_package_o));
    if (!sqlite_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite'.");
        goto error;
    }

    /* Define ::cortex::sqlite */
    if (!cx_checkState(sqlite_o, CX_DEFINED)) {
        sqlite_o->url = NULL;
        if (cx_define(sqlite_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server */
    sqlite_server_o = cx_declare(sqlite_o, "server", cx_type(cx_class_o));
    if (!sqlite_server_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server'.");
        goto error;
    }

    /* Declare ::cortex::sqlite::server::construct() */
    sqlite_server_construct_o = cx_declare(sqlite_server_o, "construct()", cx_type(cx_method_o));
    if (!sqlite_server_construct_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::construct()'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::construct() */
    if (!cx_checkState(sqlite_server_construct_o, CX_DEFINED)) {
        cx_function(sqlite_server_construct_o)->returnType = cx_resolve_ext(sqlite_server_construct_o, NULL, "::cortex::lang::int16", FALSE, "element ::cortex::sqlite::server::construct().returnType");
        cx_function(sqlite_server_construct_o)->returnsReference = FALSE;
        sqlite_server_construct_o->virtual = FALSE;
        
        /* Bind ::cortex::sqlite::server::construct() with C-function */
        cx_function(sqlite_server_construct_o)->kind = CX_PROCEDURE_CDECL;
        void __sqlite_server_construct(void *args, void *result);
        cx_function(sqlite_server_construct_o)->impl = (cx_word)__sqlite_server_construct;
        if (cx_define(sqlite_server_construct_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::construct()'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server::destruct() */
    sqlite_server_destruct_o = cx_declare(sqlite_server_o, "destruct()", cx_type(cx_method_o));
    if (!sqlite_server_destruct_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::destruct()'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::destruct() */
    if (!cx_checkState(sqlite_server_destruct_o, CX_DEFINED)) {
        cx_function(sqlite_server_destruct_o)->returnType = cx_resolve_ext(sqlite_server_destruct_o, NULL, "::cortex::lang::void", FALSE, "element ::cortex::sqlite::server::destruct().returnType");
        cx_function(sqlite_server_destruct_o)->returnsReference = FALSE;
        sqlite_server_destruct_o->virtual = FALSE;
        
        /* Bind ::cortex::sqlite::server::destruct() with C-function */
        cx_function(sqlite_server_destruct_o)->kind = CX_PROCEDURE_CDECL;
        void __sqlite_server_destruct(void *args, void *result);
        cx_function(sqlite_server_destruct_o)->impl = (cx_word)__sqlite_server_destruct;
        if (cx_define(sqlite_server_destruct_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::destruct()'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server::events */
    sqlite_server_events_o = cx_declare(sqlite_server_o, "events", cx_type(cx_member_o));
    if (!sqlite_server_events_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::events'.");
        goto error;
    }

    /* Declare ::cortex::sqlite::server::onDeclare() */
    sqlite_server_onDeclare_o = cx_declare(sqlite_server_o, "onDeclare()", cx_type(cx_observer_o));
    if (!sqlite_server_onDeclare_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::onDeclare()'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::onDeclare() */
    if (!cx_checkState(sqlite_server_onDeclare_o, CX_DEFINED)) {
        sqlite_server_onDeclare_o->observable = NULL;
        sqlite_server_onDeclare_o->mask = 0x0;
        sqlite_server_onDeclare_o->expression = NULL;
        sqlite_server_onDeclare_o->template = 0;
        sqlite_server_onDeclare_o->dispatcher = NULL;
        sqlite_server_onDeclare_o->me = NULL;
        
        /* Bind ::cortex::sqlite::server::onDeclare() with C-function */
        cx_function(sqlite_server_onDeclare_o)->kind = CX_PROCEDURE_CDECL;
        void __sqlite_server_onDeclare(void *args, void *result);
        cx_function(sqlite_server_onDeclare_o)->impl = (cx_word)__sqlite_server_onDeclare;
        if (cx_define(sqlite_server_onDeclare_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::onDeclare()'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server::onDefine() */
    sqlite_server_onDefine_o = cx_declare(sqlite_server_o, "onDefine()", cx_type(cx_observer_o));
    if (!sqlite_server_onDefine_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::onDefine()'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::onDefine() */
    if (!cx_checkState(sqlite_server_onDefine_o, CX_DEFINED)) {
        sqlite_server_onDefine_o->observable = NULL;
        sqlite_server_onDefine_o->mask = 0x0;
        sqlite_server_onDefine_o->expression = NULL;
        sqlite_server_onDefine_o->template = 0;
        sqlite_server_onDefine_o->dispatcher = NULL;
        sqlite_server_onDefine_o->me = NULL;
        
        /* Bind ::cortex::sqlite::server::onDefine() with C-function */
        cx_function(sqlite_server_onDefine_o)->kind = CX_PROCEDURE_CDECL;
        void __sqlite_server_onDefine(void *args, void *result);
        cx_function(sqlite_server_onDefine_o)->impl = (cx_word)__sqlite_server_onDefine;
        if (cx_define(sqlite_server_onDefine_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::onDefine()'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server::post(event e) */
    sqlite_server_post_o = cx_declare(sqlite_server_o, "post(event e)", cx_type(cx_method_o));
    if (!sqlite_server_post_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::post(event e)'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::post(event e) */
    if (!cx_checkState(sqlite_server_post_o, CX_DEFINED)) {
        cx_function(sqlite_server_post_o)->returnType = cx_resolve_ext(sqlite_server_post_o, NULL, "::cortex::lang::void", FALSE, "element ::cortex::sqlite::server::post(event e).returnType");
        cx_function(sqlite_server_post_o)->returnsReference = FALSE;
        sqlite_server_post_o->virtual = FALSE;
        
        /* Bind ::cortex::sqlite::server::post(event e) with C-function */
        cx_function(sqlite_server_post_o)->kind = CX_PROCEDURE_CDECL;
        void __sqlite_server_post(void *args, void *result);
        cx_function(sqlite_server_post_o)->impl = (cx_word)__sqlite_server_post;
        if (cx_define(sqlite_server_post_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::post(event e)'.");
            goto error;
        }
    }

    /* Declare ::cortex::sqlite::server::thread */
    sqlite_server_thread_o = cx_declare(sqlite_server_o, "thread", cx_type(cx_member_o));
    if (!sqlite_server_thread_o) {
        cx_error("sqlite_load: failed to declare object '::cortex::sqlite::server::thread'.");
        goto error;
    }

    /* Define ::cortex::sqlite::server::thread */
    if (!cx_checkState(sqlite_server_thread_o, CX_DEFINED)) {
        sqlite_server_thread_o->type = cx_resolve_ext(sqlite_server_thread_o, NULL, "::cortex::lang::word", FALSE, "element ::cortex::sqlite::server::thread.type");
        sqlite_server_thread_o->modifiers = 0x3;
        sqlite_server_thread_o->state = 0x6;
        sqlite_server_thread_o->weak = FALSE;
        sqlite_server_thread_o->id = 0;
        if (cx_define(sqlite_server_thread_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::thread'.");
            goto error;
        }
    }

    /* Define ::cortex::sqlite::server::events */
    if (!cx_checkState(sqlite_server_events_o, CX_DEFINED)) {
        sqlite_server_events_o->type = cx_resolve_ext(sqlite_server_events_o, NULL, "::cortex::lang::list{event,0}", FALSE, "element ::cortex::sqlite::server::events.type");
        sqlite_server_events_o->modifiers = 0x3;
        sqlite_server_events_o->state = 0x6;
        sqlite_server_events_o->weak = FALSE;
        sqlite_server_events_o->id = 1;
        if (cx_define(sqlite_server_events_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server::events'.");
            goto error;
        }
    }

    /* Define ::cortex::sqlite::server */
    if (!cx_checkState(sqlite_server_o, CX_DEFINED)) {
        cx_type(sqlite_server_o)->defaultType = cx_resolve_ext(sqlite_server_o, NULL, "::cortex::lang::member", FALSE, "element ::cortex::sqlite::server.defaultType");
        cx_type(sqlite_server_o)->parentType = NULL;
        cx_type(sqlite_server_o)->parentState = 0x0;
        cx_interface(sqlite_server_o)->base = NULL;
        cx_struct(sqlite_server_o)->baseAccess = 0x0;
        sqlite_server_o->implements.length = 1;
        sqlite_server_o->implements.buffer = cx_malloc(sizeof(cx_interface));
        sqlite_server_o->implements.buffer[0] = cx_resolve_ext(sqlite_server_o, NULL, "::cortex::lang::dispatcher", FALSE, "(null) ::cortex::sqlite::server.implements[0]");
        if (cx_define(sqlite_server_o)) {
            cx_error("sqlite_load: failed to define object '::cortex::sqlite::server'.");
            goto error;
        }
    }

    if (cx_type(sqlite_server_o)->size != sizeof(struct sqlite_server_s)) {
        cx_error("sqlite_load: calculated size '%d' of type '::cortex::sqlite::server' doesn't match C-type size '%d'", cx_type(sqlite_server_o)->size, sizeof(struct sqlite_server_s));
    }

    if (_a_) {
        cx_free(_a_);
    }

    return 0;
error:
    return -1;
}

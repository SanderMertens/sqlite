#include "cx.h"
#include "cx_depresolver.h"


int sqlite_resolveDeclareAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

int sqlite_resolveDefineAction(cx_object o, void *userData) {
    CX_UNUSED(o);
    CX_UNUSED(userData);
    return 0;
}

cx_equalityKind sqlite_compareObjectId(cx_type _this, const void *o1, const void *o2) {
    CX_UNUSED(_this);
    int result = strcmp(o1, o2);
    return result > 0 ? CX_GT : (result < 0 ? CX_LT : CX_EQ);
}

void sqlite_storeDependencies(cx_rbtree tree,
    const char *object, const char *parent, const char *type,
    char **outObject, char **outParent, char **outType) {
    if (!cx_rbtreeHasKey(tree, object, (void **)outObject)) {
        *outObject = cx_malloc(strlen(object) + 1);
        cx_rbtreeSet(tree, strcpy(*outObject, object), *outObject);
    }
    if (!cx_rbtreeHasKey(tree, parent, (void **)outParent)) {
        *outParent = cx_malloc(strlen(parent) + 1);
        cx_rbtreeSet(tree, strcpy(*outParent, parent), *outParent);
    }
    if (!cx_rbtreeHasKey(tree, type, (void **)outType)) {
        *outType = cx_malloc(strlen(type) + 1);
        cx_rbtreeSet(tree, strcpy(*outType, type), *outType);
    }
}

void sqlite_setDependencies(cx_depresolver resolver,
        const char *object, const char *parent, const char *type,
        cx_uint8 parentState) {
    printf("name=%s, parent=%s, type=%s, parentState=%d\n", object, parent, type, parentState);
    /* TODO remove cast */
    cx_depresolver_depend(resolver, (void *)object, CX_DECLARED, (void *)parent, parentState);
    if (parentState == CX_DECLARED) {
        /* TODO remove cast */
        cx_depresolver_depend(resolver, (void *)parent, CX_DEFINED, (void *)object, CX_DEFINED);
    }
    /* TODO remove cast */
    cx_depresolver_depend(resolver, (void *)object, CX_DECLARED, (void *)type, CX_DEFINED);
}

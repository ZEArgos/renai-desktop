#include "Ambiguous.h"

void AssignAmbiguousType(AmbiguousType* affected, AmbiguousTypeSpecifier member,
                         void* value)
{
    // Define the body expression of the switch statement. In this case, we
    // simply assign a value to the given member.
#define ASSIGN(state, size)                                                    \
    affected->__FIELD(state, size) = VPTT(state##size, value);
    // Construct the switch statement using our funky little macro.
    __AMBIGUOUS_BODY(member, ASSIGN);
}

__AMBIGUOUS GetAmbiguousType(AmbiguousType* affected,
                             AmbiguousTypeSpecifier member)
{
    // Define the body expression of this function's switch. Here, we simply
    // return the member requested.
#define READ(state, size) return TTVP(affected->__FIELD(state, size));
    // Construct the switch statement using the above body.
    __AMBIGUOUS_BODY(member, READ);
    // This part can't ever happen, but it's here to remove compiler warnings.
    return NULL;
}

__BOOLEAN CompareAmbiguousType(AmbiguousType* affected,
                               AmbiguousTypeSpecifier member, void* value)
{
    // Define the body expression for this particular function's switch
    // statement. Here, we compare the value given and the value of the given
    // member, and return the result.
#define COMPARE(state, size)                                                   \
    if (affected->__FIELD(state, size) == VPTT(state##size, value))            \
        return true;                                                           \
    return false;
    // Construct the switch statement.
    __AMBIGUOUS_BODY(member, COMPARE);
    // This part can never happen, but it's here to assuage compiler concerns.
    return false;
}

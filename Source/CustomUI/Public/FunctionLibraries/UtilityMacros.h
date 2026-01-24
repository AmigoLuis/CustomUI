// Amigo Luis all rights reserved

#pragma once

// produce getter and setter, setter's parameter is passing in by 
// const reference
#define GETTER_AND_SETTER_REFERENCE(Type, ClassMemberName) \
[[nodiscard]] FORCEINLINE Type Get##ClassMemberName() const\
{\
return ClassMemberName;\
}\
void Set##ClassMemberName(const Type& In##ClassMemberName)\
{\
ClassMemberName = In##ClassMemberName;\
}

// produce getter and setter, setter's parameter is passing in by 
// value
#define GETTER_AND_SETTER_VALUE(Type, ClassMemberName) \
[[nodiscard]] FORCEINLINE Type Get##ClassMemberName() const\
{\
return ClassMemberName;\
}\
FORCEINLINE void Set##ClassMemberName(Type In##ClassMemberName)\
{\
ClassMemberName = In##ClassMemberName;\
}
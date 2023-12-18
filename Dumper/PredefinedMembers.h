#pragma once
#include <string>
#include <vector>

#include "Enums.h"

struct PredefinedMember
{
    std::string Type;
    std::string Name;

    int32 Offset;
    int32 Size;
    int32 ArrayDim;
    int32 Alignment;

    bool bIsStatic;

    bool bIsBitField;
    uint8 BitIndex;
};

struct PredefinedFunction
{
    std::string RetType;
    std::string Name;

    std::vector<std::pair<std::string, std::string>> Params;

    std::string Body;

    bool bIsStatic;
    bool bIsBodyInline;
};

struct PredefinedElements
{
    std::vector<PredefinedMember> Members;
    std::vector<PredefinedFunction> Functions;
};

struct PredefinedStruct
{
    std::string UniqueName;
    int32 Size;
    int32 Alignment;
    bool bUseExplictAlignment;
    bool bIsFinal;

    const PredefinedStruct* Super;

    std::vector<PredefinedMember> Properties;
    std::vector<PredefinedFunction> Functions;
};

/* unordered_map<StructIndex, Members/Functions> */
using PredefinedMemberLookupMapType = std::unordered_map<int32 /* StructIndex */, PredefinedElements /* Members/Functions */>;

// requires strict weak ordering
inline bool CompareUnrealProperties(UEProperty Left, UEProperty Right)
{
    return Left.GetOffset() < Right.GetOffset();
};

// requires strict weak ordering
inline bool ComparePredefinedMembers(const PredefinedMember& Left, const PredefinedMember& Right)
{
    // if both members are static, sort lexically
    if (Left.bIsStatic && Right.bIsStatic)
        return Left.Name < Right.Name;

    // if one member is static, return true if Left is static, false if Right
    if (Left.bIsStatic || Right.bIsStatic)
        return Left.bIsStatic > Right.bIsStatic;

    return Left.Offset < Right.Offset;
};

/*
Order:
    static non-inline
    non-inline
    static inline
    inline
*/

// requires strict weak ordering
inline bool CompareUnrealFunctions(UEFunction Left, UEFunction Right)
{
    const bool bIsLeftStatic = Left.HasFlags(EFunctionFlags::Static);
    const bool bIsRightStatic = Right.HasFlags(EFunctionFlags::Static);

    // Static members come first
    if (bIsLeftStatic != bIsRightStatic)
        return bIsLeftStatic > bIsRightStatic;

    return Left.GetIndex() < Right.GetIndex();
};

// requires strict weak ordering
inline bool ComparePredefinedFunctions(const PredefinedFunction& Left, const PredefinedFunction& Right)
{
    // Non-inline members come first
    if (Left.bIsBodyInline != Right.bIsBodyInline)
        return Left.bIsBodyInline < Right.bIsBodyInline;

    // Static members come first
    if (Left.bIsStatic != Right.bIsStatic)
        return Left.bIsStatic > Right.bIsStatic;

    return Left.Name < Right.Name;
};


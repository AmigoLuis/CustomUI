// Amigo Luis all rights reserved

#pragma once

enum ELogLevelUI : uint8
	{
		/** Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
		Fatal = 1,

		/** 
		 * Prints an error to console (and log file). 
		 * Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
		 */
		Error,

		/** 
		 * Prints a warning to console (and log file).
		 * Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
		 */
		Warning,

		/** Prints a message to console (and log file) */
		Display,

		/** Prints a message to a log file (does not print to console) */
		Log,

		/** 
		 * Prints a verbose message to a log file (if Verbose logging is enabled for the given category, 
		 * usually used for detailed logging) 
		 */
		Verbose,

		/** 
		 * Prints a verbose message to a log file (if VeryVerbose logging is enabled, 
		 * usually used for detailed logging that would otherwise spam output) 
		 */
		VeryVerbose
	};
// 获取任意符号的字符串（无法用于模板）
#define SYMBOL_NAME_TEXT(x) TEXT(#x)
// 定义日志类别
#define M_CUSTOM_LOG_CATEGORY A_LogOfFrontEndUI
DECLARE_LOG_CATEGORY_EXTERN(A_LogOfFrontEndUI, Log, All);

// 留下这个注释以做警示，后续搞一个功能之前，一定一定要查一下有没有已经存在的功能！！
#define M_LOCAL_LOG_PARAMS TEXT("%s"), *Message
inline void PrintInLog(const FString& Message, 
	const ELogLevelUI LogLevelUI = ELogLevelUI::Warning)
{
	switch (LogLevelUI)
	{
	case ELogLevelUI::Fatal:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Fatal, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::Error:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Error, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::Warning:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Warning, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::Display:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Display, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::Log:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Log, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::Verbose:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, Verbose, M_LOCAL_LOG_PARAMS);
		break;
	case ELogLevelUI::VeryVerbose:
		UE_LOG(M_CUSTOM_LOG_CATEGORY, VeryVerbose, M_LOCAL_LOG_PARAMS);
		break;
	}
}

#define LOG_ENTER_FUNCTION() \
PrintInLog(TEXT("Entered Function: ") TEXT(__FUNCTION__) TEXT(", line: ") \
INT_TO_STR(__LINE__) TEXT("."), ELogLevelUI::Display);
// 两级宏：先展开，再字符串化
#define SYMBOL_NAME_STR(x) #x
#define INT_TO_STR(x) TEXT(SYMBOL_NAME_STR(x))

// 生成 函数名 + 行号字符串的宏
#define IN_FUNC_AND_LINE TEXT("in function: ") TEXT(__FUNCTION__) TEXT(", line: ") INT_TO_STR(__LINE__) TEXT(".")
// 日志空指针错误宏
#define LOG_NULL_PTR(PTR) \
PrintInLog(SYMBOL_NAME_TEXT(PTR) TEXT(" is nullptr ") IN_FUNC_AND_LINE, ELogLevelUI::Error);

// 日志空指针警告宏
#define LOG_NULL_PTR_WARN(PTR) \
PrintInLog(SYMBOL_NAME_TEXT(PTR) TEXT(" is nullptr ") IN_FUNC_AND_LINE, ELogLevelUI::Warning);

// 检查参数1是否为nullptr，是的话打印Error日志然后返回参数2
#define CHECK_NULL_RETURN_VALUE(PTR, VALUE) \
if (PTR == nullptr)\
{\
LOG_NULL_PTR(PTR);\
return VALUE;\
}

// 检查参数1是否为nullptr，是的话打印Warn日志然后返回参数2
#define CHECK_NULL_RETURN_VALUE_WARN(PTR, VALUE) \
if (PTR == nullptr)\
{\
LOG_NULL_PTR_WARN(PTR);\
return VALUE;\
}

// 检查参数是否为nullptr，是的话打印Error日志然后返回
#define CHECK_NULL_RETURN(PTR) \
if (PTR == nullptr)\
{\
LOG_NULL_PTR(PTR);\
return;\
}

// 检查参数是否为nullptr，是的话打印Warn日志然后返回
#define CHECK_NULL_RETURN_WARN(PTR) \
if (PTR == nullptr)\
{\
LOG_NULL_PTR_WARN(PTR);\
return;\
}

// 日志bool为真警告宏
#define LOG_BOOL_TRUE_WARN(Condition) \
	do{\
		if(Condition)\
			PrintInLog(SYMBOL_NAME_TEXT(Condition) TEXT(" is true ") IN_FUNC_AND_LINE, ELogLevelUI::Warning);\
	}while(0)

// 日志bool为假警告宏
#define LOG_BOOL_FALSE_WARN(Condition) \
	do{\
		if(!Condition)\
			PrintInLog(SYMBOL_NAME_TEXT(Condition) TEXT(" is false ") IN_FUNC_AND_LINE, ELogLevelUI::Warning);\
	}while(0)

// 检查参数Condition是否为true，是的话打印Warn日志然后返回
#define CHECK_BOOL_TRUE_RETURN_WARN(Condition) \
	do{\
		if (Condition){\
			PrintInLog(SYMBOL_NAME_TEXT(Condition) TEXT(" is true ") IN_FUNC_AND_LINE, ELogLevelUI::Warning);\
			return;}\
	}while(0)

// 检查参数Condition是否为false，是的话打印Warn日志然后返回
#define CHECK_BOOL_FALSE_RETURN_WARN(Condition) \
	do{\
		if (!Condition){\
			PrintInLog(SYMBOL_NAME_TEXT(Condition) TEXT(" is false ") IN_FUNC_AND_LINE, ELogLevelUI::Warning);\
			return;}\
	}while(0)


// 日志打印字符串值
#define LOG_STRING_PTR(StringVariable) \
do{PrintInLog(SYMBOL_NAME_TEXT(StringVariable) TEXT(" is ") + StringVariable +\
TEXT(" in ") IN_FUNC_AND_LINE, ELogLevelUI::Display);}while(0);



// 定义一个 ValidateCompiledDefaults 函数，检查ClassName类的MemberVariableToValidate是否为空
// 需要先 #include "Editor/WidgetCompilerLog.h" ，然后宏需要包含在 #if WITH_EDITOR 和 #endif 之间
#define M_VALIDATE_COMPILED_DEFAULTS_DEFINE(ClassName, MemberVariableToValidate, MemberVariableClass) \
void ClassName::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const\
{\
	Super::ValidateCompiledDefaults(CompileLog);\
	if (!MemberVariableToValidate)/*这里是判断条件*/\
	{\
		const FString& ErrorMessage = FString::Format(TEXT("The variable {0} has not been set.\n"\
			"Please set it to a valid subclass of {1}.\n"\
			"{2} needs a valid {0} to function properly."),\
			{SYMBOL_NAME_TEXT(MemberVariableToValidate),\
				SYMBOL_NAME_TEXT(MemberVariableClass),\
				GetClass()->GetName()});\
		CompileLog.Error(FText::FromString(ErrorMessage));\
	}\
}\

// 定义一个 ValidateCompiledDefaults 函数，检查ClassName类的MemberVariableToValidate是否为空
// 需要先 #include "Editor/WidgetCompilerLog.h" ，然后宏需要包含在 #if WITH_EDITOR 和 #endif 之间
#define M_VALIDATE_COMPILED_DEFAULTS_DEFINE_CUSTOM_CONDITION(ClassName, ErrorCondition, \
	MemberVariableToValidate, MemberVariableClass) \
void ClassName::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const\
{\
Super::ValidateCompiledDefaults(CompileLog);\
if (ErrorCondition)/*这里是判断条件*/\
{\
const FString& ErrorMessage = FString::Format(TEXT("The variable {0} has not been set.\n"\
"Please set it to a valid subclass of {1}.\n"\
"{2} needs a valid {0} to function properly."),\
{SYMBOL_NAME_TEXT(MemberVariableToValidate),\
SYMBOL_NAME_TEXT(MemberVariableClass),\
GetClass()->GetName()});\
CompileLog.Error(FText::FromString(ErrorMessage));\
}\
}\

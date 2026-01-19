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
DECLARE_LOG_CATEGORY_EXTERN(M_CUSTOM_LOG_CATEGORY, Log, All);

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
PrintInLog(TEXT("Entered Function: ") TEXT(__FUNCTION__) TEXT("."), ELogLevelUI::Display);

#define LOG_NULL_PTR(PTR) \
PrintInLog(SYMBOL_NAME_TEXT(PTR) TEXT(" is nullptr in Function:") TEXT(__FUNCTION__) TEXT("."), ELogLevelUI::Error);


%module Core

%csmethodmodifiers XnorCore::Logger::Log<> "private";
%csmethodmodifiers XnorCore::Logger::LogTempDebug<> "private";
%csmethodmodifiers XnorCore::Logger::LogDebug<> "private";
%csmethodmodifiers XnorCore::Logger::LogInfo<> "private";
%csmethodmodifiers XnorCore::Logger::LogWarning<> "private";
%csmethodmodifiers XnorCore::Logger::LogError<> "private";
%csmethodmodifiers XnorCore::Logger::LogFatal<> "private";

%ignore XnorCore::Logger::Stop;

%typemap(cscode) XnorCore::Logger
%{
    public static void Log(Logger.LogLevel level, [global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogString(level, String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }

    public static void LogTempDebug(string message, [global::System.Runtime.CompilerServices.CallerFilePath] string file = "", [global::System.Runtime.CompilerServices.CallerLineNumber] int line = 0)
    {
        LogTempDebugString(message, file, line);
    }

    public static void LogDebug([global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogDebugString(String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }

    public static void LogInfo([global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogInfoString(String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }

    public static void LogWarning([global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogWarningString(String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }

    public static void LogError([global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogErrorString(String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }

    public static void LogFatal([global::System.Diagnostics.CodeAnalysis.StringSyntax(global::System.Diagnostics.CodeAnalysis.StringSyntaxAttribute.CompositeFormat)] string format, params object[] args)
    {
        LogFatalString(String.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
    }
%}

%include "utils/logger.hpp"

%template(LogString) XnorCore::Logger::Log<>;
%template(LogTempDebugString) XnorCore::Logger::LogTempDebug<>;
%template(LogDebugString) XnorCore::Logger::LogDebug<>;
%template(LogInfoString) XnorCore::Logger::LogInfo<>;
%template(LogWarningString) XnorCore::Logger::LogWarning<>;
%template(LogErrorString) XnorCore::Logger::LogError<>;
%template(LogFatalString) XnorCore::Logger::LogFatal<>;

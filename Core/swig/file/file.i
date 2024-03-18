%module Core

%rename(GetFileType) XnorCore::File::GetType;

%include "file/file.hpp"

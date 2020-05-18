#pragma once

#include <functional>
#include <optional>
#include <string>

#include <Windows.h>

class FileSystemWatcher {

public:
	FileSystemWatcher(const std::string& absolute_directory);
	~FileSystemWatcher();

	bool check_changes(int timeout_ms = 100);

private:

	HANDLE m_change_handle;
};